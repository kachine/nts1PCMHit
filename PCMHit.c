/*
 * File: pcmHit.c
 *
 * NTS-1 LoFi PCM Orchestral Hit Oscillator
 *
 */
#include <PCMHit.h>

// PCM oscillator
float osc_pcm(uint8_t notenumber, uint8_t pitchmodifier){
  float sample = 0.f;

  // Read PCM waveform (w/o loop point) using linear interpolation
  float position = PCMPOSCOEFF * osc_w0f_for_note(notenumber, pitchmodifier) * k_samplerate * VOICE.samplePosition;
  float dec = position - (int16_t) position;  // Decimal part of position
  if(position < PCMLEN){
    sample = dec * PCMWAVE[MIN((int16_t) position + 1, PCMLEN-1)] + (1.f - dec) * PCMWAVE[(int16_t) position];
    sample = (sample - 128) / 128;  // Convert 8bit unsigned data to signed value within -1 to 1
    VOICE.samplePosition++;
  }

  return sample;
}

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"

// Initialize function
//  called once on instantiation of the oscillator
void OSC_INIT(uint32_t platform, uint32_t api)
{
  // Initialize voice parameters
  float pcmRootFreq = midi_to_hz_lut_f[PCMROOTNOTE]; // Original frequency of PCMWAVE
  PCMPOSCOEFF = (float) k_samplerate / PCMFS / pcmRootFreq;
  VOICE.notenumber = DUMMYNOTE;
  VOICE.samplePosition = 0;
}

// Wave Generation function
//  callbacked for each sample (or frames)
void OSC_CYCLE(const user_osc_param_t * const params,
               int32_t *yn,
               const uint32_t frames)
{
  // Pointer to output buffer
  q31_t * __restrict y = (q31_t *) yn;

  // Last address of output buffer
  const q31_t * y_e = y + frames;

  // MIDI note# and pitch modifier of current process
  // If pitch bend message has already received, note value may be differ from actual MIDI note#
  // Pitch modifier value takes within 0 to 255, the value indicate 1/255 of semitone
  // The pitch modifier is always upperward, so downer pitch bend is processed as a combination of note# decrement and adequate upperward pitch modifier.
  uint8_t note = params->pitch >> 8;
  uint8_t mod = params->pitch & 0xFF;

  // Working memory to store current sample value
  // Effective range is -1.0 <= sample < 1.0 to convert into Q31 format later
  float sample = 0.f;

  // Note change detection
  if(note != VOICE.notenumber){
    // Reset sample position for re-trigger
    VOICE.samplePosition = 0;
    // Store current notenumber to detect note change in the next hook
    VOICE.notenumber = note;
  }

  // Process one sample by sample in frames
  while( y != y_e ) {
    // Generate wave sample
    sample = osc_pcm(note, mod);
    // Convert a sample into Q31 format, and write to output
    *(y++) = f32_to_q31(sample);
  }
}

// MIDI note-on event process function
//  * This function is not hooked if active note is already exist
void OSC_NOTEON(const user_osc_param_t * const params)
{
  VOICE.samplePosition = 0;
}

// MIDI note-off event process function
//  * This function is not hooked if active note remains
void OSC_NOTEOFF(const user_osc_param_t * const params)
{
  // Nothing to do mandatory in NTS-1
}

// Parameter change event process funnction
void OSC_PARAM(uint16_t index, uint16_t value)
{
  // 0-200 for bipolar percent parameters. 0% at 100, -100% at 0.
  // 0-100 for unipolar percent and typeless parameters.
  // 10 bit resolution for shape/shift-shape.
  switch (index) {
    case k_user_osc_param_id1: // Not used
      // Choose this param by pressing OSC and tweaking TYPE knob, then input by B knob
      break;
    case k_user_osc_param_id2: // Not used
      // Choose this param by pressing OSC and tweaking TYPE knob, then input by B knob
      break;
    case k_user_osc_param_id3: // Not used
      // Choose this param by pressing OSC and tweaking TYPE knob, then input by B knob
      break;
    case k_user_osc_param_id4: // Not used
      // Choose this param by pressing OSC and tweaking TYPE knob, then input by B knob
      break;
    case k_user_osc_param_id5: // Not used
      // Choose this param by pressing OSC and tweaking TYPE knob, then input by B knob
      break;
    case k_user_osc_param_id6: // Not used
      // Choose this param by pressing OSC and tweaking TYPE knob, then input by B knob
      break;
    case k_user_osc_param_shape: // Not used
      // 10bit parameter, 0 <= value <= 1023
      // Mapped to OSC mode A knob(shape) and MIDI CC#54
      //   CC#54 value=0:   uint16_t value=0
      //   CC#54 value=32:  uint16_t value=256
      //   CC#54 value=64:  uint16_t value=512
      //   CC#54 value=126: uint16_t value=1008
      //   CC#54 value=127: uint16_t value=1023
      //    --> CC#54 value*8 = uint16_t value, except CC value=127
      // You can use param_val_to_f32(value) to convert value to the range within 0 to 1 in float format
      break;
    case k_user_osc_param_shiftshape: // Not used
      // Similar to k_user_osc_param_shape, but mapped to OSC mode B knob(alt) and MIDI CC#55
      break;
    default:
      break;
  }
}
#pragma GCC diagnostic pop

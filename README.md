# PCM Hit
"ORCHESTRAL HIT" Lo-Fi PCM oscillator for KORG NTS-1


## What's this
A PCM oscillator for KORG NTS-1 with built-in "ORCHESTRAL HIT" waveform.
Only one Lo-Fi (8bit/32KHz) waveform is embedded, because the NTS-1 user oscillator program has a size limitation within 32KB.
The waveform was sampled from early 90's equipment, and processed by recent software technologies.


## Prerequisite
[logue-sdk](https://github.com/korginc/logue-sdk)

If you use pre-built binary, logue-sdk is not required. Simply transfer **PCMHit.ntkdigunit** to your NTS-1 using NTS-1 digital Librarian software.


## How to build
You have to change PLATFORMDIR variable in Makefile, it should correspond to where you installed logue-sdk (something like below).
```Makefile
PLATFORMDIR = $(PATHTO}/logue-sdk/platform/nutekt-digital
```

Then, simply type "make" to build this project.
```sh
$ make
```


---
The names of products or companies are the trademarks or registered trademarks of their respective companies. These companies have no relation to this project.

audio-samples
=============

Codes for generating audio samples for music or other purposes

## Dependencies

* [libao](https://www.xiph.org/ao/)

## Building

Examples can be built using gcc command lines like the following:

    gcc -o impulse_wav impulse_wav.c -I/usr/local/include/ -L/usr/local/lib/ -lao 

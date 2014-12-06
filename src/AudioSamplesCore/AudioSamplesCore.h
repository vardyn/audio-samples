/*
 * AudioSamplesCore.h
 * Header file for Audio Samples Core Library
 */

#ifndef AUDIO_SAMPLES_CORE_H_INCLUDED
#define AUDIO_SAMPLES_CORE_H_INCLUDED

#define WAV16BIT_MAX  32767
#define WAV16BIT_MIN -32767
#define WAV24BIT_MAX  8388607
#define WAV24BIT_MIN -8388607

int write_wav_sample_16bit(char *sample, int value);
int write_wav_sample_24bit(char *sample, int value);

#endif

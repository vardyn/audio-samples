/*
 * AudioSamplesCore.h
 * Header file for Audio Samples Core Library
 */

#ifndef AUDIO_SAMPLES_CORE_H_INCLUDED
#define AUDIO_SAMPLES_CORE_H_INCLUDED

#define ASC_WAV16BIT_MAX  32767
#define ASC_WAV16BIT_MIN -32767
#define ASC_WAV24BIT_MAX  8388607
#define ASC_WAV24BIT_MIN -8388607

#include "ASCLogging.h"
#include "ASCSimpleLogging.h"

/* WAV format helpers */
int asc_write_wav_sample_16bit(char *sample, int value);
int asc_write_wav_sample_24bit(char *sample, int value);

/* Audio signal generation */
double asc_envelope_AD(double peak, double attack, double decay,
                       double attack_slope, double decay_slope,
                       int sample, int sample_rate);
double asc_sine(double amplitude, double frequency,
                double start_phase, int sample, int sample_rate);

#endif

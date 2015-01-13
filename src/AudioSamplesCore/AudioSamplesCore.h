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

typedef struct asc_func_s asc_func_t;

struct asc_func_s {
    double current_value;
    int current_sample;
    int sample_rate;
    void (*function)(asc_func_t *, int);
    void *state;
};

/* ASC function helpers */
asc_func_t *asc_func_new(int);
double asc_func_eval(asc_func_t *func, int sample);
void asc_func_free(asc_func_t *);

/* Audio signal generation */
asc_func_t *asc_constant_gen(double constant, int sample,
                             int sample_rate);
asc_func_t *asc_envelope_AD_gen(double peak, double attack,
                                double decay, double attack_slope,
                                double decay_slope, int sample,
                                int sample_rate);
asc_func_t *asc_sine_gen(asc_func_t *amplitude, asc_func_t *frequency,
                         asc_func_t *phase, int sample,
                         int sample_rate);

#endif

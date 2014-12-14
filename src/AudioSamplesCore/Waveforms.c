/*
 * Waveforms.c
 * Audio Samples Core waveform generators
 */

#include "AudioSamplesCore.h"

#include <math.h>
#include <stddef.h>
#include <stdlib.h>

/* run-function declarations */
void asc_constant_run(asc_func_t *, int);
void asc_sine_run(asc_func_t *, int);

typedef struct asc_sine_state_s {
    asc_func_t *amplitude;
    asc_func_t *frequency;
    asc_func_t *phase;
} asc_sine_state_t;

asc_func_t *asc_constant_gen(double constant, int sample,
                             int sample_rate)
{
    asc_func_t *func;

    func = (asc_func_t *) asc_func_new(sample_rate);
    if (NULL == func)
    {
        ASC_ERROR("failed to create asc_func");
        return NULL;
    }

    func->current_value = constant;
    func->current_sample = sample;
    func->function = asc_constant_run;
    func->state = NULL;

    return func;
}

void asc_constant_run(asc_func_t *func, int sample)
{
}

asc_func_t *asc_sine_gen(asc_func_t *amplitude, asc_func_t *frequency,
                         asc_func_t *phase, int sample,
                         int sample_rate)
{
    asc_func_t *func;
    asc_sine_state_t *state;

    state = (asc_sine_state_t *) malloc(sizeof(asc_sine_state_t));
    if (NULL == state)
    {
        ASC_ERROR("out of memory when allocating state");
        return NULL;
    }
    state->amplitude = amplitude;
    state->frequency = frequency;
    state->phase = phase;

    func = (asc_func_t *) asc_func_new(sample_rate);
    if (NULL == func)
    {
        ASC_ERROR("failed to create asc_func");
        free(state);
        return NULL;
    }

    func->current_value = 0.0;
    func->current_sample = sample;
    func->function = asc_sine_run;
    func->state = state;

    return func;
}

void asc_sine_run(asc_func_t *func, int sample)
{
    double time, phase;
    asc_sine_state_t *state;

    state = (asc_sine_state_t *) func->state;

    time = ((double) sample)/func->sample_rate;
    phase = 2.0*M_PI*asc_func_eval(state->frequency, sample)*time +
            asc_func_eval(state->phase, sample);

    func->current_value = asc_func_eval(state->amplitude, sample)*
                          sin(phase);
}

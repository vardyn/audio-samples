/*
 * Envelopes.c
 * Audio Samples Core envelope generators
 */

#include "AudioSamplesCore.h"

#include <math.h>
#include <stddef.h>
#include <stdlib.h>

/* run-function declarations */
void asc_envelope_AD_run(asc_func_t *, int);

typedef struct asc_envelope_AD_state_s
{
    double peak;
    double attack;
    double decay;
    double attack_slope_exponent;
    double decay_slope_exponent;
} asc_envelope_AD_state_t;

asc_func_t *asc_envelope_AD_gen(double peak, double attack,
                                double decay, double attack_slope,
                                double decay_slope, int sample,
                                int sample_rate)
{
    asc_func_t *func;
    asc_envelope_AD_state_t *state;
    double x_slope, y_slope;

    state = (asc_envelope_AD_state_t *)
            malloc(sizeof(asc_envelope_AD_state_t));
    if (NULL == state)
    {
        ASC_ERROR("out of memory when allocating state");
        return NULL;
    }

    state->peak = peak;

    state->attack = attack;
    x_slope = 0.5 - attack_slope*0.25;
    y_slope = 0.5 + attack_slope*0.25;
    state->attack_slope_exponent = log(y_slope)/log(x_slope);

    state->decay = decay;
    x_slope = 0.5 - decay_slope*0.25;
    y_slope = 0.5 + decay_slope*0.25;
    state->decay_slope_exponent = log(y_slope)/log(x_slope);

    func = asc_func_new(sample_rate);
    if (NULL == func)
    {
        ASC_ERROR("failed to create asc_func");
        free(state);
        return NULL;
    }

    func->current_value = 0.0;
    func->current_sample = sample;
    func->function = asc_envelope_AD_run;
    func->state = state;

    return func;
}

void asc_envelope_AD_run(asc_func_t *func, int sample)
{
    double time, segment_time;
    asc_envelope_AD_state_t *state;

    state = (asc_envelope_AD_state_t *) func->state;

    time = ((double) sample)/func->sample_rate;

    if (time >= state->attack + state->decay)
        func->current_value = 0.0;
    else if (time >= state->attack)
    {
        segment_time = (time - state->attack)/state->decay;
        func->current_value = state->peak*(1.0 - pow(segment_time,
                                                     state->decay_slope_exponent));
    }
    else if (time >= 0)
    {
        if (state->attack > 0.0)
        {
            segment_time = time/state->attack;
            func->current_value = state->peak*(1.0 -
                                               pow(1.0 - segment_time,
                                                   state->attack_slope_exponent));
        }
        else
            func->current_value = state->peak;
    }
}

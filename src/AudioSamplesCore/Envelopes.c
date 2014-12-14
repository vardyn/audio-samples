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
    double attack_slope;
    double decay_slope;
} asc_envelope_AD_state_t;

asc_func_t *asc_envelope_AD_gen(double peak, double attack,
                                double decay, double attack_slope,
                                double decay_slope, int sample,
                                int sample_rate)
{
    asc_func_t *func;
    asc_envelope_AD_state_t *state;

    state = (asc_envelope_AD_state_t *)
            malloc(sizeof(asc_envelope_AD_state_t));
    if (NULL == state)
    {
        ASC_ERROR("out of memory when allocating state");
        return;
    }

    state->peak = peak;
    state->attack = attack;
    state->decay = decay;
    state->attack_slope = attack_slope;
    state->decay_slope = decay_slope;

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
    double time, attack_slope_exponent, decay_slope_exponent, m;
    asc_envelope_AD_state_t *state;

    state = (asc_envelope_AD_state_t *) func->state;

    time = ((double) sample)/func->sample_rate;

    if (time > state->attack + state->decay)
        func->current_value = 0.0;
    else if (time > state->attack)
        if (state->decay > 0.0)
        {
            decay_slope_exponent = exp(-1.0*state->decay_slope);
            m = 1.0/pow(state->decay, decay_slope_exponent);
            func->current_value = state->peak*(1.0 -
                                               m*pow(time -
                                                     state->attack,
                                                     decay_slope_exponent));
        }
        else
            func->current_value = 0.0;
    else if (time >= 0)
        if (state->attack > 0.0)
        {
            attack_slope_exponent = exp(1.0*state->attack_slope);
            m = 1.0/pow(state->attack, attack_slope_exponent);
            func->current_value = state->peak*(1.0 -
                                               m*pow(state->attack -
                                                     time,
                                                     attack_slope_exponent));
        }
        else
            func->current_value = state->peak;
}

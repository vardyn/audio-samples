/*
 * Envelopes.c
 * Audio Samples Core envelope generators
 */

#include "AudioSamplesCore.h"

#include <math.h>

double asc_envelope_AD(double peak, double attack, double decay,
                       double attack_slope, double decay_slope,
                       int sample, int sample_rate)
{
    double time, attack_slope_exponent, decay_slope_exponent, m;

    time = ((double) sample)/sample_rate;
    
    if (time > attack + decay)
        return 0.0;
    else if (time > attack)
        if (decay > 0.0)
        {
            decay_slope_exponent = exp(-1.0*decay_slope);
            m = 1.0/pow(decay, decay_slope_exponent);
            return peak*(1.0 - m*pow(time - attack,
                                     decay_slope_exponent));
        }
        else
            return 0.0;
    else if (time >= 0)
        if (attack > 0.0)
        {
            attack_slope_exponent = exp(1.0*attack_slope);
            m = 1.0/pow(attack, attack_slope_exponent);
            return peak*(1.0 - m*pow(attack - time,
                                     attack_slope_exponent));
        }
        else
            return peak;
    
    return 0.0;
}

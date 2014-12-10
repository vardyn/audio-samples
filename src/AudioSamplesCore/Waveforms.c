/*
 * Waveforms.c
 * Audio Samples Core waveform generators
 */

#include "AudioSamplesCore.h"
#include <math.h>

double asc_sine(double amplitude, double frequency,
                double start_phase, int sample, int sample_rate)
{
    double time, phase;

    time = ((double) sample)/sample_rate;
    phase = 2.0*M_PI*frequency*time + start_phase;
    
    return amplitude*sin(phase);
}

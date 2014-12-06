/*
 * AudioSamplesCore.h
 * Implementation of Audio Samples Core Library
 */

#include "AudioSamplesCore.h"

int write_wav_sample_16bit(char *sample, int value)
{
    if (WAV16BIT_MAX < value)
        value = WAV16BIT_MAX;
    else if (WAV16BIT_MIN > value)
        value = WAV16BIT_MIN;
    
    sample[0] = value & 0xff;
    sample[1] = (value >> 8) & 0xff;
    
    return 0;
}

int write_wav_sample_24bit(char *sample, int value)
{
    if (WAV24BIT_MAX < value)
        value = WAV24BIT_MAX;
    else if (WAV24BIT_MIN > value)
        value = WAV24BIT_MIN;
    
    sample[0] = value & 0xff;
    sample[1] = (value >> 8) & 0xff;
    sample[2] = (value >> 16) & 0xff;
    
    return 0;
}

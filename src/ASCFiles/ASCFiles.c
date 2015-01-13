/*
 * ASCFiles.c
 * Implementation of Audio Samples Files Library
 */

#include "ASCFiles.h"

int asc_write_wav_sample_16bit(unsigned char *sample, int value)
{
    if (ASC_WAV16BIT_MAX < value)
        value = ASC_WAV16BIT_MAX;
    else if (ASC_WAV16BIT_MIN > value)
        value = ASC_WAV16BIT_MIN;
    
    sample[0] = value & 0xff;
    sample[1] = (value >> 8) & 0xff;
    
    return 0;
}

int asc_write_wav_sample_24bit(unsigned char *sample, int value)
{
    if (ASC_WAV24BIT_MAX < value)
        value = ASC_WAV24BIT_MAX;
    else if (ASC_WAV24BIT_MIN > value)
        value = ASC_WAV24BIT_MIN;
    
    sample[0] = value & 0xff;
    sample[1] = (value >> 8) & 0xff;
    sample[2] = (value >> 16) & 0xff;
    
    return 0;
}

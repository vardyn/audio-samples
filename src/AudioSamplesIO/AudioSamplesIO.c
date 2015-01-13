/*
 * AudioSamplesIO.c
 * Implementation of Audio Samples IO Library
 */

#include "AudioSamplesIO.h"
#include "ASCLogging.h"

#include <stdint.h>

int asio_write_wav_sample_16bit(uint8_t *sample, int32_t value)
{
    if (ASIO_WAV16BIT_MAX < value)
    {
        ASC_DEBUG("clipped 16-bit sample larger than maximum");
        value = ASIO_WAV16BIT_MAX;
    }
    else if (ASIO_WAV16BIT_MIN > value)
    {
        ASC_DEBUG("clipped 16-bit sample smaller than minimum");
        value = ASIO_WAV16BIT_MIN;
    }
    
    sample[0] = value & 0xff;
    sample[1] = (value >> 8) & 0xff;
    
    return 0;
}

int asio_write_wav_sample_24bit(uint8_t *sample, int32_t value)
{
    if (ASIO_WAV24BIT_MAX < value)
    {
        ASC_DEBUG("clipped 24-bit sample larger than maximum");
        value = ASIO_WAV24BIT_MAX;
    }
    else if (ASIO_WAV24BIT_MIN > value)
    {
        ASC_DEBUG("clipped 24-bit sample smaller than minimum");
        value = ASIO_WAV24BIT_MIN;
    }
    
    sample[0] = value & 0xff;
    sample[1] = (value >> 8) & 0xff;
    sample[2] = (value >> 16) & 0xff;
    
    return 0;
}

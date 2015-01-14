/*
 * AudioSamplesIO.h
 * Header file for Audio Samples IO Library
 */

#ifndef AUDIO_SAMPLES_IO_H_INCLUDED
#define AUDIO_SAMPLES_IO_H_INCLUDED

#include <stdint.h>

#define ASIO_STATUS_SUCCESS 0
#define ASIO_STATUS_ERROR   1

#define ASIO_WAV16BIT_MAX  32767
#define ASIO_WAV16BIT_MIN -32767
#define ASIO_WAV24BIT_MAX  8388607
#define ASIO_WAV24BIT_MIN -8388607

/* WAV format helpers */
int asio_write_wav_sample_16bit(uint8_t *sample, int32_t value);
int asio_write_wav_sample_24bit(uint8_t *sample, int32_t value);

#endif

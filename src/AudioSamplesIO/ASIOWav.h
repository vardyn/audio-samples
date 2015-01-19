/*
 * ASIOWav.h
 * Header file for Audio Samples IO WAV support
 */

#ifndef ASIO_WAV_H_INCLUDED
#define ASIO_WAV_H_INCLUDED

#include "ASIORiff.h"

#include <stdint.h>

#define ASIO_FOURCC_WAVE 0x45564157
#define ASIO_FOURCC_FMT  0x20746d66

typedef struct asio_wav_fmt_s asio_wav_fmt_t;

struct asio_wav_fmt_s {
    uint16_t compression_code;
    uint16_t channels;
    uint32_t sample_rate;
    uint32_t byte_rate;
    uint16_t block_align;
    uint16_t bits_per_sample;
    uint16_t extra_bytes_size;
    void *extra_bytes;
};

asio_wav_fmt_t *asio_wav_fmt_init();
void asio_wav_fmt_free(asio_wav_fmt_t *);
int asio_wav_fmt_unpack(asio_wav_fmt_t *, asio_riff_chunk_t *);
int asio_wav_fmt_pack(asio_wav_fmt_t *, asio_riff_chunk_t *);

#endif // ASIO_WAV_H_INCLUDED

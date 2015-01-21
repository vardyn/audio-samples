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
#define ASIO_FOURCC_SMPL 0x6c706d73
#define ASIO_FOURCC_INST 0x74736e69

typedef struct asio_wav_fmt_s asio_wav_fmt_t;
typedef struct asio_wav_fmt_ext_s asio_wav_fmt_ext_t;
typedef struct asio_wav_smpl_s asio_wav_smpl_t;
typedef struct asio_wav_smpl_loop_s asio_wav_smpl_loop_t;
typedef struct asio_wav_inst_s asio_wav_inst_t;

struct asio_wav_fmt_s {
    uint16_t format_tag;
    uint16_t channels;
    uint32_t sample_rate;
    uint32_t byte_rate;
    uint16_t block_align;
    uint16_t bits_per_sample;
    uint16_t extra_bytes_size;
    void *extra_bytes;
};

struct asio_wav_fmt_ext_s {
    uint16_t samples_value;
    uint32_t channel_mask;
    uint32_t uuid1;
    uint16_t uuid2;
    uint16_t uuid3;
    uint8_t  uuid4[8];
};

struct asio_wav_smpl_s {
    uint32_t manufacturer;
    uint32_t product;
    uint32_t sample_period;
    uint32_t midi_root_note;
    uint32_t midi_pitch_fraction;
    uint32_t smpte_format;
    uint32_t smpte_offset;
    uint32_t sample_loops_count;
    uint32_t sampler_data_size;
    asio_wav_smpl_loop_t *sample_loops;
    void *sampler_data;
};

struct asio_wav_smpl_loop_s {
    uint32_t cue_point_id;
    uint32_t type;
    uint32_t start_offset;
    uint32_t end_offset;
    uint32_t fraction;
    uint32_t play_count;
};

struct asio_wav_inst_s {
    uint8_t root_note;
    int8_t fine_tune;
    int8_t gain;
    uint8_t low_note;
    uint8_t high_note;
    uint8_t low_velocity;
    uint8_t high_velocity;
};

asio_wav_fmt_t *asio_wav_fmt_init();
void asio_wav_fmt_free(asio_wav_fmt_t *);
int asio_wav_fmt_unpack(asio_wav_fmt_t *, asio_riff_chunk_t *);

asio_wav_fmt_ext_t *asio_wav_fmt_ext_init();
void asio_wav_fmt_ext_free(asio_wav_fmt_ext_t *);
int asio_wav_fmt_ext_unpack(asio_wav_fmt_ext_t *, asio_wav_fmt_t *);

asio_wav_smpl_t *asio_wav_smpl_init();
void asio_wav_smpl_free(asio_wav_smpl_t *);
int asio_wav_smpl_unpack(asio_wav_smpl_t *, asio_riff_chunk_t *);

asio_wav_inst_t *asio_wav_inst_init();
void asio_wav_inst_free(asio_wav_inst_t *);
int asio_wav_inst_unpack(asio_wav_inst_t *, asio_riff_chunk_t *);
int asio_wav_inst_pack(asio_wav_inst_t *, asio_riff_chunk_t *);

/*
 * WAVE codec format tags
 */

#define ASIO_WAVE_FORMAT_UNKNOWN    0x0000
#define ASIO_WAVE_FORMAT_PCM        0x0001
#define ASIO_WAVE_FORMAT_IEEE_FLOAT 0x0003
#define ASIO_WAVE_FORMAT_EXTENSIBLE 0xfffe

#endif // ASIO_WAV_H_INCLUDED

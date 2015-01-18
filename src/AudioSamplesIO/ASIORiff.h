/*
 * ASIORiff.h
 * Header file for Audio Samples IO RIFF support
 */

#ifndef ASIO_RIFF_H_INCLUDED
#define ASIO_RIFF_H_INCLUDED

#include <stdint.h>

#define ASIO_FOURCC_NULL 0x20202020
#define ASIO_FOURCC_RIFF 0x52494646
#define ASIO_FOURCC_WAVE 0x57415645

typedef struct asio_riff_file_s asio_riff_file_t;
typedef struct asio_riff_chunk_s asio_riff_chunk_t;

struct asio_riff_file_s {
    uint32_t file_size;
    uint32_t file_type;
    uint32_t chunk_count;
    asio_riff_chunk_t **chunks;
};

struct asio_riff_chunk_s {
    uint32_t type;
    uint32_t data_size;
    void *data;
};

asio_riff_file_t *asio_riff_file_init();
void asio_riff_file_free(asio_riff_file_t *file);
int asio_riff_file_open(asio_riff_file_t *file, const char *filename);

asio_riff_chunk_t *asio_riff_chunk_init();
void asio_riff_chunk_free(asio_riff_chunk_t *);

#endif // ASIO_RIFF_H_INCLUDED

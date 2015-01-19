/*
 * ASIOWav.c
 * Implementation of Audio Samples IO WAV support
 */

#include "ASCLogging.h"
#include "ASIORiff.h"
#include "ASIOWav.h"
#include "AudioSamplesIO.h"

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

asio_wav_fmt_t *asio_wav_fmt_init()
{
    asio_wav_fmt_t *fmt;

    fmt = (asio_wav_fmt_t *) calloc(1, sizeof(asio_wav_fmt_t));

    if (NULL == fmt)
    {
        ASC_ERROR("out of memory when allocating WAV format");
        return NULL;
    }

    ASC_DEBUG("allocated WAV format at %p", fmt);

    return fmt;
}

void asio_wav_fmt_free(asio_wav_fmt_t *fmt)
{
    if (NULL == fmt)
        return;

    if (NULL != fmt->extra_bytes)
        free(fmt->extra_bytes);

    free(fmt);
    ASC_DEBUG("freed WAV format at %p", fmt);
}

int asio_wav_fmt_unpack(asio_wav_fmt_t *fmt, asio_riff_chunk_t *chunk)
{
    if (NULL == chunk)
    {
        ASC_ERROR("chunk must not be null");
        goto error;
    }

    if (ASIO_FOURCC_FMT != chunk->type)
    {
        ASC_ERROR("incorrect chunk type %u found", chunk->type);
        goto error;
    }

    if (16 > chunk->data_size)
    {
        ASC_ERROR("WAV format chunk data must be at least 16 bytes");
        goto error;
    }

    if (NULL == chunk->data)
    {
        ASC_ERROR("chunk data must not be null");
        goto error;
    }

    fmt->compression_code = *((uint16_t *) chunk->data);
    ASC_DEBUG("read WAV format compression code %u", fmt->compression_code);
    fmt->channels = *((uint16_t *) (chunk->data + 2));
    ASC_DEBUG("read WAV format number of channels %u", fmt->channels);
    fmt->sample_rate = *((uint32_t *) (chunk->data + 4));
    ASC_DEBUG("read WAV format sample rate %u", fmt->sample_rate);
    fmt->byte_rate = *((uint32_t *) (chunk->data + 8));
    ASC_DEBUG("read WAV format average byte rate %u", fmt->byte_rate);
    fmt->block_align = *((uint16_t *) (chunk->data + 12));
    ASC_DEBUG("read WAV format block alignment %u", fmt->block_align);
    fmt->bits_per_sample = *((uint16_t *) (chunk->data + 14));
    ASC_DEBUG("read WAV format bits per sample %u", fmt->bits_per_sample);

    if (18 <= chunk->data_size)
    {
        fmt->extra_bytes_size = *((uint16_t *) (chunk->data + 16));
        ASC_DEBUG("read WAV format extra bytes length %u",
                  fmt->extra_bytes_size);
    }
    else
        fmt->extra_bytes_size = 0;

    if (0 < fmt->extra_bytes_size)
    {
        fmt->extra_bytes = malloc(fmt->extra_bytes_size);
        if (NULL == fmt->extra_bytes)
        {
            ASC_ERROR("out of memory when allocating WAV format extra bytes");
            goto error;
        }

        memcpy(fmt->extra_bytes, chunk->data + 18, fmt->extra_bytes_size);
        ASC_DEBUG("read WAV format extra bytes");
    }
    else
        fmt->extra_bytes = NULL;

    return ASIO_STATUS_SUCCESS;

error:
    return ASIO_STATUS_ERROR;
}

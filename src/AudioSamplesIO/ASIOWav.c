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

    if (NULL == fmt)
    {
        ASC_ERROR("fmt must not be null");
        goto error;
    }

    fmt->format_tag = *((uint16_t *) chunk->data);
    ASC_DEBUG("read WAV format tag %u", fmt->format_tag);
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

asio_wav_fmt_ext_t *asio_wav_fmt_ext_init()
{
    asio_wav_fmt_ext_t *fmt_ext;

    fmt_ext = (asio_wav_fmt_ext_t *) calloc(1, sizeof(asio_wav_fmt_ext_t));
    if (NULL == fmt_ext)
    {
        ASC_ERROR("out of memory when allocating extensible WAV format");
        return NULL;
    }

    ASC_DEBUG("allocated extensible WAV format at %p", fmt_ext);

    return fmt_ext;
}

void asio_wav_fmt_ext_free(asio_wav_fmt_ext_t *fmt_ext)
{
    if (NULL == fmt_ext)
        return;

    free(fmt_ext);
    ASC_DEBUG("freed extensible WAV format at %p", fmt_ext);
}

int asio_wav_fmt_ext_unpack(asio_wav_fmt_ext_t *fmt_ext, asio_wav_fmt_t *fmt)
{
    if (NULL == fmt)
    {
        ASC_ERROR("fmt must not be null");
        goto error;
    }

    if (ASIO_WAVE_FORMAT_EXTENSIBLE != fmt->format_tag)
    {
        ASC_ERROR("fmt format tag must be %#06x", ASIO_WAVE_FORMAT_EXTENSIBLE);
        goto error;
    }

    if (22 != fmt->extra_bytes_size)
    {
        ASC_ERROR("fmt extra bytes size must be 22");
        goto error;
    }

    if (NULL == fmt_ext)
    {
        ASC_ERROR("fmt_ext must not be null");
        goto error;
    }

    fmt_ext->samples_value = *((uint16_t *) fmt->extra_bytes);
    ASC_DEBUG("read extensible WAV format samples value %u",
              fmt_ext->samples_value);
    fmt_ext->channel_mask = *((uint32_t *) (fmt->extra_bytes + 2));
    ASC_DEBUG("read extensible WAV format channel mask %#010x",
              fmt_ext->channel_mask);
    memcpy(fmt_ext->guid, fmt->extra_bytes + 6, 16);
    ASC_DEBUG("read extensible WAV format GUID %02x%02x%02x%02x-%02x%02x-"
              "%02x%02x-%02x%02x-%02x%02x%02x%02x%02x%02x", fmt_ext->guid[0],
              fmt_ext->guid[1], fmt_ext->guid[2], fmt_ext->guid[3],
              fmt_ext->guid[4], fmt_ext->guid[5], fmt_ext->guid[6],
              fmt_ext->guid[7], fmt_ext->guid[8], fmt_ext->guid[9],
              fmt_ext->guid[10], fmt_ext->guid[11], fmt_ext->guid[12],
              fmt_ext->guid[13], fmt_ext->guid[14], fmt_ext->guid[15]);

    return ASIO_STATUS_SUCCESS;

error:
    return ASIO_STATUS_ERROR;
}

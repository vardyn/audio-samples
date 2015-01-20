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
        ASC_ERROR("incorrect chunk type %#010x found", chunk->type);
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
    fmt_ext->uuid1 = *((uint32_t *) (fmt->extra_bytes + 6));
    fmt_ext->uuid2 = *((uint16_t *) (fmt->extra_bytes + 10));
    fmt_ext->uuid3 = *((uint16_t *) (fmt->extra_bytes + 12));
    memcpy(fmt_ext->uuid4, fmt->extra_bytes + 14, 8);
    ASC_DEBUG("read extensible WAV format GUID %08x-%04x-%04x-%02x%02x-"
              "%02x%02x%02x%02x%02x%02x", fmt_ext->uuid1, fmt_ext->uuid2,
              fmt_ext->uuid3, fmt_ext->uuid4[0], fmt_ext->uuid4[1],
              fmt_ext->uuid4[2], fmt_ext->uuid4[3], fmt_ext->uuid4[4],
              fmt_ext->uuid4[5], fmt_ext->uuid4[6], fmt_ext->uuid4[7]);

    return ASIO_STATUS_SUCCESS;

error:
    return ASIO_STATUS_ERROR;
}

asio_wav_smpl_t *asio_wav_smpl_init()
{
    asio_wav_smpl_t *smpl;

    smpl = (asio_wav_smpl_t *) calloc(1, sizeof(asio_wav_smpl_t));
    if (NULL == smpl)
    {
        ASC_ERROR("out of memory when allocating smpl");
        return NULL;
    }

    ASC_DEBUG("allocated smpl at %p");

    return smpl;
}

void asio_wav_smpl_free(asio_wav_smpl_t *smpl)
{
    if (NULL == smpl)
        return;

    if (smpl->sample_loops)
        free(smpl->sample_loops);

    if (smpl->sampler_data)
        free(smpl->sampler_data);

    free(smpl);
    ASC_DEBUG("freed smpl at %p", smpl);
}

int asio_wav_smpl_unpack(asio_wav_smpl_t *smpl, asio_riff_chunk_t *chunk)
{
    uint32_t computed_chunk_data_size;
    void *chunk_data_pointer;
    int i;

    if (NULL == chunk)
    {
        ASC_ERROR("chunk must not be null");
        goto error;
    }

    if (ASIO_FOURCC_SMPL != chunk->type)
    {
        ASC_ERROR("incorrect chunk type %#010x found", chunk->type);
        goto error;
    }

    if (36 > chunk->data_size)
    {
        ASC_ERROR("WAV format chunk data must be at least 36 bytes");
        goto error;
    }

    if (NULL == chunk->data)
    {
        ASC_ERROR("chunk data must not be null");
        goto error;
    }

    if (NULL == smpl)
    {
        ASC_ERROR("smpl must not be null");
        goto error;
    }

    smpl->manufacturer = *((uint32_t *) chunk->data);
    ASC_DEBUG("read WAV smpl manufacturer %#010x", smpl->manufacturer);
    smpl->product = *((uint32_t *) (chunk->data + 4));
    ASC_DEBUG("read WAV smpl product %#010x", smpl->product);
    smpl->sample_period = *((uint32_t *) (chunk->data + 8));
    ASC_DEBUG("read WAV smpl sample period %u", smpl->sample_period);
    smpl->midi_root_note = *((uint32_t *) (chunk->data + 12));
    ASC_DEBUG("read WAV smpl MIDI root note %03u", smpl->midi_root_note);
    smpl->midi_pitch_fraction = *((uint32_t *) (chunk->data + 16));
    ASC_DEBUG("read WAV smpl MIDI pitch fraction %#010x",
              smpl->midi_pitch_fraction);
    smpl->smpte_format = *((uint32_t *) (chunk->data + 20));
    ASC_DEBUG("read WAV smpl SMPTE format %02u", smpl->smpte_format);
    smpl->smpte_offset = *((uint32_t *) (chunk->data + 24));
    ASC_DEBUG("read WAV smpl SMPTE offset %#010x", smpl->smpte_offset);
    smpl->sample_loops_count = *((uint32_t *) (chunk->data + 28));
    ASC_DEBUG("read WAV smpl sample loops count %#010x",
              smpl->sample_loops_count);
    smpl->sampler_data_size = *((uint32_t *) (chunk->data + 32));
    ASC_DEBUG("read WAV smpl sampler data size %#010x",
              smpl->sampler_data_size);

    computed_chunk_data_size = 36 + smpl->sample_loops_count*24 +
                               smpl->sampler_data_size;
    if (computed_chunk_data_size != chunk->data_size)
    {
        ASC_ERROR("WAV smpl data implies chunk data size of %u but chunk size "
                  "is %u", computed_chunk_data_size, chunk->data_size);
        goto error;
    }

    smpl->sample_loops = (asio_wav_smpl_loop_t *)
                         calloc(smpl->sample_loops_count,
                                sizeof(asio_wav_smpl_loop_t));
    if (NULL == smpl->sample_loops)
    {
        ASC_ERROR("out of memory when allocating smpl sample loops");
        goto error;
    }

    chunk_data_pointer = chunk->data + 36;
    for (i = 0;
         i < smpl->sample_loops_count;
         i++, chunk_data_pointer += sizeof(asio_wav_smpl_loop_t))
    {
        smpl->sample_loops[i].cue_point_id = *((uint32_t *)
                                               chunk_data_pointer);
        smpl->sample_loops[i].type = *((uint32_t *) (chunk_data_pointer + 4));
        smpl->sample_loops[i].start_offset = *((uint32_t *)
                                               (chunk_data_pointer + 8));
        smpl->sample_loops[i].end_offset = *((uint32_t *)
                                             (chunk_data_pointer + 12));
        smpl->sample_loops[i].fraction = *((uint32_t *)
                                           (chunk_data_pointer + 16));
        smpl->sample_loops[i].play_count = *((uint32_t *)
                                             (chunk_data_pointer + 20));
        ASC_DEBUG("read WAV smpl sample loop %#010x %#010x %#010x %#010x "
                  "%#010x %#010x", smpl->sample_loops[i].cue_point_id,
                  smpl->sample_loops[i].type,
                  smpl->sample_loops[i].start_offset,
                  smpl->sample_loops[i].end_offset,
                  smpl->sample_loops[i].fraction,
                  smpl->sample_loops[i].play_count);
    }

    smpl->sampler_data = malloc(smpl->sampler_data_size);
    if (NULL == smpl->sampler_data)
    {
        ASC_ERROR("out of memory when allocation smpl sampler data");
        goto error_free_sample_loops;
    }

    memcpy(smpl->sampler_data, chunk->data + 36 + smpl->sample_loops_count*24,
           smpl->sampler_data_size);
    ASC_DEBUG("read %u bytes of WAV smpl sampler data",
              smpl->sampler_data_size);

    return ASIO_STATUS_SUCCESS;

error_free_sample_loops:
    if (NULL != smpl->sample_loops)
        free(smpl->sample_loops);
error:
    return ASIO_STATUS_ERROR;
}

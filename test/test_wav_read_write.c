#include "AudioSamplesCore.h"
#include "AudioSamplesIO.h"
#include "ASIORiff.h"
#include "ASIOWav.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int main(int argc, char **argv)
{
    asio_riff_file_t *file;
    char *fourcc_ascii;
    int i;
    asio_wav_fmt_t *fmt;
    asio_wav_fmt_ext_t *fmt_ext;
    asio_wav_smpl_t *smpl;
    asio_wav_inst_t *inst;
    asio_riff_chunk_t *inst_chunk;

    asc_simple_logging_init();

    ASC_INFO("attempting to open simple_kick.wav in the current directory");

    file = asio_riff_file_init();
    if (NULL == file)
    {
        ASC_ERROR("failed to create RIFF file");
        goto error;
    }

    if (ASIO_STATUS_ERROR == asio_riff_file_open(file, "simple_kick.wav"))
    {
        ASC_ERROR("failed to open simple_kick.wav");
        goto error_free_file;
    }
    else
        ASC_INFO("successfully opened simple_kick.wav");

    fourcc_ascii = asio_fourcc_to_ascii(file->file_type);
    if (NULL != fourcc_ascii)
    {
        ASC_INFO("RIFF file type: '%s' (%#010x)", fourcc_ascii,
                 file->file_type);
        free(fourcc_ascii);
    }
    else
        ASC_INFO("RIFF file type: %#010x", file->file_type);

    if (ASIO_FOURCC_WAVE != file->file_type)
    {
        ASC_ERROR("RIFF file type must be WAVE");
        goto error_free_file;
    }

    ASC_INFO("listing %u chunks", file->chunk_count);
    for (i = 0; i < file->chunk_count; i++)
    {
        fourcc_ascii = asio_fourcc_to_ascii(file->chunks[i]->type);
        if (NULL != fourcc_ascii)
        {
            ASC_INFO("chunk type: '%s' (%#010x)", fourcc_ascii,
                     file->chunks[i]->type);
            free(fourcc_ascii);
        }
        else
            ASC_INFO("chunk type: %#010x", file->chunks[i]->type);

        ASC_INFO("chunk size: %u", file->chunks[i]->data_size);

        if (ASIO_FOURCC_FMT == file->chunks[i]->type)
        {
            fmt = asio_wav_fmt_init();
            if (NULL == fmt)
            {
                ASC_WARNING("failed to create WAV format");
                continue;
            }

            if (ASIO_STATUS_SUCCESS != asio_wav_fmt_unpack(fmt,
                                                           file->chunks[i]))
            {
                ASC_WARNING("failed to unpack WAV format");
                continue;
            }

            if (ASIO_WAVE_FORMAT_EXTENSIBLE == fmt->format_tag)
            {
                fmt_ext = asio_wav_fmt_ext_init();
                if (NULL == fmt_ext)
                {
                    ASC_WARNING("failed to create extensible WAV format");
                    asio_wav_fmt_free(fmt);
                    continue;
                }

                if (ASIO_STATUS_SUCCESS != asio_wav_fmt_ext_unpack(fmt_ext,
                                                                   fmt))
                    ASC_WARNING("failed to unpack extensible WAV format");

                asio_wav_fmt_ext_free(fmt_ext);
            }

            asio_wav_fmt_free(fmt);
        }

        if (ASIO_FOURCC_SMPL == file->chunks[i]->type)
        {
            smpl = asio_wav_smpl_init();
            if (NULL == smpl)
            {
                ASC_WARNING("failed to create WAV smpl");
                continue;
            }

            if (ASIO_STATUS_SUCCESS != asio_wav_smpl_unpack(smpl,
                                                            file->chunks[i]))
                ASC_WARNING("failed to unpack WAV smpl");

            asio_wav_smpl_free(smpl);
        }
    }

    inst = asio_wav_inst_init();
    if (NULL == inst)
    {
        ASC_ERROR("failed to create WAV inst");
        goto error_free_file;
    }

    inst->root_note = 40; /* E2 */
    inst->fine_tune = 0;
    inst->gain = 0;
    inst->low_note = 28; /* E1 */
    inst->high_note = 45; /* A2 */
    inst->low_velocity = 1;
    inst->high_velocity = 127;

    inst_chunk = asio_riff_chunk_init();
    if (NULL == inst_chunk)
    {
        ASC_ERROR("failed to create chunk for WAV inst");
        goto error_free_inst;
    }

    if (ASIO_STATUS_SUCCESS != asio_wav_inst_pack(inst, inst_chunk))
    {
        ASC_ERROR("failed to pack WAV inst into chunk");
        goto error_free_inst_chunk;
    }

    asio_wav_inst_free(inst);

    if (ASIO_STATUS_SUCCESS != asio_riff_file_add_chunk(file, inst_chunk))
    {
        ASC_ERROR("failed to add inst chunk to file");
        goto error_free_inst_chunk;
    }

    ASC_DEBUG("attempting to write simple_kick_copy.wav in the current "
              "directory");

    asio_riff_file_write(file, "simple_kick_copy.wav");

    asio_riff_file_free(file);

    asc_logging_end();

    return 0;

error_free_inst_chunk:
    asio_riff_chunk_free(inst_chunk);
error_free_inst:
        asio_wav_inst_free(inst);
error_free_file:
    asio_riff_file_free(file);
error:
    asc_logging_end();
    return 1;
}

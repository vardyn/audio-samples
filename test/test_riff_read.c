#include "AudioSamplesCore.h"
#include "AudioSamplesIO.h"
#include "ASIORiff.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int main(int argc, char **argv)
{
    asio_riff_file_t *file;
    char *fourcc_ascii;
    int i;

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
    }

    asio_riff_file_free(file);

    asc_logging_end();

    return 0;

error_free_file:
    asio_riff_file_free(file);
error:
    asc_logging_end();
    return 1;
}

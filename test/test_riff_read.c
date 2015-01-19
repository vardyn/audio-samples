#include "AudioSamplesCore.h"
#include "AudioSamplesIO.h"
#include "ASIORiff.h"

#include <stdio.h>
#include <string.h>

int main(int argc, char **argv)
{
    asio_riff_file_t *file;

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

    asio_riff_file_free(file);

    asc_logging_end();

    return 0;

error_free_file:
    asio_riff_file_free(file);
error:
    asc_logging_end();
    return 1;
}

#include "AudioSamplesCore.h"
#include "AudioSamplesIO.h"

#include <string.h>
#include <ao/ao.h>

int main(int argc, char **argv)
{
    struct ao_sample_format format;
    int driver_id;
    struct ao_device *device;
    unsigned char samples[45*3];

    asc_simple_logging_init();
    ASC_INFO("impulse example");
    ASC_INFO("writes impulse.wav in the current directory");

    ao_initialize();
    driver_id = ao_driver_id("wav");

    format.bits = 24;
    format.rate = 44100;
    format.channels = 1;
    format.byte_format = AO_FMT_LITTLE;
    format.matrix = NULL;
    device = ao_open_file(driver_id, "impulse.wav", 1, &format, NULL);

    memset((void *)&samples, 0, 45*3*sizeof(char));
    asio_write_wav_sample_24bit(&samples[0], (1 << 23) - 1);
    asio_write_wav_sample_24bit(&samples[3], (1 << 23) - 1);

    ao_play(device, (char *)&samples, 45*3*sizeof(char));

    ao_close(device);
    ao_shutdown();

    asc_logging_end();

    return 0;
}

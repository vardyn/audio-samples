#include "AudioSamplesCore.h"
#include "AudioSamplesIO.h"

#include <stdint.h>
#include <string.h>
#include <ao/ao.h>

uint32_t prbs13(uint32_t current_value);

int main(int argc, char **argv)
{
    struct ao_sample_format format;
    int driver_id;
    struct ao_device *device;
    uint8_t samples[8191*3];
    uint32_t i, current_value;

    asc_simple_logging_init();
    ASC_INFO("prbs13 example");
    ASC_INFO("writes prbs13.wav in the current directory");

    ao_initialize();
    driver_id = ao_driver_id("wav");

    format.bits = 24;
    format.rate = 44100;
    format.channels = 1;
    format.byte_format = AO_FMT_LITTLE;
    format.matrix = NULL;
    device = ao_open_file(driver_id, "prbs13.wav", 1, &format, NULL);

    memset((void *)&samples, 0, 8191*3*sizeof(uint8_t));
    for (i = 0, current_value = 1; i < 8191; i++)
    {
        asio_write_wav_sample_24bit(&samples[3*i], ((1 << 23) - 1)*
                                                   (2*(current_value & 1) - 1));
        current_value = prbs13(current_value);
    }

    ao_play(device, (char *)samples, 8191*3*sizeof(uint8_t));

    ao_close(device);
    ao_shutdown();

    asc_logging_end();

    return 0;
}

uint32_t prbs13(uint32_t current_value)
{
    uint32_t input_bit, i, ntaps;
    uint32_t taps[] = {13, 12, 11, 8};
    ntaps = 4;

    input_bit = ((current_value >> (taps[0] - 1)) & 1)^
                ((current_value >> (taps[1] - 1)) & 1);
    for (i = 2; i < ntaps; i++)
        input_bit ^= ((current_value >> (taps[i] - 1)) & 1);
    current_value <<= 1;
    current_value |= input_bit;
    current_value &= (1 << taps[0]) - 1;
    return current_value;
}

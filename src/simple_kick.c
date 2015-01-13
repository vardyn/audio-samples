#include "AudioSamplesCore.h"
#include "AudioSamplesIO.h"

#include <ao/ao.h>
#include <math.h>
#include <stdio.h>
#include <string.h>

int main(int argc, char **argv)
{
    struct ao_sample_format format;
    int driver_id, i;
    struct ao_device *device;
    unsigned char samples[22050*3];
    asc_func_t *envelope, *frequency, *phase, *sine;
    double sample;

    asc_simple_logging_init();
    ASC_INFO("simple kick example");
    ASC_INFO("writes simple_kick.wav in the current directory");

    ao_initialize();
    driver_id = ao_driver_id("wav");

    format.bits = 24;
    format.rate = 44100;
    format.channels = 1;
    format.byte_format = AO_FMT_LITTLE;
    format.matrix = NULL;
    device = ao_open_file(driver_id, "simple_kick.wav", 1, &format, NULL);

    memset((void *)&samples, 0, 22050*3*sizeof(char));

    envelope = asc_envelope_AD_gen(1.0, 0.001, 0.117, 0.0, 0.5, -1,
                                   44100);
    frequency = asc_constant_gen(82.41, -1, 44100);
    phase = asc_constant_gen(0.25*2.0*M_PI, -1, 44100);
    sine = asc_sine_gen(envelope, frequency, phase, -1, 44100);

    for (i = 0; i < 22050; i++)
    {
        sample = asc_func_eval(sine, i);
        asio_write_wav_sample_24bit(&samples[i*3],
                                    (int) (sample*ASC_WAV24BIT_MAX));
    }

    ao_play(device, (char *)&samples, 22050*3*sizeof(char));

    asc_func_free(phase);
    asc_func_free(frequency);
    asc_func_free(envelope);
    asc_func_free(sine);

    ao_close(device);
    ao_shutdown();

    asc_logging_end();

    return 0;
}

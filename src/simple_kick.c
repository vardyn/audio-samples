#include "AudioSamplesCore.h"

#include <ao/ao.h>
#include <math.h>
#include <stdio.h>
#include <string.h>

int main(int argc, char **argv)
{
    struct ao_sample_format format;
    int driver_id, i;
    struct ao_device *device;
    char samples[22050*3];
    double sample;
    
    asc_simple_logging_init();
    ASC_INFO("simple kick example");
    
    ao_initialize();
    driver_id = ao_driver_id("wav");

    format.bits = 24;
    format.rate = 44100;
    format.channels = 1;
    format.byte_format = AO_FMT_LITTLE;
    format.matrix = NULL;
    device = ao_open_file(driver_id, "simple_kick.wav", 1, &format, NULL);
    
    memset((void *)&samples, 0, 22050*3*sizeof(char));
    
    for (i = 0; i < 22050; i++)
    {
        sample = asc_sine(1.0, 82.41, 0.25*2.0*M_PI, i, 44100)*
                 asc_envelope_AD(1.0, 0.001, 0.117, 1.0, -1.0, i, 44100);
        asc_write_wav_sample_24bit(&samples[i*3],
                                   (int) (sample*ASC_WAV24BIT_MAX));
    }
    
    ao_play(device, (char *)&samples, 22050*3*sizeof(char));
    
    ao_close(device);
    ao_shutdown();
    
    asc_logging_end();
    
    return 0;
}

/*
 * ASCFiles.h
 * Header file for Audio Samples Files Library
 */

#ifndef ASC_FILES_H_INCLUDED
#define ASC_FILES_H_INCLUDED

#define ASC_WAV16BIT_MAX  32767
#define ASC_WAV16BIT_MIN -32767
#define ASC_WAV24BIT_MAX  8388607
#define ASC_WAV24BIT_MIN -8388607

/* WAV format helpers */
int asc_write_wav_sample_16bit(unsigned char *sample, int value);
int asc_write_wav_sample_24bit(unsigned char *sample, int value);

#endif

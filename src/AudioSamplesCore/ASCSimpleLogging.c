/*
 * ASCSimpleLogging.c
 * Audio Samples Core Simple Logging implementation
 */

#include "ASCLogging.h"
#include "ASCSimpleLogging.h"

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

void asc_simple_logging_init()
{
    asc_logging_init(asc_simple_logging_err, asc_simple_logging_err,
                     asc_simple_logging_out, asc_simple_logging_out);
}

void asc_simple_logging_err(const char *format, va_list args)
{
    vfprintf(stderr, format, args);
    fprintf(stderr, "\n");
}

void asc_simple_logging_out(const char *format, va_list args)
{
    vfprintf(stdout, format, args);
    fprintf(stdout, "\n");
}

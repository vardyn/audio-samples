/*
 * ASCLogging.c
 * Audio Samples Core Logging implementation
 */

#include "ASCLogging.h"

#include <errno.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>

void (*asc_error_func)(const char *, va_list) = NULL;
void (*asc_warning_func)(const char *, va_list) = NULL;
void (*asc_info_func)(const char *, va_list) = NULL;
void (*asc_debug_func)(const char *, va_list) = NULL;

void asc_logging_init(void (*asc_error_ptr)(const char *, va_list),
                      void (*asc_warning_ptr)(const char *, va_list),
                      void (*asc_info_ptr)(const char *, va_list),
                      void (*asc_debug_ptr)(const char *, va_list))
{
    asc_error_func = asc_error_ptr;
    asc_warning_func = asc_warning_ptr;
    asc_info_func = asc_info_ptr;
    asc_debug_func = asc_debug_ptr;
}

void asc_logging_end()
{
    asc_error_func = NULL;
    asc_warning_func = NULL;
    asc_info_func = NULL;
    asc_debug_func = NULL;
}

void asc_error(const char *format, ...)
{
    va_list args;
    if (asc_error_func)
    {
        va_start(args, format);
        asc_error_func(format, args);
        va_end(args);
    }
}

void asc_warning(const char *format, ...)
{
    va_list args;
    if (asc_warning_func)
    {
        va_start(args, format);
        asc_warning_func(format, args);
        va_end(args);
    }
}

void asc_info(const char *format, ...)
{
    va_list args;
    if (asc_info_func)
    {
        va_start(args, format);
        asc_info_func(format, args);
        va_end(args);
    }
}

void asc_debug(const char *format, ...)
{
    va_list args;
    if (asc_debug_func)
    {
        va_start(args, format);
        asc_debug_func(format, args);
        va_end(args);
    }
}

char *asc_all_strerror_r(int errnum)
{
    char *message;
    size_t size;

    size = 1024;
    message = (char *) malloc(size);
    if (NULL == message)
        return NULL;

    while (ERANGE == strerror_r(errnum, message, size))
    {
        size *= 2;
        message = realloc(message, size);
        if (NULL == message)
            return NULL;
    }

    return message;
}

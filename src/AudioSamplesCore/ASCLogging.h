/*
 * ASCLogging.h
 * Header file for Audio Samples Core Logging
 */

#ifndef ASC_LOGGING_H_INCLUDED
#define ASC_LOGGING_H_INCLUDED

#define ASC_ERROR(format, ...) asc_error("%s: " format, __func__, ##\
                                         __VA_ARGS__)

#define ASC_WARNING(format, ...) asc_warning("%s: " format, __func__,\
                                             ## __VA_ARGS__)

#define ASC_INFO(format, ...) asc_info("%s: " format, __func__, ##\
                                       __VA_ARGS__)

#define ASC_DEBUG(format, ...) asc_debug("%s: " format, __func__, ##\
                                         __VA_ARGS__)

#include <stdarg.h>

void asc_logging_init(void (*asc_error_ptr)(const char *, va_list),
                      void (*asc_warning_ptr)(const char *, va_list),
                      void (*asc_info_ptr)(const char *, va_list),
                      void (*asc_debug_ptr)(const char *, va_list));
void asc_logging_end();

void asc_error(const char *, ...)
    __attribute__((format(printf, 1, 2)));

void asc_warning(const char *, ...)
    __attribute__((format(printf, 1, 2)));

void asc_info(const char *, ...)
    __attribute__((format(printf, 1, 2)));

void asc_debug(const char *, ...)
    __attribute__((format(printf, 1, 2)));

#endif

/*
 * ASCSimpleLogging.h
 * Audio Samples Core Simple Logging header
 */

#ifndef ASC_SIMPLE_LOGGING_H_INCLUDED
#define ASC_SIMPLE_LOGGING_H_INCLUDED

#include <stdarg.h>

void asc_simple_logging_init();

void asc_simple_logging_err(const char *, va_list);
void asc_simple_logging_out(const char *, va_list);

#endif

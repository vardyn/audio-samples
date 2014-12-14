/*
 * ASCFunctions.c
 * Audio Samples Core function objects implementation
 */

#include "AudioSamplesCore.h"

#include <stddef.h>
#include <stdlib.h>

asc_func_t *asc_func_new(int sample_rate)
{
    asc_func_t *func;

    func = (asc_func_t *) malloc(sizeof(asc_func_t));

    if (NULL == func)
        ASC_ERROR("out of memory");

    func->current_value = 0.0;
    func->current_sample = 0;
    func->sample_rate = sample_rate;
    func->function = NULL;
    func->state = NULL;

    return func;
}

double asc_func_eval(asc_func_t *func, int sample)
{
    if (func->current_sample != sample)
        func->function(func, sample);

    return func->current_value;
}

void asc_func_free(asc_func_t *func)
{
    if (NULL == func)
    {
        ASC_ERROR("asc_func_free called on null pointer");
        return;
    }

    if (NULL != func->state)
        free(func->state);

    free(func);
}

/*
    Echoizer

    Copyright (C) 2017 Inaudible Effects
    Copyright (c) 2017 Cyriaque 'Cisoun' Skrapits

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program. If not, see <http://www.gnu.org/licenses/>.
*/


#include <math.h>
#include <stdlib.h>
#include "lv2/lv2plug.in/ns/lv2core/lv2.h"


#define PLUGIN_URI "http://inaudibleeffects.github.io/echoizer"
#define TIME_MAX 2000 // 2000 ms


typedef enum {
    ECHOIZER_INPUT      = 0,
    ECHOIZER_OUTPUT     = 1,
    ECHOIZER_TIME       = 2,
    ECHOIZER_FEEDBACK   = 3,
    ECHOIZER_BLEND      = 4
} PortIndex;

typedef struct {
    // Port buffers
    const float*    time;
    const float*    feedback;
    const float*    blend;
    const float*    input;
    float*          output;
} Echoizer;


float* buffer;
size_t buffer_size;
double sample_rate;
unsigned int sample;


static LV2_Handle
instantiate(const LV2_Descriptor*       descriptor,
            double                      rate,
            const char*                 bundle_path,
            const LV2_Feature* const*   features)
{
    Echoizer* echoizer = (Echoizer*)calloc(1, sizeof(Echoizer));

    // Store rate and put initial sample of buffer at 0.
    sample = 0;
    sample_rate = rate;

    // Create buffer.
    buffer_size = (int)(TIME_MAX / 1000 * (int)rate);
    buffer = (float*)calloc(buffer_size, sizeof(float));

    return (LV2_Handle)echoizer;
}

static void
connect_port(LV2_Handle instance,
             uint32_t   port,
             void*      data)
{
    Echoizer* echoizer = (Echoizer*)instance;

    switch ((PortIndex)port)
    {
        case ECHOIZER_INPUT:
            echoizer->input = (float*)data;
            break;
        case ECHOIZER_OUTPUT:
            echoizer->output = (float*)data;
            break;
        case ECHOIZER_TIME:
            echoizer->time = (const float*)data;
            break;
        case ECHOIZER_FEEDBACK:
            echoizer->feedback = (const float*)data;
            break;
        case ECHOIZER_BLEND:
            echoizer->blend = (const float*)data;
            break;
    }
}

static void
activate(LV2_Handle instance)
{
}

static void
run(LV2_Handle instance, uint32_t samples)
{
    const Echoizer* echoizer = (const Echoizer*)instance;

    const float*        input       = echoizer->input;
    float*              output      = echoizer->output;
    const float         time        = *(echoizer->time);
    const float         feedback    = *(echoizer->feedback);
    const float         blend       = *(echoizer->blend);

    const int           limit       = (int)(time / TIME_MAX * buffer_size);

    for (uint32_t pos = 0; pos < samples; pos++)
    {
        // Compute output from buffer and feedback.
        // Weird stuff happens if you increase the time value.
        const float input_blend = fminf(1.f - blend, 0.5f) / 0.5f * input[pos];
        const float delay_blend = fminf(blend, 0.5f) / 0.5f * buffer[sample] * feedback;
        output[pos] = input_blend + delay_blend;
        buffer[sample] = input[pos] + buffer[sample] * feedback;

        sample++;
        if (sample >= limit) // Limit reached ?
            sample = 0;
    }
}

static void
deactivate(LV2_Handle instance)
{
}

static void
cleanup(LV2_Handle instance)
{
    free(instance);
    free(buffer);
}

static const void*
extension_data(const char* uri)
{
    return NULL;
}

static const LV2_Descriptor descriptor = {
    PLUGIN_URI,
    instantiate,
    connect_port,
    activate,
    run,
    deactivate,
    cleanup,
    extension_data
};

LV2_SYMBOL_EXPORT
const LV2_Descriptor*
lv2_descriptor(uint32_t index)
{
    switch (index) {
        case 0:  return &descriptor;
        default: return NULL;
    }
}

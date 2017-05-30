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
#include <stdint.h>

#include "lv2/lv2plug.in/ns/ext/atom/forge.h"
#include "lv2/lv2plug.in/ns/ext/atom/util.h"
#include "lv2/lv2plug.in/ns/ext/log/log.h"
#include "lv2/lv2plug.in/ns/ext/log/logger.h"
#include "lv2/lv2plug.in/ns/ext/midi/midi.h"
#include "lv2/lv2plug.in/ns/ext/patch/patch.h"
#include "lv2/lv2plug.in/ns/ext/state/state.h"
#include "lv2/lv2plug.in/ns/ext/worker/worker.h"
#include "lv2/lv2plug.in/ns/lv2core/lv2.h"

#include "echoizer.h"

#define ECHOIZER_URI "http://inaudibleeffects.github.io/echoizer"
#define TIME_MAX 2000 // 2000 ms

typedef struct {
    // Port buffers
    const float*                input;
    float*                      output;

    // Instantiation settings
    double                      rate;
    uint32_t                    sample;
    float*                      buffer;
    size_t                      buffer_size;
    float*                      delay;
    float*                      feedback;
    float*                      blend;

    // UI state
    bool                        ui_active;
    bool                        send_settings_to_ui;
} Echoizer;

static LV2_Handle
instantiate(const LV2_Descriptor*       descriptor,
            double                      rate,
            const char*                 bundle_path,
            const LV2_Feature* const*   features)
{
    printf("Echoizer::instantiate...\n");

    Echoizer* self = (Echoizer*)calloc(1, sizeof(Echoizer));
    if (!self)
        return NULL;

    // Store rate and put initial sample of buffer at 0.
    self->sample = 0;
    self->rate = rate;

    // Create buffer.
    self->buffer_size = (int)(TIME_MAX / 1000 * (int)rate);
    self->buffer = (float*)calloc(self->buffer_size, sizeof(float));
    if (!self->buffer) {
        return NULL;
    }

    return (LV2_Handle)self;
}

static void
connect_port(LV2_Handle handle,
             uint32_t   port,
             void*      data)
{
    Echoizer* echoizer = (Echoizer*)handle;

    switch ((PortIndex)port)
    {
        case INPUT:
            echoizer->input = (float*)data;
            break;
        case OUTPUT:
            echoizer->output = (float*)data;
            break;
        case DELAY:
            echoizer->delay = (float*)data;
            break;
        case FEEDBACK:
            echoizer->feedback = (float*)data;
            break;
        case BLEND:
            echoizer->blend = (float*)data;
            break;
    }
}

static void
activate(LV2_Handle instance)
{
    printf("Echoizer::activate...\n");
}

static void
run(LV2_Handle handle, uint32_t samples)
{
    Echoizer* self      = (Echoizer*)handle;

    const float*input   = self->input;
    float* output       = self->output;

    const int limit     = (int)(*self->delay / TIME_MAX * self->buffer_size);

    // Process audio data
    for (uint32_t pos = 0; pos < samples; pos++)
    {
        // Compute output from buffer and feedback.
        // Weird stuff happens if you increase the time value.
        const float input_blend = fminf(1.f - *self->blend, 0.5f) / 0.5f * input[pos];
        const float delay_blend = fminf(*self->blend, 0.5f) / 0.5f * self->buffer[self->sample] * *self->feedback;
        output[pos] = input_blend + delay_blend;
        self->buffer[self->sample] = input[pos] + self->buffer[self->sample] * *self->feedback;

        self->sample++;
        if (self->sample >= limit) // Limit reached ?
            self->sample = 0;
    }
}

static LV2_State_Status
state_save(LV2_Handle                instance,
           LV2_State_Store_Function  store,
           LV2_State_Handle          handle,
           uint32_t                  flags,
           const LV2_Feature* const* features)
{
    /*Echoizer* self = (Echoizer*)instance;

    if (!self) {
        return LV2_STATE_SUCCESS;
    }

    store(handle,
          self->uris.ui_Delay,
          (void*)&self->time, sizeof(float),
          self->uris.atom_Float,
          LV2_STATE_IS_POD);

    store(handle,
          self->uris.ui_Feedback,
          (void*)&self->feedback, sizeof(float),
          self->uris.atom_Float,
          LV2_STATE_IS_POD);

    store(handle,
          self->uris.ui_Blend,
          (void*)&self->blend, sizeof(float),
          self->uris.atom_Float,
          LV2_STATE_IS_POD);*/

    return LV2_STATE_SUCCESS;
}

static LV2_State_Status
state_restore(LV2_Handle                  instance,
              LV2_State_Retrieve_Function retrieve,
              LV2_State_Handle            handle,
              uint32_t                    flags,
              const LV2_Feature* const*   features)
{
    /*Echoizer* self = (Echoizer*)instance;

    size_t   size;
    uint32_t type;
    uint32_t valflags;

    const void* delay = retrieve(handle, self->uris.ui_Delay, &size, &type, &valflags);
    const void* feedback = retrieve(handle, self->uris.ui_Feedback, &size, &type, &valflags);
    const void* blend = retrieve(handle, self->uris.ui_Blend, &size, &type, &valflags);

    if (delay && size == sizeof(float) && type == self->uris.atom_Float)
    {
        self->time                = *((const float*)delay);
        self->send_settings_to_ui = true;
    }

    if (feedback && size == sizeof(float) && type == self->uris.atom_Float)
    {
        self->feedback            = *((const float*)feedback);
        self->send_settings_to_ui = true;
    }

    if (blend && size == sizeof(float) && type == self->uris.atom_Float)
    {
        self->blend               = *((const float*)blend);
        self->send_settings_to_ui = true;
    }*/

    return LV2_STATE_SUCCESS;
}

static const void*
extension_data(const char* uri)
{
    printf("Echoizer::extension_data...\n");

    static const LV2_State_Interface state = { state_save, state_restore };
    if (!strcmp(uri, LV2_STATE__interface)) {
        return &state;
    }
    return NULL;
}

static void
deactivate(LV2_Handle handle)
{
    printf("Echoizer::deactivate...\n");
}

static void
cleanup(LV2_Handle handle)
{
    Echoizer* self = (Echoizer*)handle;
    free(self->buffer);
    free(self);
}


static const LV2_Descriptor descriptor = {
    ECHOIZER_URI,
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

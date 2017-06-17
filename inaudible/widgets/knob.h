#ifndef __INAUDIBLE_WIDGET_KNOB_H__
#define __INAUDIBLE_WIDGET_KNOB_H__

#include <stdbool.h>

#include "pixbuf.h"
#include "widget.h"

typedef struct
{
    int number_of_tiles;
    int size;
    float max;
    float min;
    float value;

    InaudiblePixbuf* tiles;

} InaudibleKnob;

InaudibleWidget* inaudible_knob_new(InaudiblePixbuf* tiles);
void             inaudible_knob_destroy(void* knob);

void             inaudible_knob_draw(void* knob, cairo_t* context);
bool             inaudible_knob_set_tiles(InaudibleKnob* knob, InaudiblePixbuf* tiles);
void             inaudible_knob_set_value(InaudibleKnob* knob, int value);

#endif

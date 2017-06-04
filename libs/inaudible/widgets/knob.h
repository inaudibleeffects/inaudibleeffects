#ifndef __INAUDIBLE_WIDGET_KNOB_H__
#define __INAUDIBLE_WIDGET_KNOB_H__

#include <stdbool.h>

#include "inaudible/widget.h"

typedef struct
{
    InaudibleWidget* parent;

    int number_of_tiles;
    int size;
    float max;
    float min;
    float value;

    InaudiblePixbuf* tiles;

} InaudibleKnob;

InaudibleKnob*  inaudible_knob_new(InaudiblePixbuf* tiles);
void            inaudible_knob_destroy(InaudibleKnob* knob);
void            inaudible_knob_draw(InaudibleKnob* knob, cairo_t* context);
bool            inaudible_knob_set_tiles(InaudibleKnob* knob, InaudiblePixbuf* tiles);

#endif

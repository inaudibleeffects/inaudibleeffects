#ifndef __INAUDIBLE_WIDGET_KNOB_H__
#define __INAUDIBLE_WIDGET_KNOB_H__


#include <stdbool.h>
#include "pixbuf.h"
#include "widget.h"


#define INAUDIBLE_KNOB(obj) (INAUDIBLE_FROM_WIDGET(obj))


typedef struct InaudibleKnob InaudibleKnob;


InaudibleWidget* inaudible_knob_new       (InaudiblePixbuf* tiles);
void             inaudible_knob_destroy   (InaudibleWidget* widget);

void             inaudible_knob_draw      (InaudibleWidget* knob,
                                           cairo_t** context);
float            inaudible_knob_get_value (InaudibleKnob* knob);
bool             inaudible_knob_set_tiles (InaudibleKnob* knob,
                                           InaudiblePixbuf* tiles);
void             inaudible_knob_set_value (InaudibleKnob* knob,
                                           float value);


#endif

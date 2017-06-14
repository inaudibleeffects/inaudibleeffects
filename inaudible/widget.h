#ifndef __INAUDIBLE_WIDGET_H__
#define __INAUDIBLE_WIDGET_H__

#include <cairo/cairo.h>
#include "macros.h"

typedef struct {
    int x;
    int y;
    void* child;
    void (*draw)(void* widget, cairo_t* context);
} InaudibleWidget;

#endif

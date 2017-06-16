#ifndef __INAUDIBLE_WIDGET_H__
#define __INAUDIBLE_WIDGET_H__

#include <cairo/cairo.h>
#include "macros.h"

typedef struct widget {
    int x;
    int y;
    void* child;
    void (*destroy)(void* widget);
    void (*draw)(void* widget, cairo_t* context);
} InaudibleWidget;

void inaudible_widget_destroy(InaudibleWidget* widget);

#endif

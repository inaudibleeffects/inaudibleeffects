#ifndef __INAUDIBLE_WIDGET_H__
#define __INAUDIBLE_WIDGET_H__

#include "drawable.h"
#include "macros.h"
#include "pixbuf.h"

typedef struct {
    void* child;
    void (*draw)(void* widget, cairo_t* context);
    int x;
    int y;
} InaudibleWidget;

#endif

#ifndef __INAUDIBLE_WIDGET_IMAGE_H__
#define __INAUDIBLE_WIDGET_IMAGE_H__

#include "pixbuf.h"
#include "widget.h"

typedef struct
{
    InaudiblePixbuf* pixbuf;
} InaudibleImage;

InaudibleWidget* inaudible_image_new(InaudiblePixbuf* pixbuf);
void             inaudible_image_destroy(void* widget);

void             inaudible_image_draw(void* widget, cairo_t* context);

#endif

#ifndef __INAUDIBLE_WIDGET_IMAGE_H__
#define __INAUDIBLE_WIDGET_IMAGE_H__

#include "pixbuf.h"
#include "widget.h"

typedef struct
{
    InaudibleWidget* parent;
    InaudiblePixbuf* pixbuf;
} InaudibleImage;

InaudibleImage* inaudible_image_new(InaudiblePixbuf* pixbuf);
void            inaudible_image_destroy(InaudibleImage* image);

void            inaudible_image_draw(void* widget, cairo_t* context);

#endif
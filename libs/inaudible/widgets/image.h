#ifndef __INAUDIBLE_WIDGET_IMAGE_H__
#define __INAUDIBLE_WIDGET_IMAGE_H__

#include "inaudible/widget.h"

typedef struct
{
    InaudiblePixbuf* pixbuf;
} InaudibleImage;

InaudibleImage* inaudible_image_new(const char* stream, const size_t size);
void            inaudible_image_destroy(InaudibleImage* image);

void            inaudible_image_draw(InaudibleImage* image, cairo_t* context);

#endif

#ifndef __INAUDIBLE_WIDGET_IMAGE_H__
#define __INAUDIBLE_WIDGET_IMAGE_H__


#include "pixbuf.h"
#include "widget.h"


#define INAUDIBLE_IMAGE(obj) (INAUDIBLE_FROM_WIDGET(obj))


typedef struct InaudibleImage InaudibleImage;


InaudibleWidget* inaudible_image_new     (InaudiblePixbuf* pixbuf);
void             inaudible_image_destroy (InaudibleWidget* widget);

void             inaudible_image_draw    (InaudibleWidget* widget,
                                          cairo_t**        context);


#endif

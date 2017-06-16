#include "widgets/image.h"
#include <stdio.h>

InaudibleWidget*
inaudible_image_new(InaudiblePixbuf* pixbuf)
{
    InaudibleWidget* widget = INAUDIBLE_NEW(InaudibleWidget);
    InaudibleImage* image = INAUDIBLE_NEW(InaudibleImage);

    image->pixbuf = pixbuf;

    widget->x = 0;
    widget->y = 0;
    widget->child = image;

    widget->destroy = &inaudible_image_destroy;
    widget->draw = &inaudible_image_draw;

    return widget;
}

void
inaudible_image_destroy(void* widget)
{
    InaudibleWidget* w = widget;
    INAUDIBLE_DESTROY(w->child);
    INAUDIBLE_DESTROY(w);
}

void
inaudible_image_draw(void* widget, cairo_t* context)
{
    InaudibleWidget* base = widget; // TODO : Refactor
    InaudibleImage* image = base->child;

    cairo_set_source_surface(context, image->pixbuf->surface, base->x, base->y);
    cairo_paint(context);
}

#include "ui/widgets/image.h"
#include <stdio.h>


struct InaudibleImage
{
    InaudiblePixbuf* pixbuf;
};


InaudibleWidget*
inaudible_image_new(InaudiblePixbuf* pixbuf)
{
    InaudibleWidget* widget = INAUDIBLE_NEW(InaudibleWidget);
    InaudibleImage* image = INAUDIBLE_NEW(InaudibleImage);

    // Private

    image->pixbuf = pixbuf;

    // Widget

    widget->x = 0;
    widget->y = 0;
    widget->child = image;

    widget->destroy = &inaudible_image_destroy;
    widget->draw = &inaudible_image_draw;

    return widget;
}

void
inaudible_image_destroy(InaudibleWidget* widget)
{
    INAUDIBLE_DESTROY(INAUDIBLE_IMAGE(widget));
}

void
inaudible_image_draw(InaudibleWidget* widget,
                     cairo_t**        context)
{
    cairo_t* ctx = *context;
    InaudibleImage* image = widget->child;

    cairo_set_source_surface(
        ctx,
        inaudible_pixbuf_get_surface(image->pixbuf),
        widget->x,
        widget->y
    );
    cairo_paint(ctx);
}

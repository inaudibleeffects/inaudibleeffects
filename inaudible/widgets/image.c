#include "widgets/image.h"

InaudibleImage*
inaudible_image_new(InaudiblePixbuf* pixbuf)
{
    InaudibleImage* image = INAUDIBLE_NEW(InaudibleImage);
    image->pixbuf = pixbuf;

    image->parent = INAUDIBLE_NEW(InaudibleWidget);
    image->parent->x = 0;
    image->parent->y = 0;
    image->parent->child = image;
    image->parent->draw = &inaudible_image_draw;
}

void
inaudible_image_destroy(InaudibleImage* image)
{
    //INAUDIBLE_DESTROY(image->pixbuf);
    INAUDIBLE_DESTROY(image);
}

void
inaudible_image_draw(void* widget, cairo_t* context)
{
    InaudibleImage* image = widget;
    cairo_set_source_surface(context, image->pixbuf->surface, 0, 0);
    cairo_paint(context);
}

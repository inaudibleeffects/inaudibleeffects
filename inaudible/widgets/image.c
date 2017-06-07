#include "widgets/image.h"

InaudibleImage*
inaudible_image_new(InaudiblePixbuf* pixbuf)
{
    InaudibleImage* image = INAUDIBLE_NEW(InaudibleImage);
    image->pixbuf = pixbuf;
}

void
inaudible_image_destroy(InaudibleImage* image)
{
    //INAUDIBLE_DESTROY(image->pixbuf);
    INAUDIBLE_DESTROY(image);
}

void
inaudible_image_draw(InaudibleImage* image, cairo_t* context)
{
    cairo_set_source_surface(context, image->pixbuf->surface, 0, 0);
    cairo_paint(context);
}

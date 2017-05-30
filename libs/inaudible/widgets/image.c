#include "inaudible/widgets/image.h"

InaudibleImage*
inaudible_image_new(const char* stream, const size_t size)
{
    InaudibleImage* image = INAUDIBLE_NEW(InaudibleImage);
    image->pixbuf = inaudible_pixbuf_new(stream, size);
}

void
inaudible_image_destroy(InaudibleImage* image)
{
    INAUDIBLE_DESTROY(image->pixbuf);
    INAUDIBLE_DESTROY(image);
}

void
inaudible_image_draw(InaudibleImage* image, cairo_t* context)
{
    cairo_set_source_surface(context, image->pixbuf->surface, 0, 0);
    cairo_paint(context);
}

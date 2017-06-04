#include "inaudible/pixbuf.h"

InaudiblePixbuf*
inaudible_pixbuf_new(const char* data, size_t size)
{
    InaudiblePixbuf* pixbuf = INAUDIBLE_NEW(InaudiblePixbuf);

    // Load datas into surface.
    InaudiblePngStream stream;
    stream.data = data;
    stream.size = size;
    stream.pos = 0;
    pixbuf->surface = cairo_image_surface_create_from_png_stream(&inaudible_pixbuf_read, &stream);
}

void
inaudible_pixbuf_destroy(InaudiblePixbuf* pixbuf)
{
    cairo_surface_destroy(pixbuf->surface);
    free(pixbuf);
    pixbuf = NULL;
}

int
inaudible_pixbuf_get_height(InaudiblePixbuf* pixbuf)
{
    return cairo_image_surface_get_height(pixbuf->surface);
}

cairo_surface_t*
inaudible_pixbuf_get_surface(InaudiblePixbuf* pixbuf)
{
    return pixbuf->surface;
}

int
inaudible_pixbuf_get_width(InaudiblePixbuf* pixbuf)
{
    return cairo_image_surface_get_width(pixbuf->surface);
}

static cairo_status_t
inaudible_pixbuf_read(void* closure, unsigned char* data, unsigned int length)
{
    InaudiblePngStream* stream = (InaudiblePngStream*)closure;

    if ((stream->pos + length) > (stream->size))
        return CAIRO_STATUS_READ_ERROR;

    memcpy(data, (stream->data + stream->pos), length);
    stream->pos += length;

    return CAIRO_STATUS_SUCCESS;
}

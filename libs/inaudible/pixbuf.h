#ifndef __INAUDIBLE_PIXBUF_H__
#define __INAUDIBLE_PIXBUF_H__

#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include <cairo/cairo.h>

#include "inaudible/macros.h"

typedef struct
{
    cairo_surface_t* surface;
} InaudiblePixbuf;

typedef struct
{
    const char *data;
    unsigned int size;
    unsigned int pos;
} InaudiblePngStream;


InaudiblePixbuf*        inaudible_pixbuf_new(const char* data, size_t size);
void                    inaudible_pixbuf_destroy(InaudiblePixbuf* pixbuf);
int                     inaudible_pixbuf_get_height(InaudiblePixbuf* pixbuf);
cairo_surface_t*        inaudible_pixbuf_get_surface(InaudiblePixbuf* pixbuf);
int                     inaudible_pixbuf_get_width(InaudiblePixbuf* pixbuf);
static cairo_status_t   inaudible_pixbuf_read(void* closure, unsigned char* data, unsigned int length);

#endif

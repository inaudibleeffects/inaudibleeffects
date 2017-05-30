#include "inaudible/macros.h"
#include "inaudible/pixbuf.h"

typedef struct
{
    InaudiblePixbuf* pixbuf;
} InaudibleImage;

InaudibleImage* inaudible_image_new(const char* stream, const size_t size);
void            inaudible_image_destroy(InaudibleImage* image);

void            inaudible_image_draw(InaudibleImage* image, cairo_t* context);

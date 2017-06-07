#include "widgets/knob.h"

InaudibleKnob*
inaudible_knob_new(InaudiblePixbuf* tiles)
{
    InaudibleKnob* knob = INAUDIBLE_NEW(InaudibleKnob);
    //knob->base = INAUDIBLE_NEW(InaudibleDrawable);

    knob->min = 0.0f;
    knob->max = 1.0f;
    knob->value = 0.0f;

    inaudible_knob_set_tiles(knob, tiles);

    return knob;
}

void
inaudible_knob_destroy(InaudibleKnob* knob)
{
    //inaudible_pixbuf_destroy(knob->tiles);
    //INAUDIBLE_DESTROY(knob.base);
    INAUDIBLE_DESTROY(knob);
}

void
inaudible_knob_draw(InaudibleKnob* knob, cairo_t* context)
{
    int height = inaudible_pixbuf_get_height(knob->tiles);
    int width = inaudible_pixbuf_get_width(knob->tiles);

    printf("Height : %d ; Width : %d\n", height, width);

    cairo_set_source_surface(context, inaudible_pixbuf_get_surface(knob->tiles), 0, 0);
    cairo_rectangle(context, 0, 0, knob->size, knob->size);
    cairo_fill(context);
}

bool
inaudible_knob_set_tiles(InaudibleKnob* knob, InaudiblePixbuf* pixbuf)
{
    int height = inaudible_pixbuf_get_height(pixbuf);
    int width = inaudible_pixbuf_get_width(pixbuf);

    printf("Height : %d ; Width : %d\n", height, width);

    if (height % width > 0)
        return false;

    knob->number_of_tiles = height / width;
    knob->size = width;
    knob->tiles = pixbuf;

    return true;
}

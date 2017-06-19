#include "widgets/knob.h"

static void inaudible_knob_on_button_press(InaudibleWidget* widget,
                                           const PuglEventButton* event);

static void inaudible_knob_on_button_release(InaudibleWidget* widget,
                                             const PuglEventButton* event);

static void inaudible_knob_on_mouse_move(InaudibleWidget* widget,
                                         const PuglEventMotion* event);

InaudibleWidget*
inaudible_knob_new(InaudiblePixbuf* tiles)
{
    InaudibleWidget* widget = INAUDIBLE_NEW(InaudibleWidget);
    InaudibleKnob* knob = INAUDIBLE_NEW(InaudibleKnob);

    knob->min = 0.0f;
    knob->max = 1.0f;
    knob->value = 0.0f;

    inaudible_knob_set_tiles(knob, tiles);

    widget->x = 0;
    widget->y = 0;
    widget->height = knob->size;
    widget->width = knob->size;
    widget->child = knob;

    widget->destroy = &inaudible_knob_destroy;
    widget->draw = &inaudible_knob_draw;
    widget->on_button_press = &inaudible_knob_on_button_press;
    widget->on_button_release = &inaudible_knob_on_button_release;
    widget->on_mouse_move = &inaudible_knob_on_mouse_move;

    return widget;
}

void
inaudible_knob_destroy(void* widget)
{
    InaudibleWidget* w = widget;
    INAUDIBLE_DESTROY(w->child);
    INAUDIBLE_DESTROY(w);
}

void
inaudible_knob_draw(void* widget, cairo_t* context)
{
    InaudibleWidget* w = widget;
    InaudibleKnob* knob = w->child;

    int height = inaudible_pixbuf_get_height(knob->tiles);
    int width = inaudible_pixbuf_get_width(knob->tiles);

    cairo_set_source_surface(context,
        inaudible_pixbuf_get_surface(knob->tiles),
        w->x,
        w->y - knob->value * knob->size);
    cairo_rectangle(context, w->x, w->y, knob->size, knob->size);
}

static void
inaudible_knob_on_button_press(InaudibleWidget* widget,
                               const PuglEventButton* event)
{
    widget->has_grab = true;
}

static void
inaudible_knob_on_button_release(InaudibleWidget* widget,
                               const PuglEventButton* event)
{
    printf("SORTIE\n");
    widget->has_grab = false;
}

static void
inaudible_knob_on_mouse_move(InaudibleWidget* widget,
                             const PuglEventMotion* event)
{
    widget->has_focus = true;

    InaudibleKnob* knob = widget->child;
    if (widget->has_grab)
        inaudible_knob_set_value(knob, event->y);
}

bool
inaudible_knob_set_tiles(InaudibleKnob* knob, InaudiblePixbuf* pixbuf)
{
    int height = inaudible_pixbuf_get_height(pixbuf);
    int width = inaudible_pixbuf_get_width(pixbuf);

    if (height % width > 0)
        return false;

    knob->number_of_tiles = height / width;
    knob->size = width;
    knob->tiles = pixbuf;

    return true;
}

void
inaudible_knob_set_value(InaudibleKnob* knob, int value)
{
    //printf("SET VALUE : %d\n", value);
    knob->value = (float)value;
}

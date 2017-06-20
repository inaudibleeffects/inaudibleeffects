#include "widgets/knob.h"
#include <math.h>


static int   last_y;
static float last_value;


struct InaudibleKnob
{
    int number_of_tiles;
    int size;
    float max;
    float min;
    float value;
    float range;

    InaudiblePixbuf* tiles;
};


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
    knob->range = knob->max - knob->min;

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
inaudible_knob_destroy(InaudibleWidget* widget)
{
    INAUDIBLE_DESTROY(INAUDIBLE_KNOB(widget));
}

void
inaudible_knob_draw(InaudibleWidget* widget, cairo_t** context)
{
    cairo_t* ctx = *context;
    InaudibleKnob* knob = INAUDIBLE_KNOB(widget);

    // Compute current frame index.
    float ratio = (knob->value - knob->min) / (knob->max - knob->min);
    int frame = (int)(ratio * (knob->number_of_tiles - 1));

    // Draw frame from surface.
    cairo_set_source_surface(ctx,
        inaudible_pixbuf_get_surface(knob->tiles),
        widget->x,
        widget->y - frame * knob->size);
    cairo_rectangle(ctx, widget->x, widget->y, knob->size, knob->size);
    cairo_fill(ctx);
}

float
inaudible_knob_get_value(InaudibleKnob* knob)
{
    return knob->value;
}

static void
inaudible_knob_on_button_press(InaudibleWidget* widget,
                               const PuglEventButton* event)
{
    InaudibleKnob* knob = INAUDIBLE_FROM_WIDGET(widget);
    last_y = event->y;
    last_value = knob->value;

    widget->has_grab = true;
}

static void
inaudible_knob_on_button_release(InaudibleWidget* widget,
                               const PuglEventButton* event)
{
    widget->has_grab = false;
}

static void
inaudible_knob_on_mouse_move(InaudibleWidget* widget,
                             const PuglEventMotion* event)
{
    widget->has_focus = true;

    InaudibleKnob* knob = INAUDIBLE_FROM_WIDGET(widget);

    float range = (knob->max - knob->min) / 300;

    if (widget->has_grab)
    {
        inaudible_knob_set_value(
            knob,
            last_value + (last_y - event->y) * range
        );
    }
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
inaudible_knob_set_value(InaudibleKnob* knob, float value)
{
    if (value < knob->min)
        value = knob->min;
    else if (value > knob->max)
        value = knob->max;

    knob->value = value;
}

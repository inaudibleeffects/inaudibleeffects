#include "ui/widgets/knob.h"
#include <math.h>


static int   last_y;
static float last_value;


struct _InaudibleKnobPrivate
{
    int number_of_tiles;
    int size;
    float max;
    float min;
    float value;
    float range;

    InaudiblePixbuf* tiles;
};



static void
inaudible_knob_on_button_press(InaudibleWidget*       widget,
                               const PuglEventButton* event);

static void
inaudible_knob_on_button_release(InaudibleWidget*       widget,
                                 const PuglEventButton* event);

static void
inaudible_knob_on_mouse_move(InaudibleWidget*       widget,
                             const PuglEventMotion* event);


InaudibleWidget*
inaudible_knob_new(InaudiblePixbuf* tiles)
{
    INAUDIBLE_DECLARE(InaudibleKnob, knob);

    InaudibleWidget* widget = INAUDIBLE_NEW(InaudibleWidget);

    // Private

    private->min = 0.0f;
    private->max = 1.0f;
    private->value = 0.0f;
    private->range = private->max - private->min;

    inaudible_knob_set_tiles(knob, tiles);

    // Widget

    widget->x = 0;
    widget->y = 0;
    widget->height = private->size;
    widget->width = private->size;
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
    InaudibleKnob* knob = INAUDIBLE_KNOB(widget);
    InaudibleKnobPrivate* private = INAUDIBLE_PRIVATE(knob);
    inaudible_widget_destroy(widget);
    inaudible_pixbuf_destroy(private->tiles);
    INAUDIBLE_DESTROY(private);
    INAUDIBLE_DESTROY(knob);
}

void
inaudible_knob_draw(InaudibleWidget* widget,
                    cairo_t**        context)
{
    cairo_t* ctx = *context;
    InaudibleKnob* knob = INAUDIBLE_KNOB(widget);
    InaudibleKnobPrivate* private = INAUDIBLE_PRIVATE(knob);

    // Compute current frame index.
    float ratio = (private->value - private->min) / (private->max - private->min);
    int frame = (int)(ratio * (private->number_of_tiles - 1));

    // Draw frame from surface.
    cairo_set_source_surface(ctx,
        inaudible_pixbuf_get_surface(private->tiles),
        widget->x,
        widget->y - frame * private->size);
    cairo_rectangle(ctx, widget->x, widget->y, private->size, private->size);
    cairo_fill(ctx);
}

float
inaudible_knob_get_value(InaudibleKnob* knob)
{
    InaudibleKnobPrivate* private = INAUDIBLE_PRIVATE(knob);
    return private->value;
}

static void
inaudible_knob_on_button_press(InaudibleWidget*       widget,
                               const PuglEventButton* event)
{
    InaudibleKnob* knob = INAUDIBLE_FROM_WIDGET(widget);
    InaudibleKnobPrivate* private = INAUDIBLE_PRIVATE(knob);
    last_y = event->y;
    last_value = private->value;

    widget->has_grab = true;
}

static void
inaudible_knob_on_button_release(InaudibleWidget*     widget,
                               const PuglEventButton* event)
{
    widget->has_grab = false;
}

static void
inaudible_knob_on_mouse_move(InaudibleWidget*       widget,
                             const PuglEventMotion* event)
{
    widget->has_focus = true;

    InaudibleKnob* knob = INAUDIBLE_FROM_WIDGET(widget);
    InaudibleKnobPrivate* private = INAUDIBLE_PRIVATE(knob);

    float range = (private->max - private->min) / 300;

    if (widget->has_grab)
    {
        inaudible_knob_set_value(
            knob,
            last_value + (last_y - event->y) * range
        );
    }
}

bool
inaudible_knob_set_tiles(InaudibleKnob*   knob,
                         InaudiblePixbuf* pixbuf)
{
    InaudibleKnobPrivate* private = INAUDIBLE_PRIVATE(knob);

    int height = inaudible_pixbuf_get_height(pixbuf);
    int width = inaudible_pixbuf_get_width(pixbuf);

    if (height % width > 0)
        return false;

    private->number_of_tiles = height / width;
    private->size = width;
    private->tiles = pixbuf;

    return true;
}

void
inaudible_knob_set_value(InaudibleKnob* knob,
                         float          value)
{
    InaudibleKnobPrivate* private = INAUDIBLE_PRIVATE(knob);

    if (value < private->min)
        value = private->min;
    else if (value > private->max)
        value = private->max;

    private->value = value;

    if (knob->value_changed)
        knob->value_changed(knob, value);
}

#include "knob.h"
#include <cairo.h>

struct _InaudibleKnobPrivate
{
    gint cursor_size;
    gint size;
    gdouble dead_angle;
    gdouble mouse_grab_y;
    gdouble old_value;
};

G_DEFINE_TYPE_WITH_PRIVATE(InaudibleKnob, inaudible_knob, GTK_TYPE_RANGE)

static gboolean
inaudible_knob_draw(GtkWidget  *widget,
                    cairo_t    *cr)
{
    InaudibleKnob* knob = INAUDIBLE_KNOB(widget);
    InaudibleKnobPrivate* private = inaudible_knob_get_instance_private(knob);
    gdouble value = inaudible_knob_get_value(knob);
    gdouble max = gtk_adjustment_get_upper(gtk_range_get_adjustment(GTK_RANGE(knob)));

    /*cairo_set_source_rgba (cr, 1, 0, 0, 0.80);
    cairo_rectangle (cr, 0, 0, 80, 80);
    cairo_fill(cr);*/

    gdouble rad = ((280 * (value / max)) - 320) / (180 / M_PI);

    gdouble x = -23.0 * sin(rad) + 40 - 7;
    gdouble y = 23.0 * cos(rad) + 40 - 7;

    gdk_cairo_set_source_pixbuf(
        cr,
        knob->cursor,
        x,
        y
    );
    cairo_paint(cr);
    cairo_fill (cr);

    return FALSE;
}

static gboolean
inaudible_knob_button_press_event(GtkWidget* widget,
					              GdkEventButton* event)
{
    InaudibleKnob* knob = INAUDIBLE_KNOB(widget);
    InaudibleKnobPrivate* private = inaudible_knob_get_instance_private(knob);
    private->mouse_grab_y = event->y;
    private->old_value = inaudible_knob_get_value(knob);

    gtk_grab_add(widget);
    gtk_widget_queue_draw(widget);
}

static gboolean
inaudible_knob_button_release_event(GtkWidget* widget,
					                GdkEventButton* event)
{
    if (gtk_widget_has_grab(widget))
        gtk_grab_remove(widget);
    gtk_widget_queue_draw(widget);

    return TRUE;
}

static gboolean
inaudible_knob_motion_notify_event(GtkWidget* widget,
					               GdkEventMotion* event)
{
    InaudibleKnob* knob = INAUDIBLE_KNOB(widget);
    InaudibleKnobPrivate* private = inaudible_knob_get_instance_private(knob);

    if (gtk_widget_has_grab(widget))
    {
        gdouble max = gtk_adjustment_get_upper(gtk_range_get_adjustment(GTK_RANGE(knob)));
        gdouble value = private->old_value - ((event->y - private->mouse_grab_y) / 200);

        printf("Value is now : %f\n", value);

        inaudible_knob_set_value(knob, value);
    }

    return TRUE;
}


static void
inaudible_knob_class_init(InaudibleKnobClass* klass)
{
    GtkWidgetClass* widget_class = (GtkWidgetClass*)klass;
    widget_class->draw = inaudible_knob_draw;
    widget_class->button_press_event = inaudible_knob_button_press_event;
    widget_class->button_release_event = inaudible_knob_button_release_event;
    widget_class->motion_notify_event = inaudible_knob_motion_notify_event;
    widget_class->size_allocate = inaudible_knob_size_allocate;
}

static void
inaudible_knob_init(InaudibleKnob* knob)
{
    InaudibleKnobPrivate* private = inaudible_knob_get_instance_private(knob);
    private->cursor_size = 0;
    private->dead_angle = 50;

    GError* error = NULL;
    knob->cursor = gdk_pixbuf_new_from_file("knob_hole.png", &error);

    gtk_widget_queue_draw(GTK_WIDGET(knob));
}

static gboolean
inaudible_knob_value_changed(gpointer obj)
{
    return TRUE;
}

GtkWidget*
inaudible_knob_new(void)
{
    GtkWidget* widget = GTK_WIDGET(g_object_new(INAUDIBLE_TYPE_KNOB, NULL));
    if (widget)
    {
        GtkAdjustment* adjustment = (GtkAdjustment*)gtk_adjustment_new(0, 0, 1, 0.01, 0.5, 0);
        gtk_range_set_adjustment(GTK_RANGE(widget), adjustment);

        g_signal_connect(GTK_WIDGET(widget), "value-changed", G_CALLBACK(inaudible_knob_value_changed), widget);
        printf("RANGE VALUE = %f\n", gtk_range_get_value(GTK_RANGE(widget)));


    }
    return widget;
}

static gdouble
inaudible_knob_get_value(InaudibleKnob* knob)
{
    return gtk_range_get_value(GTK_RANGE(knob));
}

static void
inaudible_knob_set_range(InaudibleKnob* knob, gdouble min, gdouble max)
{
    gtk_range_set_range(GTK_RANGE(knob), min, max);
}

static void
inaudible_knob_set_value(InaudibleKnob* knob, gdouble value)
{
    gtk_range_set_value(GTK_RANGE(knob), value);
    gtk_widget_queue_draw(GTK_WIDGET(knob));
}

void
inaudible_knob_size_allocate(GtkWidget* widget,
                             GdkRectangle* allocation)
{
    printf("Size allocate : %d;%d\n", allocation->width, allocation->height);
    InaudibleKnob* knob = INAUDIBLE_KNOB(widget);
    InaudibleKnobPrivate* private = inaudible_knob_get_instance_private(knob);
    private->size = allocation->width < allocation->height ? allocation->width : allocation->height;

    gtk_widget_set_allocation(widget, allocation);
}

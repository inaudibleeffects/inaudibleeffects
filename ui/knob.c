#include "knob.h"
#include <cairo.h>

gdouble x, y = 0;
gdouble angle = 0.0;

struct _InaudibleKnobPrivate
{
    gint size;
    gint cursor_size;
};

G_DEFINE_TYPE_WITH_PRIVATE(InaudibleKnob, inaudible_knob, GTK_TYPE_RANGE)
//G_DEFINE_TYPE_WITH_CODE (InaudibleKnob, inaudible_knob, GTK_TYPE_RANGE, G_ADD_PRIVATE (InaudibleKnob))

static gboolean
inaudible_knob_draw(GtkWidget  *widget,
                    cairo_t    *cr)
{
    InaudibleKnob* knob = INAUDIBLE_KNOB(widget);

    /*cairo_set_source_rgba (cr, 1, 0, 0, 0.80);
    cairo_rectangle (cr, 0, 0, 80, 80);
    cairo_fill(cr);*/

    gdk_cairo_set_source_pixbuf(cr, knob->cursor, x, y);
    cairo_paint(cr);
    cairo_fill (cr);

    return FALSE;
}

static gboolean
inaudible_knob_button_press_event(GtkWidget* widget,
					              GdkEventButton* event)
{
    gtk_grab_add(widget);
    gtk_widget_queue_draw(widget);

    return TRUE;
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

    gint iw = gdk_pixbuf_get_width(knob->cursor);
    gint ih = gdk_pixbuf_get_height(knob->cursor);

    if (gtk_widget_has_grab(widget))
    {
        gint Ox = private->size / 2;
        gint Oy = private->size / 2;

        gdouble Mx = event->x - Ox;
        gdouble My = event->y - Oy;

        gdouble ratio = 23.0 / sqrt(Mx * Mx + My * My);

        printf("Size : %d\n", ((int)Mx - ((int)Ox)));

        x = Ox + Mx * ratio - iw / 2;
        y = Oy + My * ratio - ih / 2;
    }
    gtk_widget_queue_draw(widget);
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
    GtkWidget* widget = GTK_WIDGET(knob);
    InaudibleKnobPrivate* private = inaudible_knob_get_instance_private(knob);
    private->cursor_size = 0;

    GError* error = NULL;
    knob->cursor = gdk_pixbuf_new_from_file("knob_hole.png", &error);
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

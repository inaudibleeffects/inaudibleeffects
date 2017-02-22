#include "knob.h"
#include <cairo.h>

static void         inaudible_knob_class_init(InaudibleKnobClass* klass);
static gboolean     inaudible_knob_expose(GtkWidget *widget, GdkEventExpose *event);
static void         inaudible_knob_init(InaudibleKnob* knob);
static gboolean     inaudible_knob_value_changed(gpointer obj);

GType               inaudible_knob_get_type(void);
GtkWidget*          inaudible_knob_new(void);
void                inaudible_knob_set_size(InaudibleKnob* knob, int size);

gdouble x, y = 0;
gboolean changing = FALSE;

static gboolean
inaudible_knob_draw (GtkWidget  *widget,
                     cairo_t    *cr)
{
    GdkPixbuf *pix;
    GError *err = NULL;

    pix = gdk_pixbuf_new_from_file("knob_hole.png", &err);
    if(err)
    {
        printf("Error : %s\n", err->message);
        g_error_free(err);
        return FALSE;
    }

    /*cairo_set_source_rgba (cr, 1, 0, 0, 0.80);
    cairo_rectangle (cr, 0, 0, 80, 80);
    cairo_fill(cr);*/

    gdk_cairo_set_source_pixbuf(cr, pix, x, y);
    cairo_paint(cr);
    cairo_fill (cr);

    return FALSE;
}

static gboolean
inaudible_knob_button_press_event(GtkWidget* widget,
					              GdkEventButton* event)
{
    changing = TRUE;
    gtk_widget_queue_draw(widget);
    return TRUE;
}

static gboolean
inaudible_knob_button_release_event(GtkWidget* widget,
					                GdkEventButton* event)
{
    changing = FALSE;
    gtk_widget_queue_draw(widget);
    return TRUE;
}

static gboolean
inaudible_knob_motion_notify_event(GtkWidget* widget,
					               GdkEventMotion* event)
{
    if (changing == TRUE)
    {
        x = event->x;
        y = event->y;
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
}

static void
inaudible_knob_init(InaudibleKnob* knob)
{
    GtkWidget* widget = GTK_WIDGET(knob);
    GError* error = NULL;

    gtk_widget_set_can_focus(widget, TRUE);
    //knob->image = gdk_pixbuf_new_from_file("knob_hole.png", &error);
}

static gboolean
inaudible_knob_value_changed(gpointer obj)
{
    GtkWidget *widget = (GtkWidget *)obj;
    gtk_widget_queue_draw(widget);
    return FALSE;
}

static gboolean
inaudible_knob_expose(GtkWidget *widget,
                      GdkEventExpose *event)// (GtkWidget *da, GdkEvent *event, gpointer data)
{
    /*(void)event;
    cairo_t *cr;
    cr = gdk_cairo_create(widget);
    gdk_cairo_set_source_pixbuf(cr, pix, 0, 0);
    cairo_paint(cr);
    cairo_fill (cr);
    cairo_destroy (cr);*/
    gtk_widget_queue_draw(widget);
    return FALSE;
}


GType
inaudible_knob_get_type(void)
{
    static GType type = 0;
    if (!type)
    {
        static const GTypeInfo type_info = {
            sizeof(InaudibleKnobClass),
            NULL, /* base_init */
            NULL, /* base_finalize */
            (GClassInitFunc)inaudible_knob_class_init,
            NULL, /* class_finalize */
            NULL, /* class_data */
            sizeof(InaudibleKnob),
            0,    /* n_preallocs */
            (GInstanceInitFunc)inaudible_knob_init
        };

        for (int i = 0; ; i++)
        {
            const char *name = "InaudibleKnob";

            if (g_type_from_name(name))
                continue;

            type = g_type_register_static(GTK_TYPE_RANGE,
                                          name,
                                          &type_info,
                                          (GTypeFlags)0);
            break;
        }
    }
    return type;
}


GtkWidget*
inaudible_knob_new(void)
{
    GtkWidget* widget = GTK_WIDGET(g_object_new(INAUDIBLE_TYPE_KNOB, NULL));
    if (widget)
    {
        //inaudible_knob_set_size(widget, 100);
        //gtk_range_set_adjustment(GTK_RANGE(widget), _adjustment);
        g_signal_connect(GTK_WIDGET(widget), "value-changed", G_CALLBACK(inaudible_knob_value_changed), widget);

/*GdkRGBA bg;
        bg.red = 1.0;
        bg.alpha = 1.0;
        gtk_widget_override_background_color(GTK_WIDGET(widget), GTK_STATE_FLAG_NORMAL, &bg);*/

        //inaudible_knob_expose(widget, NULL);
    }
    return widget;
}

void
inaudible_knob_set_size (InaudibleKnob* knob, int size)
{
    char name[128];
    //GtkWidget *widget = GTK_WIDGET(knob);
    knob->size = size;
    //sprintf(name, "%s_%d\n", gtk_widget_get_name(widget), size);
    //gtk_widget_set_name(widget, name);
    //gtk_widget_queue_resize(widget);
}

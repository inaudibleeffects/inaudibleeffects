#include "knob.h"

extern const char _binary_ui_cursor_png_start[];
extern const char _binary_ui_cursor_png_end[];
extern const char _binary_ui_knob_png_start[];
extern const char _binary_ui_knob_png_end[];

struct _InaudibleKnobPrivate
{
    GdkPixbuf* cursor;          // Cursor pixbuf
    GdkPixbuf* knob;            // Post processed knob pixbuf
    GdkPixbuf* overlay;         // Lightened knob pixbuf

    gboolean hovering;          // Mouse is hovering knob
    gint alpha;                 // Overlay pixbuf alpha
    guint tick_callback;        // Animation callback ID
    gint64 overlay_last_frame;  // Last frame timestamp of animation
    guchar hue;                 // Last hue value
    gdouble dead_angle;         // Dead angle
    gdouble last_value;         // Last value set
    gdouble mouse_grab_y;       // Mouse position in Y axis
    gdouble range;
};

G_DEFINE_TYPE_WITH_PRIVATE(InaudibleKnob, inaudible_knob, GTK_TYPE_RANGE)

static gboolean
inaudible_knob_draw(GtkWidget *widget,
                    cairo_t   *cr)
{
    InaudibleKnob* knob = INAUDIBLE_KNOB(widget);
    InaudibleKnobPrivate* private = inaudible_knob_get_instance_private(knob);

    gdouble value = inaudible_knob_get_value(knob);
    gdouble max = gtk_adjustment_get_upper(gtk_range_get_adjustment(GTK_RANGE(knob)));

    /*gchar* text;
    g_ascii_formatd(text, 5, "%.2f", value);
    text = g_strconcat(text, "", NULL);*/

    int width = gtk_widget_get_allocated_width(widget);
    int height = gtk_widget_get_allocated_width(widget);

    double ox = width / 2.0;
    double oy = height / 2.0;

    int knob_width = gdk_pixbuf_get_width(private->knob);
    int knob_height = gdk_pixbuf_get_height(private->knob);
    int knob_x = ox - knob_width / 2;
    int knob_y = oy - knob_height / 2;

    double rad = ((285 * (value / max)) - 325) / (180 / M_PI);
    double cursor_x = -23.0 * sin(rad) + ox - (gdk_pixbuf_get_width(private->cursor) / 2);
    double cursor_y = 23.0 * cos(rad) + oy - (gdk_pixbuf_get_height(private->cursor) / 2);

    GdkPixbuf* overlay = gdk_pixbuf_new(GDK_COLORSPACE_RGB, TRUE, 8, knob_width, knob_height);
    gdk_pixbuf_composite(private->overlay,
                         overlay,
                         0,
                         0,
                         knob_width,
                         knob_height,
                         0,
                         0,
                         1,
                         1,
                         GDK_INTERP_BILINEAR,
                         private->alpha);


    gdk_cairo_set_source_pixbuf(cr, private->knob, knob_x, knob_y);
    cairo_paint(cr);

    gdk_cairo_set_source_pixbuf(cr, overlay, knob_x, knob_y);
    cairo_paint(cr);

    gdk_cairo_set_source_pixbuf(cr, private->cursor, cursor_x,cursor_y);
    cairo_paint(cr);

    /*cairo_select_font_face(cr, "Steelfish",
        CAIRO_FONT_SLANT_NORMAL,
        CAIRO_FONT_WEIGHT_BOLD);

    cairo_set_font_size(cr, 12);

    cairo_set_source_rgba(cr, 0, 0, 0, private->alpha/255.0);
    cairo_move_to(cr, 30, 44);
    cairo_show_text(cr, text);

    cairo_set_source_rgba(cr, 1, 1, 1, private->alpha/255.0);
    cairo_move_to(cr, 30, 45);
    cairo_show_text(cr, text);

    g_free(text);*/

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
    private->last_value = inaudible_knob_get_value(knob);

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

gboolean
inaudible_knob_enter_notify_event(GtkWidget* widget,
                                  GdkEventCrossing* event)
{
    InaudibleKnob* knob = INAUDIBLE_KNOB(widget);
    InaudibleKnobPrivate* private = inaudible_knob_get_instance_private(knob);

    private->hovering = TRUE;

    // Remove animation if alpha is 0.
    if (private->alpha == 0)
        private->tick_callback = gtk_widget_add_tick_callback(widget, inaudible_knob_on_tick, NULL, NULL);

    return TRUE;
}

gboolean
inaudible_knob_leave_notify_event(GtkWidget*        widget,
                                  GdkEventCrossing* event)
{
    InaudibleKnob* knob = INAUDIBLE_KNOB(widget);
    InaudibleKnobPrivate* private = inaudible_knob_get_instance_private(knob);

    if (gtk_widget_has_grab(widget) == FALSE)
        private->hovering = FALSE;

    // Remove animation if alpha is more than 240.
    if (private->alpha >= 240)
        private->tick_callback = gtk_widget_add_tick_callback(widget, inaudible_knob_on_tick, NULL, NULL);

    return TRUE;
}

static gboolean
inaudible_knob_motion_notify_event(GtkWidget*      widget,
                                   GdkEventMotion* event)
{
    InaudibleKnob* knob = INAUDIBLE_KNOB(widget);
    InaudibleKnobPrivate* private = inaudible_knob_get_instance_private(knob);

    if (gtk_widget_has_grab(widget))
    {

        // TODO : Improve value computing from mouse grabbing.

        gdouble max = gtk_adjustment_get_upper(gtk_range_get_adjustment(GTK_RANGE(knob)));
        gdouble value = (private->last_value / private->range) - ((event->y - private->mouse_grab_y) / 200);

        inaudible_knob_set_value(knob, value * private->range);
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
    widget_class->enter_notify_event = inaudible_knob_enter_notify_event;
    widget_class->leave_notify_event = inaudible_knob_leave_notify_event;
    widget_class->motion_notify_event = inaudible_knob_motion_notify_event;
    widget_class->size_allocate = inaudible_knob_size_allocate;
}

static void
inaudible_knob_init(InaudibleKnob* knob)
{
    InaudibleKnobPrivate* private = inaudible_knob_get_instance_private(knob);

    // Initialization.

    private->alpha = 250; // Fade out effet at startup (and avoid glitches)
    private->dead_angle = 50;
    private->hovering = FALSE;
    private->hue = 0;
    private->overlay_last_frame = 0;
    private->tick_callback = gtk_widget_add_tick_callback(
        GTK_WIDGET(knob),
        inaudible_knob_on_tick,
        NULL,
        NULL
    );
    private->range = 1.0f;

    // Load pixbufs.

    private->cursor = inaudible_pixbuf_new_from_data(
        _binary_ui_cursor_png_start,
        _binary_ui_cursor_png_end
    );

    private->knob = inaudible_pixbuf_new_from_data(
        _binary_ui_knob_png_start,
        _binary_ui_knob_png_end
    );

    // Create knob and overlay.

    inaudible_knob_set_hue(knob, 0);
}

static gboolean
inaudible_knob_on_tick(GtkWidget*     widget,
                       GdkFrameClock* frame_clock,
                       gpointer       data)
{
    InaudibleKnob* knob = INAUDIBLE_KNOB(widget);
    InaudibleKnobPrivate* private = inaudible_knob_get_instance_private(knob);

    // Removing callback if not necessary anymore.
    if (private->alpha == 0 && private->hovering == FALSE)
    {
        gtk_widget_remove_tick_callback(widget, private->tick_callback);
        return FALSE;
    }
    if (private->alpha == 240 && private->hovering == TRUE)
    {
        gtk_widget_remove_tick_callback(widget, private->tick_callback);
        return FALSE;
    }

    // Only do 30 frames per seond.
    gint64 current_time = gdk_frame_clock_get_frame_time(frame_clock);
    if (current_time - private->overlay_last_frame < 3000) // ~1/30s
        return TRUE;

    private->overlay_last_frame = gdk_frame_clock_get_frame_time(frame_clock);

    // Change overlay alpha.
    if (private->hovering == TRUE)
    {
        if (private->alpha <= 240)
            private->alpha += 10;
    }
    else
    {
        if (private->alpha >= 10)
            private->alpha -= 10;
    }

    // Redraw.
    gtk_widget_queue_draw(widget);

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

        //g_signal_connect(GTK_WIDGET(widget), "value-changed", G_CALLBACK(inaudible_knob_value_changed), widget);
    }
    return widget;
}

gdouble
inaudible_knob_get_value(InaudibleKnob* knob)
{
    return gtk_range_get_value(GTK_RANGE(knob));
}

void
inaudible_knob_set_hue(InaudibleKnob* knob, guchar value)
{
    InaudibleKnobPrivate* private = inaudible_knob_get_instance_private(knob);

    // Compute new hue from original hue.
    value = value - private->hue;
    private->hue = value;

    inaudible_pixbuf_set_hue(private->knob, value);

    private->overlay = gdk_pixbuf_copy(private->knob);
    inaudible_pixbuf_set_contrast(private->overlay, 1.3f);

    gtk_widget_queue_draw(GTK_WIDGET(knob));
}

void
inaudible_knob_set_range(InaudibleKnob* knob, gdouble min, gdouble max)
{
    InaudibleKnobPrivate* private = inaudible_knob_get_instance_private(knob);
    private->range = max - min;
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
    InaudibleKnob* knob = INAUDIBLE_KNOB(widget);
    InaudibleKnobPrivate* private = inaudible_knob_get_instance_private(knob);

    gtk_widget_set_allocation(widget, allocation);
}

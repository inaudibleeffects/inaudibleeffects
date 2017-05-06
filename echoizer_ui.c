#include <stdio.h>
#include <stdlib.h>

#include <cairo.h>
#include <gtk/gtk.h>

#include "lv2/lv2plug.in/ns/ext/atom/atom.h"
#include "lv2/lv2plug.in/ns/ext/atom/forge.h"
#include "lv2/lv2plug.in/ns/ext/atom/util.h"
#include "lv2/lv2plug.in/ns/ext/log/logger.h"
#include "lv2/lv2plug.in/ns/ext/patch/patch.h"
#include "lv2/lv2plug.in/ns/ext/urid/urid.h"
#include "lv2/lv2plug.in/ns/extensions/ui/ui.h"
#include "lv2/lv2plug.in/ns/lv2core/lv2.h"

#include "echoizer.h"
#include "./ui/knob.h"

extern char _binary_ui_background_png_start[];
extern char _binary_ui_background_png_size[];
extern char _binary_ui_background_png_end[];

#define ECHOIZER_UI_URI "http://inaudibleeffects.github.io/echoizer#ui"

typedef struct {
    LV2UI_Write_Function    write;
    LV2UI_Controller        controller;

    GtkWidget*              window; /* For optional show interface. */
    GtkWidget*              layout;
    GtkWidget*              image;
    InaudibleKnob*          knobDelay;
    InaudibleKnob*          knobFeedback;
    InaudibleKnob*          knobBlend;
} EchoizerUI;

EchoizerUI* ui;

void
knob_value_changed(GtkRange* range,
                   gpointer  user_data)
{
    float value = gtk_range_get_value(range);

    if (range == GTK_RANGE(ui->knobDelay))
        ui->write(ui->controller, DELAY, sizeof(float), 0, &value);
    if (range == GTK_RANGE(ui->knobFeedback))
        ui->write(ui->controller, FEEDBACK, sizeof(float), 0, &value);
    if (range == GTK_RANGE(ui->knobBlend))
        ui->write(ui->controller, BLEND, sizeof(float), 0, &value);
}


static LV2UI_Handle
instantiate(const LV2UI_Descriptor*   descriptor,
            const char*               plugin_uri,
            const char*               bundle_path,
            LV2UI_Write_Function      write_function,
            LV2UI_Controller          controller,
            LV2UI_Widget*             widget,
            const LV2_Feature* const* features)
{

    printf("Instantiating UI...\n");
    ui = (EchoizerUI*)malloc(sizeof(EchoizerUI));

    // Check memory
    if (!ui) {
        fprintf(stderr, "Echoizer.lv2 UI: out of memory\n");
        return NULL;
    }
    // Initialize private data structure
    ui->write = write_function;
    ui->controller = controller;

    // Build interface
    ui->window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_default_size(GTK_WINDOW(ui->window), 600, 150);
    gtk_window_set_position(GTK_WINDOW(ui->window), GTK_WIN_POS_CENTER);

    ui->layout = gtk_layout_new(NULL, NULL);
    //gtk_container_add(GTK_CONTAINER(ui->window), ui->layout);
    //gtk_widget_show(ui->layout);

    GError* error = NULL;

    ui->image = gtk_image_new_from_pixbuf(
        inaudible_pixbuf_new_from_data(
            _binary_ui_background_png_start,
            _binary_ui_background_png_end
        )
    );

    gtk_layout_put(GTK_LAYOUT(ui->layout), ui->image, 0, 0);

    ui->knobDelay = (InaudibleKnob*)inaudible_knob_new();
    gtk_layout_put(GTK_LAYOUT(ui->layout), (GtkWidget*)ui->knobDelay, 55, 35);
    gtk_widget_set_size_request((GtkWidget*)ui->knobDelay, 80, 80);
    inaudible_knob_set_range(ui->knobDelay, 1.f, 2000.f);
    //inaudible_knob_set_hue(ui->knobDelay, 20);
    g_signal_connect(ui->knobDelay, "value-changed", (GCallback)knob_value_changed, NULL);

    ui->knobFeedback = (InaudibleKnob*)inaudible_knob_new();
    gtk_layout_put(GTK_LAYOUT(ui->layout), (GtkWidget*)ui->knobFeedback, 174, 35);
    gtk_widget_set_size_request((GtkWidget*)ui->knobFeedback, 80, 80);
    //inaudible_knob_set_hue(ui->knobFeedback, 60);
    g_signal_connect(ui->knobFeedback, "value-changed", (GCallback)knob_value_changed, NULL);


    ui->knobBlend = (InaudibleKnob*)inaudible_knob_new();
    gtk_layout_put(GTK_LAYOUT(ui->layout), (GtkWidget*)ui->knobBlend, 293, 35);
    gtk_widget_set_size_request((GtkWidget*)ui->knobBlend, 80, 80);
    //inaudible_knob_set_hue(ui->knobBlend, 100);
    g_signal_connect(ui->knobBlend, "value-changed", (GCallback)knob_value_changed, NULL);

    *widget = ui->layout;

    gtk_widget_set_size_request(*widget, 600, 150);

    return ui;
}

static void
cleanup(LV2UI_Handle handle)
{
    EchoizerUI* ui = (EchoizerUI*)handle;
    gtk_widget_destroy(GTK_WIDGET(ui->layout));
    gtk_widget_destroy(GTK_WIDGET(ui->image));
    gtk_widget_destroy(GTK_WIDGET(ui->knobDelay));
    gtk_widget_destroy(GTK_WIDGET(ui->knobFeedback));
    gtk_widget_destroy(GTK_WIDGET(ui->knobBlend));
    free(ui);
}

static void
port_event(LV2UI_Handle handle,
           uint32_t     port_index,
           uint32_t     buffer_size,
           uint32_t     format,
           const void*  buffer)
{
    EchoizerUI* self   = (EchoizerUI*)handle;
    if (!format) {
        float value = *(float*)buffer;
        switch ((PortIndex)port_index) {
            case DELAY:
                inaudible_knob_set_value(self->knobDelay, value);
                break;
            case FEEDBACK:
                inaudible_knob_set_value(self->knobFeedback, value);
                break;
            case BLEND:
                inaudible_knob_set_value(self->knobBlend, value);
                break;
        }
    }
}

/* Optional non-embedded UI show interface. */
static int
ui_show(LV2UI_Handle handle)
{
    EchoizerUI* ui = (EchoizerUI*)handle;

    printf("ui_show\n");

    int argc = 0;
    gtk_init(&argc, NULL);

    //ui->window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    //gtk_container_add(GTK_CONTAINER(ui->window), ui->box);
    gtk_widget_show_all(ui->window);
    gtk_window_present(GTK_WINDOW(ui->window));

    return 0;
}

/* Optional non-embedded UI hide interface. */
static int
ui_hide(LV2UI_Handle handle)
{
    return 0;
}

/* Idle interface for optional non-embedded UI. */
static int
ui_idle(LV2UI_Handle handle)
{
    EchoizerUI* ui = (EchoizerUI*)handle;
    if (ui->window) {
        gtk_main_iteration();
    }
    return 0;
}

static const void*
extension_data(const char* uri)
{
    static const LV2UI_Show_Interface show = { ui_show, ui_hide };
    static const LV2UI_Idle_Interface idle = { ui_idle };
    if (!strcmp(uri, LV2_UI__showInterface)) {
        return &show;
    } else if (!strcmp(uri, LV2_UI__idleInterface)) {
        return &idle;
    }
    return NULL;
}

static const LV2UI_Descriptor descriptor = {
    ECHOIZER_UI_URI,
    instantiate,
    cleanup,
    port_event,
    extension_data
};

LV2_SYMBOL_EXPORT
const LV2UI_Descriptor*
lv2ui_descriptor(uint32_t index)
{
    switch (index) {
    case 0:
        return &descriptor;
    default:
        return NULL;
    }
}

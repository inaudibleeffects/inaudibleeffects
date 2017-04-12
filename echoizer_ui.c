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

#include "./uris.h"
#include "./ui/knob.h"

extern char _binary_ui_background_png_start[];
extern char _binary_ui_background_png_size[];
extern char _binary_ui_background_png_end[];

#define ECHOIZER_UI_URI "http://inaudibleeffects.github.io/echoizer#ui"

typedef struct {
    LV2_Atom_Forge          forge;
    LV2_URID_Map*           map;
    LV2_Log_Logger          logger;
    EchoizerURIs            uris;

    LV2UI_Write_Function    write;
    LV2UI_Controller        controller;

    GtkWidget*              window; /* For optional show interface. */
    GtkWidget*              layout;
    GtkWidget*              image;
	InaudibleKnob*          knobDelay;
    InaudibleKnob*          knobFeedback;
    InaudibleKnob*          knobBlend;

    uint8_t forge_buf[1024];
} EchoizerUI;

EchoizerUI* ui;

/*
    Send current UI settings to backend.
 */
/*static void
send_ui_state(LV2UI_Handle handle)
{
    EchoizerUI* ui   = (EchoizerUI*)handle;

    const float delay = inaudible_knob_get_value(ui->knobDelay);
    const float feedback = inaudible_knob_get_value(ui->knobFeedback);
    const float blend = inaudible_knob_get_value(ui->knobBlend);

    // Use local buffer on the stack to build atom
    uint8_t obj_buf[1024];
    lv2_atom_forge_set_buffer(&ui->forge, obj_buf, sizeof(obj_buf));

    // Start a ui:State object
    LV2_Atom_Forge_Frame frame;
    LV2_Atom*            msg = (LV2_Atom*)lv2_atom_forge_object(&ui->forge,
                                                                &frame,
                                                                0,
                                                                ui->uris.ui_State);


    // msg[amplitude] = float
    lv2_atom_forge_float(&ui->forge, ui->uris.ui_Delay);
    lv2_atom_forge_float(&ui->forge, ui->uris.ui_Feedback);
    lv2_atom_forge_float(&ui->forge, ui->uris.ui_Blend);

    // Finish ui:State object
    lv2_atom_forge_pop(&ui->forge, &frame);

    // Send message to plugin portLV2_Log_Logger logger;LV2_Log_Logger logger; '0'
    ui->write(ui->controller,
              0,
              lv2_atom_total_size(msg),
              ui->uris.atom_eventTransfer,
              msg);
}*/

/*
    Gtk widget callback.
 */
/*static gboolean
on_cfg_changed(GtkWidget* widget, gpointer data)
{
    send_ui_state(data);
    return TRUE;
}*/




/*
 * Notify backend that UI is closed.
 */
/*static void
send_ui_disable(LV2UI_Handle handle)
{
    EchoizerUI* ui = (EchoizerUI*)handle;
    send_ui_state(handle);

    uint8_t obj_buf[64];
    lv2_atom_forge_set_buffer(&ui->forge, obj_buf, sizeof(obj_buf));

    LV2_Atom_Forge_Frame frame;
    LV2_Atom*   msg = (LV2_Atom*)lv2_atom_forge_object(
                &ui->forge,
                &frame,
                0,
                ui->uris.ui_Off
    );
    lv2_atom_forge_pop(&ui->forge, &frame);
    ui->write(ui->controller,
        0,
        lv2_atom_total_size(msg),
        ui->uris.atom_eventTransfer,
        msg
    );
}*/

/*
 * Notify backend that UI is active.
 * The plugin should send state and enable data transmission.
*/
/*static void
send_ui_enable(LV2UI_Handle handle)
{
    EchoizerUI* ui = (EchoizerUI*)handle;

    uint8_t obj_buf[64];
    lv2_atom_forge_set_buffer(&ui->forge, obj_buf, sizeof(obj_buf));

    LV2_Atom_Forge_Frame frame;
    LV2_Atom* msg = (LV2_Atom*)lv2_atom_forge_object(
        &ui->forge,
        &frame,
        0,
        ui->uris.ui_On
    );

    lv2_atom_forge_pop(&ui->forge, &frame);
    ui->write(
        ui->controller,
        0,
        lv2_atom_total_size(msg),
        ui->uris.atom_eventTransfer,
        msg
    );
}*/

void
knob_value_changed(GtkRange* range,
                   gpointer  user_data)
{
    if (range == GTK_RANGE(ui->knobDelay))
        printf("Delay\n");
    if (range == GTK_RANGE(ui->knobFeedback))
        printf("Feedback\n");
    if (range == GTK_RANGE(ui->knobBlend))
        printf("Blend\n");
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


    // Get host features
    // Get host features
	/*const char* missing = lv2_features_query(
		features,
		LV2_LOG__log,  &ui->logger.log, false,
		LV2_URID__map, &ui->map,        true,
		NULL);
	lv2_log_logger_set_map(&ui->logger, ui->map);
	if (missing) {
    	lv2_log_error(&ui->logger, "Missing feature <%s>\n", missing);
    	free(ui);
    	return NULL;
    }*/



    //map_echoizer_uris(ui->map, &ui->uris);
    //lv2_atom_forge_init(&ui->forge, ui->map);

    // Build interface
    ui->window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_default_size(GTK_WINDOW(ui->window), 600, 150);
    gtk_window_set_position(GTK_WINDOW(ui->window), GTK_WIN_POS_CENTER);

    ui->layout = gtk_layout_new(NULL, NULL);
    //gtk_container_add(GTK_CONTAINER(ui->window), ui->layout);
    //gtk_widget_show(ui->layout);


    char* background_png;
    background_png = _binary_ui_background_png_start;

    GdkPixbufLoader *loader;
    loader = gdk_pixbuf_loader_new();

    GError* error = NULL;
    ///http://stackoverflow.com/questions/14121166/gdk-pixbuf-load-image-from-memory
    guint size = (unsigned int)(_binary_ui_background_png_end - _binary_ui_background_png_start);
    printf("Loading background (%d)\n", size);
    gdk_pixbuf_loader_write(loader, background_png, size, &error);
    ui->image = gtk_image_new_from_pixbuf(gdk_pixbuf_loader_get_pixbuf(loader));
    //printf("Null ? %d -> %s\n", ui->image == NULL ? 1 : 0, error->message);

    //printf("Loading background (%d)\n", _binary_ui_background_png_size);
    /*GdkPixbuf* background = gdk_pixbuf_new_from_data (background_png,
                              GDK_COLORSPACE_RGB,
                              FALSE,
                              8,
                              600,
                              150,
                              1800,
                              NULL,
                              NULL);
    ui->image = gtk_image_new_from_pixbuf(background); //gtk_image_new_from_file("background.png");*/
    gtk_layout_put(GTK_LAYOUT(ui->layout), ui->image, 0, 0);

    ui->knobDelay = (InaudibleKnob*)inaudible_knob_new();
    gtk_layout_put(GTK_LAYOUT(ui->layout), (GtkWidget*)ui->knobDelay, 55, 35);
    gtk_widget_set_size_request((GtkWidget*)ui->knobDelay, 80, 80);
    inaudible_knob_set_hue(ui->knobDelay, 20);
    g_signal_connect(ui->knobDelay, "value-changed", (GCallback)knob_value_changed, NULL);

    ui->knobFeedback = (InaudibleKnob*)inaudible_knob_new();
    gtk_layout_put(GTK_LAYOUT(ui->layout), (GtkWidget*)ui->knobFeedback, 174, 35);
    gtk_widget_set_size_request((GtkWidget*)ui->knobFeedback, 80, 80);
    inaudible_knob_set_hue(ui->knobFeedback, 20);
    g_signal_connect(ui->knobFeedback, "value-changed", (GCallback)knob_value_changed, NULL);


    ui->knobBlend = (InaudibleKnob*)inaudible_knob_new();
    gtk_layout_put(GTK_LAYOUT(ui->layout), (GtkWidget*)ui->knobBlend, 293, 35);
    gtk_widget_set_size_request((GtkWidget*)ui->knobBlend, 80, 80);
    inaudible_knob_set_hue(ui->knobBlend, 20);
    g_signal_connect(ui->knobBlend, "value-changed", (GCallback)knob_value_changed, NULL);

    // Request state (filename) from plugin
	/*lv2_atom_forge_set_buffer(&ui->forge, ui->forge_buf, sizeof(ui->forge_buf));
	LV2_Atom_Forge_Frame frame;
	LV2_Atom*            msg = (LV2_Atom*)lv2_atom_forge_object(
		&ui->forge, &frame, 0, ui->uris.patch_Get);
	lv2_atom_forge_pop(&ui->forge, &frame);

	ui->write(ui->controller, 0, lv2_atom_total_size(msg),
	          ui->uris.atom_eventTransfer,
              msg);*/


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
    EchoizerUI*      ui   = (EchoizerUI*)handle;
	/*if (format == ui->uris.atom_eventTransfer) {
		const LV2_Atom* atom = (const LV2_Atom*)buffer;
		if (lv2_atom_forge_is_object_type(&ui->forge, atom->type)) {
			const LV2_Atom_Object* obj = (const LV2_Atom_Object*)atom;
		} else {
			//lv2_log_error(&ui->logger, "Unknown message type\n");
		}
	} else {
		//lv2_log_warning(&ui->logger, "Unknown port event format\n");
    }*/
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

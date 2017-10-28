#include <stdio.h>
#include <stdlib.h>

#include "lv2/lv2plug.in/ns/ext/atom/atom.h"
#include "lv2/lv2plug.in/ns/ext/atom/forge.h"
#include "lv2/lv2plug.in/ns/ext/atom/util.h"
#include "lv2/lv2plug.in/ns/ext/log/logger.h"
#include "lv2/lv2plug.in/ns/ext/patch/patch.h"
#include "lv2/lv2plug.in/ns/ext/urid/urid.h"
#include "lv2/lv2plug.in/ns/extensions/ui/ui.h"
#include "lv2/lv2plug.in/ns/lv2core/lv2.h"

#include "inaudible.h"

#include "echoizer.h"

#define ECHOIZER_UI_URI "http://inaudibleeffects.github.io/echoizer#ui"


INAUDIBLE_EXTERNAL_PNG(knob);
INAUDIBLE_EXTERNAL_PNG(background);


typedef struct {
    LV2UI_Write_Function    write;
    LV2UI_Controller        controller;

    InaudibleWindow*        window;
    InaudiblePixbuf*        backgroundPixbuf;
    InaudiblePixbuf*        knobPixbuf;
    InaudibleWidget*        background;
    InaudibleWidget*        knobDelay;
    InaudibleWidget*        knobFeedback;
    InaudibleWidget*        knobBlend;
} EchoizerUI;

EchoizerUI* ui;

void
knob_value_changed(InaudibleKnob* knob,
                    const float   value)
{
    if (knob == INAUDIBLE_KNOB(ui->knobDelay))
        ui->write(ui->controller, DELAY, sizeof(float), 0, &value);
    if (knob == INAUDIBLE_KNOB(ui->knobFeedback))
        ui->write(ui->controller, FEEDBACK, sizeof(float), 0, &value);
    if (knob == INAUDIBLE_KNOB(ui->knobBlend))
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

    inaudible_app();

    // Build interface
    ui->backgroundPixbuf = INAUDIBLE_PIXBUF_FROM(background);
    ui->background = inaudible_image_new(ui->backgroundPixbuf);

    ui->knobPixbuf = INAUDIBLE_PIXBUF_FROM(knob);
    ui->knobDelay = inaudible_knob_new(ui->knobPixbuf);
    ui->knobDelay->x = 55;
    ui->knobDelay->y = 36;

    ui->knobFeedback = inaudible_knob_new(ui->knobPixbuf);
    ui->knobFeedback->x = 174;
    ui->knobFeedback->y = 36;

    ui->knobBlend = inaudible_knob_new(ui->knobPixbuf);
    ui->knobBlend->x = 293;
    ui->knobBlend->y = 36;

    inaudible_knob_set_value(INAUDIBLE_KNOB(ui->knobDelay), 0.5f);
    inaudible_knob_set_value(INAUDIBLE_KNOB(ui->knobFeedback), 0.5f);
    inaudible_knob_set_value(INAUDIBLE_KNOB(ui->knobBlend), 0.5f);

    printf("Loading view...\n");



    // Get parent window and resize API from features
	PuglNativeWindow parent = 0;
	LV2UI_Resize*    resize = NULL;
	for (int i = 0; features && features[i]; ++i) {
		if (!strcmp(features[i]->URI, LV2_UI__parent)) {
			parent = (PuglNativeWindow)features[i]->data;
		} else if (!strcmp(features[i]->URI, LV2_UI__resize)) {
			resize = (LV2UI_Resize*)features[i]->data;
		}
	}

	if (!parent) {
		fprintf(stderr, "error: glamp_ui: No parent window provided.\n");
		free(ui);
		return NULL;
	}

	if (resize) {
		resize->ui_resize(resize->handle, 600, 150);
	}

    ui->window = inaudible_window_new(parent, "Echoizer", 600, 150, false);
    inaudible_window_add_widget(ui->window, ui->background);
    inaudible_window_add_widget(ui->window, ui->knobDelay);
    inaudible_window_add_widget(ui->window, ui->knobFeedback);
    inaudible_window_add_widget(ui->window, ui->knobBlend);

    INAUDIBLE_CONNECT(INAUDIBLE_KNOB(ui->knobDelay), value_changed, knob_value_changed);

	*widget = (void*)puglGetNativeWindow(inaudible_window_get_view(ui->window));

    inaudible_app_show_window(ui->window);

    return ui;
}

static int
idle(LV2UI_Handle handle)
{
	//EchoizerUI* ui = (EchoizerUI*)handle;
    inaudible_app_iteration();
	return 0;
}

static void
cleanup(LV2UI_Handle handle)
{
    EchoizerUI* ui = (EchoizerUI*)handle;
    inaudible_widget_destroy(ui->background);
    inaudible_widget_destroy(ui->knobDelay);
    inaudible_widget_destroy(ui->knobFeedback);
    inaudible_widget_destroy(ui->knobBlend);
    inaudible_pixbuf_destroy(ui->backgroundPixbuf);
    inaudible_pixbuf_destroy(ui->knobPixbuf);
    inaudible_window_destroy(ui->window);
    free(ui);
}

static void
port_event(LV2UI_Handle handle,
           uint32_t     port_index,
           uint32_t     buffer_size,
           uint32_t     format,
           const void*  buffer)
{
    // EchoizerUI* ui = (EchoizerUI*)handle;
    // if (!format) {
    //     float value = *(float*)buffer;
    //     switch ((PortIndex)port_index) {
    //         case DELAY:
    //             inaudible_knob_set_value(INAUDIBLE_KNOB(ui->knobDelay), value);
    //             break;
    //         case FEEDBACK:
    //             inaudible_knob_set_value(INAUDIBLE_KNOB(ui->knobFeedback), value);
    //             break;
    //         case BLEND:
    //             inaudible_knob_set_value(INAUDIBLE_KNOB(ui->knobBlend), value);
    //             break;
    //     }
    // }
}

static const LV2UI_Idle_Interface idle_iface = { idle };

static const void*
extension_data(const char* uri)
{
    if (!strcmp(uri, LV2_UI__idleInterface)) {
        return &idle_iface;
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

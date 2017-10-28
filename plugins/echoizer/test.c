#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "inaudible.h"

INAUDIBLE_EXTERNAL_PNG(knob);
INAUDIBLE_EXTERNAL_PNG(background);

static InaudibleWidget* image;
static InaudibleWidget* knobDelay;
static InaudibleWidget* knobFeedback;
static InaudibleWidget* knobBlend;
static InaudiblePixbuf* backgroundPixbuf;
static InaudiblePixbuf* knobPixbuf;
static InaudibleWindow* window;

static void value_changed(InaudibleKnob* knob, const float value)
{
    printf("Value = %f\n", value);
}

int
main(int argc, char** argv)
{
    printf("Loading...\n");

    inaudible_app();


    printf("Loading background...\n");

    backgroundPixbuf = INAUDIBLE_PIXBUF_FROM(background);

    image = inaudible_image_new(backgroundPixbuf);


    printf("Loading knobs...\n");

    knobPixbuf = INAUDIBLE_PIXBUF_FROM(knob);

    knobDelay = inaudible_knob_new(knobPixbuf);
    knobDelay->x = 55;
    knobDelay->y = 36;

    INAUDIBLE_CONNECT(INAUDIBLE_KNOB(knobDelay), value_changed, value_changed);

    knobFeedback = inaudible_knob_new(knobPixbuf);
    knobFeedback->x = 174;
    knobFeedback->y = 36;

    knobBlend = inaudible_knob_new(knobPixbuf);
    knobBlend->x = 293;
    knobBlend->y = 36;

    inaudible_knob_set_value(INAUDIBLE_KNOB(knobDelay), 0.5f);
    inaudible_knob_set_value(INAUDIBLE_KNOB(knobFeedback), 0.5f);
    inaudible_knob_set_value(INAUDIBLE_KNOB(knobBlend), 0.5f);


    printf("Loading view...\n");

    window = inaudible_window_new(NULL, "Echoizer", 600, 150, false);
    inaudible_window_add_widget(window, image);
    inaudible_window_add_widget(window, knobDelay);
    inaudible_window_add_widget(window, knobFeedback);
    inaudible_window_add_widget(window, knobBlend);


    printf("Loading window...\n");

    inaudible_app_show_window(window);


    printf("Running app...\n");

    inaudible_app_run();


    printf("Removing resources...\n");

    inaudible_widget_destroy(image);
    inaudible_widget_destroy(knobDelay);
    inaudible_widget_destroy(knobFeedback);
    inaudible_widget_destroy(knobBlend);
    inaudible_pixbuf_destroy(backgroundPixbuf);
    inaudible_pixbuf_destroy(knobPixbuf);
    inaudible_window_destroy(window);

    inaudible_app_quit();

	return 0;
}

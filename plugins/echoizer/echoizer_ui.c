// See https://github.com/drobilla/pugl/blob/master/pugl_cairo_test.c

#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include <cairo/cairo.h>

#include "inaudible.h"


extern char _binary_knob_png_start[];
extern char _binary_knob_png_end[];

extern char _binary_background_png_start[];
extern char _binary_background_png_end[];

static InaudibleImage* image;
static InaudiblePixbuf* backgroundPixbuf;
static InaudiblePixbuf* knobPixbuf;
static InaudibleKnob* knob;
static InaudibleWindow* window;
//static InaudibleKnob* knob;


int
main(int argc, char** argv)
{
    printf("Loading...\n");

    inaudible_app();

    // Pre-load data
    const char* start;
    const char* end;
    size_t size;

    printf("Loading background...\n");

    start = _binary_background_png_start;
    end = _binary_background_png_end;
    size = (size_t)(end - start);

    backgroundPixbuf = inaudible_pixbuf_new(start, size);
    image = inaudible_image_new(backgroundPixbuf);

    printf("Loading knob...\n");

    start = _binary_knob_png_start;
    end = _binary_knob_png_end;
    size = (size_t)(end - start);

    knobPixbuf = inaudible_pixbuf_new(start, size);
    knob = inaudible_knob_new(knobPixbuf);

    printf("Loading view...\n");

    window = inaudible_window_new("Echoizer", 600, 150, false);
    inaudible_window_add_widget(window, &(image->parent));

    printf("Loading window...\n");

    inaudible_app_show_window(window);

    printf("Running app...\n");

    inaudible_app_run();

    printf("Removing resources...\n");

    inaudible_image_destroy(image);
    inaudible_pixbuf_destroy(backgroundPixbuf);
    inaudible_pixbuf_destroy(knobPixbuf);
    inaudible_knob_destroy(knob);
    inaudible_window_destroy(window);

    inaudible_app_quit();

	return 0;
}

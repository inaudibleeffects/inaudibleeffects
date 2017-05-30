// See https://github.com/drobilla/pugl/blob/master/pugl_cairo_test.c

#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include <cairo/cairo.h>

//#include "inaudible/knob.h"
#include "inaudible/widgets/image.h"
#include "pugl/pugl.h"

extern char _binary_background_png_start[];
extern char _binary_background_png_end[];

static int  quit    = 0;
static InaudibleImage* image;
//static InaudibleKnob* knob;

static void
onDisplay(PuglView* view)
{
	cairo_t* cr = puglGetContext(view);

    // Draw background
	int width, height;
	puglGetSize(view, &width, &height);

    inaudible_image_draw(image, cr);

    //inaudible_knob_draw(knob);
    /*cairo_set_source_rgb (cr, 0, 1, 0);
    cairo_rectangle (cr, 0, 0, 256, 256);
    cairo_fill (cr);*/

    printf("Done\n");
}

static void
onClose(PuglView* view)
{
    printf("Quitting\n");
	quit = 1;
    inaudible_image_destroy(image);
    //inaudible_knob_destroy(knob);
}

static void
onEvent(PuglView* view, const PuglEvent* event)
{
	switch (event->type) {
	case PUGL_KEY_PRESS:
		if (event->key.character == 'q' ||
		    event->key.character == 'Q' ||
		    event->key.character == PUGL_CHAR_ESCAPE) {
			onClose(view);
		}
        if (event->key.character == 'r')
        {
            onDisplay(view);
        }
		break;
	case PUGL_BUTTON_PRESS:
		// Nothing.
		break;
	case PUGL_ENTER_NOTIFY:
		//puglPostRedisplay(view);
		break;
	case PUGL_LEAVE_NOTIFY:
		//puglPostRedisplay(view);
		break;
	case PUGL_EXPOSE:
		onDisplay(view);
		break;
	case PUGL_CLOSE:
		onClose(view);
		break;
	default: break;
	}
}

int
main(int argc, char** argv)
{
	bool ignoreKeyRepeat = false;
	bool resizable       = false;

// Pre-load data
    const char* start = _binary_background_png_start;
    const char* end = _binary_background_png_end;
    size_t size = (size_t)(end - start);

    image = inaudible_image_new(start, size);
    //knob = inaudible_knob_new(image);


	PuglView* view = puglInit(NULL, NULL);
	puglInitWindowSize(view, 600, 150);
	puglInitResizable(view, resizable);
	puglInitContextType(view, PUGL_CAIRO);

	puglIgnoreKeyRepeat(view, ignoreKeyRepeat);
	puglSetEventFunc(view, onEvent);

	puglCreateWindow(view, "INAUDIBLE Effects | Echoizer");
	puglShowWindow(view);

	while (!quit) {
		puglWaitForEvent(view);
		puglProcessEvents(view);
	}

	puglDestroy(view);
	return 0;
}

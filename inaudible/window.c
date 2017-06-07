#include "window.h"

InaudibleWindow* inaudible_window_new(const char* title,
                                      const int   width,
                                      const int   height,
                                      bool        resizable)
{
    InaudibleWindow* self = INAUDIBLE_NEW(InaudibleWindow);

    PuglView* view = puglInit(NULL, NULL);
    puglInitWindowSize(view, width, height);
    puglInitResizable(view, resizable);
    puglInitContextType(view, PUGL_CAIRO);

    puglIgnoreKeyRepeat(view, true);
    puglSetEventFunc(view, onEvent);

    puglCreateWindow(view, title);

    self->title = title;
    self->view = view;
    self->widgets = INAUDIBLE_NEW(InaudibleLinkedList);
}

void
inaudible_window_destroy(InaudibleWindow* window)
{
    puglDestroy(window->view);
    //INAUDIBLE_DESTROY(widgets);
    INAUDIBLE_DESTROY(window);
}

static void
onDisplay(PuglView* view)
{
	cairo_t* cr = puglGetContext(view);

    printf("Done\n");
}

static void
onClose(PuglView* view)
{
    printf("Closing window...\n");
    puglDestroy(view);
}

static void
onEvent(PuglView*        view,
        const PuglEvent* event)
{
	switch (event->type)
    {
    	case PUGL_KEY_PRESS:
    		if (event->key.character == 'q' ||
    		    event->key.character == 'Q' ||
    		    event->key.character == PUGL_CHAR_ESCAPE)
            {
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
        case PUGL_SCROLL:
            //printf("Scroll : %f\n", event->scroll.dy); // For futur purpose.
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

void
inaudible_window_show(InaudibleWindow* window)
{
    printf("%d - %s\n", &window, window->title);
    puglShowWindow(window->view);
}

void
inaudible_window_add_widget(InaudibleWindow* window, InaudibleWidget* widget)
{

}

PuglView*
inaudible_window_get_view(InaudibleWindow* window)
{
    return window->view;
}

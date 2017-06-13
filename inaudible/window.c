#include "window.h"
#include "app.h"

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

    self->state = 0;
    self->title = title;
    self->view = view;
    self->widgets = INAUDIBLE_NEW(InaudibleLinkedList);

    inaudible_dictionary_add(&views, view, self);

    return self;
}

void
inaudible_window_destroy(InaudibleWindow* window)
{
    inaudible_dictionary_remove(&views, window->view);
    puglDestroy(window->view);
    INAUDIBLE_DESTROY(window->widgets);
    INAUDIBLE_DESTROY(window);
}

void
inaudible_window_close(InaudibleWindow* window)
{
    window->state = 1;
}

void
inaudible_window_show(InaudibleWindow* window)
{
    printf("Showing window...\n");
    puglShowWindow(window->view);
}

void
inaudible_window_add_widget(InaudibleWindow* window,
                            InaudibleWidget* widget)
{
    //inaudible_linkedlist_add(window->widgets, widget);
}

static void
onDisplay(PuglView* view)
{
	cairo_t* cr = puglGetContext(view);

    /*InaudibleWindow* window = inaudible_dictionary_get_value(views, view);
    InaudibleLinkedList* widgets = window->widgets;

    while (widgets)
    {

    }*/

    printf("Done\n");
}

static void
onClose(PuglView* view)
{
    printf("Closing window...\n");
    InaudibleWindow* window = inaudible_dictionary_get_value(views, view);
    inaudible_app_close_window(window);
}

static void
onEvent(PuglView*        view,
        const PuglEvent* event)
{
    printf("onEvent\n");
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


// TODO : Remove ?
PuglView*
inaudible_window_get_view(InaudibleWindow* window)
{
    return window->view;
}

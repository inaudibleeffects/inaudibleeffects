#include "window.h"
#include "app.h"
#include <stdio.h>

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
    puglSetHandle(view, self);
    puglSetEventFunc(view, onEvent);

    puglCreateWindow(view, title);

    self->closing = false;
    self->title = title;
    self->view = view;
    self->widgets = INAUDIBLE_NEW(InaudibleLinkedList);

    return self;
}

void
inaudible_window_destroy(InaudibleWindow* window)
{
    puglDestroy(window->view);
    INAUDIBLE_DESTROY(window->widgets);
    INAUDIBLE_DESTROY(window);
}

void
inaudible_window_close(InaudibleWindow* window)
{
    window->closing = true;
}

void
inaudible_window_show(InaudibleWindow* window)
{
    puglShowWindow(window->view);
}

void
inaudible_window_add_widget(InaudibleWindow* window,
                            InaudibleWidget* widget)
{
    inaudible_linkedlist_add(&(window->widgets), widget);
    printf("Widgets : %d\n", inaudible_linkedlist_count(window->widgets));
}

static void
onDisplay(PuglView* view)
{
    cairo_t* cr = puglGetContext(view);

    InaudibleWindow* window = puglGetHandle(view);

    InaudibleLinkedList* widgets = window->widgets;
    InaudibleWidget* widget;
    printf("Widgets to render : %d\n", inaudible_linkedlist_count(widgets));
    while (widgets)
    {
        widget = inaudible_linkedlist_get_value(widgets);
        printf("Widget->x : %p\n", &widget);
        widget->draw(widget, cr);
        widgets = widgets->next;
    }

    cairo_fill(cr);
}

static void
onClose(PuglView* view)
{
    InaudibleWindow* window = puglGetHandle(view);
    inaudible_app_close_window(window);
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


// TODO : Remove ?
PuglView*
inaudible_window_get_view(InaudibleWindow* window)
{
    return window->view;
}

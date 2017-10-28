#include "ui/window.h"
#include "app.h"
#include <stdio.h>

static bool
mouse_in_widget(InaudibleWidget* widget, const int x, const int y)
{
    return  (x >= widget->x && x < widget->x + widget->width) && \
            (y >= widget->y && y < widget->y + widget->height);
}

static void inaudible_window_on_button_press(InaudibleWindow*       window,
                                             const PuglEventButton* event);

static void inaudible_window_on_button_release(InaudibleWindow*       window,
                                               const PuglEventButton* event);

static void inaudible_window_on_mouse_move(InaudibleWindow*       window,
                                           const PuglEventMotion* event);

InaudibleWindow* inaudible_window_new(PuglNativeWindow parent,
                                      const char*      title,
                                      const int        width,
                                      const int        height,
                                      const bool       resizable)
{
    InaudibleWindow* self = INAUDIBLE_NEW(InaudibleWindow);

    PuglView* view = puglInit(NULL, NULL);
    puglInitWindowParent(view, parent);
    puglInitWindowSize(view, width, height);
    puglInitResizable(view, resizable);
    puglInitContextType(view, PUGL_CAIRO);

    puglIgnoreKeyRepeat(view, true);
    puglSetHandle(view, self);
    puglSetEventFunc(view, onEvent);

    puglCreateWindow(view, title);
    //puglGetHandle(view);

    self->closing = false;
    self->title = title;
    self->view = view;
    self->widgets = inaudible_linkedlist_new();

    self->on_button_press = &inaudible_window_on_button_press;
    self->on_button_release = &inaudible_window_on_button_release;
    self->on_mouse_move = &inaudible_window_on_mouse_move;

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
inaudible_window_add_widget(InaudibleWindow* window,
                            InaudibleWidget* widget)
{
    inaudible_linkedlist_add(&(window->widgets), widget);
}

void
inaudible_window_close(InaudibleWindow* window)
{
    window->closing = true;
}

PuglView*
inaudible_window_get_view(InaudibleWindow* window)
{
    return window->view;
}

static void
inaudible_window_on_button_release(InaudibleWindow*       window,
                                   const PuglEventButton* event)
{
    InaudibleLinkedList* widgets = window->widgets;

    int x = event->x;
    int y = event->y;

    while (widgets)
    {
        InaudibleWidget* widget = inaudible_linkedlist_get_value(widgets);
        if (widget->has_grab)
            widget->on_button_release(widget, event);
        widgets = widgets->next;
    }
}

static void inaudible_window_on_button_press(InaudibleWindow*       window,
                                             const PuglEventButton* event)
{
    InaudibleLinkedList* widgets = window->widgets;

    int x = event->x;
    int y = event->y;

    while (widgets)
    {
        InaudibleWidget* widget = inaudible_linkedlist_get_value(widgets);
        if (mouse_in_widget(widget, x, y))
            widget->on_button_press(widget, event);
        widgets = widgets->next;
    }
}

static void inaudible_window_on_mouse_move(InaudibleWindow*       window,
                                           const PuglEventMotion* event)
{
    InaudibleLinkedList* widgets = window->widgets;

    int x = event->x;
    int y = event->y;

    while (widgets)
    {
        InaudibleWidget* widget = inaudible_linkedlist_get_value(widgets);
        if (mouse_in_widget(widget, x, y) || widget->has_grab)
            widget->on_mouse_move(widget, event);
        widgets = widgets->next;
    }
}

void
inaudible_window_show(InaudibleWindow* window)
{
   puglShowWindow(window->view);
}

static void
onButtonPress(PuglView*        view,
              const PuglEvent* event)
{
    InaudibleWindow* window = puglGetHandle(view);
    if (window->on_button_press)
        window->on_button_press(window, &(event->button));
}

static void
onButtonRelease(PuglView*        view,
                const PuglEvent* event)
{
    InaudibleWindow* window = puglGetHandle(view);
    if (window->on_button_release)
        window->on_button_release(window, &(event->button));
}

static void
onClose(PuglView* view)
{
    InaudibleWindow* window = puglGetHandle(view);
    inaudible_app_close_window(window);
}

static void
onDisplay(PuglView* view)
{
    cairo_t* cr = puglGetContext(view);

    InaudibleWindow* window = puglGetHandle(view);
    InaudibleLinkedList* widgets = window->widgets;
    InaudibleWidget* widget;

    while (widgets)
    {
        widget = inaudible_linkedlist_get_value(widgets);
        widget->draw(widget, &cr);
        cairo_fill(cr); // Ensure widget to be drawn.
        widgets = widgets->next;
    }
}

static void
onMotion(PuglView*        view,
         const PuglEvent* event)
{
    InaudibleWindow* window = puglGetHandle(view);
    if (window->on_mouse_move)
        window->on_mouse_move(window, &(event->motion));
    onDisplay(view);
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
            onButtonPress(view, event);
    		break;
        case PUGL_BUTTON_RELEASE:
            onButtonRelease(view, event);
            break;
    	case PUGL_ENTER_NOTIFY:
    		//puglPostRedisplay(view);
    		break;
    	case PUGL_LEAVE_NOTIFY:
    		//puglPostRedisplay(view);
    		break;
        case PUGL_MOTION_NOTIFY:
            onMotion(view, event);
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

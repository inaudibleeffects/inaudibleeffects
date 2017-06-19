#ifndef __INAUDIBLE_WIDGET_H__
#define __INAUDIBLE_WIDGET_H__

#include <cairo/cairo.h>
#include "pugl/pugl.h"
#include "macros.h"

typedef struct widget {

    int     x;
    int     y;
    int     height;
    int     width;

    void*   child;

    bool    has_focus;
    bool    has_grab;

    void    (*destroy)(void* widget);
    void    (*draw)(void* widget, cairo_t* context);
    void    (*on_button_press)(struct widget* widget, const PuglEventButton* event);
    void    (*on_button_release)(struct widget* widget, const PuglEventButton* event);
    void    (*on_mouse_move)(struct widget* widget, const PuglEventMotion* event);

} InaudibleWidget;

void inaudible_widget_destroy(InaudibleWidget* widget);

#endif

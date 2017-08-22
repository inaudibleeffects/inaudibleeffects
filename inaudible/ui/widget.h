#ifndef __INAUDIBLE_WIDGET_H__
#define __INAUDIBLE_WIDGET_H__


#include <cairo/cairo.h>
#include "pugl/pugl.h"
#include "macros.h"


#define INAUDIBLE_FROM_WIDGET(obj) obj->child


typedef struct widget {

    int     x;
    int     y;
    int     height;
    int     width;

    void*   child;

    bool    has_focus;
    bool    has_grab;

    void    (*destroy)          (struct widget*         widget);
    void    (*draw)             (struct widget*         widget,
                                 cairo_t**              context);
    void    (*on_button_press)  (struct widget*         widget,
                                 const PuglEventButton* event);
    void    (*on_button_release)(struct widget*         widget,
                                 const PuglEventButton* event);
    void    (*on_mouse_move)    (struct widget*         widget,
                                 const PuglEventMotion* event);

} InaudibleWidget;


void inaudible_widget_destroy(InaudibleWidget* widget);


#endif //__INAUDIBLE_WIDGET_H__

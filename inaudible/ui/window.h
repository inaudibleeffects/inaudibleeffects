#ifndef __INAUDIBLE_WINDOW_H__
#define __INAUDIBLE_WINDOW_H__


#include <cairo/cairo.h>
#include "ui/widget.h"
#include "ui/window.h"
#include "types/dictionary.h"
#include "types/linkedlist.h"


typedef struct Window {

    bool                 closing;
    const char*          title;
    PuglView*            view;
    InaudibleLinkedList* widgets;

    void                 (*on_button_press)     (struct Window*         window,
                                                 const PuglEventButton* event);
    void                 (*on_button_release)   (struct Window*         window,
                                                 const PuglEventButton* event);
    void                 (*on_mouse_move)       (struct Window*         window,
                                                 const PuglEventMotion* event);

} InaudibleWindow;


InaudibleWindow* inaudible_window_new       (PuglNativeWindow parent,
                                             const char*      title,
                                             const int        width,
                                             const int        height,
                                             const bool       resizable);
void             inaudible_window_destroy   (InaudibleWindow* window);

void             inaudible_window_close     (InaudibleWindow* window);
void             inaudible_window_show      (InaudibleWindow* window);
void             inaudible_window_add_widget(InaudibleWindow* window,
                                             InaudibleWidget* widget);
PuglView*        inaudible_window_get_view  (InaudibleWindow* window);

static void      onButtonPress              (PuglView*        view,
                                             const PuglEvent* event);
static void      onButtonRelease            (PuglView*        view,
                                             const PuglEvent* event);
static void      onDisplay                  (PuglView*        view);
static void      onClose                    (PuglView*        view);
static void      onEvent                    (PuglView*        view,
                                             const PuglEvent* event);


#endif //__INAUDIBLE_WINDOW_H__

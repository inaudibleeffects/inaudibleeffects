#ifndef __INAUDIBLE_WINDOW_H__
#define __INAUDIBLE_WINDOW_H__

#include <cairo/cairo.h>
#include "pugl/pugl.h"
#include "widget.h"
#include "types/dictionary.h"
#include "types/linkedlist.h"

typedef struct {
    bool                 closing;
    const char*          title;
    PuglView*            view;
    InaudibleLinkedList* widgets;

    void                 (*onMouseMove)(const int x, const int y);
} InaudibleWindow;

InaudibleWindow* inaudible_window_new(const char* title, const int width, const int height, bool resizable);
void             inaudible_window_destroy(InaudibleWindow* window);

void             inaudible_window_close(InaudibleWindow* window);
void             inaudible_window_show(InaudibleWindow* window);
void             inaudible_window_add_widget(InaudibleWindow* window, InaudibleWidget* widget);
PuglView*        inaudible_window_get_view(InaudibleWindow* window);

static void      onDisplay(PuglView* view);
static void      onClose(PuglView* view);
static void      onEvent(PuglView* view, const PuglEvent* event);

#endif //__INAUDIBLE_WINDOW_H__

#ifndef __INAUDIBLE_APP_H__
#define __INAUDIBLE_APP_H__

#include "types/linkedlist.h"
#include "ui/window.h"

typedef struct {
    int quit;
    InaudibleLinkedList* windows;
    //InaudibleWindow* window;
} InaudibleApp;

InaudibleApp* app;

void          inaudible_app();
void          inaudible_app_run();
void          inaudible_app_quit();

void          inaudible_app_show_window(InaudibleWindow* window);
void          inaudible_app_close_window(InaudibleWindow* window);

#endif //__INAUDIBLE_APP_H__

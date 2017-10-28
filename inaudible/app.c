#include "app.h"

#include <stdio.h>
#include <unistd.h>

bool running;
void
inaudible_app()
{
    app = INAUDIBLE_NEW(InaudibleApp);
    app->windows = NULL;
    app->quit = 0;

    running = false;
}

void
inaudible_app_iteration()
{
    InaudibleLinkedList* windows = app->windows;

    if (!windows)
        return;

    InaudibleWindow* window = windows->data;

    while (window)
    {
        puglWaitForEvent(window->view);
        puglProcessEvents(window->view);

        if (window->closing)
            break;

        if (windows->next != NULL)
        {
            windows = windows->next;
            window = windows->data;
        }
        else
        {
            break;
        }
	}
}

void
inaudible_app_run()
{
    if (running)
        return;

    running = true;

    InaudibleLinkedList* windows = app->windows;
    InaudibleWindow* window = windows->data;

    while (window)
    {
        puglWaitForEvent(window->view);
        puglProcessEvents(window->view);

        if (window->closing)
            break;

        if (windows->next != NULL)
            windows = windows->next;
        else
            windows = app->windows;

        window = windows->data;
	}
}

void
inaudible_app_quit()
{
    printf("Quitting...\n");
    inaudible_linkedlist_destroy(app->windows);
    INAUDIBLE_DESTROY(app);
}

void
inaudible_app_show_window(InaudibleWindow* window)
{
    inaudible_linkedlist_add(&(app->windows), window);
    inaudible_window_show(window);
}

void
inaudible_app_close_window(InaudibleWindow* window)
{
    inaudible_linkedlist_remove(&(app->windows), window);
    inaudible_window_close(window);
}

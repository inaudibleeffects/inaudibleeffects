#include "app.h"

void
inaudible_app()
{
    app = INAUDIBLE_NEW(InaudibleApp);
    app->windows = NULL;
    app->quit = 0;
}

void
inaudible_app_run()
{
    if (app->windows == NULL)
        return;

    InaudibleLinkedList* list = app->windows;
    InaudibleWindow* window = list->data;

    while (!app->quit)
    {
        if (window->view == NULL)
        {
            printf("Closing...\n");
            inaudible_window_destroy(window);
            list->data = NULL;
            app->quit = 1;
            break;
        }

        puglWaitForEvent(window->view);
    	puglProcessEvents(window->view);

        if (list->next == NULL)
            list = app->windows;
        else
            list = list->next;
	}
}

void
inaudible_app_quit()
{
    app->quit = 1;
    if (app->windows != NULL)
        inaudible_linkedlist_destroy(app->windows);
    INAUDIBLE_DESTROY(app);
}

void
inaudible_app_show_window(InaudibleWindow* window)
{
    if (app->windows == NULL)
        app->windows = inaudible_linkedlist_new(window);
    else
        app->windows->data = window;

    printf("%d - %s\n", &window, window->title);
    inaudible_window_show(window);
}

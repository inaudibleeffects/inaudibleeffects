#include "eventhandler.h"

struct InaudibleEventHandler
{
    InaudibleLinkedList* events;
};

InaudibleEventHandler*
inaudible_eventhandler_new()
{
    InaudibleEventHandler* handler = INAUDIBLE_NEW(InaudibleEventHandler);
    handler->events = inaudible_linkedlist_new();
    return handler;
}

void inaudible_eventhandler_destroy(InaudibleEventHandler* handler) {
    inaudible_linkedlist_destroy(handler->events);
    INAUDIBLE_DESTROY(handler);
}

void
inaudible_eventhandler_register(InaudibleEventHandler* handler,
                         void                          (*event))
{
    inaudible_linkedlist_add(&(handler->events), &event);
}

void
inaudible_eventhandler_unregister(InaudibleEventHandler* handler,
                                void                     (*event))
{
    inaudible_linkedlist_remove(&(handler->events), &event);
}

void
inaudible_eventhandler_raise(InaudibleEventHandler* handler)
{
    InaudibleLinkedList* events = handler->events;
    void* callback;
    while (events)
    {
        callback = inaudible_linkedlist_get_value(events);
        events = events->next;
    }
}

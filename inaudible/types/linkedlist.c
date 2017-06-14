#include "types/linkedlist.h"
#include <stdio.h>

InaudibleLinkedList*
inaudible_linkedlist_new(void* data)
{
    return NULL;
}

void
inaudible_linkedlist_destroy(InaudibleLinkedList* list)
{
    if (!list)
        return;

    if (list->next)
        inaudible_linkedlist_destroy(list->next);

    INAUDIBLE_DESTROY(list);
}

void
inaudible_linkedlist_add(InaudibleLinkedList** list,
                         void*                 data)
{
    InaudibleLinkedList* root = *list;

    while (root)
    {
        // Don't add the same key.
        if (root->data == data)
            return;

        root = root->next;
    }

    root = INAUDIBLE_NEW(InaudibleLinkedList);
    root->data = data;
    root->next = NULL;

    *list = root;
}

int
inaudible_linkedlist_count(InaudibleLinkedList* list)
{
    int i = 0;
    while (list)
    {
        i++;
        list = list->next;
    }
    return i;
}

void*
inaudible_linkedlist_get_value(InaudibleLinkedList* list)
{
    return list->data;
}

void inaudible_linkedlist_remove(InaudibleLinkedList** list,
                                 void*                 data)
{
    InaudibleLinkedList* root = *list;
    InaudibleLinkedList* previous = NULL;

    while (root)
    {
        if (root->data == data)
        {
            InaudibleLinkedList* current = root;

            if (previous != NULL)
                previous->next = root->next;
            else
                *list = root->next;

            INAUDIBLE_DESTROY(current);

            return;
        }

        previous = root;
    }
}

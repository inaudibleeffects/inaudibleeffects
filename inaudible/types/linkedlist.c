#include "types/linkedlist.h"
#include <stdio.h>

InaudibleLinkedList*
inaudible_linkedlist_new()
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

    // Create new node.
    InaudibleLinkedList* new = INAUDIBLE_NEW(InaudibleLinkedList);
    new->data = data;
    new->next = NULL;

    // Add at root if list is empty.
    if (!*list)
    {
        *list = new;
        return;
    }

    // Put it at the end.
    while (root->next)
        root = root->next;

    root->next = new;
}

int
inaudible_linkedlist_count(InaudibleLinkedList* list)
{
    InaudibleLinkedList* root = list;

    int i = 0;

    while (root)
    {
        i++;
        root = root->next;
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

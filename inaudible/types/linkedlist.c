#include "types/linkedlist.h"

InaudibleLinkedList*
inaudible_linkedlist_new(void* data)
{
    InaudibleLinkedList* list = INAUDIBLE_NEW(InaudibleLinkedList);
    list->data = data;
    list->next = NULL;

    return list;
}

void
inaudible_linkedlist_destroy(InaudibleLinkedList* list)
{
    if (list->next != NULL)
        inaudible_linkedlist_destroy(list->next);

    INAUDIBLE_DESTROY(list);
}

void
inaudible_linkedlist_add(InaudibleLinkedList* list,
                         void*                data)
{
    InaudibleLinkedList* root = list;

    if (root != NULL)
    {
        while (root->next != NULL)
            root = root->next;
    }

    root->next = INAUDIBLE_NEW(InaudibleLinkedList);
    root->next->data = data;
    root->next->next = NULL;
}

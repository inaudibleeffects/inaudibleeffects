#ifndef __INAUDIBLE_LINKEDLIST_H__
#define __INAUDIBLE_LINKEDLIST_H__

#include "macros.h"

typedef struct linkedlist {
    void* data;
    struct linkedlist* next;
} InaudibleLinkedList;

InaudibleLinkedList* inaudible_linkedlist_new(void* data);
void                 inaudible_linkedlist_destroy(InaudibleLinkedList* list);

void                 inaudible_linkedlist_add(InaudibleLinkedList** list, void* data);
int                  inaudible_linkedlist_count(InaudibleLinkedList* list);
void                 inaudible_linkedlist_remove(InaudibleLinkedList** list, void* data);
void                 inaudible_linkedlist_remove2(InaudibleLinkedList* list, void* data);

#endif //__INAUDIBLE_LINKEDLIST_H__

#ifndef __INAUDIBLE_LINKEDLIST_H__
#define __INAUDIBLE_LINKEDLIST_H__

#include "macros.h"

typedef struct node {
    void* data;
    struct node* next;
} InaudibleLinkedList;

InaudibleLinkedList* inaudible_linkedlist_new(void* data);
void                 inaudible_linkedlist_destroy(InaudibleLinkedList* list);

void                 inaudible_linkedlist_add(InaudibleLinkedList* list, void* data);

#endif

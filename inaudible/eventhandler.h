#ifndef __INAUDIBLE_EVENTHANDLER_H__
#define __INAUDIBLE_EVENTHANDLER_H__


#include "types/linkedlist.h"

typedef struct InaudibleEventHandler InaudibleEventHandler;

InaudibleEventHandler* inaudible_eventhandler_new();
void                   inaudible_eventhandler_destroy();

void                   inaudible_eventhandler_raise(InaudibleEventHandler* handler);
void                   inaudible_eventhandler_register(InaudibleEventHandler* handler,void (*event));
void                   inaudible_eventhandler_unregister(InaudibleEventHandler* handler, void (*event));


#endif //__INAUDIBLE_EVENTHANDLER_H__

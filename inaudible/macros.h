#ifndef __INAUDIBLE_MACROS_H__
#define __INAUDIBLE_MACROS_H__

#include <stdlib.h>

#define INAUDIBLE_NEW(type) (type*)calloc(1, sizeof(type))
#define INAUDIBLE_DESTROY(object) \
    free(object); \
    object = NULL

#define INAUDIBLE_WIDGET(widget) &(widget->parent))

#endif

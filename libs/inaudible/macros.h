#include <stdlib.h>

#define INAUDIBLE_NEW(type) (type*)calloc(1, sizeof(type))
#define INAUDIBLE_DESTROY(object) \
    free(object); \
    object = NULL

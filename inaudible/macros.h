#ifndef __INAUDIBLE_MACROS_H__
#define __INAUDIBLE_MACROS_H__

#include <stdlib.h>

#define INAUDIBLE_NEW(type) (type*)calloc(1, sizeof(type))
#define INAUDIBLE_DESTROY(object) \
    free(object); \
    object = NULL

#define INAUDIBLE_EXTERNAL_PNG(obj) extern char _binary_##obj##_png_start[]; \
                                    extern char _binary_##obj##_png_end[]
#define INAUDIBLE_EXTERNAL_PNG_START(obj) _binary_##obj##_png_start
#define INAUDIBLE_EXTERNAL_PNG_SIZE(obj) _binary_##obj##_png_start - \
                                         _binary_##obj##_png_end

#endif

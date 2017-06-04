#ifndef __INAUDIBLE_WIDGET_H__
#define __INAUDIBLE_WIDGET_H__

#include "inaudible/drawable.h"
#include "inaudible/macros.h"
#include "inaudible/pixbuf.h"

typedef struct
{
    InaudibleDrawable base;

    int x;
    int y;
} InaudibleWidget;

#endif

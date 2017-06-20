#include "widget.h"

void inaudible_widget_destroy(InaudibleWidget* widget)
{
    widget->destroy(widget);
    INAUDIBLE_DESTROY(widget);
}

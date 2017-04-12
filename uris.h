#ifndef ECHOIZER_URIS_H
#define ECHOIZER_URIS_H

#include "lv2/lv2plug.in/ns/ext/atom/atom.h"
#include "lv2/lv2plug.in/ns/ext/atom/forge.h"
#include "lv2/lv2plug.in/ns/ext/parameters/parameters.h"
#include "lv2/lv2plug.in/ns/ext/urid/urid.h"
#include "lv2/lv2plug.in/ns/ext/log/log.h"
#include "lv2/lv2plug.in/ns/ext/state/state.h"
#include "lv2/lv2plug.in/ns/ext/patch/patch.h"

#define ECHOIZER_URI "http://inaudibleeffects.github.io/echoizer"

typedef struct {
    //LV2_URID atom_Float;
	//LV2_URID atom_Resource;
    //LV2_URID atom_Sequence;
	LV2_URID atom_URID;
    //LV2_URID atom_eventTransfer;

    LV2_URID ui_Delay;
    LV2_URID ui_Feedback;
    LV2_URID ui_Blend;

    //LV2_URID patch_Get;
	//LV2_URID patch_Set;
    //LV2_URID patch_accept;
	//LV2_URID patch_property;
    //LV2_URID patch_value;
} EchoizerURIs;

static inline void
map_echoizer_uris(LV2_URID_Map* map, EchoizerURIs* uris)
{
    printf("Mapping...\n");
    //uris->atom_Float         = map->map(map->handle, LV2_ATOM__Float);
    printf("Did map\n");
    //uris->atom_Resource      = map->map(map->handle, LV2_ATOM__Resource);
    //uris->atom_Sequence      = map->map(map->handle, LV2_ATOM__Sequence);
    uris->atom_URID          = map->map(map->handle, LV2_ATOM__URID);
    //uris->atom_eventTransfer = map->map(map->handle, LV2_ATOM__eventTransfer);

    uris->ui_Delay           = map->map(map->handle, ECHOIZER_URI "#uiDelay");
    uris->ui_Feedback        = map->map(map->handle, ECHOIZER_URI "#uiFeedback");
    uris->ui_Blend           = map->map(map->handle, ECHOIZER_URI "#uiBlend");

    //uris->patch_Get          = map->map(map->handle, LV2_PATCH__Get);
    //uris->patch_Set          = map->map(map->handle, LV2_PATCH__Set);
    //uris->patch_accept       = map->map(map->handle, LV2_PATCH_PREFIX "accept" );
    //uris->patch_property     = map->map(map->handle, LV2_PATCH__property);
    //uris->patch_value        = map->map(map->handle, LV2_PATCH__value);
}

#endif

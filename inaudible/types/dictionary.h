#ifndef __INAUDIBLE_DICTIONARY_H__
#define __INAUDIBLE_DICTIONARY_H__

#include "macros.h"

typedef struct dictionary {
    void* key;
    void* value;
    struct dictionary* next;
} InaudibleDictionary;

InaudibleDictionary* inaudible_dictionary_new();
void                 inaudible_dictionary_destroy(InaudibleDictionary* dictionary);

void                 inaudible_dictionary_add(InaudibleDictionary** dictionary, void* key, void* value);
void*                inaudible_dictionary_get_value(InaudibleDictionary* dictionary, void* key);
void                 inaudible_dictionary_remove(InaudibleDictionary** dictionary, void* key);

#endif //__INAUDIBLE_DICTIONARY_H__

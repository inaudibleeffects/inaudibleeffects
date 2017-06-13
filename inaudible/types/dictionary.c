#include "types/dictionary.h"

InaudibleDictionary* inaudible_dictionary_new()
{
    return NULL;
}

void inaudible_dictionary_destroy(InaudibleDictionary* dictionary)
{
    INAUDIBLE_DESTROY(dictionary);
}

void inaudible_dictionary_add(InaudibleDictionary** dictionary,
                              void*                 key,
                              void*                 value)
{
    InaudibleDictionary* root = *dictionary;

    while (root)
    {
        // Don't add the same key.
        if (root->key == key)
            return;

        root = root->next;
    }

    root = INAUDIBLE_NEW(InaudibleDictionary);
    root->key = key;
    root->value = value;
    root->next = NULL;

    *dictionary = root;
}

void inaudible_dictionary_remove(InaudibleDictionary** dictionary,
                                 void*                key)
{
    InaudibleDictionary* root = *dictionary;
    InaudibleDictionary* previous = NULL;

    while (root)
    {
        if (root->key == key)
        {
            InaudibleDictionary* current = root;

            if (previous != NULL)
                previous->next = root->next;
            else
                *dictionary = root->next;

            INAUDIBLE_DESTROY(current);

            return;
        }

        previous = root;
    }
}

void* inaudible_dictionary_get_value(InaudibleDictionary* dictionary, void* key)
{
    InaudibleDictionary* root = dictionary;

    while (root)
    {
        if (root->key == key)
            return root->value;

        root = root->next;
    }

    return NULL;
}

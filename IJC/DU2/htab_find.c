#include "htab_public.h"
#include "htab.h"

htab_pair_t* htab_find(htab_t* t, htab_key_t key)
{
    for(size_t i = 0; i < t->arr_size; i++)
    {
        htab_item *tepItem = t->htabitemPole[i];
        while(tepItem != NULL)
        {
            if(strcmp(tepItem->key, key) == 0)
            {
                return (htab_pair_t*)tepItem;
            }

            tepItem = t->htabitemPole[i]->next;
        }
    }

    return NULL;
}

#include <stdlib.h>

#include "htab.h"
#include "htab_public.h"

void htab_clear(htab_t * t)
{
    for(size_t i = 0; i< t->arr_size; i++)
    {
        if(t->htabitemPole[i] == NULL)
        {
            continue;
        }

        htab_item* tempItem = t->htabitemPole[i];

        while(tempItem->next != NULL)
        {
            htab_item* tempItemNext = tempItem->next;

            free((char*)tempItem->key);
            free(tempItem);

            tempItem = tempItemNext;
        }

        if(tempItem != NULL)
        {
            free((char*)tempItem->key);
            free(tempItem);
        }
    }
}

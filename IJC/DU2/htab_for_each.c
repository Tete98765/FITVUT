#include <stdlib.h>

#include "htab.h"
#include "htab_public.h"

void htab_for_each(const htab_t* t, void (*f)(htab_pair_t *data))
{
    for(size_t i = 0; i < t->arr_size; i++)
    {
        htab_item* tempItem = t->htabitemPole[i];

        while(tempItem != NULL)
        {
            (*f)((htab_pair_t*)tempItem);
            tempItem = tempItem->next;
        }
    }

}

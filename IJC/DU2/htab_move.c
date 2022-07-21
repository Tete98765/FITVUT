#include <stdlib.h>
#include "htab_public.h"

htab_t *htab_move(size_t n, htab_t *from)
{
    htab_t* novaTabulka = (htab_t*)calloc(n, (sizeof(htab_t)) + (n * sizeof(htab_item)));

    if(from != NULL)
    {
        novaTabulka->arr_size = from->arr_size;
        novaTabulka->size = from->size;

        for(size_t i = 0; i < from->arr_size; i++)
        {
            novaTabulka->htabitemPole[i] = from->htabitemPole[i];
            from->htabitemPole[i] = NULL;
        }

    }

    return novaTabulka;
}

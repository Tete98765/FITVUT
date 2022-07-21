#include <stdlib.h>

#include "htab.h"
#include "htab_public.h"

htab_t *htab_init(size_t n)
{
    htab_t* tabulka = calloc(n, sizeof(htab_t) + (n * sizeof(htab_item)));

    if(tabulka == NULL)
        return NULL;

    if(tabulka->htabitemPole == NULL)
    {
        return NULL;
    }

    tabulka->arr_size = n;
    tabulka->size = 0;

    return tabulka;
}

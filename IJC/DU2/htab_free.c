#include <stdlib.h>

#include "htab.h"
#include "htab_public.h"

void htab_free(htab_t * t)
{
    htab_clear(t);
    free(t);
}

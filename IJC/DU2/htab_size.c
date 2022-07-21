#include "htab.h"
#include "htab_public.h"

size_t htab_size(const htab_t* t)
{
    return t->size;
}

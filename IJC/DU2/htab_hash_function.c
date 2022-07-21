#include <stdint.h>
#include "htab.h"

size_t htab_hash_function(const char *str)
{
    uint32_t h=0;
    const unsigned char *p;

    for(p=(const unsigned char*)str; *p!='\0'; p++)
        h = 65599*h + *p;

    return h;
}

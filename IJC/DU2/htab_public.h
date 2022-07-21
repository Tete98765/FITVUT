#ifndef _HTAB_PUBLIC_H
#define _HTAB_PUBLIC_H

#include <string.h>
#include "htab.h"


typedef struct htab_item
{
    htab_key_t    key;
    htab_value_t  value;
    struct htab_item *next;
}htab_item;

struct htab
{
   size_t arr_size; // vrati pocet prvkov v poli
   size_t size; // vrati pocet prvkov v tabulke
   struct htab_item *htabitemPole[];
};

/*
typedef struct htab
{
   size_t arr_size; // vrati pocet prvkov v poli
   size_t size; // vrati pocet prvkov v tabulke
   struct htab_item *htabitemPole[];
}htab_t;
*/

#endif

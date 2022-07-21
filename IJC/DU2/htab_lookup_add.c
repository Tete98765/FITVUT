#include <stdio.h>
#include <stdlib.h>

#include "htab_public.h"
#include "htab.h"
#include "string.h"


htab_pair_t* htab_lookup_add(htab_t * t, htab_key_t key)
{

    size_t hashIndex = (htab_hash_function(key) % t->arr_size);
    htab_item* zaznam = t->htabitemPole[hashIndex];

    // hladanie ci uz zaznam existuje ak ano zvys value a vrat htab_pair_t*
    while(zaznam != NULL)
    {
        if(strcmp(zaznam->key, key) == 0)
        {
            zaznam->value++;
            return (htab_pair_t*)zaznam;
        }

        if(zaznam->next != NULL) // ak su este v zozname pod vygenerovanim hashIndex nejake slova skontroluj aj tie
            zaznam = zaznam->next;
        else
            break; // ak nie sme na konci break;
    }

    // sekcia : pridavanie noveho itemu
    htab_item* novyZaznam = (htab_item*)calloc(1, sizeof(htab_item));

    if(novyZaznam == NULL)
    {
        fprintf(stderr, "Chyba pri ukladani pozloky: nepodarilo sa alokovat pamat pre novu polozku\n");
        return NULL;
    }

    novyZaznam->key = calloc(1, sizeof(char)*(strlen(key) + 1)); // +1 kvoli ukoncovaciemu znaku

	if (novyZaznam->key == NULL)
	{
		fprintf(stderr,"Alokacia sa nepodarila!\n");
		return NULL;
	}

	// okopirovanie klucu
    strcpy((char*)novyZaznam->key, (char*)key);

    novyZaznam->next = NULL; // zatial na dalsi pointer nebude ukazovat kedze ide na koniec alebo kedze je to prve slovo kedze taky indexHash este nebol vygenerovani
    novyZaznam->value = 1; // zvysenie na hodnotu 1 - vyskyt

    if(zaznam == NULL) // ak takyto indexHash este nebol vynegerovani ukazuje novy item na index s hodnotou NULL
    {
        t->htabitemPole[hashIndex] = novyZaznam;
    }
    else
    {
        zaznam->next = novyZaznam;
    }

    return (htab_pair_t*)novyZaznam;
}


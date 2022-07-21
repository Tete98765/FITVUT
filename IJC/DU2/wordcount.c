#include <stdlib.h>
#include <stdio.h>

#include "htab.h"
#include "htab_public.h"
#include "io.h"

#define VELKOST_TABULKY 1024
#define MAX_DLZKA_SLOVA 127

/*
    Po testovani aplikacie som usudila ze velkost 1024 je najoptimalnejsia pre moj testovaci subor ktory obsahoval 3000 slov
    Pri malej velkosti tabulky program pracoval pomalsie no pri vacsej velkosti pracoval rychlejsie.
    Pri velkosti 2048 bola tabulka prilis velka a teda zbytocne prealokovana
*/

void vypis(htab_pair_t* data)
{
    printf("%s  %d\n", data->key, data->value);
}

int main()
{

    htab_t* tabulka = htab_init(VELKOST_TABULKY);

    if(tabulka == NULL)
    {
        fprintf(stderr, "Alokacia tabulky sa nepodarila \n");
        return 1;
    }

    char* slovo = calloc(MAX_DLZKA_SLOVA, sizeof(char));

    if(slovo == NULL)
    {
        fprintf(stderr, "Alokacia slova o velkosti MAX - 127 sa nepodarila \n");
        return 1;
    }

    while(read_word(slovo, MAX_DLZKA_SLOVA, stdin) != -1)
    {
        htab_pair_t* pair = htab_lookup_add(tabulka, slovo);

        if(pair == NULL)
            break;

        memset(slovo, 0, MAX_DLZKA_SLOVA);
    }

    htab_for_each(tabulka, &vypis);

    htab_free(tabulka);
    free(slovo);

    return 0;
}

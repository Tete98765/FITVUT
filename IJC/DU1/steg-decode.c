#include <stdio.h>
/*steg-decode.c
 * @Řešení IJC-DU1, příklad b), 23.3.2021
 * @Fakulta: VUT FIT
 * @přeloženo gcc 2.32-1
 */

#include <stdlib.h>
#include <stdbool.h>
#include "ppm.h"
#include "bitset.h"
#include "eratosthenes.h"
#include "error.h"

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        error_exit("ZLy pocet vstupnych argumentov aplikace. Zadajte steg-decode a nazov ppm suboru \n");
    }

    struct ppm* ppmObrazok = ppm_read(argv[1]);
    if (!ppmObrazok)
    {
        error_exit("Zly obsah obrazku \n");
    }

    unsigned long velkost = ppmObrazok->xsize * ppmObrazok->ysize * RGB * CHAR_BIT; // size of char mozno chyba pozor
    unsigned long* ptrPole;
    bitset_alloc(ptrPole, velkost);
    Eratosthenes(ptrPole); // nastavenie prvocisiel
    bool koniecRetazca = false;
    char pismeno = ' ';

    int poziciaBitu = 0;
    unsigned long znak[2] = {0};

    for (int i = 23; i < velkost; i++)
    {
        if (bitset_getbit(ptrPole, i) == 0)
        {
            pismeno |= ((ppmObrazok->data[i] & 0x01) << poziciaBitu);   
            poziciaBitu++;

            if (poziciaBitu == CHAR_BIT)
            {
            	
                printf("%c", pismeno);

                if (pismeno == '\0')
                {
                    koniecRetazca = true;
                    break;
                }

                pismeno = '\0';
                poziciaBitu = 0;
            }
        }
    }

    if (!koniecRetazca)
    {
        bitset_free(ptrPole);
        ppm_free(ppmObrazok);
        error_exit("Zasifrovana sprava neobsahuje ukoncovaci znak retazca \n");
    }

    bitset_free(ptrPole);
    ppm_free(ppmObrazok);

    return 0;
}
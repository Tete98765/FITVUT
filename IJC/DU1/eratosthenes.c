/*eratosthenes.c
 * @Řešení IJC-DU1, příklad a), 23.3.2021
 * @Fakulta: VUT FIT
 * @přeloženo gcc 2.32-1
 */

#include <stdio.h>
#include <math.h>
#include "eratosthenes.h"

void Eratosthenes(bitset_t pole)
{
    unsigned long velkost = bitset_size(pole);
    unsigned long velkostOdmocnena = sqrt(velkost);

    bitset_setbit(pole, 0,  1);
    bitset_setbit(pole, 1,  1);
    
    // Eratosthenovo alghorithm
    for(unsigned long i = 2; i <= velkostOdmocnena; i++)
    {
        if(bitset_getbit(pole, i) == 0)
        {
            for(unsigned long j = 2; i * j < velkost; j++)
            {
                bitset_setbit(pole,  i * j, 1);
            }
        }
    }
}


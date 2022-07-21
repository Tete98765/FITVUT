/*primes.c
 * @Řešení IJC-DU1, příklad b), 23.3.2021
 * @Fakulta: VUT FIT
 * @přeloženo gcc 2.32-1
 */

#include <stdio.h>
#include <time.h>

#include "eratosthenes.h"
#include "bitset.h"

#define N 20000000


int main()
{
    clock_t start = clock();
    unsigned long polePrvoCisiel[10];
    int pocitadlo = 0;

    bitset_create(arr, N);
    Eratosthenes(arr);

    for(unsigned long i = N - 1; i > 1; i--)
    {
        if(bitset_getbit(arr, i) == 0)
        {
            polePrvoCisiel[pocitadlo++] = i;

            if(pocitadlo == 10)
                break;
        }
    }

    for(int j = pocitadlo; j >=1; j--)
    {
        printf("%ld \n", polePrvoCisiel[j]);
    }

    fprintf(stderr, "Time %.3g\n", (double)(clock() - start)/ CLOCKS_PER_SEC);
    return 0;
}

/*bitset.h
 * @Řešení IJC-DU1, příklad a), 23.3.2021
 * @Fakulta: VUT FIT
 * @přeloženo gcc 2.32-1
 */

#ifndef IJC_DU1_BITSET_H
#define IJC_DU1_BITSET_H

#define static_assert _Static_assert

#include <limits.h>
#include <assert.h>
#include <stdlib.h> 
#include "ppm.h"
#include "error.h"

typedef unsigned long bitset_t[];
typedef unsigned long bitset_index_t;

#define bitset_create(jmeno_pole, velikost) unsigned long jmeno_pole[(((velikost / (sizeof(unsigned long) * CHAR_BIT)) + (velikost % (sizeof(unsigned long) * CHAR_BIT) > 0 ? 1 : 0))) + 1] =  {velikost, 0}; \
static_assert(velikost > 0 &&  velikost < ULONG_MAX, "Zadana velkost je mensia ako nula alebo je vacsia ako maximalna hodnot  ktora sa da ulozit do unsigned long")

#define bitset_alloc(jmeno_pole, velikost) \
{\
assert(velikost < MAX_LIMIT && "bitset_alloc: Chyba alokacie pozadovanej velkosti pamati , prekrocenie maximalneho limitu \n"); \
jmeno_pole = calloc(velikost / (sizeof(unsigned long) * CHAR_BIT) + (velikost % (sizeof(unsigned long) * CHAR_BIT) > 0 ? 1 : 0) + 1, sizeof(bitset_index_t)); \
jmeno_pole[0] = velikost; \
}
                                                                                                                                                                                                          
#define BIT_GET(jmeno_pole, index) (unsigned long)((jmeno_pole[index / (sizeof(unsigned long) * CHAR_BIT) +1] & ((1UL << (index % (sizeof(unsigned long) * CHAR_BIT))))) != 0)
                                                                                
#define BIT_SET(jmeno_pole, index, vyraz) vyraz == 1 ? (jmeno_pole[index / (sizeof(unsigned long) * CHAR_BIT) + 1] |= (( 1UL << (index % (sizeof(unsigned long) * CHAR_BIT)))))  :  (jmeno_pole[index / (sizeof(unsigned long) * CHAR_BIT) + 1] &= (( 1UL << ~(index % (sizeof(unsigned long) * CHAR_BIT)))))
                                                   

#ifndef USE_INLINE

#define bitset_size(jmeno_pole) jmeno_pole[0]

#define bitset_getbit(jmeno_pole, index) (index < 0 || index > bitset_size(jmeno_pole)) ? error_exit("bitset_getbit: Index %lu mimo rozsah  0..%lu", (bitset_index_t) index, (bitset_index_t) bitset_size(jmeno_pole)), 0 : BIT_GET(jmeno_pole, index)

#define bitset_setbit(jmeno_pole, index, vyraz) (index < 0 || index > bitset_size(jmeno_pole)) ? error_exit("bitset_setbit: Index %lu mimo rozsah  0..%lu", index,  bitset_size(jmeno_pole)) : BIT_SET(jmeno_pole, index, vyraz)


#define bitset_free(jmeno_pole) free(jmeno_pole)

#else

inline bitset_index_t bitset_size(bitset_t jmeno_pole)
{
    return jmeno_pole[0];
}

inline void bitset_bit(bitset_t jmeno_pole, bitset_index_t index, int vyraz)
{
    if(index < 0 || index > bitset_size(jmeno_pole) - 1)
        error_exit("bitset_setbit: Index %lu mimo rozsah inline 0..%lu", (bitset_index_t) index, (bitset_index_t) bitset_size(jmeno_pole));
    else
        BIT_SET(jmeno_pole, index, vyraz);
}

inline unsigned long bitset_getbit(bitset_t jmeno_pole, bitset_index_t index)
{
    if(index < 0 || index > bitset_size(jmeno_pole) - 1)
        error_exit("bitset_getbit: Index %lu mimo rozsah inline 0..%lu", (bitset_index_t) index, (bitset_index_t) bitset_size(jmeno_pole));
    else
         return BIT_GET(jmeno_pole, index);
}


inline void bitset_free(bitset_t jmeno_pole)
{
    free(jmeno_pole);
}

#endif

#endif


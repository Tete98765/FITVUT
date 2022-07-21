/*bitset.c
 * @Řešení IJC-DU1, příklad a), 23.3.2021
 * @Fakulta: VUT FIT
 * @přeloženo gcc 2.32-1
 */

#include "bitset.h"

extern void bitset_bit(bitset_t jmeno_pole, bitset_index_t index, int vyraz);

extern unsigned long bitset_getbit(bitset_t jmeno_pole, bitset_index_t index);

extern bitset_index_t bitset_size(bitset_t jmeno_pole);

extern void bitset_free(bitset_t jmeno_pole);



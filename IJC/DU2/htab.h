#ifndef _HTAB_H
#define _HTAB_H

#include <string.h>     // size_t
#include <stdbool.h>    // bool

// tabulka
struct htab;    // neúplná deklarace struktury - uživatel nevidí obsah
typedef struct htab htab_t;

// typy
typedef const char* htab_key_t;        // typ klíče
typedef int htab_value_t;               // typ hodnoty

typedef struct htab_pair {
    htab_key_t    key;          // kluc
    htab_value_t  value;        // asociovana hodnota - hodnota pod klucom
} htab_pair_t;                  // typedef podla zadania

// funkcie pre pracu s tabulkou
htab_t *htab_init(size_t n);   // konstruktor tabulky
htab_t *htab_move(size_t n, htab_t *from); // presun dat do novej tabulky
size_t htab_size(const htab_t * t);  // vrati pocet zaznamov v tabulke
size_t htab_bucket_count(const htab_t * t);  // vrati velkost pola

// ???
size_t htab_hash_function(htab_key_t str);

// hladania
htab_pair_t * htab_find(htab_t * t, htab_key_t key);
htab_pair_t * htab_lookup_add(htab_t * t, htab_key_t key);

// rusi dany zaznam - vyhodi ho z tabulky
bool htab_erase(htab_t * t, htab_key_t key);

// for_each: prejde vsetky zaznamy a zavola nad nimi funkciu
void htab_for_each(const htab_t * t, void (*f)(htab_pair_t *data));

// zrusi vsetky zaznamy v tabulke
void htab_clear(htab_t * t);
// destruktor tabulky - vymaze samotnuy tabulku
void htab_free(htab_t * t);

#endif

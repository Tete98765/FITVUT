/*
 * Tabuľka s rozptýlenými položkami
 *
 * S využitím dátových typov zo súboru hashtable.h a pripravených kostier
 * funkcií implementujte tabuľku s rozptýlenými položkami s explicitne
 * zreťazenými synonymami.
 *
 * Pri implementácii uvažujte veľkosť tabuľky HT_SIZE.
 */

#include "hashtable.h"
#include <stdlib.h>
#include <string.h>

int HT_SIZE = MAX_HT_SIZE;

/*
 * Rozptyľovacia funkcia ktorá pridelí zadanému kľúču index z intervalu
 * <0,HT_SIZE-1>. Ideálna rozptyľovacia funkcia by mala rozprestrieť kľúče
 * rovnomerne po všetkých indexoch. Zamyslite sa nad kvalitou zvolenej funkcie.
 */
int get_hash(char *key) {
  int result = 1;
  int length = strlen(key);
  for (int i = 0; i < length; i++) {
    result += key[i];
  }
  return (result % HT_SIZE);
}

/*
 * Inicializácia tabuľky — zavolá sa pred prvým použitím tabuľky.
 */
void ht_init(ht_table_t *table) {

  for (int i = 0; i < HT_SIZE; i++) {
    (*table)[i] = NULL;
  }
}

/*
 * Vyhľadanie prvku v tabuľke.
 *
 * V prípade úspechu vráti ukazovateľ na nájdený prvok; v opačnom prípade vráti
 * hodnotu NULL.
 */
ht_item_t *ht_search(ht_table_t *table, char *key) {

  for(size_t i = 0; i < HT_SIZE; i++) {
    ht_item_t *tep_item = (*table)[get_hash(key)];
    while(tep_item != NULL) {
        
      if(strcmp(tep_item->key, key) == 0) {
      return (ht_item_t*)tep_item;
    }

    tep_item = tep_item->next;
    }
  }

  return NULL;
}

/*
 * Vloženie nového prvku do tabuľky.
 *
 * Pokiaľ prvok s daným kľúčom už v tabuľke existuje, nahraďte jeho hodnotu.
 *
 * Pri implementácii využite funkciu ht_search. Pri vkladaní prvku do zoznamu
 * synonym zvoľte najefektívnejšiu možnosť a vložte prvok na začiatok zoznamu.
 */
void ht_insert(ht_table_t *table, char *key, float value) {

  ht_item_t *tep_item = ht_search(table, key);
  if (tep_item != NULL) {
    tep_item -> value = value;
    return;
  }
  else {
    ht_item_t *tep_item_new = malloc(sizeof(struct ht_item));
    if (tep_item_new == NULL) return;

    //int h_key = *table[get_hash(key)];

    tep_item_new -> key = key;
    tep_item_new -> value = value;
    tep_item_new -> next = (*table)[get_hash(key)];
    (*table)[get_hash(key)] = tep_item_new;  
  }
}

/*
 * Získanie hodnoty z tabuľky.
 *
 * V prípade úspechu vráti funkcia ukazovateľ na hodnotu prvku, v opačnom
 * prípade hodnotu NULL.
 *
 * Pri implementácii využite funkciu ht_search.
 */
float *ht_get(ht_table_t *table, char *key) {

  ht_item_t *tep_item = ht_search(table, key);
  if (tep_item != NULL) {
    return &(tep_item -> value);
  }
  
  return NULL;
}

/*
 * Zmazanie prvku z tabuľky.
 *
 * Funkcia korektne uvoľní všetky alokované zdroje priradené k danému prvku.
 * Pokiaľ prvok neexistuje, nerobte nič.
 *
 * Pri implementácii NEVYUŽÍVAJTE funkciu ht_search.
 */
void ht_delete(ht_table_t *table, char *key) {
  
  if (table == NULL) return;

	ht_item_t *tep_item = (*table)[get_hash(key)];
  ht_item_t *next_tep_item = NULL;

	while (tep_item -> next != NULL) {
		if (strcmp(tep_item -> key, key) == 0) {

			if (next_tep_item == NULL) (*table)[get_hash(key)] = tep_item ->next; //bol na zaciatku zoznamu
			else next_tep_item -> next = tep_item -> next;
			
			free(tep_item);
			return;
		}
		next_tep_item = tep_item;
		tep_item = tep_item->next;
	}
  
}

/*
 * Zmazanie všetkých prvkov z tabuľky.
 *
 * Funkcia korektne uvoľní všetky alokované zdroje a uvedie tabuľku do stavu po
 * inicializácii.
 */
void ht_delete_all(ht_table_t *table) {

    for(int i = 0; i < HT_SIZE; i++) {
    
        if(table[i] == NULL) {
          continue;
        }

        ht_item_t *temp_item = (*table)[i];

        while(temp_item != NULL) {
        
            ht_item_t *temp_item_next = temp_item -> next;
            free(temp_item);
            temp_item = temp_item_next;
        }
        (*table)[i] = NULL;

        if(temp_item != NULL){
            free(temp_item);
        }
    } 
}

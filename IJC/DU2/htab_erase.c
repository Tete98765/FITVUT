#include <stdlib.h>

#include "htab.h"
#include "htab_public.h"


bool htab_erase(htab_t * t, htab_key_t key)
{

    for(size_t i = 0; i < t->arr_size; i++)
    {
        if(t->htabitemPole[i] == NULL) // ak pod konkretnym indexom pola zaznamov je hned prvy item/zaznam NULL netreba pokracovat skocime na dalsi zoznam v poli zaznamov zvysenim indexu (continue)
            continue;

        if(strcmp(t->htabitemPole[i]->key, key) == 0) // ak hned prvy item obsahuje kluc ktory ideme mazat
        {
            // pripad kedy je to prvy item a existuje nieco za nimr
            if(t->htabitemPole[i]->next != NULL)
            {
                htab_item* nextTemp = t->htabitemPole[i]->next; // zoberieme dalsi item za nim do zalohy
                free(t->htabitemPole[i]);
                t->htabitemPole[i] = NULL; // vynulujeme ho
                t->htabitemPole[i] = nextTemp; // nahradime ho tym ktory bol povodne za nim
                break; // v poli zaznamov su len unikatne key preto mozme zvysit index pola zaznamov (continue) a ist na dalsi zoznam
            }
            else // ak uz nie je za nim nic len ho vynulujeme a ponechame a skaceme na dalsi index
            {
                free(t->htabitemPole[i]);
                t->htabitemPole[i] = NULL;
                break; // v poli zaznamov su len unikatne key preto mozme zvysit index pola zaznamov (continue) a ist na dalsi zoznam
            }
        }

        bool najdeni = false;
        // ak prvy item neobsahol hladany key ulozim si ho ako povodny
        htab_item* povodnyItem = t->htabitemPole[i];
        while(povodnyItem->next != NULL) // vykonavam cyklus pokial za prvym itemom je dalsi alebo ak
        {
            htab_item* aktualnyItem = povodnyItem->next; // zoberieme aktualny item ktory budem porovnavat
            htab_item* nasledujuciItem = aktualnyItem->next; // zoberiem za aktualnym itemom nasledujuci v pripade ak sa bude mazat aktualnyItem

            if(strcmp(aktualnyItem->key, key) == 0) // ak aktualne spracovavani item ma ten isty kluc
            {
                if(nasledujuciItem != NULL) // kontrola ci nasledujuci item je NULL
                {
                    free(aktualnyItem); // vymazeme aktualny item
                    povodnyItem->next = nasledujuciItem; // nastavime povodnemuItemu->next nasledujuciItem kedze aktualny je uz zmazany
                    najdeni = true;
                    break; // prerusime cyklus while nakolko len unikatne retazce su v poli zaznamov
                }
                else // ak uz za najdenim itemom nie je nic tak ho len vymazeme
                {
                    free(aktualnyItem); // vymazeme aktualny item
                    najdeni = true;
                    break; // prerusime cyklus while nakolko dalsi item je uz NULL
                }
            }
            // ak aktualny item nebol zmazani stane sa povodnym itemom aby sme mohli odsledovat dalsi item ktory je za nim v next
            htab_item* povodnyItem = aktualnyItem;

            if(povodnyItem == NULL)
                break;
        }

        if(najdeni == true)
                break;
    }

    return true;
}


/* ******************************* c204.c *********************************** */
/*  Předmět: Algoritmy (IAL) - FIT VUT v Brně                                 */
/*  Úkol: c204 - Převod infixového výrazu na postfixový (s využitím c202)     */
/*  Referenční implementace: Petr Přikryl, listopad 1994                      */
/*  Přepis do jazyka C: Lukáš Maršík, prosinec 2012                           */
/*  Upravil: Kamil Jeřábek, září 2019                                         */
/*           Daniel Dolejška, září 2021                                       */
/* ************************************************************************** */
/*
** Implementujte proceduru pro převod infixového zápisu matematického výrazu
** do postfixového tvaru. Pro převod využijte zásobník (Stack), který byl
** implementován v rámci příkladu c202. Bez správného vyřešení příkladu c202
** se o řešení tohoto příkladu nepokoušejte.
**
** Implementujte následující funkci:
**
**    infix2postfix ... konverzní funkce pro převod infixového výrazu
**                      na postfixový
**
** Pro lepší přehlednost kódu implementujte následující pomocné funkce:
**    
**    untilLeftPar ... vyprázdnění zásobníku až po levou závorku
**    doOperation .... zpracování operátoru konvertovaného výrazu
**
** Své řešení účelně komentujte.
**
** Terminologická poznámka: Jazyk C nepoužívá pojem procedura.
** Proto zde používáme pojem funkce i pro operace, které by byly
** v algoritmickém jazyce Pascalovského typu implemenovány jako procedury
** (v jazyce C procedurám odpovídají funkce vracející typ void).
**
**/

#include "c204.h"

int solved;

/**
 * Pomocná funkce untilLeftPar.
 * Slouží k vyprázdnění zásobníku až po levou závorku, přičemž levá závorka bude
 * také odstraněna.
 * Pokud je zásobník prázdný, provádění funkce se ukončí.
 *
 * Operátory odstraňované ze zásobníku postupně vkládejte do výstupního pole
 * znaků postfixExpression.
 * Délka převedeného výrazu a též ukazatel na první volné místo, na které se má
 * zapisovat, představuje parametr postfixExpressionLength.
 *
 * Aby se minimalizoval počet přístupů ke struktuře zásobníku, můžete zde
 * nadeklarovat a používat pomocnou proměnnou typu char.
 *
 * @param stack Ukazatel na inicializovanou strukturu zásobníku
 * @param postfixExpression Znakový řetězec obsahující výsledný postfixový výraz
 * @param postfixExpressionLength Ukazatel na aktuální délku výsledného postfixového výrazu
 */
void untilLeftPar( Stack *stack, char *postfixExpression, unsigned *postfixExpressionLength ) {
    //skontroluje, či je zásobník prázdny
    if (Stack_IsEmpty(stack) != 0) return;

    char tmp; //pomocná premenná pre uloženie najvyššej hodnoty na zásobníku
    Stack_Top(stack, &tmp); //do pomocnej premennej uloží vrchol zásobníka
    Stack_Pop(stack); //odstráni znak z vrcholu zásobníka

    while (tmp != '(' && (Stack_IsEmpty(stack) == 0)) { //Kým nenarazí na ľavú zátvorku alebo kým zásobník nie je prázdny, tak cyklus beží.
        postfixExpression[*postfixExpressionLength] = tmp; //umiestni znak z vrcholu zásobníka na koniec nového reťazca
        (*postfixExpressionLength)++; //vytvorí v reťazci miesto pre ďalší znak
        Stack_Top(stack, &tmp); //skopíruje nový vrchol zo zásobníka
        Stack_Pop(stack); //odstráni vrchol
    }
}

/**
 * Pomocná funkce doOperation.
 * Zpracuje operátor, který je předán parametrem c po načtení znaku ze
 * vstupního pole znaků.
 *
 * Dle priority předaného operátoru a případně priority operátoru na vrcholu
 * zásobníku rozhodneme o dalším postupu.
 * Délka převedeného výrazu a taktéž ukazatel na první volné místo, do kterého
 * se má zapisovat, představuje parametr postfixExpressionLength, výstupním
 * polem znaků je opět postfixExpression.
 *
 * @param stack Ukazatel na inicializovanou strukturu zásobníku
 * @param c Znak operátoru ve výrazu
 * @param postfixExpression Znakový řetězec obsahující výsledný postfixový výraz
 * @param postfixExpressionLength Ukazatel na aktuální délku výsledného postfixového výrazu
 */
void doOperation( Stack *stack, char c, char *postfixExpression, unsigned *postfixExpressionLength ) {

    if (Stack_IsEmpty(stack) != 0) {
        Stack_Push(stack, c); //ak je zásobník prázdny, rovno vložíme operátor z vrcholu zásobníka
        return; 
    }

    char tmp; //pomocná premenná pre uloženie hodnoty na vrchole zásobníka
    Stack_Top(stack, &tmp); //vloží do pomocnej premennej vrchol zásobíka

    switch (tmp) { //switch pre jednotlivé typy operátorov
        case '(' :  Stack_Push(stack, c); //ak sa na vrchole nachádza ľavá zátvorka, rovno operátor vloží na vrchol zásobníka
                    break;
        case '+' : 
        case '-' :  if(c == '*' || c == '/') { //ak je na vrchole zásobníka operátor s nižšou prioritou, vloží operátor s vyššou prioritou
                        Stack_Push(stack, c);
                    }
                    else { //pokiaľ sa tam operátor s vyššou prioritou nenacháza, vloží operátor z vrcholu zásobníka
                        postfixExpression[*postfixExpressionLength] = tmp; //vloží operátor na koniec výstupného reťazca
                        (*postfixExpressionLength)++; //vytvorí miesto pre nový operátor
                        Stack_Pop(stack); //odstráni operátor z vrcholu zásobníka
                        doOperation(stack, c, postfixExpression, postfixExpressionLength);  //rekurzíve volá funkciu až dokiaľ sa nevloží operátor na zásobník
                    }
                    break;
        case '*' : 
        case '/' :  postfixExpression[*postfixExpressionLength] = tmp; // '*' a '/' majú vyššiu prioritu, rovno ich vloží do nového reťazca
                    (*postfixExpressionLength)++;
                    Stack_Pop(stack);
                    doOperation(stack, c, postfixExpression, postfixExpressionLength);
                    break;
        default :   postfixExpression[*postfixExpressionLength] = tmp; //v prípade iných operátorov
                    (*postfixExpressionLength)++;
                    Stack_Pop(stack);
                    doOperation(stack, c, postfixExpression, postfixExpressionLength);
                    break;  
    }
}

/**
 * Konverzní funkce infix2postfix.
 * Čte infixový výraz ze vstupního řetězce infixExpression a generuje
 * odpovídající postfixový výraz do výstupního řetězce (postup převodu hledejte
 * v přednáškách nebo ve studijní opoře).
 * Paměť pro výstupní řetězec (o velikosti MAX_LEN) je třeba alokovat. Volající
 * funkce, tedy příjemce konvertovaného řetězce, zajistí korektní uvolnění zde
 * alokované paměti.
 *
 * Tvar výrazu:
 * 1. Výraz obsahuje operátory + - * / ve významu sčítání, odčítání,
 *    násobení a dělení. Sčítání má stejnou prioritu jako odčítání,
 *    násobení má stejnou prioritu jako dělení. Priorita násobení je
 *    větší než priorita sčítání. Všechny operátory jsou binární
 *    (neuvažujte unární mínus).
 *
 * 2. Hodnoty ve výrazu jsou reprezentovány jednoznakovými identifikátory
 *    a číslicemi - 0..9, a..z, A..Z (velikost písmen se rozlišuje).
 *
 * 3. Ve výrazu může být použit předem neurčený počet dvojic kulatých
 *    závorek. Uvažujte, že vstupní výraz je zapsán správně (neošetřujte
 *    chybné zadání výrazu).
 *
 * 4. Každý korektně zapsaný výraz (infixový i postfixový) musí být uzavřen
 *    ukončovacím znakem '='.
 *
 * 5. Při stejné prioritě operátorů se výraz vyhodnocuje zleva doprava.
 *
 * Poznámky k implementaci
 * -----------------------
 * Jako zásobník použijte zásobník znaků Stack implementovaný v příkladu c202.
 * Pro práci se zásobníkem pak používejte výhradně operace z jeho rozhraní.
 *
 * Při implementaci využijte pomocné funkce untilLeftPar a doOperation.
 *
 * Řetězcem (infixového a postfixového výrazu) je zde myšleno pole znaků typu
 * char, jenž je korektně ukončeno nulovým znakem dle zvyklostí jazyka C.
 *
 * Na vstupu očekávejte pouze korektně zapsané a ukončené výrazy. Jejich délka
 * nepřesáhne MAX_LEN-1 (MAX_LEN i s nulovým znakem) a tedy i výsledný výraz
 * by se měl vejít do alokovaného pole. Po alokaci dynamické paměti si vždycky
 * ověřte, že se alokace skutečně zdrařila. V případě chyby alokace vraťte namísto
 * řetězce konstantu NULL.
 *
 * @param infixExpression Znakový řetězec obsahující infixový výraz k převedení
 *
 * @returns Znakový řetězec obsahující výsledný postfixový výraz
 */
char *infix2postfix( const char *infixExpression ) {
    //alokácia výstupného reťazca
	char *post_expr = (char *) malloc(MAX_LEN * sizeof(char));
	if (post_expr == NULL) { //kontrola, či malloc zlyhal
		return NULL;
	}

	// alokácia zásobníka
	Stack *stack = (Stack *) malloc(sizeof(Stack));
	if (stack == NULL) {
		free(post_expr); //v prípade, že malloc zlyhal, uvoľní predtým alokované miesto pre výstupný reťazec
		return NULL;
	}    

    Stack_Init(stack); //inicializuje zásobník

    unsigned post_expr_len = 0; //premenná pre dĺžku výstupného reťazca + jej inicializácia
    char c = *infixExpression; //deklarácia prehľadávaného prvku v reťazci

    while (c != '\0') { //prehľadáva raťazec až dokiaľ nenazí na jeho koniec
        //pri narazeni na písmená/čísla ich vloží rovno do výstupného reťazca
        if((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') || (c >= '0' && c <= '9')){
            post_expr[post_expr_len++] = c;
        }

        switch (c) { //switch pre jednotlivé typy operátorov
        case '(' :  Stack_Push(stack, c); //ľvú zátvorku rovno vloží na zásobník
                    break;
        case '+' :
        case '-' :
        case '*' :
        case '/' :  doOperation(stack, c, post_expr, &post_expr_len);
                    break; //pokiaľ narazí na operátory, pošle ich do funkcie doOperation a pomocou nej posunie
        case ')' :  untilLeftPar(stack, post_expr, &post_expr_len);
                    break; //pravú zátvorku pošle na vyprázdnenie až po ľavú zátvorku
        case '=' :   while(!Stack_IsEmpty(stack)) { //pokiaľ nie je zásobník prázdny a narazí na '=',vloží ho do výstupného reťazca a vyprázdni
                        Stack_Top(stack, &post_expr[post_expr_len]);
                        post_expr_len++;
                        Stack_Pop(stack);
                    }
                    post_expr[post_expr_len++] = '='; //pridá znak '=' na koniec výstupného reťazca
                    break;
        }   
        c = *(++infixExpression);     
    }

    post_expr[post_expr_len++] = '\0'; //ukončenie novo vzniknutého, výstupého reťazca
    free(stack); //uvoľníme miesto po spracovaní zásobníka
    return post_expr; //funkcia vráti postfixový výraz
}

/* Konec c204.c */

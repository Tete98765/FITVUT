
/* ******************************* c206.c *********************************** */
/*  Předmět: Algoritmy (IAL) - FIT VUT v Brně                                 */
/*  Úkol: c206 - Dvousměrně vázaný lineární seznam                            */
/*  Návrh a referenční implementace: Bohuslav Křena, říjen 2001               */
/*  Vytvořil: Martin Tuček, říjen 2004                                        */
/*  Upravil: Kamil Jeřábek, září 2020                                         */
/*           Daniel Dolejška, září 2021                                       */
/* ************************************************************************** */
/*
** Implementujte abstraktní datový typ dvousměrně vázaný lineární seznam.
** Užitečným obsahem prvku seznamu je hodnota typu int. Seznam bude jako datová
** abstrakce reprezentován proměnnou typu DLList (DL znamená Doubly-Linked
** a slouží pro odlišení jmen konstant, typů a funkcí od jmen u jednosměrně
** vázaného lineárního seznamu). Definici konstant a typů naleznete
** v hlavičkovém souboru c206.h.
**
** Vaším úkolem je implementovat následující operace, které spolu s výše
** uvedenou datovou částí abstrakce tvoří abstraktní datový typ obousměrně
** vázaný lineární seznam:
**
**      DLL_Init ........... inicializace seznamu před prvním použitím,
**      DLL_Dispose ........ zrušení všech prvků seznamu,
**      DLL_InsertFirst .... vložení prvku na začátek seznamu,
**      DLL_InsertLast ..... vložení prvku na konec seznamu,
**      DLL_First .......... nastavení aktivity na první prvek,
**      DLL_Last ........... nastavení aktivity na poslední prvek,
**      DLL_GetFirst ....... vrací hodnotu prvního prvku,
**      DLL_GetLast ........ vrací hodnotu posledního prvku,
**      DLL_DeleteFirst .... zruší první prvek seznamu,
**      DLL_DeleteLast ..... zruší poslední prvek seznamu,
**      DLL_DeleteAfter .... ruší prvek za aktivním prvkem,
**      DLL_DeleteBefore ... ruší prvek před aktivním prvkem,
**      DLL_InsertAfter .... vloží nový prvek za aktivní prvek seznamu,
**      DLL_InsertBefore ... vloží nový prvek před aktivní prvek seznamu,
**      DLL_GetValue ....... vrací hodnotu aktivního prvku,
**      DLL_SetValue ....... přepíše obsah aktivního prvku novou hodnotou,
**      DLL_Previous ....... posune aktivitu na předchozí prvek seznamu,
**      DLL_Next ........... posune aktivitu na další prvek seznamu,
**      DLL_IsActive ....... zjišťuje aktivitu seznamu.
**
** Při implementaci jednotlivých funkcí nevolejte žádnou z funkcí
** implementovaných v rámci tohoto příkladu, není-li u funkce explicitně
 * uvedeno něco jiného.
**
** Nemusíte ošetřovat situaci, kdy místo legálního ukazatele na seznam
** předá někdo jako parametr hodnotu NULL.
**
** Svou implementaci vhodně komentujte!
**
** Terminologická poznámka: Jazyk C nepoužívá pojem procedura.
** Proto zde používáme pojem funkce i pro operace, které by byly
** v algoritmickém jazyce Pascalovského typu implemenovány jako procedury
** (v jazyce C procedurám odpovídají funkce vracející typ void).
**
**/

#include "c206.h"

int error_flag;
int solved;

/**
 * Vytiskne upozornění na to, že došlo k chybě.
 * Tato funkce bude volána z některých dále implementovaných operací.
 */
void DLL_Error() {
	printf("*ERROR* The program has performed an illegal operation.\n");
	error_flag = TRUE;
}

/**
 * Provede inicializaci seznamu list před jeho prvním použitím (tzn. žádná
 * z následujících funkcí nebude volána nad neinicializovaným seznamem).
 * Tato inicializace se nikdy nebude provádět nad již inicializovaným seznamem,
 * a proto tuto možnost neošetřujte.
 * Vždy předpokládejte, že neinicializované proměnné mají nedefinovanou hodnotu.
 *
 * @param list Ukazatel na strukturu dvousměrně vázaného seznamu
 */
void DLL_Init( DLList *list ) {
    //kedže je zoznam prázdny, ukazetele v zozname sa nastavia na NULL
    list -> activeElement = NULL;
    list -> firstElement = NULL;
    list -> lastElement = NULL;
}

/**
 * Zruší všechny prvky seznamu list a uvede seznam do stavu, v jakém se nacházel
 * po inicializaci.
 * Rušené prvky seznamu budou korektně uvolněny voláním operace free.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 */
void DLL_Dispose( DLList *list ) {

    if (list -> firstElement == NULL) return; //pokiaľ je zoznam prázdny, nič sa nevykoná
    DLLElementPtr tmp; //pomocná premenná do ktorej sa bude ukladať další prvok zo zoznamu
    while (list -> firstElement != NULL) { //cyklus prechádza zoznamom až kým nie je prázdny
        tmp = list -> firstElement -> nextElement; //do pomocnej premennej uloží nasledujúci prvok v zozname
        free(list -> firstElement); //uvolní prvý prvok
        list -> firstElement = tmp; //druhý prvok presunieme na miesto prveho
    }
    
    //keďže zoznam je prázdny, znovu všetky prvky nastaví na NULL
    list -> activeElement = NULL;
    list -> firstElement = NULL;
    list -> lastElement = NULL;
}

/**
 * Vloží nový prvek na začátek seznamu list.
 * V případě, že není dostatek paměti pro nový prvek při operaci malloc,
 * volá funkci DLL_Error().
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 * @param data Hodnota k vložení na začátek seznamu
 */
void DLL_InsertFirst( DLList *list, int data ) {
    //alokácia pamäte pre nový prvok
    DLLElementPtr new_element = malloc(sizeof(struct DLLElement));
    if (new_element == NULL) {
        DLL_Error();
        return;
    }
    
    new_element -> data = data; //novému prvku priradí dáta
    new_element -> previousElement = NULL; //prvok má byť na začiatku týmm pádom nemá pre sebou prvok - nastaví ukazateľ na NULL
    new_element -> nextElement = list -> firstElement; //prvý prvok bude pre nový nasledujúci

    if (list -> firstElement == NULL) { //ak je zoznam prázdny na miesto prvého a posledného sa rovno vloží nový prvok
        list -> firstElement = new_element;
        list -> lastElement = new_element;
    }
    else {
        list -> firstElement -> previousElement = new_element; //ukazateľ na predchádzajúci element pre prvý prvok bude ukazovať na nový prvok
        list -> firstElement = new_element; //novým prvým prvkom je new_element
    }  
}

/**
 * Vloží nový prvek na konec seznamu list (symetrická operace k DLL_InsertFirst).
 * V případě, že není dostatek paměti pro nový prvek při operaci malloc,
 * volá funkci DLL_Error().
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 * @param data Hodnota k vložení na konec seznamu
 */
void DLL_InsertLast( DLList *list, int data ) {
    //alokácia pamäte pre nový prvok
    DLLElementPtr new_element = malloc(sizeof(struct DLLElement));
    if (new_element == NULL) {
        DLL_Error();
        return;
    }

    new_element -> data = data; //novému prvku priradí dáta
    new_element -> nextElement = NULL; //prvok je na konci, za nim sa nič nenachádza preto ukazateľ nasledujúceho prvku nastaví na NULL
    new_element -> previousElement = list -> lastElement; //posledný prvok bude pre nový prvok jeho predchádzajúcim 

    if (list -> lastElement == NULL) { //ak je zoznam prázdny na miesto prvého a posledného sa rovno vloží nový prvok
        list -> firstElement = new_element;
        list -> lastElement = new_element;
    }
    else {
        list -> lastElement -> nextElement = new_element; //ukazateľ na nasledujúci element pre posledný prvok bude ukazovať na nový prvok
        list -> lastElement = new_element; //novým posledným prvkom je new_element
    }
}

/**
 * Nastaví první prvek seznamu list jako aktivní.
 * Funkci implementujte jako jediný příkaz (nepočítáme-li return),
 * aniž byste testovali, zda je seznam list prázdný.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 */
void DLL_First( DLList *list ) {

    list -> activeElement = list -> firstElement; //prvý prvok sa priradí ako aktívy
}

/**
 * Nastaví poslední prvek seznamu list jako aktivní.
 * Funkci implementujte jako jediný příkaz (nepočítáme-li return),
 * aniž byste testovali, zda je seznam list prázdný.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 */
void DLL_Last( DLList *list ) {

    list -> activeElement = list -> lastElement; //posledný prvok sa priradí ako aktívny
}

/**
 * Prostřednictvím parametru dataPtr vrátí hodnotu prvního prvku seznamu list.
 * Pokud je seznam list prázdný, volá funkci DLL_Error().
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 * @param dataPtr Ukazatel na cílovou proměnnou
 */
void DLL_GetFirst( DLList *list, int *dataPtr ) {

    if (list -> firstElement == NULL) {
        DLL_Error();
        return;
    }

    *dataPtr = list -> firstElement -> data; //priradí dáta na mieste prvého prvku do dataPtr
}

/**
 * Prostřednictvím parametru dataPtr vrátí hodnotu posledního prvku seznamu list.
 * Pokud je seznam list prázdný, volá funkci DLL_Error().
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 * @param dataPtr Ukazatel na cílovou proměnnou
 */
void DLL_GetLast( DLList *list, int *dataPtr ) {

    if (list -> firstElement == NULL) {
        DLL_Error();
        return;
    }

    *dataPtr = list -> lastElement -> data; //priradí dáta na mieste posledného prvku do dataPtr
}

/**
 * Zruší první prvek seznamu list.
 * Pokud byl první prvek aktivní, aktivita se ztrácí.
 * Pokud byl seznam list prázdný, nic se neděje.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 */
void DLL_DeleteFirst( DLList *list ) {

    if (list -> firstElement == NULL) return; //kontrola či je list prázdny

    if (list -> activeElement == list -> firstElement) list -> activeElement = NULL; //pokiaľ sa aktívny prvok rovná prvému nastaví aktívny prvok na NULL

    if (list -> firstElement == list -> lastElement) { //ak má zoznam len jeden prvok(je teda prvým a zároveň posledným), tak ho nastavíme na NULL
        list -> firstElement = NULL;
        list -> lastElement = NULL;
    }
    else {
        list -> firstElement = list -> firstElement -> nextElement; //nastaví nový začiatok zoznamu, novým začiatkom bude prvol, ktorý následuje za prvým 
        free(list -> firstElement -> previousElement); //uvoľní/vymaže pôvodý prvý prvok
        list -> firstElement -> previousElement = NULL; //nastaví ukazateľ predchádzajúceho prvku na NULL
    }
}

/**
 * Zruší poslední prvek seznamu list.
 * Pokud byl poslední prvek aktivní, aktivita seznamu se ztrácí.
 * Pokud byl seznam list prázdný, nic se neděje.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 */
void DLL_DeleteLast( DLList *list ) {

    if (list -> firstElement == NULL) return; //kontrola či je list prázdny

    if (list -> activeElement == list -> lastElement) list -> activeElement = NULL; //pokiaľ sa aktívny prvok rovná prvému nastaví aktívny prvok na NULL

    if (list -> firstElement == list -> lastElement) { //ak má zoznam len jeden prvok(je teda prvým a zároveň posledným), tak ho nastavíme na NULL
        list -> firstElement = NULL;
        list -> lastElement = NULL;
    }
    else {
        list -> lastElement = list -> lastElement -> previousElement; //nastaví nový koniec zoznamu, novým koncom bude prvok, ktorý sa nachádza pred posledným 
        free(list -> lastElement -> nextElement); //uvoľní/vymaže pôvodý posledný prvok
        list -> lastElement -> nextElement = NULL; //nastaví ukazateľ nasledujúceho prvku na NULL
    }
}

/**
 * Zruší prvek seznamu list za aktivním prvkem.
 * Pokud je seznam list neaktivní nebo pokud je aktivní prvek
 * posledním prvkem seznamu, nic se neděje.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 */
void DLL_DeleteAfter( DLList *list ) {

    if (list -> activeElement == NULL || list ->activeElement == list -> lastElement) return; //kontrola aktivity a prvkov zoznamu

    DLLElementPtr tmp = list -> activeElement -> nextElement -> nextElement; //pomocná premenná do ktorej sa uloží prvok, ktorý sa nachádza za tým, ktorý sa ide mazať

    if(list -> activeElement -> nextElement == list -> lastElement) { //pozrie či náhodou prvkom, ktorý sa nachádza za aktívnym, nie je poslený
        free(list -> lastElement); //uvoľní/zmaže posledný prvok
        list -> activeElement -> nextElement = NULL; //ukazateľ na nasledujúci prvok nastaví na NULL
        list -> lastElement = list -> activeElement; //posleným prvkom sa stane aktívny prvok
    }
    else {
        free(list -> activeElement -> nextElement); //uvoľní/vymaže prvok za aktívnym prvkom
        list -> activeElement -> nextElement = tmp; //novým nasledujúcim prvkom bude ten, ktorý je v pomocnej premennej
        list -> activeElement -> nextElement -> previousElement = list -> activeElement; //presunie aktívny prvok na správne miesto
    } 

    
}

/**
 * Zruší prvek před aktivním prvkem seznamu list .
 * Pokud je seznam list neaktivní nebo pokud je aktivní prvek
 * prvním prvkem seznamu, nic se neděje.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 */
void DLL_DeleteBefore( DLList *list ) {

    if (list -> activeElement == NULL || list ->activeElement == list -> firstElement) return; //kontrola aktivity a prvkov zoznamu

    DLLElementPtr tmp = list -> activeElement -> previousElement -> previousElement; //pomocná premenná do ktorej sa uloží prvok, ktorý sa nachádza pred tým, ktorý sa ide mazať

    if (list -> activeElement -> previousElement == list -> firstElement) { //pozrie či náhodou prvkom, ktorý sa nachádza pred aktívnym, nie je prvý
        free(list -> firstElement); //uvoľní/zmaže prvý prvok
        list -> activeElement -> previousElement = NULL; //ukazateľ na prvý prvok nastaví na NULL
        list -> firstElement = list -> activeElement; //prvým prvkom sa stane aktívny prvok
    }
    else {
        free(list -> activeElement -> previousElement); //uvoľní/vymaže prvok pred aktívnym prvkom
        list -> activeElement -> previousElement = tmp; //novým predchádzajúcim prvkom bude ten, ktorý je v pomocnej premennej
        list -> activeElement -> previousElement -> nextElement = list -> activeElement; //presunie aktívny prvok na správne miesto
    } 
}

/**
 * Vloží prvek za aktivní prvek seznamu list.
 * Pokud nebyl seznam list aktivní, nic se neděje.
 * V případě, že není dostatek paměti pro nový prvek při operaci malloc,
 * volá funkci DLL_Error().
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 * @param data Hodnota k vložení do seznamu za právě aktivní prvek
 */
void DLL_InsertAfter( DLList *list, int data ) {

    if (list -> activeElement == NULL) return; //kontrola či je nejaký prvok aktívny
    //alokácia miesta pre nový prvok
    DLLElementPtr new_element = malloc(sizeof(struct DLLElement));
    if (new_element == NULL) {
        DLL_Error();
        return;
    }

    new_element -> data = data; //nastaví dáta novému prvku
    new_element -> nextElement = list -> activeElement -> nextElement; //nasledujúcim prvkom pre nový bude ten, ktorý ide za aktívnym
    new_element -> previousElement = list -> activeElement; //aktívny prvok sa bude nachádzať pred novo vloženým prvkom
    
    if (list -> activeElement == list -> lastElement) { //ak je aktívnym prvkom posledný, tak na jeho miesto vloží nový prvok
        list -> lastElement = new_element;
    }
    else {
        list -> activeElement -> nextElement -> previousElement = new_element; //prvok, ktorý je za aktívym elementom, bude mať pred sebou nový prvok 
    }
}

/**
 * Vloží prvek před aktivní prvek seznamu list.
 * Pokud nebyl seznam list aktivní, nic se neděje.
 * V případě, že není dostatek paměti pro nový prvek při operaci malloc,
 * volá funkci DLL_Error().
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 * @param data Hodnota k vložení do seznamu před právě aktivní prvek
 */
void DLL_InsertBefore( DLList *list, int data ) {

    if (list -> activeElement == NULL) return; //kontrola či máme aktívny prvok
    //alokácia miesta pre nový prvok 
    DLLElementPtr new_element = malloc(sizeof(struct DLLElement));
    if (new_element == NULL) {
        DLL_Error();
        return;
    }

    new_element -> data = data; //nastaví dáta novému prvku
    new_element -> previousElement = list -> activeElement -> previousElement; //predchádzajúcim prvkom pre nový bude ten, ktorý sa nachádza pred aktívnym
    new_element -> nextElement = list -> activeElement; //predchádzajúcim prvkom pre nový bude aktívny prvok
    
    if (list -> activeElement == list -> firstElement) { //pokiaľ je aktívnym prvkom prvý prvok, vloží na jeho miesto nový prvok
        list -> firstElement = new_element;
    }
    else {
        list -> activeElement -> previousElement ->nextElement = new_element; //prvok, ktorý je pred aktívnym elementom, bude mať pred sebou nový prvok
    }
}

/**
 * Prostřednictvím parametru dataPtr vrátí hodnotu aktivního prvku seznamu list.
 * Pokud seznam list není aktivní, volá funkci DLL_Error ().
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 * @param dataPtr Ukazatel na cílovou proměnnou
 */
void DLL_GetValue( DLList *list, int *dataPtr ) {

    if (list -> activeElement == NULL) { //ak je zoznam prázdny vyhodí chybu
        DLL_Error();
        return; 
    }
    *dataPtr = list -> activeElement -> data; //dáta z aktívneho prvku sa skopírujú do dataPtr
}

/**
 * Přepíše obsah aktivního prvku seznamu list.
 * Pokud seznam list není aktivní, nedělá nic.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 * @param data Nová hodnota právě aktivního prvku
 */
void DLL_SetValue( DLList *list, int data ) {

    if (list -> activeElement == NULL) return; 
    list -> activeElement -> data = data; //pokiaľ nie je zoznam prázdny, pôvodné dáta prepíšeme novými
}

/**
 * Posune aktivitu na následující prvek seznamu list.
 * Není-li seznam aktivní, nedělá nic.
 * Všimněte si, že při aktivitě na posledním prvku se seznam stane neaktivním.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 */
void DLL_Next( DLList *list ) {

    if (list -> activeElement == NULL) return; 
    list -> activeElement = list -> activeElement -> nextElement; //pokiaľ nie je zoznam prázdny, aktivita sa posuie na nasledujúci prvok
}


/**
 * Posune aktivitu na předchozí prvek seznamu list.
 * Není-li seznam aktivní, nedělá nic.
 * Všimněte si, že při aktivitě na prvním prvku se seznam stane neaktivním.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 */
void DLL_Previous( DLList *list ) {

    if (list -> activeElement == NULL) return; 
    list -> activeElement = list -> activeElement -> previousElement; //pokiaľ nie je zoznam prázdny, aktivita sa posunie na predchádzajúci prvok
}

/**
 * Je-li seznam list aktivní, vrací nenulovou hodnotu, jinak vrací 0.
 * Funkci je vhodné implementovat jedním příkazem return.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 *
 * @returns Nenulovou hodnotu v případě aktivity prvku seznamu, jinak nulu
 */
int DLL_IsActive( DLList *list ) {

    return (list -> activeElement != NULL ? 1 : 0); //ak sa zoznam nerovná NULL tak je aktívny - podmienka vráti 1
}

/* Konec c206.c */

/*ppm.c
 * @Řešení IJC-DU1, příklad b), 23.3.2021
 * @Fakulta: VUT FIT
 * @přeloženo gcc 2.32-1
 */

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h>
#include "ppm.h"
#include "error.h"

void ppm_free(struct ppm* p)
{
    free(p);
}

struct ppm* ppm_read(const char* filename)
{
    char typSuboru[3] = {0};
    int sirkaObrazku = 0;
    int vyskaObrazku = 0;
    int maxHodFarby = 0;
    int pocetZnakovHlavicka = 0;
    unsigned long pocPrecitanychBajtov = 0;
    unsigned long velkost = 0;
    
    struct ppm* ppmStrukt = NULL;
    
    FILE* ppmSubor = fopen(filename, "r");

    if (!ppmSubor)
    {
        warning_msg("Nepodarilo sa otvorit subor %s : \n", filename);
        return NULL;
    }

    pocetZnakovHlavicka = fscanf(ppmSubor, "%s %d %d %d ", typSuboru, &sirkaObrazku, &vyskaObrazku, &maxHodFarby);
    
    if (pocetZnakovHlavicka != 4)
    {
        warning_msg("Subor obsahuje zly pocet udajov v hlavicke suboru : %s \n", filename);
        fclose(ppmSubor);
        return NULL;
    }

    if (strcmp(typSuboru, "P6"))
    {
        warning_msg("Zly typ suboru %s , pozadovany typ je P6 \n", filename);
        fclose(ppmSubor);
        return NULL;
    }

    if ( maxHodFarby < 0 || maxHodFarby > 255)
    {
        warning_msg("Velkost max hodnoty pre farbu pixela je zadana mimo intervalu <0, 255> v hlavicke suboru : %s. \n", filename);
        fclose(ppmSubor);
        return NULL;
    }

    velkost = sizeof(struct ppm) + (RGB * sirkaObrazku * vyskaObrazku);
  
    if(velkost > MAX_LIMIT)
    {
        warning_msg("Prekrocena hranica maxilnej podporovanej velkosti obrazku. \n");
        fclose(ppmSubor);
        return NULL;
    }

    ppmStrukt = malloc(velkost);

    if (!ppmStrukt)
    {
        warning_msg("Nepodarilo sa alokovat pozadovanu pamat o velkosti : %ld \n" , velkost);
        fclose(ppmSubor);
        return NULL;
    }

    ppmStrukt->xsize = sirkaObrazku;
    ppmStrukt->ysize = vyskaObrazku;
    
    pocPrecitanychBajtov = fread(ppmStrukt->data, CHAR_BIT, velkost, ppmSubor);
  	
    if(pocPrecitanychBajtov == 0)
    {
    	warning_msg("Pri nacitavani znakov zo suboru %s do pola doslo k chybe : %s \n", filename);
        fclose(ppmSubor);
        free(ppmStrukt);
    }
    
    if (fgetc(ppmSubor) != EOF)
    {
        warning_msg("Nenasiel sa znak konca suboru v subore : %s \n", filename);
        fclose(ppmSubor);
        free(ppmStrukt);
        return NULL;
    }

    fclose(ppmSubor);

    return ppmStrukt;
}

#include <stdbool.h>
#include <stdio.h>
#include "io.h"

int read_word(char* s, int max, FILE *f)
{
    static bool prvaChybaPrekrocenia = false;
    int pocetZnakovVslove = 0;

    char znak;
    while((znak = fgetc(f)) != EOF)
    {
        if(znak == ' ' || znak == '\n')
        {
            s[pocetZnakovVslove] = '\0';
            break;
        }

        if(pocetZnakovVslove == max - 1)
        {
            s[pocetZnakovVslove] = '\0';

            if(!prvaChybaPrekrocenia)
            {
                fprintf(stderr, "Slovo obsahuje viac ako MAX limit povoleny na jedno slovo (127 znakov) \n");
                prvaChybaPrekrocenia = true;

                return 0;
            }

            return 0;
        }

        s[pocetZnakovVslove++] = znak;
    }

    if(znak == EOF)
        return -1;


    return pocetZnakovVslove;
}

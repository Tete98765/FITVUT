#include <read_word.h>
#include <stdbool.h>

int read_word(char* s, int max, FILE *f)
{
    static bool prvaChybaPrekrocenia = false;
    int dlzkaSlova = 0;
    char znak;

    int pocetZnakovVslove = 0;
    while((znak = fgetc()) != EOF)
    {
        if(znak == ' ')
        {
            s[pocetZnakovVslove] = '\0';
            break;
        }

        if(pocetZnakovVslove == max - 1)
        {
            s[pocetZnakovVslove] = '\0';

            if(!prvaChybaPrekrocenia)
            {
                fprint(stderr, "Slovo obsahuje viac ako MAX limit povoleny na jedno slovo (127 znakov) \n");
                prvaChybaPrekrocenia = true;

                return -1;
            }

        }

        s[pocetZnakovVslove++] = c;
    }

    if(znak == EOF)
        return -1;


    return pocetZnakovVslove;
}

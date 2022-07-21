#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <stdbool.h>

#define DEFAULT_COUNT_LINES 10

#define DEFAULT_BUFFER_SIZE 512

int convertStringToInt(char* cntLine)
{
    char tempCntLine[10] = {0};
    int counterLine = 0;

    for(size_t i = 0; i < strlen(cntLine); i++)
    {
        if(cntLine[i] >= '0' && cntLine[i] <= '9')
        {
            tempCntLine[counterLine++] = cntLine[i];
        }
    }

    char* ptr = NULL;
    int pocetRiadkov = (int)strtoul(tempCntLine, &ptr, 10);

    return pocetRiadkov;
}

// 1. zada subor a neda riadky takze poslednych 10 riadkov zoberiem
// 2. zada subor a da riadky bez + a zoberiem pocet riadkov od zadu kolko zadal
// 3. zada subor a da riazky z + a vtedy bere hodnoty zo suboru od danej hodnoty co zadal takze ak zada +3 amame 10 riadkov zobereieme len 7 riadkov nie 10

// 4. nezada subor a neda riadky takze poslednych 10 riadkov zoberiem z konzoly
// 5. nezada subor a da riadky bez + a zoberiem pocet riadkov od zadu kolko zadal z konzoly
// 6. nezada subor a da riadky z + a vtedy bere sa bere poslednych 10 riadkov od danej hodnoty + z konzoly


bool citajRiadkyAvypis(bool citajZoSuboru, bool citajOd, bool zadalPocetRiadkov, int pocetRiadkov, int citajOdRiadok, FILE* file)
{

    char buffer[DEFAULT_BUFFER_SIZE] = {0};

    FILE* fileTemp = NULL;
    int pocetCitanychRiadkov = 0;
    pocetCitanychRiadkov++; // kvoli compiler warningu tato blbost
    pocetCitanychRiadkov--; // kvoli compiler warningu tato blbost

    if(!citajZoSuboru) // ak nebude citat zo suboru treba citat z konzoly pouzitim stdin
        fileTemp = stdin;
    else
        fileTemp = file;

    if(!zadalPocetRiadkov)
        pocetCitanychRiadkov = DEFAULT_COUNT_LINES;
    else
        pocetCitanychRiadkov = pocetRiadkov;

    int pocetNacitanychRiadkov = 0;

    int pocetRiadkovSubor = 0;
    fseek(fileTemp, 0, SEEK_SET);

    char znak;
    while((znak = fgetc(fileTemp)) != EOF)
    {
        if(znak == '\n')
            pocetRiadkovSubor++;
    }

    fseek(fileTemp, 0, SEEK_SET);

    bool bolVypis = false;
    char* riadok = NULL;

    int temp = citajOd ? 0 : 1;

    while((riadok = fgets(buffer, DEFAULT_BUFFER_SIZE, fileTemp)) != NULL)
    {
        pocetNacitanychRiadkov++;

        if( (zadalPocetRiadkov && pocetNacitanychRiadkov > pocetRiadkovSubor - pocetRiadkov + temp)
                || (citajOd && pocetNacitanychRiadkov > citajOdRiadok + temp)
                || (!citajOd && !zadalPocetRiadkov && pocetNacitanychRiadkov > pocetRiadkovSubor - DEFAULT_COUNT_LINES + temp))
        {

            if(buffer[DEFAULT_BUFFER_SIZE -1] != '\0')
            {
                if(!bolVypis)
                {
                    fprintf(stderr, "Prekroceni maximalny pocet pri nacitani retazca\n");
                    bolVypis = true;
                }

                char* nacitanyRiadok = (char*)calloc(514, sizeof(char));
                strcat(nacitanyRiadok, buffer);
                nacitanyRiadok[512] = '\n';
                nacitanyRiadok[513] = '\0';

                printf("%s", nacitanyRiadok);

                memset(buffer, 0, DEFAULT_BUFFER_SIZE);
                free(nacitanyRiadok);
                continue;
            }

            // tu vypis
            printf("%s", buffer);
            memset(buffer, 0, DEFAULT_BUFFER_SIZE);
        }
    }


    // https://www.tutorialspoint.com/c_standard_library/c_function_fseek.htm
    // https://www.tutorialspoint.com/c_standard_library/c_function_ftell.htm
    // ziskat pocet riadkov pomocou ftel() a fseek()
    // citat riadky zo suboru alebo stdin  -- to mame
    // ohandlovat to ze riadok moze byt vacsi ako 512 znakov -- to mame
    // a ked sa splnia podmienky vypisat nacitani riadok -- nemame
    // zvysovat pocet precitanych riadkov  -- to mame
    // vyprazdnit buffer


    return true;
}

void openFile(char* fileName, FILE** file)
{
    *file = fopen(fileName, "r");
    if(file == NULL)
    {
        fprintf(stderr, "Nepodarilo sa nacitat subor, kod chyby : ");
        perror("Error:");
        exit(-1);
    }
}

int main(int argc, char* argv[])
{
    FILE* file = NULL;

    if(argc == 2 || argc == 1) // vypis 10tich riadkov co je defaul ak zada len subor
    {
        bool citajZoSuboru = false;
        if(argc != 1)
        {
            citajZoSuboru = argv[1][0] != '<';

            if(citajZoSuboru)
                openFile(argv[1], &file);
        }

        citajRiadkyAvypis(citajZoSuboru, false, false, 0, 0, file);

        if(citajZoSuboru)
            fclose(file);
    }


    else
    {

        if(strcmp(argv[1], "-n") == 0)
        {
            if(argv[2][0] != '+') // ak sa nerovna napr. +3 teda  len 3
            {
                int pocetRiadkov = convertStringToInt(argv[2]);

                if(pocetRiadkov == 0)
                    return 1;

                if(argc == 4)   // ak zadal subor vieme ze citat riadky mame zo suboru
                {

                    bool citajZoSuboru = argv[3][0] != '<';

                    if(citajZoSuboru)
                        openFile(argv[3], &file);

                    citajRiadkyAvypis(citajZoSuboru, false, true, pocetRiadkov, 0, file);

                    if(citajZoSuboru)
                        fclose(file);
                }
                else
                {
                    citajRiadkyAvypis(false, false, true, pocetRiadkov, 0, file);
                }
            }

            if(argv[2][0] == '+')
            {
                int citajRiadokOd = convertStringToInt(argv[2]);

                if(citajRiadokOd == 0)
                    return 1;

                if(argc == 4)   // ak zadal subor vieme ze citat riadky mame zo suboru
                {
                    bool citajZoSuboru = argv[3][0] != '<';

                    if(citajZoSuboru)
                        openFile(argv[3], &file);

                    citajRiadkyAvypis(citajZoSuboru, true, false, 0, citajRiadokOd, file);

                    if(citajZoSuboru)
                        fclose(file);
                }
                else
                {
                    citajRiadkyAvypis(false, true, false, 0, citajRiadokOd, file);
                }

            }
        }
        else if(argv[2][0] != '+')
        {
            int pocetRiadkov = convertStringToInt(argv[2]);

            if(pocetRiadkov == 0)
                return 1;

            if(argc == 3)
            {

                bool citajZoSuboru = argv[3][0] != '<';

                if(citajZoSuboru)
                    openFile(argv[3], &file);

                citajRiadkyAvypis(citajZoSuboru, false, false, 0, 0, file);

                if(citajZoSuboru)
                    fclose(file);
            }
            else
            {
                  citajRiadkyAvypis(false, false, false, 0, 0, file);
            }
        }
        else if(argv[2][0] == '+')
        {
            int citajRiadokOd = convertStringToInt(argv[2]);

            if(citajRiadokOd == 0)
                return 1;

            if(argc == 3)
            {
                bool citajZoSuboru = argv[3][0] != '<';

                if(citajZoSuboru)
                    openFile(argv[3], &file);


                citajRiadkyAvypis(citajZoSuboru, true, false, 0, citajRiadokOd, file);

                if(citajZoSuboru)
                    fclose(file);

            }
            else
            {
                citajRiadkyAvypis(false, true, false, 0, citajRiadokOd, file);
            }
        }

    }

    return 0;
}

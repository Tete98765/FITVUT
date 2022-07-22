/* 
    datum: 14.11.2020
    Projekt 1 - Praca s textom
    IZP
    
*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h> 
#include <ctype.h>
#define MAX 10242
#define AROW 1
#define ACOL 2
#define ICOL 3
#define IROW 4
#define DCOL 5
#define DROWS 6
#define DCOLS 7
#define DROW 8
#define COPY 9
#define TOLOWER 10
#define ROWS 11
#define CSET 12
#define INF __INT_MAX__
#define TOUPPER 13
#define ROUND 14
#define INT 15
#define SWAP 16
#define MOVE 17
#define BEGINSWITH 18
#define CONTAINS 19

int is_delimiter(char * array, char c);
void irow (int col_count, char d);
void icol (char d);
void totiny (char *s);
void tobig (char *s);
void toround (char *s);
void toint (char *s);
void getcol (char *s, char *d, int c, char *b);


int main(int argc, char * argv[]){   
    char row[MAX];
    char *delimiters = " ";
    int cc = -1;
    int command = ROWS;
    int command_2 = 0;
    int arg1 = 1;
    int arg2 = INF;
    int arg3;
    int arg4;
    char *args; 
    char *args2;

    //nacitavanie jednotlivych prikazov  
    for (int i = 0; i < argc; i++){
        if (!strcmp(argv[i], "-d")){
            delimiters = argv[i+1];
        }
        else if (!strcmp(argv[i], "arow")){
            command = AROW;
        }
        else if (!strcmp(argv[i], "acol")){
            command = ACOL;
        }
        else if (!strcmp(argv[i], "icol")){
            command = ICOL;
            if (argc <= i+1) {
                fprintf(stderr, "Nezadal si cislo argumentu\n");
                return -1;
            }
            arg1 = atoi(argv[i+1]);
            if (arg1 < 1) {
                fprintf(stderr, "Nespravny argument prikazu icol\n");
                return -1;
            }
        }
        else if (!strcmp(argv[i], "irow")){
            command = IROW;
            if (argc <= i+1) {
                fprintf(stderr, "Nezadal si cislo argumentu\n");
                return -1;
            }
            arg1 = atoi(argv[i+1]);
            if (arg1 < 1) {
                fprintf(stderr, "Nespravny argument prikazu irow\n");
                return -1;
            }
        }
        else if (!strcmp(argv[i], "dcol")){
            command = DCOLS;
            if (argc <= i+2) {
                fprintf(stderr, "Nezadal si cislo argumentu\n");
                return -1;
            }
            arg1 = atoi(argv[i+1]);
            arg2 = arg1;
            if (arg1 < 1) {
                fprintf(stderr, "Nespravny argument prikazu dcol\n");
                return -1;
            }
        }
        else if (!strcmp(argv[i], "drows")){
            command = DROWS;
            if (argc <= i+2) {
                fprintf(stderr, "Nezadal si cislo argumentu\n");
                return -1;
            }
            arg1 = atoi(argv[i+1]);
            arg2 = atoi(argv[i+2]);
            if (arg1 < 1 || arg2 < 1 || arg2 < arg1) {
                fprintf(stderr, "Nespravny argument prikazu drows\n");
                return -1;
            }
        }
        else if (!strcmp(argv[i], "drow")){
            command = DROW;
            if (argc <= i+1) {
                fprintf(stderr, "Nezadal si cislo argumentu\n");
                return -1;
            }
            arg1 = atoi(argv[i+1]);
            if (arg1 < 1) {
                fprintf(stderr, "Nespravny argument prikazu drow\n");
                return -1;
            }
        }
        else if (!strcmp(argv[i], "dcols")){
            command = DCOLS;
            if (argc <= i+2) {
                fprintf(stderr, "Nezadal si cislo argumentu\n");
                return -1;
            }
            arg1 = atoi(argv[i+1]);
            arg2 = atoi(argv[i+2]);
            if (arg1 < 1 || arg2 < 1 || arg2 < arg1) {
                fprintf(stderr, "Nespravny argument prikazu drows\n");
                return -1;
            }
        }
        else if (!strcmp(argv[i], "rows")) {
            command = ROWS;
            if (argc <= i+2) {
                fprintf(stderr, "Nezadal si cislo argumentu\n");
                return -1;
            }
            if (!strcmp(argv[i+1], "-") && !strcmp(argv[i+2], "-")) {
               arg1 = arg2 = INF; 
            }
            else if (!strcmp(argv[i+2], "-")) {
                arg2 = INF;
                arg1 = atoi(argv[i+1]);
            }
            else {
                arg1 = atoi(argv[i+1]);
                arg2 = atoi(argv[i+2]);            
                
            }
            
            if (arg1 < 1 || arg2 < 1 || arg2 < arg1) {
                fprintf(stderr, "Nespravny argument prikazu rows\n");
                return -1;
            }  
        
        }
        else if (!strcmp(argv[i], "cset")){
            command_2 = CSET;
            if (argc <= i+2) {
                fprintf(stderr, "Nezadal si cislo argumentu\n");
                return -1;
            }
            arg3 = atoi(argv[i+1]);
            args = argv[i+2];
            if (arg3 < 1) {
                fprintf(stderr, "Nespravny argument prikazu cset\n");
                return -1;
            }
        }
        else if (!strcmp(argv[i], "copy")){
            command_2 = COPY;
            if (argc <= i+2) {
                fprintf(stderr, "Nezadal si cislo argumentu\n");
                return -1;
            }
            arg3 = atoi(argv[i+1]);
            arg4 = atoi(argv[i+2]);
            if (arg3 < 1 || arg4 < 1) {
                fprintf(stderr, "Nespravny argument prikazu copy\n");
                return -1;
            }
            else if (arg3 == arg4) command_2 = 0;
        }
        else if (!strcmp(argv[i], "tolower")){
            command_2 = TOLOWER;
            if (argc <= i+1) {
                fprintf(stderr, "Nezadal si cislo argumentu\n");
                return -1;
            }
            arg3 = atoi(argv[i+1]);
            if (arg3 < 1) {
                fprintf(stderr, "Nespravny argument prikazu cset\n");
                return -1;
            }
        }
        else if (!strcmp(argv[i], "toupper")){
            command_2 = TOUPPER;
            if (argc <= i+1) {
                fprintf(stderr, "Nezadal si cislo argumentu\n");
                return -1;
            }
            arg3 = atoi(argv[i+1]);
            if (arg3 < 1) {
                fprintf(stderr, "Nespravny argument prikazu toupper\n");
                return -1;
            }
        }
        else if (!strcmp(argv[i], "round")){
            command_2 = ROUND;
            if (argc <= i+1) {
                fprintf(stderr, "Nezadal si cislo argumentu\n");
                return -1;
            }
            arg3 = atoi(argv[i+1]);
            if (arg3 < 1) {
                fprintf(stderr, "Nespravny argument prikazu round\n");
                return -1;
            }
        
        }
        else if (!strcmp(argv[i], "int")){
            command_2 = INT;
            if (argc <= i+1) {
                fprintf(stderr, "Nezadal si cislo argumentu\n");
                return -1;
            }
            arg3 = atoi(argv[i+1]);
            if (arg3 < 1) {
                fprintf(stderr, "Nespravny argument prikazu int\n");
                return -1;
            }
        
        }
        else if (!strcmp(argv[i], "swap")){
            command_2 = SWAP;
        
            if (argc <= i+2) {
                fprintf(stderr, "Nezadal si cislo argumentu\n");
                return -1;
            }
            arg3 = atoi(argv[i+1]);
            arg4 = atoi(argv[i+2]);
            if (arg3 < 1 || arg4 < 1) {
                fprintf(stderr, "Nespravny argument prikazu swap\n");
                return -1;
            }
            else if (arg3 == arg4) command_2 = 0;
        }
        else if (!strcmp(argv[i], "move")){
            command_2 = MOVE;
            if (argc <= i+2) {
                fprintf(stderr, "Nezadal si cislo argumentu\n");
                return -1;
            }
            arg3 = atoi(argv[i+1]);
            arg4 = atoi(argv[i+2]);
            if (arg3 < 1 || arg4 < 1) {
                fprintf(stderr, "Nespravny argument prikazu move\n");
                return -1;
            }
            else if (arg3 == arg4) command_2 = 0;
        }
        else if (!strcmp(argv[i], "beginswith")) {
            command = BEGINSWITH;
            if (argc <= i+2) {
                fprintf(stderr, "Nezadal si cislo argumentu\n");
                return -1;
            }
            arg1 = atoi(argv[i+1]);
            args2 = argv[i+2];
            if (arg1 < 1) {
                fprintf(stderr, "Nespravny argument prikazu beginswith\n");
                return -1;
            }
        }
        else if (!strcmp(argv[i], "contains")) {
            command = CONTAINS;
            if (argc <= i+2) {
                fprintf(stderr, "Nezadal si cislo argumentu\n");
                return -1;
            }
            arg1 = atoi(argv[i+1]);
            args2 = argv[i+2];
            if (arg1 < 1) {
                fprintf(stderr, "Nespravny argument prikazu contains\n");
                return -1;
            }
        }
    }

    int row_count = 0;


    while (fgets(row, MAX, stdin)){
        char column[101]; 
        char help[101];
        int col_count = 0;
        int x = 0;
        // ratanie poctu stlpcov
        for (int i = 0; row[i] != '\n' && row[i] != '\0'; i++){
            if (is_delimiter(delimiters, row[i])){
                col_count++;
            }

        }
        col_count++;
        //cc je pomocna premenna do ktorej ukladam pocet stlpcov z prveho riadku aby som mala pocet ostatnych riadkov s cim porovnat
        if (cc == -1) cc = col_count;
        else if (cc != col_count){
            fprintf(stderr, "Chyba, zly pocet stlpcov na %d riadku\n", row_count+1);
            return -1;
        }

        if(command == IROW && row_count + 1 == arg1) {
            irow(cc, delimiters[0]); 
        }
        else if (command == DROW && row_count + 1 == arg1) {
            row_count++;  //pokial je zadany prikaz drow nevypisem nic a pokracujem dalej od foru
            continue;
        }
        else if (command == DROWS) {
            if (arg1 <= row_count + 1 && row_count + 1 <= arg2) {  //pokial so, v zadanom intervale tak vypis tychto riadkov preskocim
                row_count++;
                continue;
            }
        }
        

        col_count = 0;
       //sluzi este pre vypisanie posledneho stlpca
        if(command == ICOL && col_count + 1 == arg1) {
            icol(delimiters[0]);
        }
        
        
        for (int i = 0; row[i] != '\n'  && row[i] != '\0'; i++){
            int deleting = 0; //nastavenie vymazavania na 0, lebo nic mazat nechcem
            if (command == DCOLS && arg1 <= col_count + 1 && col_count + 1 <= arg2) {
                 deleting = 1; //pokial narazim na prikaz dcol a dcols, mazat chcem, nastavim ho na 1
            }
            if (is_delimiter(delimiters, row[i])){
                if (command == DCOLS) {
                    if (!deleting) {    //v nejakych pripadoch chcem delimiter vypisat v nejakych nie, pri negacii mazania zistime kedy delimiter vypisat chcem
                        column[x] = '\0';
                        printf("%s", column);
                        if (arg2 < cc || col_count + 2 != arg1) {
                            printf("%c", delimiters[0]);
                        }
                        
                    }
                    
                }
                else {
                    column[x] = '\0';
                    int result = command == ROWS && ((arg1 <= row_count + 1 && row_count + 1 <= arg2) || (arg1 == INF && arg2 == INF && feof(stdin))); 
                    if (command == BEGINSWITH) {
                        getcol(row, delimiters, arg1 - 1, help); //help je pomocne pole kde sa ulozi vysledok z funkcie 
                        result = strncmp(help, args2, strlen(args2)) == 0; 
                    }
                    else if (command == CONTAINS) {
                        getcol(row, delimiters, arg1 - 1, help);
                        result = strstr(help, args2) != 0;
                    }
                    if (result) {
                        if (command_2 == CSET && col_count + 1 == arg3) {
                            printf("%s", args);
                        }  
                        else if (command_2 == TOLOWER && col_count + 1 == arg3) { 
                            totiny(column);
                            
                        }
                        else if (command_2 == TOUPPER && col_count + 1 == arg3) { 
                            tobig(column);
                            
                        }
                        else if (command_2 == INT && col_count + 1 == arg3) {
                            toint(column);  
                        }
                        else if (command_2 == ROUND && col_count + 1 == arg3) {
                            toround(column);  
                        }
                        else if (command_2 == COPY && col_count + 1 == arg4) {
                           getcol(row, delimiters, arg3 - 1, help); //do funkcie poslem argument 1 a tým pádom sa miesto stlpca zadaneho arg4 vypise stlpec arg3
                           printf("%s", help);  
                        }
                        else if(command_2 == SWAP && col_count + 1 == arg4) {
                           getcol(row, delimiters, arg3 - 1, help); 
                           printf("%s", help);  
                        }
                        else if(command_2 == SWAP && col_count + 1 == arg3) {
                           getcol(row, delimiters, arg4 - 1, help); 
                           printf("%s", help);  
                        }
                        else if(command_2 == MOVE && col_count + 1 == arg4) {
                           getcol(row, delimiters, arg3 - 1, help); 
                           printf("%s%c%s", help, delimiters[0], column);
                        
                        
                        }
                        else if(command_2 == MOVE && col_count + 1 == arg3) {
                         //pokial stlpec zadany arg3 presunieme pred stlpec zadany arg4 tak ho na povodne miesto nevypisujeme preto je else if prazdny  
                        }
                        else {
                            printf("%s", column);
                        }
                        
                    }
                    else {
                        printf("%s", column);
                    }
                    
                    if(!(command_2 == MOVE && col_count + 1 == arg3)) printf("%c", delimiters[0]);
                }
                
                
                x = 0;

                if(command == ICOL && col_count + 1 == arg1) {
                    icol(delimiters[0]);
                }
                
                col_count++; 
                               
            }
            else if  (!deleting) {
                column[x] = row[i];
                x++;   
            }
            
        }

        //sluzi na spracovanie este posledneho riadku 
        column[x] = '\0';
        int result = command == ROWS && ((arg1 <= row_count + 1 && row_count + 1 <= arg2) || (arg1 == INF && arg2 == INF && feof(stdin)));
        if (command == BEGINSWITH) {
            getcol(row, delimiters, arg1 - 1, help);
            result = strncmp(help, args2, strlen(args2)) == 0;
        }
        else if (command == CONTAINS) {
            getcol(row, delimiters, arg1 - 1, help);
            result = strstr(help, args2) != 0;
        } 
        if (result) {
            if (command_2 == CSET && col_count + 1 == arg3) {
                printf("%s", args);
            }  
            else if (command_2 == TOLOWER && col_count + 1 == arg3) { 
                totiny(column);
                            
            }
            else if (command_2 == TOUPPER && col_count + 1 == arg3) { 
                tobig(column);
                            
            }
            else if (command_2 == INT && col_count + 1 == arg3) {
                toint(column);  
            }
            else if (command_2 == ROUND && col_count + 1 == arg3) {
                toround(column);  
            }
            else if (command_2 == COPY && col_count + 1 == arg4) {
                getcol(row, delimiters, arg3 - 1, help); 
                printf("%s", help);  
            }
            else if(command_2 == SWAP && col_count + 1 == arg4) {
                getcol(row, delimiters, arg3 - 1, help); 
                printf("%s", help);  
            }
            else if(command_2 == SWAP && col_count + 1 == arg3) {
                getcol(row, delimiters, arg4 - 1, help); 
                printf("%s", help);  
            }
            else if(command_2 == MOVE && col_count + 1 == arg4) {
                getcol(row, delimiters, arg3 - 1, help); 
                printf("%s%c%s", help, delimiters[0], column);
            
            
            }
            else if(command_2 == MOVE && col_count + 1 == arg3) {
                           
            }
            else {
                printf("%s", column);
            }
                        
        }
        else {
             printf("%s", column);
        }

    
       
        if(command == ACOL){
            icol(delimiters[0]);
        }
        

        printf("\n");

        row_count++; 
    }
    
    if(command == AROW) irow(cc, delimiters[0]);
     
    return 0;
}

//funkcia na rozpoznanie delimitera
int is_delimiter(char * array, char c) { //prijima pole znakov v riadku a delimiter
    for(int i = 0; array[i] != '\0'; i++){
        if (array[i] == c) return 1; 
    }

    return 0;
}

//funkcia na pridavanie riadka
void irow (int col_count, char d){ //funkcia prijima pocet stlpcov aby vedela kolko ich ma pridat a delimiter
    for (int i = 0; i < col_count - 1; i++){
        printf("%c", d);
    }
    printf("\n");
}

//funkcia na pridavanie prazdneho stlpca
void icol (char d){ //funkcia prijima na spracovanie delimiter, ktory nasledne len vypise
    printf("%c", d);
}

//funkcia prevadza vsetky pismena na male
void totiny (char *s){ //funkcia prijima na spracovanie konretu bunku
    for(int i = 0; s[i] != '\0'; i++){
        printf("%c", tolower(s[i]));
    }
}

//funkcia prevadza vsetky pismena na velke
void tobig (char *s){ //funkcia prijima na spracovanie konretu bunku
    for(int i = 0; s[i] != '\0'; i++){
        printf("%c", toupper(s[i]));
    }
}

//funkcia odstranuje desatinu cast z cisla
void toint (char *s) { //posielam do nej konkretnu bunku, ktoru potom spracovavam
    char *end;
    double d = strtod(s, &end); //prevadzanie znakov v bunke na double
    if (*end == '\0') printf("%d",(int) d); //pokial narazim na cislo pretypujem ho na int cim sa odstrani desatina cast
    else printf("%s", s);
}

//funkcia na zaokruhlovanie
void toround (char *s) { //posielam do nej konkretnu bunku
    char *end;
    double d = strtod(s, &end);  //prevadzanie znakov v bunke na double
    if (*end == '\0') {
        if (d > 0) printf("%d",(int) (d + 0.5)); 
        //pokial je cislo kladke, priratam k nemu 0.5 a pretypujem na int vdaka comu sa desatina cast odstrani, 
        //0.5 preto lebo ked mam cisla konciace od 0.1 po 0.4 tak hodnota cisla pred desatinou ciarkou sa mi nezmeni, teda po "useknuti" desatinej casti som ako keby zaokruhlila nadol
        //cislam od 0.5 po 0.9 sa hodnota pred desatinou ciarkou zvysi o jedna, teda po "useknuti" desatnej casti som ako keby zaokruhlila nahor
        else printf("%d",(int) (d - 0.5)); //pokial je cislo zaporne, odcitam 0.5 a pretypujem na int
    }
    else printf("%s", s);   
}

//pomocna funkcia, posielam do nej stĺpec, ktorý chceme práve vypisovať, delimiter, cislo stlpca, ktory chcem spracovavat a pomocne pole, kde vysledok ulozim
void getcol (char *s, char *d, int c, char *b) {

    int col_count = 0; 
    int pos = 0;
    for (int i = 0; s[i] != '\n' && s[i] != '\0'; i++) {
        if (is_delimiter(d, s[i])) {
            col_count++;
            if (col_count > c) break;
        }
        else if (col_count == c) {
            b[pos] = s[i];
            pos++;
            
        }  
    }
    b[pos] = '\0';
}





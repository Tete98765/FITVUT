/* 
    datum: 6.12.2020
    Projekt 2 - Praca s datovymi strukturami 
    IZP
    
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define TYPE_NUMBER 0
#define TYPE_STRING 1

typedef char *cell_t;

typedef struct {
    cell_t *cells;
    int cell_count;
    int max_cells;
}row_t;

typedef row_t * row_t_ptr; 

typedef struct {
    row_t_ptr *rows;
    int col_count;
    int row_count;
    int max_rows;
} table_t;

typedef table_t * table_t_ptr; 

typedef struct {
    double x;
    char *str;
    int type; 
} var_t;

typedef struct {
    int r;
    int c;
} rc_t;


int is_delimiter(char * array, char c); 
cell_t read_cell(FILE *file, char *delimiters, int *result_code);
row_t_ptr read_row(FILE *file, char *delimiters, int *result_code); 
table_t_ptr read_table(FILE *file, char *delimiters);
void print_table(FILE *file, table_t_ptr table, char d); 
void irow (table_t_ptr table, int r); 
void set(table_t_ptr table, int r, int c, int r1, int c1, char *text); 
void arow(table_t_ptr table, int r);
void drow(table_t_ptr table, int r);
void icol(table_t_ptr table, int c);
void acol(table_t_ptr table, int c);
void dcol(table_t_ptr table, int c);
void clear(table_t_ptr table, int r, int c); 
void swap(table_t_ptr table, int r, int c, int r1, int c1, int r2, int c2);
void sum(table_t_ptr table, int r, int c, int r1, int c1, int r2, int c2); 
void avg(table_t_ptr table, int r, int c, int r1, int c1, int r2, int c2);
void count(table_t_ptr table, int r, int c, int r1, int c1, int r2, int c2);
void len(table_t_ptr table, int r, int c, int r2, int c2);
rc_t find_max(table_t_ptr table, int r, int c, int r1, int c1);
rc_t find_min(table_t_ptr table, int r, int c, int r1, int c1);
rc_t find(table_t_ptr table, int r, int c, int r1, int c1, char *str);
void free_table(table_t_ptr table);

int main(int argc, char *argv[]) {
    char *delimiters = " ";
    char *sequence = 0; //pole pre nacitanie selekcie prikazov
    char *file_name;
    char commands[1000][1001];
    int command_count = 0; //pocitadlo poctu prikazov 
    if (argc == 5) {  //pripad kedy sa pri spustani programu zada delimiter
        if (!strcmp(argv[1], "-d")) {
            delimiters = argv[2];
        }
        sequence = argv[3];
        file_name = argv[4];
    }
    else if (argc == 3) { //pripad kedy pri spustani programu delimiter zadany nie je 
        sequence = argv[1];
        file_name = argv[2];
       
    }
    else {
       fprintf(stderr, "Neplatny pocet argumentov\n"); 
       return -1;
    } 

    FILE *file = fopen(file_name,"r");
    if (!file) {
        fprintf(stderr, "Subor sa nepodarilo otvorit");
        return -1;
    }


    table_t_ptr table = read_table(file, delimiters);
    fclose(file);

    if (sequence) { 
        int  j = 0; // premenna ktora pocita pocet znakov jedneho prikazu 
        for(int i = 0; sequence[i]!= '\0'; i++) {
            if (sequence[i] == ';') {
                commands[command_count][j] = '\0'; //ked nacitame cely prikaz ukoncime ho terminatorom
                j = 0;
                command_count++;
            }
            else {
                commands[command_count][j] = sequence[i];
                j++;
            }
        }
        commands[command_count][j] = '\0';
        command_count++;
    }
    
    long r = 0,c = 0; 
    int r1 = 0,c1 = 0;
    int r2 = 0,c2 = 0;
    int r_ = 0,c_ = 0;
    int r1_ = 0,c1_ = 0;
    char *end; 
    var_t vars[10]; //pole vyuzivane pri praci s docasnymi premennymi
    rc_t rc; //struktura na pomoc pri hladani 
    for(int i = 0; i < command_count; i++) { 
        char *command = commands[i];
        if (!strcmp(command,"[max]")) {
            rc = find_max(table, r-1, c-1, r1-1, c1-1);
            r = rc.r+1;
            c = rc.c+1;
            r1 = rc.r+1;
            c1 = rc.c+1;
        }
        else if (!strcmp(command,"[min]")){
            rc = find_min(table, r-1, c-1, r1-1, c1-1); 
            r = rc.r+1;
            c = rc.c+1;
            r1 = rc.r+1;
            c1 = rc.c+1;
        }
        else if (!strncmp(command,"[find ",6)) {
            int len = strlen(command); 
            command[len-1] = '\0'; //miesto ] napiseme \0 
            char *str = command + 6;
            rc_t rc = find(table, r-1, c-1, r1-1, c1-1, str); //funkcia find nam vrati poziciu bunky kde sa hladany retazec nachadza
            r = rc.r+1;
            c = rc.c+1;
            r1 = rc.r+1;
            c1 = rc.c+1;
        }
        else if (!strncmp(command,"def _",5)) {
            int v = command[5] - '0';  //od znaku odcitam hodnotu znaku 0 cim dostanem moje zadane cislo
            double d = strtod(table->rows[r-1]->cells[c-1], &end); 
            //podla toho ci bunka obsahuje cislo alebo retazec rozlisim typ docasnej premennej
                if (*end == '\0') {
                    vars[v].x = d;
                    vars[v].type = TYPE_NUMBER;
                } 
                else {
                    vars[v].str = table->rows[r-1]->cells[c-1];
                    vars[v].type = TYPE_STRING;
                }
                
        }
        else if (!strncmp(command,"use _",5)) {
            int v = command[5] - '0';
            if (vars[v].type == TYPE_NUMBER) {
               char help[100];
                sprintf(help,"%g", vars[v].x); //do pola help sa ulozi obsaj x a nasladne sa cez funkciu set vytlaci 
                set(table, r-1, c-1, r1-1, c1-1, help); 
            }
            else set(table, r-1, c-1, r1-1, c1-1, vars[v].str);
            
            
        }
        else if (!strncmp(command,"inc _",5)) {
           int v = command[5] - '0'; 
           if (vars[v].type == TYPE_NUMBER) vars[v].x++; 
           else {
               vars[v].type = TYPE_NUMBER; 
               free(vars[v].str);
               vars[v].x = 1; 
           }

        }
        else if (!strcmp(command,"[set]")) {
            //do pomocnych premennych ulozim hodnotu aktualnej pozicie
            r_ = r;
            c_ = c;
            r1_ = r1;
            c1_ = c1;
        }
        else if (!strcmp(command,"[_]")) {
            r = r_;
            c = c_;
            r1 = r1_;
            c1 = c1_;
        }
        
        
        else if (command[0] == '[') { // [_,2] 
            //prve cislo
            if (command[1] == '_') {
                r = 1;
                r1 = table->row_count; 
                end = command + 2; 
            }
            else {
               r = strtol(command + 1, &end, 10); 
               r1 = r;
               if (r < 1) {
                   fprintf(stderr,"Neplatne cislo\n");
                   free_table(table);
                    return -1;
                } 
            }
            //druhe cislo
            if (*(end + 1) == '_') {
                c = 1;
                c1 = table->col_count;
                end+= 2;
            }
            else {
                c = strtol(end + 1, &end, 10);
                c1 = c;
                if (c < 1) {
                   fprintf(stderr,"Neplatne cislo\n");
                   free_table(table);
                    return -1;
                }
            }
            //v pripade ak mam zvolenu selekciu pre 4 
            if (*end == ',') {
                //tretie cislo
                if (*(end + 1) == '-') {
                    r1 = table->row_count;
                    end+= + 2;
                }
                else {
                   r1 = strtol(end + 1, &end, 10); 
                   if (r1 < 1) {
                        fprintf(stderr,"Neplatne cislo\n");
                        free_table(table);
                        return -1;
                    }
                }
                //stvrte cislo
                if (*(end + 1) == '-') {
                    c1 = table->col_count;
                    end+= + 2;
                }
                else {
                   c1 = strtol(end + 1, &end, 10); 
                   if (c1 < 1) {
                        fprintf(stderr,"Neplatne cislo\n");
                        free_table(table);
                        return -1;
                    }
                }
            
            }
            if (r1 < r || c1 < c) {
                fprintf(stderr,"Neplatny vyber oblasti buniek\n");
                free_table(table);
                return -1;
            }     
            
              
        }
        //rozpoznavanie prikazov
        else if (!strcmp(command,"irow")) {
            irow(table,r);
        }
        else if(!strncmp(command, "set ", 4)) {
            set(table, r-1, c-1, r1-1, c1-1, command + 4);  
        }
        else if(!strcmp(command,"arow")) {
            arow(table,r);
            
        }
        else if(!strcmp(command,"drow")) {
            drow(table,r);
        }
        else if(!strcmp(command,"icol")) {
            icol(table,c);
        }
        else if(!strcmp(command,"acol")) {
            acol(table,c);
        }
        else if(!strcmp(command,"dcol")) {
            dcol(table,c);
        }
        else if(!strcmp(command,"clear")) {
            clear(table,r,c);  
        }
        else if(!strncmp(command, "swap ", 5)) {
            r2 = strtol(command + 6,&end, 10);
            c2 = strtol(end + 1,&end, 10);
            if (r2 < 1 || c2 < 1 || table->row_count <r2 || table->col_count < c2) {
                fprintf(stderr,"Neplatne cislo\n");
                free_table(table);
                return -1;
            }
            swap(table,r,c,r1,c1,r2,c2);
        } 
        else if(!strncmp(command, "sum ", 4)) {
            r2 = strtol(command + 5,&end, 10);
            c2 = strtol(end + 1,&end, 10);
            if (r2 < 1 || c2 < 1 || table->row_count <r2 || table->col_count < c2) {
                fprintf(stderr,"Neplatne cislo\n");
                free_table(table);
                return -1;
            }
            sum(table,r-1,c-1,r1-1,c1-1,r2-1,c2-1);
        } 
        else if(!strncmp(command, "avg ", 4)) {
            r2 = strtol(command + 5,&end, 10);
            c2 = strtol(end + 1,&end, 10);
            if (r2 < 1 || c2 < 1 || table->row_count <r2 || table->col_count < c2) {
                fprintf(stderr,"Neplatne cislo\n");
                free_table(table);
                return -1;
            }
            avg(table,r-1,c-1,r1-1,c1-1,r2-1,c2-1);
        } 
        else if(!strncmp(command, "count ", 6)) {
            r2 = strtol(command + 7,&end, 10);
            c2 = strtol(end + 1,&end, 10);
            if (r2 < 1 || c2 < 1 || table->row_count <r2 || table->col_count < c2) {
                fprintf(stderr,"Neplatne cislo\n");
                free_table(table);
                return -1;
            }
            count(table,r-1,c-1,r1-1,c1-1,r2-1,c2-1);
        } 
        else if(!strncmp(command, "len ", 4)) {
            r2 = strtol(command + 5,&end, 10);
            c2 = strtol(end + 1,&end, 10);
            if (r2 < 1 || c2 < 1 || table->row_count <r2 || table->col_count < c2) {
                fprintf(stderr,"Neplatne cislo\n");
                free_table(table);
                return -1;
            }
            len(table,r-1,c-1,r2-1,c2-1);
        }
        else {
            fprintf(stderr,"Neplatne cislo\n");
            free_table(table);
            return -1;
        } 
    }

    file = fopen(file_name,"w"); //subor znovu otvorime na zapis
    print_table(file, table, delimiters[0]);
    free_table(table); 
    fclose(file);  

    return 0;
}

int is_delimiter(char * array, char c) { 
    for(int i = 0; array[i] != '\0'; i++){
        if (array[i] == c) return 1; 
    }

    return 0;
}

cell_t read_cell(FILE *file, char *delimiters, int *result_code) {
    int size = 16;
    cell_t cell =  (cell_t) malloc((size + 1) * sizeof(char)); //alokujeme velkost + 1 bajt navyse pre /0
    if (!cell) return 0;
    int i = 0;
    while (1) {
        char c = fgetc(file);
        if (is_delimiter(delimiters, c)) { //ked sme narazili na delimiter bunku uz dalej nenacitavame
            *result_code = 0;
            break; // cyklus mozme ukoncit
        }
        else if(c == '\n') { //ked sme narazili na koniec riadku uz dalej nenacitavame
            *result_code = 1;
            break; // cyklus mozme ukoncit
            
        }
        else if (c == EOF) { //ked sme narazili na koniec suboru uz dalej nenacitavame
            *result_code = 2;
            break; // cyklus mozme ukoncit
        }
        else {
            if (i == size) {
                size*=2;
                cell = (cell_t) realloc(cell, size + 1);
            }
            cell[i] = c;
            i++;
        }
        
    }
    cell[i] = '\0'; 
    
    return cell;
}

row_t_ptr read_row(FILE *file, char *delimiters, int *result_code) {
    row_t_ptr row = (row_t_ptr) malloc(sizeof(row_t));
    if (!row) return 0;
    row->max_cells = 16;
    row->cells = (cell_t *) malloc(row->max_cells * sizeof(cell_t));
    row->cell_count = 0;
    while (1) {
        int rc;
        cell_t cell = read_cell(file, delimiters, &rc);
        if (row->cell_count == row->max_cells) {
            row->max_cells*=2;
            row->cells = (cell_t*) realloc(row->cells, row->max_cells*sizeof(cell_t));
        }
        row->cells[row->cell_count] = cell;
        row->cell_count++;
        if (rc == 1) {  //ked sme narazili na koniec riadku uz dalej nenacitavame
            *result_code = 1;
            break;
        }
        else if (rc == 2) { //ked sme narazili na koniec suboru uz dalej nenacitavame
            *result_code = 2;
            break;
        }
        
    }

   return row; 
}

table_t_ptr read_table(FILE *file, char *delimiters) {
    int rc;
    table_t_ptr table = (table_t_ptr) malloc(sizeof(table_t));
    if (!table) return 0;
    table->max_rows = 16;
    table->row_count = 0;
    table->col_count = 0;
    table->rows = (row_t_ptr*) malloc(table->max_rows * sizeof(row_t_ptr));
    while (!feof(file)) {
        char c = fgetc(file);
        if (c == EOF) break; //program sa pozera dopredu ci ma este vyznam dalej nacitavat
        else ungetc (c,file);
        row_t_ptr row = read_row(file, delimiters, &rc);
        if (table->row_count == table->max_rows) {
            table->max_rows*=2;
            table->rows = (row_t_ptr*) realloc(table->rows, table->max_rows*sizeof(row_t_ptr));
        }
        table->rows[table->row_count] = row; 
        table->row_count++;
        if (row->cell_count > table->col_count) table->col_count = row->cell_count;
               
    }
    return table;
}

void print_table(FILE *file, table_t_ptr table, char d) {
    for (int i = 0; i < table->row_count; i++) { 
        if (table->rows[i]->cell_count && table->rows[i]->cells[0]) fprintf(file, "%s", table->rows[i]->cells[0]);
        for (int j = 1; j < table->col_count; j++) {
            fprintf(file, "%c", d);
            if (table->rows[i]->cells[j]) fprintf(file, "%s", table->rows[i]->cells[j]);
        }
        if (i != table->row_count-1) fprintf(file,"\n"); //kazdy riadok sa ukonci /n
    }
}

void irow (table_t_ptr table, int r) {
    if (table->row_count == table->max_rows) {
        table->max_rows*=2;
        table->rows = (row_t_ptr*) realloc(table->rows, table->max_rows*sizeof(row_t_ptr));
    }
    r--;
    table->row_count++;
    for (int i = table->row_count - 1; i > r; i--) { //od konca program prechadza jednotlive riadky a pocuva ich o 1(vratne zadaneho riadku)
        table->rows[i] = table->rows[i - 1];
    }
    table->rows[r] = (row_t_ptr) malloc(sizeof(row_t)); //novo vzniknuty riadok treba zaalokovat
    if (!table->rows[r]) return 0;
    table->rows[r]->cell_count = table->col_count;
    table->rows[r]->cells = (cell_t *) calloc(table->col_count, sizeof(cell_t));
}

void set(table_t_ptr table, int r, int c, int r1, int c1, char *text) {
    int len = strlen(text); 
    for (int i = r; i <= r1; i++) {
        for (int j = c; j <= c1; j++) {
            free(table->rows[i]->cells[j]); //pred zapisom treba povodny obsah bunky uvolnit
            table->rows[i]->cells[j] = (cell_t) malloc(len+1); //znovu zaalokovat pre novy obsah 
            strcpy(table->rows[i]->cells[j], text); 
            
        }  
    }
}

void arow(table_t_ptr table, int r) {
    if (table->row_count == table->max_rows) {
        table->max_rows*=2;
        table->rows = (row_t_ptr*) realloc(table->rows, table->max_rows*sizeof(row_t_ptr));
    }
    r--;
    table->row_count++;
    for (int i = table->row_count - 1; i > r+1; i--) { //podobne ako irow len cyklus skonci skor o 1 riadok 
        table->rows[i] = table->rows[i - 1];
    }
    table->rows[r+1] = (row_t_ptr) malloc(sizeof(row_t)); //novo vzniknuty riadok treba zaalokovat
    if (!table->rows[r]) return 0;
    table->rows[r+1]->cell_count = table->col_count;
    table->rows[r+1]->cells = (cell_t *) calloc(table->col_count, sizeof(cell_t));
}

void drow(table_t_ptr table, int r) {
    r--;
    //funkcia dany riadok uvolni a tym padom sa nevypise
    for (int i = 0; i < table->rows[r]->cell_count; i++) {
        free(table->rows[r]->cells[i]); 
    }
    free(table->rows[r]);

    for (int i = r; i < table->row_count; i++) {
        table->rows[i] = table->rows[i + 1];
    }
    table->row_count--;
}

void icol(table_t_ptr table, int c) {
    c--;
    //funckia funguje podobne ako irow a arow len prechadza po stlpcoh odzanu a posva stlpce
    for (int i = 0; i < table->row_count; i++) {
        row_t_ptr row = table->rows[i];
        if (row->cell_count == row->max_cells) {
            row->max_cells*=2;
            row->cells = (cell_t*) realloc(row->cells, row->max_cells*sizeof(cell_t));
        }
        row->cell_count++;
        for (int j = row->cell_count - 1; j > c; j--) {
            row->cells[j] = row->cells[j-1];
        }
        row->cells[c] = 0;
    
    }
    table->col_count++;
}

void acol(table_t_ptr table, int c) {
    c--;
    //ako icol len skonci o jeden stlpec nez je zadany skor
    for (int i = 0; i < table->row_count; i++) {
        row_t_ptr row = table->rows[i];
        if (row->cell_count == row->max_cells) {
            row->max_cells*=2;
            row->cells = (cell_t*) realloc(row->cells, row->max_cells*sizeof(cell_t));
        }
        row->cell_count++;
        for (int j = row->cell_count - 1; j > c + 1; j--) {
            row->cells[j] = row->cells[j-1];
        }
        row->cells[c+1] = 0;
    
    }
    table->col_count++;
}

void dcol(table_t_ptr table, int c) {
    c--;
    //funkcia funguje na rovnakom principe ako drow len prechadza po stlpcoch
    for (int i = 0; i < table->row_count; i++) {
        row_t_ptr row = table->rows[i];
        free (row->cells[c]);
        for (int j = c; j < row->cell_count; j++) {
            row->cells[j] = row->cells[j+1];
        }
        row->cell_count--; 
    }
    table->col_count--;

}

void clear(table_t_ptr table, int r, int c) {
    free(table->rows[r-1]->cells[c-1]); //uvolnim vybrane bunky a tym sa odstrania 
    table->rows[r-1]->cells[c-1] = 0; 
}

void swap(table_t_ptr table, int r, int c, int r1, int c1, int r2, int c2) {
    //vymena stlpcov pomocou pomocneho pola tmp
    for (int i=r; i <= r1; i++) {
       for (int j = c; j <= c1; j++) {
            char *tmp = table->rows[i-1]->cells[j-1];
            table->rows[i-1]->cells[j-1] = table->rows[r2-1]->cells[c2-1];
            table->rows[r2-1]->cells[c2-1] = tmp;
        }  
    }
    
}

void sum(table_t_ptr table, int r, int c, int r1, int c1, int r2, int c2) {
    double sum = 0;
    for (int i = r; i <= r1; i++) {
        for (int j = c; j <= c1; j++) {
            sum+= strtod(table->rows[i]->cells[j], 0); //postupne premienam znaky cisel v jednotlivych bunkach na cislo a rovno pricitavam
            
        }  
    }
    free(table->rows[r2]->cells[c2]);
    table->rows[r2]->cells[c2] = (cell_t) malloc(16);
    if (!table->rows[r2]->cells[c2]) return 0;
    sprintf(table->rows[r2]->cells[c2],"%g",sum);
     
}

void avg(table_t_ptr table, int r, int c, int r1, int c1, int r2, int c2) {
   double sum = 0;
   int count = 0;
   char *end;
    for (int i = r; i <= r1; i++) {
        for (int j = c; j <= c1; j++) {
            double d = strtod(table->rows[i]->cells[j], &end); //postupne premienanie znakov na cisla
            if (*end == '\0') { 
                count++;
                sum+= d;
            } 
        }  
    }
    free(table->rows[r2]->cells[c2]);
    table->rows[r2]->cells[c2] = (cell_t) malloc(16);
    sprintf(table->rows[r2]->cells[c2],"%g",sum/count); 
}

void count(table_t_ptr table, int r, int c, int r1, int c1, int r2, int c2) {
   int count = 0;
    for (int i = r; i <= r1; i++) {
        for (int j = c; j <= c1; j++) {
            if (strlen(table->rows[i]->cells[j])) count++; //ked bunka nieco obsahuje, pocitadlo sa zvacsi 
            
        }  
    }
    free(table->rows[r2]->cells[c2]);
    table->rows[r2]->cells[c2] = (cell_t) malloc(16);
    if(!table->rows[r2]->cells[c2]) return 0;
    sprintf(table->rows[r2]->cells[c2],"%d",count); 
}

void len(table_t_ptr table, int r, int c, int r2, int c2) {
    free(table->rows[r2]->cells[c2]);
    table->rows[r2]->cells[c2] = (cell_t) malloc(16);
    if(!table->rows[r2]->cells[c2]) return 0;
    if (table->rows[r]->cells[c])
        sprintf(table->rows[r2]->cells[c2],"%d",(int) strlen(table->rows[r]->cells[c])); //do bunky r2 c2 vypisem dlzku retacka na pocicii r,c
    else 
        sprintf(table->rows[r2]->cells[c2],"0");
}

rc_t find_max(table_t_ptr table, int r, int c, int r1, int c1) {
    unsigned long long int inf = 0x7ff0000000000000; //hodnota nekonečna v hexadecimalnej sustave 
    double max = *((double *) &inf) * -1; //vynasobim -1 lebo vzdy od - nekonecna viem najst najvacsie cislo
    rc_t rc;
    rc.r = -1;
    rc.c = -1;
    for (int i = r; i <= r1; i++) {
        for (int j = c; j <= c1; j++) {
            char *end;
            double d = strtod(table->rows[i]->cells[j], &end); 
            if (*end == '\0') {
                if (d > max) {
                   max = d; 
                   rc.r = i;
                   rc.c = j;
                } 
            }   
        }  
    }
   return rc;
}

rc_t find_min(table_t_ptr table, int r, int c, int r1, int c1) {
    unsigned long long int inf = 0x7ff0000000000000; 
    double min = *((double *) &inf); //vzdy od nekonecna viem najst mensie cislo
    rc_t rc;
    rc.r = -1;
    rc.c = -1;
    for (int i = r; i <= r1; i++) {
        for (int j = c; j <= c1; j++) {
            char *end;
            double d = strtod(table->rows[i]->cells[j], &end); 
            if (*end == '\0') {
                if (d < min) {
                   min = d; 
                   rc.r = i;
                   rc.c = j;
                } 
            }
        }  
    }
    return rc;
}

rc_t find(table_t_ptr table, int r, int c, int r1, int c1, char *str) {
    for (int i = r; i <= r1; i++) {
        for (int j = c; j <= c1; j++) {
            if (strstr(table->rows[i]->cells[j],str)) { //porovnavanie retazcou 
                rc_t rc;
                rc.r = i;
                rc.c = j;
                return rc;
            } 
        }  
    }
    rc_t rc; //ked retazec najdem vrati sa pozicia tohto retazca
    rc.r = -1;
    rc.c = -1;
    return rc;
}

void free_table(table_t_ptr table) {
    //postupne uvolnovanie tabulky od najmensich casti(buniek) az po celu tabulku
    for (int i = 0; i < table->row_count; i++) {
        for (int j = 0; j < table->col_count; j++) {
            free(table->rows[i]->cells[j]);
        }
        free(table->rows[i]->cells);
        free(table->rows[i]); 
    }
    free(table->rows);
    free(table);
}

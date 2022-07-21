#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <signal.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>

void check_arg(int argc, char *argv[]) {
    //kontrola či užívatel zadal port
    if (argc != 2) {
        fprintf(stderr, "Please enter one param\n");
        exit(EXIT_FAILURE);
    }
}

void hostname(char *input) {
    char hostname[1024];
    int rc;
    //zistenie doménového mena
    rc = gethostname(hostname, sizeof(hostname));
    if (rc == -1) {
        perror("ERROR: gethostname");
        exit (EXIT_FAILURE);
    }
    snprintf(hostname + strlen(hostname), 1024, "\n");
    snprintf(input , 1024, "HTTP/1.1 200 OK\r\nContent-Type: text/plain; charset=utf-8\r\nContent-Length: %ld\r\n\r\n%s\r\n", strlen(hostname), hostname);
}

//zdroj https://www.codegrepper.com/code-examples/c/remove+substring+from+string+c
void delchar(char *string,int starting_pos, int no_of_char)  {
  if ((starting_pos + no_of_char - 1) <= strlen(string) )
    strcpy(&string[starting_pos-1], &string[starting_pos + no_of_char-1]);
}


void cpu_name(char *input) {
    //otvorenie informacií o CPU pre čítanie
    FILE *fp = fopen("/proc/cpuinfo", "r");
    size_t n = 0;
    char *line = NULL;
    char info[1024];
    //prechod jednotlivými riadkami, kedy sa snažím nájsť ten, ktorý obsahuje informácie o názve modelu
    while (getline(&line, &n, fp) > 0) {
        if (strstr(line, "model name") || strstr(line, "název modelu")) {
                //kopírovanie riadna obsahujúceho model name
                strcpy(info, line);
            }
    }
    free(line);
    fclose(fp); 

    //skopírovaný riadok je vo formáte model name : názov. Funkcia oreže model name: a ponechá len názov processoru
    delchar(info, 1, 13);
    snprintf(input, 1024, "HTTP/1.1 200 OK\r\nContent-Type: text/plain; charset=utf-8\r\nContent-Length: %ld\r\n\r\n%s\r\n",strlen(info), info); 
}

//zdroj https://www.binarytides.com/socket-programming-c-linux-tutorial/
void load(char *input) {
    char str[100];
	char d[2] = " ";
	char* token;
	int i = 0,times,lag;
	long int sum = 0, idle, last_sum = 0,last_idle = 0;
	long double idle_fraction;
 
    times = 5; //koľko krát sa vykoná load
    lag = 20000; //sleep 20ms

	while (times > 0){
        //otvorenie informacií o CPU pre čítanie a náslené orezanie prvého slova na prvom riadku
	    FILE* fp = fopen("/proc/stat","r");
	    i = 0;
		fgets(str, 100, fp);
		fclose(fp);
		token = strtok(str,d);
	    sum = 0;
		
        //suma všetkých nájdených časov aby sme získali celkový čas
        while (token != NULL){
			token = strtok(NULL, d);
				if(token != NULL){
					sum += atoi(token);
				    if (i==3)
					idle = atoi(token);
				    i++;
			    }
		}

        //výpočet loadu        
        idle_fraction = 100 - (idle-last_idle)*100.0/(sum-last_sum);
 
		last_idle = idle;
		last_sum = sum;

        //kvôli lepším výsledom uspatie na 20ms
		times--;
		usleep(lag);
    }

    snprintf(str, 100, "%.0Lf%%\n", idle_fraction);
    snprintf(input, 1024, "HTTP/1.1 200 OK\r\nContent-Type: text/plain; charset=utf-8\r\nContent-Length: %ld\r\n\r\n%s\r\n", strlen(str), str);
}

int main (int argc, char *argv[]) {
    check_arg(argc, argv);

    int server_socket;
    struct sockaddr_in server_adress;
    int adress_len = sizeof(server_adress);
    int port_number = atoi(argv[1]); 

    //vytvorenie socketu
    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket <= 0) {
        perror("ERROR: socket");
        exit(EXIT_FAILURE);
    }

    int optval = 1;
    int rc;
    rc = setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR, (const void*)&optval, sizeof(int)); 
    if (rc < 0){
		perror("ERROR: setsockopt");
		exit(EXIT_FAILURE);
	}

    //vyplenie informacií
    server_adress.sin_family = AF_INET;
    server_adress.sin_addr.s_addr = htonl(INADDR_ANY);
    server_adress.sin_port = htons((unsigned short)port_number);

    //nadväzovanie spojenia 
    if ((bind(server_socket, (struct sockaddr *)&server_adress, adress_len)) < 0) {
        perror("ERROR: bind");
        exit(EXIT_FAILURE);
    }

    //server je pripravený "počúvat", obmedzenie počtu pripojení na 5
    if (listen(server_socket, 5) < 0) {
        perror("ERROR: listen");
        exit(EXIT_FAILURE);
    }

    int new_socket;
    char buffer[1024];
    char input[1024];
    int valread;

    while(1) {
        //prijatie dát od klienta
        new_socket = accept(server_socket, (struct sockaddr*)&server_adress, &adress_len);
        if (new_socket < 0) {
            perror("ERROR: accept");
            exit(EXIT_FAILURE);
        }

        valread = read(new_socket , buffer, 1024);
        if (valread <= 0) break;
        
        //vyhľadávanie kľúčového slova v zadanom http
        if (strstr(buffer, " /hostname ")) {
            hostname(input);
        }  
        else if (strstr(buffer, " /cpu-name ")) {
            cpu_name(input);   
        }  
        else if (strstr(buffer, " /load ")) {
            load(input);    
        }  
        else {
            perror("ERROR: invalid argument");
            exit (EXIT_FAILURE); 
        }  

        //odoslanie správy
        send(new_socket , input, strlen(input), 0);

        close(new_socket);
    }

}
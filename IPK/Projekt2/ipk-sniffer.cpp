#include <stdio.h>
#include <ctype.h>
#include <pcap.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <time.h>
#include <netinet/ip_icmp.h>
#include <netinet/udp.h>
#include <netinet/tcp.h>
#include <netinet/ip.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <iostream>
#include <cstdio>

int main(int argc, char *argv[]) {
    bool i_arg = false; //zadane rozhranie
    bool p_arg = false; //zadany port
    bool t_arg = false; //zadane tcp
    bool u_arg = false; //zadane udp
    bool icmp_arg = false; //zadane icmp
    bool arp_arg = false; //zadane arp
    int n_arg = false; //zadanie n
    int n = 1; //hodnota parametru n
    int len = 0; //pre zistenie co nasleduje za argumentom
    char i_val[50], tmp;
    bool empty = true;

    //ked nie je zadany ziadny argument tak vypisujem vsetko
    if (argc == 1) {
        //printf("just one argument\n");

    }

    for (int i = 1; i < argc; i++) {
        if(strcmp(argv[i], "-i") == 0 || strcmp(argv[i], "--interface") == 0) {
            if (i_arg) { //ak bol uz argument raz zadany
                fprintf(stderr, "Argument -i has already been entered"); 
                exit(1);
            }

            if (argc - 1 != i) { //ak za -i este nieco nachadaza, pozriem co za argument to je 
                std::string tmp =  argv[i+1];
                if (strcmp(&tmp[0], "-") != 0) { //ak nasledujuce slovo nezacina - tak viem, ze za -i bude nasledovat specifikacia 
                    strcpy(i_val, argv[i+1]);
                    printf("%s\n", i_val);
                    empty = false;
                    i++; //preskocim kontrolu dalsieho prvku
                }
            }
            i_arg = true;
        }

        else if(strcmp(argv[i], "-p") == 0) {
            if (p_arg) { //ak bol uz argument raz zadany
                fprintf(stderr, "Argument -p has already been entered"); 
                exit(1);
            }
            if (argc - 1 != i) {  //ak za -p este nieco nachadaza, pozriem co za argument to je
                if (isdigit(*argv[i+1])) { //pozriem ci je argumet cislo, ak ano tak si ho ulozim
                    n = atoi(argv[i+1]);
                    i++;
                }
            }
            p_arg = true;
        } 

        else if (strcmp(argv[i], "-t") == 0 || strcmp(argv[i], "--tcp") == 0) {
            if (t_arg) { //ak bol uz argument raz zadany
                fprintf(stderr, "Argument -t/--tcp has already been entered"); 
                exit(1);
            }
            t_arg = true;
        }

        else if (strcmp(argv[i], "-u") == 0 || strcmp(argv[i], "--udp") == 0) {
            if (u_arg) { //ak bol uz argument raz zadany
                fprintf(stderr, "Argument -u/--udp has already been entered");
                exit(1);
            }
            u_arg = true;
        }

        else if (strcmp(argv[i], "--arp") == 0) {
            if (arp_arg) { //ak bol uz argument raz zadany
                fprintf(stderr, "Argument --arp has already been entered");
                exit(1);
            }
            
            arp_arg = true;
        }

        else if (strcmp(argv[i], "--icmp") == 0) {
            if (icmp_arg) { //ak bol uz argument raz zadany
                fprintf(stderr, "Argument --icmp has already been entered");
                exit(1);
            }
            
            icmp_arg = true;
        }

        else if (strcmp(argv[i], "-n") == 0) {
            if (n_arg) { //ak bol uz argument raz zadany
                fprintf(stderr, "Argument -n has already been entered");
                exit(1);
            }
            if (argc - 1 != i) { //ak za -n este nieco nachadaza, pozriem co za argument to je
                if (isdigit(*argv[i+1])) { //pozriem ci je argumet cislo, ak ano tak si ho ulozim
                    n = atoi(argv[i+1]);
                    printf("%d\n", n);
                    i++;
                }
            }
            n_arg = true;
        }
        else {
            fprintf(stderr, "Invalid argument");
            exit(1);
        }

    }

    //pokial je i zadane bez parametrov, vypisujem zoznam vsetkych aktivnych rozhrani
    // kód je prevzatý zo stránky https://stackoverflow.com/questions/67880616/libpcap-pcap-findalldevs-causing-memory-leak
    if (empty == true) {
       char errbuf[PCAP_ERRBUF_SIZE];
        pcap_if_t *itf = NULL;

        if(pcap_findalldevs(&itf, errbuf) == 0) {
            pcap_if_t *it = itf;
            while (it) {
                printf("%s\n", it->name);
                it = it->next;
            }
            pcap_freealldevs(itf);
        }
        else {
            printf("error: %s\n", errbuf);
            exit(-1);
        }
    }

    
}
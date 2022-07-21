/*error.c
 * @Řešení IJC-DU1, příklad b), 23.3.2021
 * @Fakulta: VUT FIT
 * @přeloženo gcc 2.32-1
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>

#include "error.h"


void print_help(va_list arg, const char* fmt)
{
    char* zaciatok = "CHYBA :";
    int velkostHlasky = strlen(zaciatok) + strlen(fmt) + 1;
    char hlaska[velkostHlasky];
    
    strcpy(hlaska, zaciatok);
    strcat(hlaska, fmt);
    hlaska[velkostHlasky] = '\0';
    
    vfprintf(stderr, hlaska, arg);
}

void warning_msg(const char *fmt, ...)
{
    va_list arg;
    va_start(arg, fmt);

    print_help(arg, fmt);

    va_end(arg);
}

void error_exit(const char *fmt, ...)
{
    va_list arg;
    va_start(arg, fmt);

    print_help(arg, fmt);
    
    va_end(arg);

    exit(1);
}

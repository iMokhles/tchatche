#include <regex.h>
#include "regexmanager.h"

#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_YELLOW  "\x1b[33m"
#define ANSI_COLOR_BLUE    "\x1b[34m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_CYAN    "\x1b[36m"
#define ANSI_COLOR_RESET   "\x1b[0m"

int main (void)
{

    printf("%d", strlen(ANSI_COLOR_RED));

    char *str_request = "bonjour @Bachir ça va ? @Boris";

    char* pointeur = str_request;
    char* result;
    char* before = (char*) calloc (1, sizeof(char));
    char* replace = (char*) calloc (1, sizeof(char));
    int incrementation = 0;


    while(*pointeur != '\0') // parcours toute la chaîne jusqu'à sa fin
    {

        if(*pointeur == '@') // tombe sur un nom (à colorier)
        {

            int size_tmp = 0;
            char* string_tmp = (char*) calloc (1, sizeof(char));
            while(*pointeur != ' ') // fin du nom (fin de la coloration)
            {
            
                *(string_tmp + (size_tmp++)) = *pointeur;
                string_tmp = (char*) realloc(string_tmp, sizeof(char) * size_tmp+1);

                pointeur++;
            }

            replace = (char*) realloc (replace, strlen(string_tmp) + 
                                                strlen(ANSI_COLOR_BLUE) + strlen(ANSI_COLOR_RESET));

            strcpy(replace, ANSI_COLOR_RED);
            strcat(replace, string_tmp);
            strcat(replace, ANSI_COLOR_RESET);

            result = (char*) realloc (result, strlen(result) + strlen(before) + strlen(replace));
            strcat(result, before); // on met ce qu'il y avait avant
            strcat(result, replace);

            incrementation = 0;
            before = (char*) realloc (before, sizeof(char));
        }

        *(before + (incrementation++)) = *pointeur;
        before = (char*) realloc(before, sizeof(char) * incrementation+1);
        pointeur++;
    }

    printf("%s", result);

}
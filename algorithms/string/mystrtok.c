#include <stdio.h>
#include <stdlib.h>

char *mystrtok(char *src, char *token)
{
    char * ps = NULL; 
    static char *save = NULL;
    char * pd = token;
    char * tmpsrc = NULL;
    char * tmptoken = pd;

    if (src != NULL)
        save = src;

    if (save == NULL)
        return NULL;

    for (ps = save, tmpsrc = save; *tmpsrc != '\0'; tmpsrc++, tmptoken = pd){
           for (tmptoken; *tmptoken != '\0' && *tmptoken != *tmpsrc; tmptoken++)
                ;
           if (*tmpsrc == *tmptoken){
               *tmpsrc = '\0';
               save = ++tmpsrc;
               return ps;
           }
    }

    if (*tmpsrc == '\0')
        save = NULL;

    return ps;
}

int main(int c, char **v)
{
    //if (c != 3)
      //  return -1;


    //printf("%s\n", strtok(v[1], v[2]));

    //char str[] ="arcd,deewrg-98r|rewe,fhfe-rdd4|5h32";

    if (c != 2){
        perror("usage...");
        return -1;
    }

   // char str[] = "南昌市    330000  峡江县  331400  彭泽县  332700  铅山县  334500";
   char *str = v[1];
   int i = 0;
   char *tmp = NULL;
    char *ch = mystrtok(str, " ");

    while(ch != NULL)
    {
        i++;
   //     printf("%s\n", ch);
        if (i%2){
            tmp = ch;
        }
        else{
            printf("%s  %s\n", tmp, ch);
        }



        ch = mystrtok(NULL, " ");
    }

    return 0;
}


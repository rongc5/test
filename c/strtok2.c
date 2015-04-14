#include<stdio.h>

#include<string.h>



int main(void)

{

    char str[] ="arcd,deewrg-98r|rewe,fhfe-rdd4|5h32";

    char *ch = strtok(str, ",-|");

    while(ch != NULL)

    {

        printf("%s\n", ch);

        ch = strtok(NULL, "-|,");

    }

}


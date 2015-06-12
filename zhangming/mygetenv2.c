#include <stdio.h>
#include <stdlib.h>
#include <string.h>

extern char **environ;
static char envbuf[512];

char * mygetenv(const char * name)
{
    int i;
    char * ptr;
    
    for (i = 0; environ[i] != NULL; i++)
    {
        if (strncmp(name, environ[i], strlen(name)) == 0 &&
                environ[i][strlen(name)] == '=')
        {
            ptr = environ[i] + strlen(name) + 1;
            strcpy(envbuf, ptr);
            return envbuf;
        }
    }
    return NULL;
}


int main(void)
{
    int i = 0;

    printf("%s\n", mygetenv("PATH"));

    return 0;
}

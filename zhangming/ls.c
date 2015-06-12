#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>

int main(int c, char **v)
{
    if (c != 2)
    {
        perror("usage...");
        exit(1);
    }
    struct dirent *r;
    DIR *p;
    int i = 0;

    p = opendir(v[1]);
    if (p == NULL)
    {
        perror("opendir");
        exit(1);
    }

    r = readdir(p);
    if (NULL == r)
    {
        perror("readdir");
        exit(1);
    }

    while ((r= readdir(p)) != NULL)
    {
        printf("%-15s", r->d_name);
        i++;
        if (i%5 == 0)
            printf("\n");
    }
    printf("\n");

    closedir(p);
    
    return 0;
}

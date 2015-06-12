#include <stdio.h>
#include <stdlib.h>

int main(void)
{
    char *lineptr = NULL;
    size_t n;
    int ret;
    FILE *fp;

    fp = fopen("/etc/passwd", "r");
    if (!fp)
    {
        perror("fopen");
        exit(1);
    }

    while (1)
    {
        ret = getline(&lineptr, &n, fp);
        if (-1 == ret)
        {
            if (feof(fp))
                break;
            else
            {
                perror("getline");
                exit(1);
            }
        }
        printf("%s", lineptr);
    }

    if (lineptr)
        free(lineptr);
    fclose(fp);
    exit(0);
}

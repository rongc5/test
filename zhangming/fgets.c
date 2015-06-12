#include <stdio.h>
#include <stdlib.h>

int main(void)
{
    FILE *fp;
    char buf[512];
    char *retptr;

    fp = fopen("1.c", "r");
    if (NULL == fp)
    {
        perror("fopen");
        exit(1);
    }

    while (1)
    {
        retptr = fgets(buf, 512, fp);
        if (retptr == NULL)
        {
            if (feof(fp))
            {
                break;
            }
            else
            {
                perror("fgets");
                exit(1);
            }
        }

        if (buf[strlen(buf) - 1] == '\n')
        {
            buf[strlen(buf) - 1] = '\0';
        }

        printf("%s\n", buf);
    }

    fclose(fp);

    return 0;
}

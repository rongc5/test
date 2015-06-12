#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <unistd.h>
#include <string.h>

int read_dir(const char *path)
{
    DIR *dirp, *nextdirp;
    struct dirent *res, *nextres;
    char nextpath[256];

    strcpy(nextpath, path);
    strcat(nextpath, "/");

    dirp = opendir(path);
    if (dirp == NULL)
    {
        perror("opendir");
        return -1;
    }

    while (1)
    {
        res = readdir(dirp);
        if (NULL == res)
            break;
        printf("%s ", res->d_name);

        if (strcmp(res->d_name, ".") != 0 && strcmp(res->d_name, "..") != 0
                && res->d_name == DT_DIR)
        {
            strcat(nextpath, res->d_name);
            nextdirp = opendir(nextpath);
            if (NULL == nextdirp)
            {
                perror("opendir");
                return -1;
            }
            while (1)
            {
                nextres = readdir(nextdirp);
                if (nextres == NULL)
                    break;
                printf("%s:%s ", res->d_name, nextres->d_name);
            }

            strcpy(nextpath, path);
            strcat(nextpath, "/");
            closedir(nextdirp);
        }
    }
    printf("\n");

    closedir(dirp);
    return 0;
}

int main(int c, char **v)
{
    if (c != 2)
    {
        fprintf(stderr, "usage...\n");
        exit(1);
    }

    read_dir(v[1]);

    return 0;
}

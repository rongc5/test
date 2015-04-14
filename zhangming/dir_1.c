#include <stdio.h>
#include <dirent.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <string.h>

int get_file_size(char *name)
{
    struct stat buf;
    int ret;

    ret = lstat(name, &buf);
    if (-1 == ret)
    {
        perror("lstat");
        return 0;
    }

    return buf.st_size;      
}

int get_size(char *name)
{
    struct dirent * res;
    DIR * p;
    int ret;
    char pwdbuf[512];
    int size = 0;

    size = get_file_size(name);

    p = opendir(name);
    if (!p)
    {
        perror("opendir");
        return 0;
    }
    ret = chdir(name);
    if (-1 == ret)
    {
        perror("chdir");
        return 0;
    }

    while (1)
    {
        res = readdir(p);
        if (res == NULL)
            break;
        if (res->d_type == DT_DIR && strcmp(res->d_name, ".") != 0&& 
                strcmp(res->d_name, "..") != 0)
        {
            size += get_size(res->d_name);
        }
        else if (res->d_type != DT_DIR)
        {
            size += get_file_size(res->d_name);
        }
    }
    printf("%-9d  %s\n", size, getcwd(pwdbuf, 512));
    chdir("../");
    closedir(p);
    return size;
}

int main(int c, char **v)
{
    printf("size = %d\n", get_size(v[1]));
    return 0;
}

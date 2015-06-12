#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/stat.h>

int main(void)
{
    struct dirent *d;
    DIR *dp;

    dp = opendir(".");
    for (d = readdir(dp); NULL != d; d = readdir(dp))
        printf("%s\n", d->d_name);
    
    return 0;
}

#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <unistd.h>
#include <string.h>
#include <sys/stat.h>

/*const char *fun(const char *v)
{
    struct dirent *d;
    DIR *dp;
    struct stat buf;
    int ret;
    ret = lstat(v, &buf);
    if (-1 == ret)
    {
        perror("stat");
        exit(1);
    }
    
    if (S_ISDIR(buf.st_mode))
    {    
        dp = opendir(v);
        for (d = dp; d != NULL; d = readdir(dp))
        {
            printf("%s\n", d->d_name);
            if (S_ISDIR(buf.st_mode))
            return v/d->d_name;
            else
            {
                
            }
        }
    }
} */                   
int main(int c, char **v)
{
    if (c != 2)
    {
        fprintf(stderr, "usage...\n");
        exit(1);
    }

    struct dirent *d;
    DIR *dp;
   // struct stat buf;
  //  int ret;
    //ret = lstat(v[1], &buf);
  //  if (-1 == ret)
  //  {
  //      perror("stst");
  //      exit(1);
  //  }
    
   // if (S_ISDIR(buf.st_mode))
  //  {    
        dp = opendir(v[1]);
        for (d = dp; d != NULL; d = readdir(dp))
       // {
            printf("%s\n", d->d_name);
            //if 
    //}
  
  return 0;
    
}

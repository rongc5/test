#include <stdio.h>
#include <stdlib.h>
#include <sys/dir.h>
#include <sys/stat.h>
#include <string.h>



int IS_DIR(const char* path)
{
         struct stat st;
         lstat(path, &st);
         return S_ISDIR(st.st_mode);
}


void fun1(const char *path)
{
         DIR *dp;
         struct dirent *d;
         char temp[256];
         dp = opendir(path);
         if(dp)
         {
                 while(d = readdir(dp))
                 {
                           
                           if(strcmp(d->d_name, ".") == 0
                                     || strcmp(d->d_name, "..") == 0)
                                   continue;

                           sprintf(temp, "%s/%s", path, d->d_name);

                           printf("%s\n", temp);
                           
                           if(IS_DIR(temp))
                           {
                                   fun1(temp);
                           }
                 }
          }
         else
         {
                  printf("opendir error:%s\n", path);
          }
          closedir(dp);

}

void fun2(const char *path)
{
          int len;
          char temp[256];
          long long size = 0;
          struct stat buf;

        
          len = strlen(path);
          strcpy(temp, path);
          if(temp[len - 1] == '/') temp[len -1] = '\0';
          
 
          if(IS_DIR(temp))
          {
                  
                   fun1(temp);
          }
         else   
         {
                  printf("%-5s", path);
                  //size = size + buf.st_size;
                  lstat(path, &buf);
                  printf("%-5lld\n", (long long)buf.st_size);
         }
}  

/*long long fun(const char*v)
{
     
} */

int main(int c, char** v)
{
          if(c != 2)
         {
                     printf("Usage: ...\n");
                     exit(1);
         }

         fun2(v[1]);
   
         return 0;
}

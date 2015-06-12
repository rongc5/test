#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>

int main(int c, char **v)
{
    struct stat std;
    int ret;
    
    if (c != 2)
    {
        fprintf(stderr, "usage...\n");
        exit(1);
    }

    ret = stat(v[1], &std);
    if (-1 == ret)
    {
        perror("stat");
        exit(1);
    }

   char* uid = std.st_uid;
   char* gid = std.st_gid;
   
   FILE *fp;
   char *reptr;
   char buf[512];
   char *tmp;

   fp = fopen("passwd", "r");
   if (NULL == fp)
   {
       perror("fopen");
       exit(1)
   }

   while (1)
   {
       reptr = fgets(buf, 512, fp);
       if (NULL == reptr)
       {
           if (feof(fp))
           {
               break;
           }
           else
           {
               perror("fgets");
               exit(!);
           }
           
       }
       
       tmp = buf;


   }

    
}

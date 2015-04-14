#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main(void)
{
    //char buf[120] = "ab abc abcd abcde abcdefgh"; 
    char buf[512] = "http://www.google.com/search?sclient=psy-ab&newwindow=1&site=&source=hp&q=vlc+%E6%BA%90%E7%A0%81%E5%88%86%E6%9E%90&btnK=Google+Search"; 
    char *src = buf;
    char *dst = "://";
    char *ptmp = NULL;

    int len = strlen(src);
    int i = 0;

    printf("%s\n", src);
    ptmp = strstr(src, dst);

    printf("%s\n", ptmp);

#if 0
    for (i = 0; i < len; i++){
        printf("%c", *(src + i));
    }
#endif

    printf("\n");

    return 0;
}

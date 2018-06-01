#include <stdio.h>  
#include <time.h> 
#include <errno.h>

int main(int argc, const char * argv[])  
{  
    struct tm tmp_time;  
    memset(&tmp_time, 0, sizeof(tmp_time));
    strptime("20180601 18:20","%Y%m%d %H:%M",&tmp_time); 
    /*strptime("18:20","%H:%M",&tmp_time);  */

    printf("%d %d %d %d %d %d\n", tmp_time.tm_year + 1900, tmp_time.tm_mon, tmp_time.tm_mday, 
            tmp_time.tm_hour, tmp_time.tm_min, tmp_time.tm_sec);

    time_t t = mktime(&tmp_time);  
    printf("%ld\n",t);  
    
    printf("%ld\n", time(NULL));

    return 0;  
} 

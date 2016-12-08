#include "utils.h"
#include <time.h>

namespace MZFRAME {


     int utils::get_date_str(char dest[], size_t dest_len, const char * format)
     {
        if (!dest || !dest_len || !format){
            return -1;
        }   

        time_t now;
        struct tm *tm_now;

        time(&now);
        tm_now = localtime(&now);
        strftime(dest, dest_len, format, tm_now);

        return 0;
     }


		uint64_t utils::GetTickCount()
		{
		    timeval tm; 
		    gettimeofday(&tm, NULL);
		    uint64_t now = tm.tv_sec * 1000 + tm.tv_usec/1000;
		    return now;
		}



}

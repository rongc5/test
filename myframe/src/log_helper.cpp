#include "log_helper.h"

namespace MZFRAME {


    log_helper::log_helper()
    {
        _file_name[0] = '\0';
        _max_size = 0;
        _max_record = 0;
    }

    void log_helper::log_write(const char *format,  va_list &ap)
    {
       if (_file_name[0] == '\0'){
            return;
       } 

       thread_lock lock(&_mutex);
       FILE * fp = fopen(_file_name, "a+");
       ASSERT(fp != NULL, printf("file_name: %s\n", _file_name));
        
       


    }




}

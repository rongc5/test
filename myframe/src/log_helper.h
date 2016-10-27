#ifndef __LOG_HELPER_H_
#define __LOG_HELPER_H_

#include "thread_helper.h"
#include <stdio.h>
#include "frm_def.h"


namespace MZFRAME {



class log_helper {
    public:
        log_helper();
        ~log_helper();
        void log_write(const char *format,  va_list &ap);

        bool 
    
    private:
        thread_mutex_t _mutex;
        char _file_name[SIZE_LEN_64];
        off_t _max_size;
        size_t _max_record;
};



}

#endif


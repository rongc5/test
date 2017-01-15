#ifndef __PASSING_DATA_PROCESS_H__
#define __PASSING_DATA_PROCESS_H__

#include "passing_msg_process.h"
#include "base_def.h"
#include "common_def.h"


namespace MZFRAME {

    class passing_data_process
    {
        public:
            passing_data_process(void *p);

            virtual ~passing_data_process(){}

            virtual size_t process_recv_buf(char *buf, size_t len);

            static passing_data_process* gen_process(void *p);

        protected:	
            passing_msg_process<passing_data_process> *_p_msg_process;
    };


}
#endif


#include "passing_data_process.h"

namespace MZFRAME {

    passing_data_process::passing_data_process(void *p)
    {
        _p_msg_process = (passing_msg_process<passing_data_process>*)p;			
    }


    size_t passing_data_process::process_recv_buf(char *buf, size_t len)
    {
        if (!buf || !len) {
            return -1;
        }

        string * tmp_str = new string(buf, len);
        _p_msg_process->put_msg(tmp_str);            
    }

    passing_data_process* passing_data_process::gen_process(void *p) 
    {   
        passing_data_process *p_tmp = new passing_data_process(p);
        return p_tmp;
    }   

}


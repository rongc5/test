#ifndef __SKHTTP_REQ_DATA_PROCESS_H__
#define __SKHTTP_REQ_DATA_PROCESS_H__

#include "base_def.h"
#include "http_base_process.h"
#include "http_base_data_process.h"

class skhttp_req_data_process: public http_base_data_process
{
    public:
        skhttp_req_data_process(http_base_process * _p_process);
        virtual ~skhttp_req_data_process(){}
    protected:

};




#endif

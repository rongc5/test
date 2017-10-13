#ifndef __KF_DATA_PROCESS_H__
#define __KF_DATA_PROCESS_H__

#include "base_def.h"
#include "http_base_data_process.h"
#include "http_base_process.h"

//class http_base_process;
//class http_base_data_process;
class kf_data_process:public http_base_data_process
{
    public:

        kf_data_process(http_base_process * _p_process);

        virtual ~kf_data_process(){}

        virtual string *get_send_body(int &result);

        virtual void header_recv_finish();

        virtual void msg_recv_finish();

        virtual string * get_send_head();

        virtual size_t process_recv_body(const char *buf, size_t len, int& result);

    protected:
        string _body;

};









#endif

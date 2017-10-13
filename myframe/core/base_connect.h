#ifndef __BASE_CONNECT_H__
#define __BASE_CONNECT_H__

#include "common_def.h"
#include "base_net_obj.h"

class base_data_process; 
class base_connect:public base_net_obj
{
    public:

        base_connect(const int32_t sock);

        virtual ~base_connect();


        virtual void event_process(int event);
        virtual int real_net_process();

        void set_process(base_data_process *p);

        void notice_send();

        virtual bool process_recv_msg(ObjId & id, normal_msg * p_msg);

        base_data_process * get_process();

    protected:
        virtual int RECV(void *buf, size_t len);

        virtual ssize_t SEND(const void *buf, const size_t len);

        void real_recv();

        void real_send();

    protected:
        string _recv_buf;
        string* _p_send_buf;	

        base_data_process *_process;
};


#endif

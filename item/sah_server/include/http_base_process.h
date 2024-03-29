#ifndef __HTTP_BASE_PROCESS_H__
#define __HTTP_BASE_PROCESS_H__

#include "base_def.h"
#include "base_net_obj.h"
#include "base_singleton.h"
#include "base_data_process.h"
#include "common_exception.h"
#include "common_def.h"
#include "http_base_msg.h"

class http_base_data_process;
class http_base_process: public base_data_process
{
    public:
        http_base_process(base_net_obj *p);

        virtual ~http_base_process();

        void set_process(http_base_data_process * data_process);

        virtual size_t process_recv_buf(const char *buf, size_t len);

        virtual string* get_send_buf();

        virtual bool process_recv_msg(ObjId & id, normal_msg * p_msg);
		
		/****************************以上是五个口子，以下是供底层调用********************************************/

        http_req_head_para &get_req_head_para();

        http_res_head_para &get_res_head_para();

		void change_http_status(HTTP_STATUS status, bool if_change_send = true);

		http_base_data_process *get_process();

        virtual void gen_send_head(string * head) = 0;

        virtual void reset();

    protected:		
		virtual size_t process_recv_body(const char *buf, size_t len, int &result) = 0;	
		
        virtual void parse_header(string & recv_head) = 0;

        virtual void parse_first_line(const string & line) = 0;

        virtual void recv_finish() = 0;
        virtual void send_finish() = 0;

        void check_head_finish(string & recv_head, string &left_str);

        HTTP_STATUS _http_status;
        http_base_data_process *_data_process;
        
        http_req_head_para _req_head_para;
        http_res_head_para _res_head_para;

};


#endif


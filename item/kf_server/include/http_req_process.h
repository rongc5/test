#ifndef __HTTP_REQ_PROCESS_H__
#define __HTTP_REQ_PROCESS_H__

#include "base_def.h"
#include "base_connect.h"
#include "base_singleton.h"
#include "http_base_process.h"
#include "common_exception.h"
#include "common_def.h"
#include "log_helper.h"
#include "http_base_msg.h"


class http_req_process:public http_base_process
{
    public:
        http_req_process(base_connect *p);

        virtual ~http_req_process();
		
		string* get_send_buf();

	  	void reset();

		void set_send_arg(http_send_arg *arg);

		http_send_arg* get_send_arg();

		void peer_close();

		string gen_req_head();

    protected:
        size_t process_recv_body(char *buf, size_t len, int &result);

        size_t get_chuncked(char *buf, size_t len, int &result);

        void parse_header();

        virtual void gen_send_head();

        void recv_finish();

        void send_finish();

    protected:
        http_req_head_para _req_head_para;
        http_res_head_para _res_head_para;

		int64_t _cur_chunked_len;
		int64_t _cur_chunked_rec_len;
		string _chunked_body;
		size_t _recv_body_length;
		http_send_arg* _p_send_arg;

		HTTP_RECV_TYPE _recv_type;
};


#endif


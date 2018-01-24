#ifndef __HTTP_REQ_PROCESS_H__
#define __HTTP_REQ_PROCESS_H__

#include "base_def.h"
#include "base_connect.h"
#include "base_singleton.h"
#include "http_base_process.h"
#include "common_exception.h"
#include "common_def.h"


class http_req_process:public http_base_process
{
    public:
        http_req_process(base_connect *p);

        virtual ~http_req_process();

	  	virtual void reset();

		void peer_close();

    protected:
        size_t process_recv_body(char *buf, size_t len, int &result);

        size_t get_chuncked(char *buf, size_t len, int &result);

        void recv_finish();

        void send_finish();
        
        virtual void parse_first_line(const string & line);

        virtual void parse_header(string & recv_head);

    protected:

		int64_t _cur_chunked_len;
		int64_t _cur_chunked_rec_len;
		string _chunked_body;
		size_t _recv_body_length;

		HTTP_RECV_TYPE _recv_type;
};


#endif


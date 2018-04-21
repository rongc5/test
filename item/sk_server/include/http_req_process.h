#ifndef __HTTP_REQ_PROCESS_H__
#define __HTTP_REQ_PROCESS_H__

#include "base_def.h"
#include "base_net_obj.h"
#include "base_singleton.h"
#include "http_base_process.h"
#include "common_exception.h"
#include "common_def.h"


class http_req_process:public http_base_process
{
    public:
        http_req_process(std::shared_ptr<base_net_obj> p);

        virtual ~http_req_process();

	  	virtual void reset();

		void peer_close();

    protected:
        virtual size_t process_recv_body(const char *buf, size_t len, int &result);

        size_t get_chuncked(const char *buf, size_t len, int &result);

        void recv_finish();

        void send_finish();
        
        virtual void parse_first_line(const std::string & line);

        virtual void parse_header(std::string & recv_head);

    protected:

		int64_t _cur_chunked_len;
		int64_t _cur_chunked_rec_len;
        std::string _chunked_body;
		size_t _recv_body_length;

		HTTP_RECV_TYPE _recv_type;
};


#endif


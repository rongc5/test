#ifndef __HTTP_RES_PROCESS_H__
#define __HTTP_RES_PROCESS_H__

#include "base_def.h"
#include "base_connect.h"
#include "base_singleton.h"
#include "base_data_process.h"
#include "common_exception.h"
#include "common_def.h"
#include "http_base_msg.h"
#include "http_base_process.h"

class http_res_process:public http_base_process
{
    public:
        http_res_process(base_connect *p);

        virtual ~http_res_process();

		virtual void reset();
		
		virtual void gen_send_head(string * head);

        static void parse_url_para(const string &url_para, map<string, string> &url_para_map);

    protected:
		virtual size_t process_recv_body(const char *buf, size_t len, int &result);
        
        virtual void parse_first_line(const string & line);

        virtual void parse_header(string & recv_head);

        virtual void recv_finish();

        virtual void send_finish();

		size_t get_boundary(const char *buf, size_t len, int &result);
      
    protected:
		enum BOUNDARY_STATUS
		{
			BOUNDARY_RECV_HEAD = 0,
			BOUNDARY_RECV_BODY = 1,
			BOUNDARY_RECV_TAIL = 2
		};
		string _recv_boundary_head;
		boundary_para _boundary_para;		
		BOUNDARY_STATUS _recv_boundary_status;
		uint32_t _recv_body_length;
		
        response_code _res_code;
};


#endif


#ifndef __HTTP_RES_DATA_PROCESS_H__
#define __HTTP_RES_DATA_PROCESS_H__

#include "http_data_process.h"
#include "tcp_connect.h"


class http_res_process:public http_data_process
{
    public:
        http_res_process(tcp_connect * t_cn);

        virtual ~http_res_process();

        http_head_para &get_req_head_para();


        http_res_head_para &get_res_head_para();


		void set_res_head_para(const http_res_head_para &para);
		

		void reset();

		
		string gen_res_head();
	
    protected:
		virtual size_t process_recv_body(char *buf, size_t len, int &result);

		
        void parse_header();
      	
        void gen_send_head();
   

        void recv_finish();


        void send_finish();


		 size_t get_boundary(char *buf, size_t len, int &result);

      
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
		
        http_head_para _req_head_para;
        http_res_head_para _res_head_para;
};



#endif

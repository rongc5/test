#ifndef __HTTP_REQ_DATA_PROCESS_H__
#define __HTTP_REQ_DATA_PROCESS_H__

#include "common_def.h"
#include "http_data_process.h"
#include "tcp_connect.h"


class http_req_data_process:public http_data_process
{
    public:
        http_req_process(tcp_connect * t_cn);


        virtual ~http_req_process();



        virtual string* get_send_buf();


        http_head_para & get_req_head_para();


        http_res_head_para& get_res_head_para();

        void set_req_head_para(const http_head_para &para);


        void reset();


        void set_send_arg(http_send_arg *arg);


        http_send_arg* get_send_arg();


        virtual void eer_close();


        string gen_req_head();


    protected:
        size_t process_recv_body(char *buf, size_t len, int &result);


        size_t get_chuncked(char *buf, size_t len, int &result);



        void parse_header();


        virtual void gen_send_head();


        void recv_finish();

        void send_finish();


    protected:
        http_head_para _req_head_para;
        http_res_head_para _res_head_para;

        int64_t _cur_chunked_len;
        int64_t _cur_chunked_rec_len;
        string _chunked_body;
        size_t _recv_body_length;
        http_send_arg* _p_send_arg;

        HTTP_RECV_TYPE _recv_type;

};




#endif

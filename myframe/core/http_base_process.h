#ifndef __HTTP_BASE_PROCESS_H__
#define __HTTP_BASE_PROCESS_H__

#include "base_def.h"
#include "base_connect.h"
#include "base_singleton.h"
#include "base_data_process.h"
#include "common_exception.h"
#include "common_def.h"
#include "log_helper.h"
#include "http_base_msg.h"


class http_base_process: public base_data_process
{
    public:
        http_base_process(base_connect *p);

        virtual ~http_base_process();

        void set_process(base_data_process * data_process);

        virtual size_t process_recv_buf(char *buf, const size_t len);

        virtual string* get_send_buf();

        virtual bool process_recv_msg(ObjId & id, normal_msg * p_msg);
        
        virtual void reset();
		
		/****************************以上是五个口子，以下是供底层调用********************************************/
        string &get_head();

		void change_http_status(HTTP_STATUS status, bool if_change_send = true);
		
		void change_to_cansend();

		base_data_process *get_process()
		{
			return _data_process;
		}

		static void parse_url_para(const string &url_para, map<string, string> &url_para_map);

    protected:		
		virtual size_t process_recv_body(char *buf, size_t len, int &result) = 0;	
		
        virtual void parse_header() = 0;
        virtual void gen_send_head() = 0;
        virtual void recv_finish() = 0;
        virtual void send_finish() = 0;
        virtual void parse_first_line(const string & line);

        void check_head_finish(string &left_str);

        HTTP_STATUS _http_status;
        string _recv_head;
        string _send_head;
        size_t _send_head_len;
        base_data_process *_data_process;
		int _send_status;	
};


//每一个http_data_process 需要实现下列7个函数即可
/*
class http_base_data_process
{
	public:
		//result:0 正常返回  1: 结束获取发送包， 一般情况下上层自动会判断
		//return NULL 底层就删除写事件, 正常返回将要发送的包体
		string *get_send_body(int &result);
		
		//头部接收完处理
		void header_recv_finish(); 
		

		//所有的数据接收完处理
		void recv_finish();
		

		//生成发送头
       	string gen_send_head();      	
       	

		//处理接收的数据
		size_t process_recv_body(const char *buf, size_t len, int& result);

		//处理外部消息 一般这个没啥用了
		void process_s(normal_obj_msg *p_msg);


		//对象生成器
		static http_base_data_process *gen_process(http_res_process<http_base_data_process> *p);		
};
*/


#endif


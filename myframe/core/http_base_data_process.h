#ifndef __HTTP_BASE_DATA_PROCESS_H__
#define __HTTP_BASE_DATA_PROCESS_H__


#include "base_def.h"
#include "base_data_process.h"

class http_base_data_process: public base_data_process
{
	public:
		//result:0 正常返回  1: 结束获取发送包， 一般情况下上层自动会判断
		//return NULL 底层就删除写事件, 正常返回将要发送的包体
        virtual string *get_send_body(int &result);

        //头部接收完处理
        void header_recv_finish(); 


        //所有的数据接收完处理
        virtual void msg_recv_finish();


        //生成发送头
        virtual string gen_send_head();      	


        //处理接收的数据
        virtual size_t process_recv_body(const char *buf, size_t len, int& result);	
};




#endif

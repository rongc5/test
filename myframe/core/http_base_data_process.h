#ifndef __HTTP_BASE_DATA_PROCESS_H__
#define __HTTP_BASE_DATA_PROCESS_H__


#include "base_def.h"
#include "base_data_process.h"


class http_base_process;
class http_base_data_process: public base_data_process
{
	public:

        http_base_data_process(http_base_process * _p_process);

		//result:0 正常返回  1: 结束获取发送包， 一般情况下上层自动会判断
		//return NULL 底层就删除写事件, 正常返回将要发送的包体
        virtual string *get_send_body(int &result);

        //头部接收完处理
        virtual void header_recv_finish(); 


        //所有的数据接收完处理
        virtual void msg_recv_finish();


        //生成发送头
        virtual string * get_send_head();      	


        //处理接收的数据
        virtual size_t process_recv_body(const char *buf, size_t len, int& result);	

    protected:
        http_base_process * _base_process;
};




#endif

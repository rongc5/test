#include "http_base_data_process.h"
#include "base_connect.h"
#include "log_helper.h"
#include "http_base_process.h"

//result:0 正常返回  1: 结束获取发送包， 一般情况下上层自动会判断
//return NULL 底层就删除写事件, 正常返回将要发送的包体
//
http_base_data_process::http_base_data_process(http_base_process * _p_process):
    base_data_process(_p_process->get_base_connect())
{
    LOG_DEBUG("%p", this);
    _base_process = _p_process;
}


string *http_base_data_process::get_send_body(int &result)
{
    LOG_DEBUG("%p", this);
}

//头部接收完处理
void http_base_data_process::header_recv_finish()
{
    LOG_DEBUG("%p", this);
}


//所有的数据接收完处理
void http_base_data_process::msg_recv_finish()
{
    LOG_DEBUG("%p", this);
}


//生成发送头
string http_base_data_process::gen_send_head()
{
    LOG_DEBUG("%p", this);
}


//处理接收的数据
size_t http_base_data_process::process_recv_body(const char *buf, size_t len, int& result)
{
    LOG_DEBUG("%p", this);
}




#include "web_socket_process.h"
#include "web_socket_msg.h"
#include "web_socket_req_process.h"
#include "common_util.h"
#include "log_helper.h"
#include "common_exception.h"
#include "web_socket_data_process.h"
#include "base_net_obj.h"


web_socket_req_process::web_socket_req_process(base_net_obj *p):web_socket_process(p)
{			
}

web_socket_req_process::~web_socket_req_process()
{
}

ws_req_head_para &web_socket_req_process::get_req_para()
{
    return _req_para;
}		

void web_socket_req_process::ping_process(const int8_t op_code)//客户端不用处理服务器的ping
{
    LOG_DEBUG("recv server ping res");
}

string* web_socket_req_process::SEND_WB_HEAD_FINISH_PROCESS()
{
    return NULL;
}

string* web_socket_req_process::SEND_WB_INIT_STAUTS_PROCESS()
{
    string *p_str = new string();
    *p_str = gen_send_http_head();
    _wb_status = WB_HEAD_FINISH;
    _send_header = *p_str;
    return p_str;
}

size_t web_socket_req_process::RECV_WB_HEAD_FINISH_PROCESS(const char *buf, const size_t len)
{
    _recv_header.append(buf, len);
    check_head_finish();
    return len; 
}

size_t web_socket_req_process::RECV_WB_INIT_STAUTS_PROCESS(const char *buf, const size_t len)
{
    THROW_COMMON_EXCEPT("web_socket_process_res can't recv data in WB_INIT_STAUTS");
}

/*
   GET /chat HTTP/1.1
Host: server.example.com
Upgrade: websocket	
Connection: Upgrade
Sec-WebSocket-Key: dGhlIHNhbXBsZSBub25jZQ==
Origin: http://example.com
Sec-WebSocket-Protocol: chat, superchat
Sec-WebSocket-Version: 13
*/      
string web_socket_req_process::gen_send_http_head()
{
    stringstream ss;
    ss << "GET "<< _req_para._s_path <<" HTTP/1.1\r\n"
        << "Host:" << _req_para._s_host << "\r\n"
        << "Upgrade: websocket\r\n"
        << "Connection: Upgrade\r\n"
        << "Sec-WebSocket-Key:" << _req_para._s_websocket_key  <<"\r\n"
        << "Origin:" << _req_para._origin << "\r\n"
        << "Sec-WebSocket-Protocol: chat, superchat\r\n"
        << "Sec-WebSocket-Version: " << _req_para._version << "\r\n"
        << "\r\n";
    return ss.str();
}

void  web_socket_req_process::parse_header()
{
    string ret_code;
    GetCaseStringByLabel(_recv_header, "HTTP/1.1 ", " ", ret_code);
    if (ret_code != "101")
    {
        THROW_COMMON_EXCEPT("ret_code "<< ret_code << " is not right ");
    }

    GetCaseStringByLabel(_recv_header, "Sec-WebSocket-Accept:", "\r\n", _s_accept_key);			
    StringTrim(_s_accept_key);
    //校验下
    string tmp = get_web_accept_key(_req_para._s_websocket_key);			
    if (tmp != _s_accept_key)
    {
        THROW_COMMON_EXCEPT("parse_header recv _s_accept_key "<< _s_accept_key << " is not right , it should be " 
                << tmp);
    }
}


bool web_socket_req_process::check_head_finish()
{
    bool ret = web_socket_process::check_head_finish();
    if (ret)
    {
        //设置可以发送数据			    
        _wb_status = WB_HANDSHAKE_OK;
        _p_data_process->on_handshake_ok();
    }
    return ret;
}





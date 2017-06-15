#include "ws_req_connect.h"
#include "base_net_thread.h"
#include "common_exception.h"
#include "log_helper.h"

void ws_req_connect::set_req_para(ws_req_head_para & req_para)
{
    _req_para = req_para;
}

ws_req_head_para &ws_req_connect::get_req_para()
{
    return _req_para;
}		

string* ws_req_connect::SEND_WB_HEAD_FINISH_PROCESS()
{
    return NULL;
}

string* ws_req_connect::SEND_WB_INIT_STAUTS_PROCESS()
{
    string *p_str = new string();
    *p_str = gen_send_http_head();
    _wb_status = WB_HEAD_FINISH;
    _send_header = *p_str;
    return p_str;
}

size_t ws_req_connect::RECV_WB_HEAD_FINISH_PROCESS(const char *buf, const size_t len)
{
    _recv_header.append(buf, len);
    check_head_finish();
    return len; 
}

size_t ws_req_connect::RECV_WB_INIT_STAUTS_PROCESS(const char *buf, const size_t len)
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
string ws_req_connect::gen_send_http_head()
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

void  ws_req_connect::parse_header()
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


bool ws_req_connect::check_head_finish()
{
    bool ret = ws_connect::check_head_finish();
    if (ret)
    {
        //设置可以发送数据			    
        _wb_status = WB_HANDSHAKE_OK;
        on_handshake_ok();
    }
    return ret;
}



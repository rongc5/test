#include "web_socket_res_process.h"
#include "base_connect.h"
#include "log_helper.h"
#include "common_exception.h"
#include "common_timer_process.h"
#include "common_util.h"
#include "base_timer.h"
#include "base_net_container.h"
#include "web_socket_data_process.h"

web_socket_res_process::web_socket_res_process(base_connect *p):web_socket_process(p)
{
    _wb_version = 0;		
    _if_send_mask = false;
    _if_upgrade = false;
}

web_socket_res_process::~web_socket_res_process()
{			
}


void web_socket_res_process::on_connect_comming()
{
    base_timer * timer = get_base_connect()->get_net_container()->get_timer();
    common_timer_process * timer_process = new 
        common_timer_process(get_base_connect()->get_net_container(), get_base_connect()->get_id());
    timer_process->init(WEB_SOCKET_HANDSHAKE_OK_TIMER_TYPE, WEB_SOCKET_HANDSHAKE_OK_TIMER_LENGTH);
    timer->add_timer(timer_process);
}

void web_socket_res_process::handle_timeout(const uint32_t timer_type)
{
    if (timer_type == WEB_SOCKET_HANDSHAKE_OK_TIMER_TYPE)
    {
        if (_wb_status != WB_HANDSHAKE_OK)
        {
            THROW_COMMON_EXCEPT("the web socket handshake time out, delete it");
        }
        else
        {
            LOG_DEBUG("web socket handshake timer arrive, status ok");
        }
    }
    else
    {
        web_socket_process::handle_timeout(timer_type);
    }
}

string* web_socket_res_process::SEND_WB_HEAD_FINISH_PROCESS()
{
    string *p_str = new string;
    *p_str = gen_send_http_head();
    _send_header = *p_str;
    if (!_if_upgrade)
    {
        _wb_status  = WB_HANDSHAKE_OK;
        _p_data_process->on_handshake_ok();
    }
    else
    {
        _wb_status  = WB_HANDSHAKE_FAIL;

        base_timer * timer = get_base_connect()->get_net_container()->get_timer();
        common_timer_process * timer_process = new 
            common_timer_process(get_base_connect()->get_net_container(), get_base_connect()->get_id());
        timer_process->init(DELAY_CLOSE_TIMER_TYPE, 3000);
        timer->add_timer(timer_process);//延迟3秒钟关闭
    }
    return p_str;
}

string* web_socket_res_process::SEND_WB_INIT_STAUTS_PROCESS()
{
    THROW_COMMON_EXCEPT("web_socket_process_res can't send data in WB_INIT_STATUS)");
}

size_t web_socket_res_process::RECV_WB_HEAD_FINISH_PROCESS(const char *buf, const size_t len)
{
    THROW_COMMON_EXCEPT("web_socket_process_res can't recv data in WB_HEAD_FINISH)");
}

size_t web_socket_res_process::RECV_WB_INIT_STAUTS_PROCESS(const char *buf, const size_t len)
{
    _recv_header.append(buf, len);
    check_head_finish();
    return len;
}	

/*		
        HTTP/1.1 101 Switching Protocols
Upgrade: websocket
Connection: Upgrade
Sec-WebSocket-Accept: s3pPLMBiTxaQ9kYGzzhZRbK+xOo=
Sec-WebSocket-Protocol: chat
*/
string web_socket_res_process::gen_send_http_head()
{		
    stringstream ss;
    if (!_if_upgrade)
    {
        _s_accept_key = get_web_accept_key(_s_websocket_key);	    	
        ss << "HTTP/1.1 101 Switching Protocols\r\n"
            << "Upgrade: websocket\r\n"
            << "Connection: Upgrade\r\n"
            << "Sec-WebSocket-Accept: " << _s_accept_key <<"\r\n";
        if (_s_ws_protocol != "")
            ss << "Sec-WebSocket-Protocol: chat\r\n";				
        ss << "\r\n";
    }
    else //
    {
        ss << "HTTP/1.1 426 Upgrade Required\r\n"
            << "Sec-WebSocket-Version: 13\r\n"
            << "\r\n";
    }
    return ss.str();
}

void  web_socket_res_process::parse_header()
{    		
    GetCaseStringByLabel(_recv_header, "Sec-WebSocket-Key:", "\r\n", _s_websocket_key);
    StringTrim(_s_websocket_key);
    GetCaseStringByLabel(_recv_header, "Sec-WebSocket-Protocol:", "\r\n", _s_ws_protocol);
    string tmp;
    GetCaseStringByLabel(_recv_header, "Sec-WebSocket-Version:", "\r\n", tmp);
    _wb_version = strtoull(tmp.c_str(), 0, 10);
    if (_wb_version != 13)
    {
        _if_upgrade = true;
    }
}

bool web_socket_res_process::check_head_finish()
{
    bool ret = web_socket_process::check_head_finish();
    if (ret)
    {			    
        _wb_status = WB_HEAD_FINISH;
        //设置可以发送数据
        _p_connect->notice_send();
    }
    return ret;
}



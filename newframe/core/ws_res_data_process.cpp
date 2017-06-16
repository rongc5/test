#include "ws_res_data_process.h"
#include "common_exception.h"
#include "tcp_connect.h"

ws_res_data_process::ws_res_data_process(tcp_connect * t_cn):ws_data_process(t_cn)
{
    _wb_version = 0;
    _if_send_mask = false;
    _if_upgrade = false;
}

void ws_res_data_process::handle_timeout(const uint32_t timer_type)
{
    //if (timer_type == WEB_SOCKET_HANDSHAKE_OK_TIMER_TYPE)
    //{
    //if (_wb_status != WB_HANDSHAKE_OK)
    //{
    //THROW_COMMON_EXCEPT(-1, "the web socket handshake time out, delete it");
    //}
    //else
    //{
    //WRITE_TRACE("web socket handshake timer arrive, status ok");
    //}
    //}
    //else
    //{
    //web_socket_process::handle_timeout(timer_type);
    //}
}

string* ws_res_data_process::SEND_WB_HEAD_FINISH_PROCESS()
{
    string *p_str = new string(gen_send_http_head());
    _send_header = *p_str;
    if (!_if_upgrade)
    {
        _wb_status  = WB_HANDSHAKE_OK;
        on_handshake_ok();
    }
    else
    {
        _wb_status  = WB_HANDSHAKE_FAIL;
        //_p_connect->delay_close(3000);//延迟3秒钟关闭
    }
    return p_str;
}

string* ws_res_data_process::SEND_WB_INIT_STAUTS_PROCESS()
{
    THROW_COMMON_EXCEPT("web_socket_process_res can't send data in WB_INIT_STATUS)");
}

size_t ws_res_data_process::RECV_WB_HEAD_FINISH_PROCESS(const char *buf, const size_t len)
{
    THROW_COMMON_EXCEPT("web_socket_process_res can't recv data in WB_HEAD_FINISH)");	
}

size_t ws_res_data_process::RECV_WB_INIT_STAUTS_PROCESS(const char *buf, const size_t len)
{
    _recv_header.append(buf, len);
    LOG_DEBUG("buf:%s _recv_header:%s", buf, _recv_header.c_str());
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
string ws_res_data_process::gen_send_http_head()
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

void  ws_res_data_process::parse_header()
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

bool ws_res_data_process::check_head_finish()
{
    bool ret = ws_data_process::check_head_finish();
    if (ret)
    {			    
        _wb_status = WB_HEAD_FINISH;
        //设置可以发送数据
        //_p_connect->add_event(EPOLLOUT);
        notice_send();
    }
    return ret;
}


void ws_res_data_process::on_ping(const char op_code,const string &ping_data)
{
    if (op_code == 0x09)
        send_ping(0x0a, "");
}

 void ws_res_data_process::on_handshake_ok()
{
    int ikeepAlive = 1; 
    int ikeepIdle = 2*60*60; 
    int ikeepInterval = 20; 
    int ikeepCount = 3; 

    setsockopt(_cnn->get_sock(), SOL_SOCKET, SO_KEEPALIVE, ( void * )&ikeepAlive, sizeof( ikeepAlive ) );
    setsockopt(_cnn->get_sock(), SOL_TCP, TCP_KEEPIDLE, ( void* )&ikeepIdle, sizeof( ikeepIdle ) );
    setsockopt(_cnn->get_sock(), SOL_TCP, TCP_KEEPINTVL, ( void * )&ikeepInterval, sizeof( ikeepInterval ) );
    setsockopt(_cnn->get_sock(), SOL_TCP, TCP_KEEPCNT, ( void * )&ikeepCount, sizeof( ikeepCount ) );

    notice_send();
}



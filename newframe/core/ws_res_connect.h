#ifndef __WS_RES_CONNECT_H__
#define __WS_RES_CONNECT_H__

#include "ws_connect.h"


template<class PROCESS_GENERATOR>
class ws_res_connect:public ws_connect
{
    public:		
        ws_res_connect(int32_t sock, base_net_thread * thread):ws_connect(sock, thread)
    {
        _wb_version = 0;		
        _if_send_mask = false;
        _if_upgrade = false;
        _p_data_process = PROCESS_GENERATOR::gen_ws_data_process(this);		
    }

        virtual ~web_socket_process_res()
        {			
        }


        void on_connect_comming()
        {
            //_p_connect->set_timer(WEB_SOCKET_HANDSHAKE_OK_TIMER_TYPE, 
                    //WEB_SOCKET_HANDSHAKE_OK_TIMER_LENGTH);
        }

        virtual void handle_timeout(const uint32_t timer_type)
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
    protected:				
        virtual string* SEND_WB_HEAD_FINISH_PROCESS()
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
                _p_connect->delay_close(3000);//延迟3秒钟关闭
            }
            return p_str;
        }

        virtual string* SEND_WB_INIT_STAUTS_PROCESS()
        {
            THROW_COMMON_EXCEPT(-1, "web_socket_process_res can't send data in WB_INIT_STATUS)");
        }

        virtual size_t RECV_WB_HEAD_FINISH_PROCESS(const char *buf, const size_t len)
        {
            THROW_COMMON_EXCEPT(-1, "web_socket_process_res can't recv data in WB_HEAD_FINISH)");	
        }

        virtual size_t RECV_WB_INIT_STAUTS_PROCESS(const char *buf, const size_t len)
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
        string gen_send_http_head()
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

        void  parse_header()
        {    		
            CToolKit::GetCaseStringByLabel(_recv_header, "Sec-WebSocket-Key:", "\r\n", _s_websocket_key);
            CToolKit::StringTrim(_s_websocket_key);
            CToolKit::GetCaseStringByLabel(_recv_header, "Sec-WebSocket-Protocol:", "\r\n", _s_ws_protocol);
            string tmp;
            CToolKit::GetCaseStringByLabel(_recv_header, "Sec-WebSocket-Version:", "\r\n", tmp);
            _wb_version = strtoull(tmp.c_str(), 0, 10);
            if (_wb_version != 13)
            {
                _if_upgrade = true;
            }
        }

        bool check_head_finish()
        {
            bool ret = web_socket_process::check_head_finish();
            if (ret)
            {			    
                _wb_status = WB_HEAD_FINISH;
                //设置可以发送数据
                _p_connect->add_event(EPOLLOUT);
            }
            return ret;
        }


    protected:
        string _s_websocket_key;
        string _s_ws_protocol;
        string _s_accept_key;
        uint32_t _wb_version;
        bool _if_upgrade;
};



#endif

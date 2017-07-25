#include "web_socket_data_process.h"
#include "web_socket_process.h"
#include "base_connect.h"


web_socket_process::web_socket_process(base_connect *p):base_data_process(p)
{
    _wb_status = WB_INIT_STAUTS;
    _if_send_mask = true;
    _p_data_process = NULL;
}

web_socket_process::~web_socket_process()
{
    if (_p_data_process != NULL)
        delete _p_data_process;

    for (list<string*>::iterator itr = _p_tmp_str.begin(); itr != _p_tmp_str.end(); ++itr)
    {
        if (*itr != NULL)
            delete *itr;
    }
}


void web_socket_process::set_data_process(web_socket_data_process * data_process)
{
    if (_p_data_process && _p_data_process != data_process) {
        delete _p_data_process;
        _p_data_process = NULL;
    }

    _p_data_process = data_process;
}

//处理接收的数据
size_t web_socket_process::process_recv_buf(const char *buf, const size_t len)
{
    size_t ret = 0;
    if (WB_INIT_STAUTS == _wb_status)
    {
        ret = RECV_WB_INIT_STAUTS_PROCESS(buf, len);
    }
    else if (WB_HANDSHAKE_OK == _wb_status)
    {
        ret = RECV_WB_HANDSHAKE_OK_PROCESS(buf, len);
    }
    else if (WB_HEAD_FINISH == _wb_status)
    {
        ret = RECV_WB_HEAD_FINISH_PROCESS(buf, len);
    }
    else
    {			
        THROW_COMMON_EXCEPT("WB_HANDSHAKE_FAIL status, can't recv and send data");
    }
    return len;
}

bool web_socket_process::process_recv_msg(ObjId & id, normal_msg * p_msg);
{
    if (_p_data_process != NULL) {
        _p_data_process->process_recv_msg(id, p_msg);
        return true;
    } else {
        REC_OBJ<normal_msg> rc(p_msg);
        return true;
    }
}

string* web_socket_process::get_send_buf()
{
    string *p_str = NULL;
    if (WB_HEAD_FINISH == _wb_status) //发送头
    {								
        p_str = SEND_WB_HEAD_FINISH_PROCESS();
    }
    else if (WB_HANDSHAKE_OK == _wb_status) // 发送数据
    {
        p_str = SEND_WB_HANDSHAKE_OK_PROCESS();
    }
    else if (WB_INIT_STAUTS == _wb_status)//不存在的状态
    {
        p_str = SEND_WB_INIT_STAUTS_PROCESS();
    }
    else//WB_HANDSHAKE_FAIL
    {
        THROW_COMMON_EXCEPT("WB_HANDSHAKE_FAIL status, can't recv and send data");
    }
    return p_str;
}

void web_socket_process::set_para()
{	
    _p_connect->set_timeout_len(_p_data_process->get_timeout_len());
}


void web_socket_process::peer_close()
{
    if (_p_data_process != NULL)
        _p_data_process->peer_close();
}

void web_socket_process::send_ping(const char op_code, const string &ping_data)
{
    if (ping_data.length() < 125) //大于125字节的ping包不让发出去了
    {
        WRITE_TRACE("send  ping to server");
        string *p_str = new string;
        *p_str = web_socket_frame_header::gen_ping_header(op_code, ping_data);
        _p_tmp_str.push_back(p_str);
        notice_send();
    }
}
/************************************************************/
string web_socket_process::get_web_accept_key(const string &ws_key)
{
    string accept_key;
    string tmp_key = ws_key + WEB_SOCKET_GUID;			
    unsigned char md[20];
    SHA1((unsigned char*)tmp_key.c_str(), tmp_key.length(), md);
    string aa = string((char*)md, 20);
    accept_key = CBase64::encode(aa);
    return accept_key;
}


web_socket_frame_header &web_socket_process::get_recent_recv_frame_header()
{
    return _recent_recv_web_header;
}

web_socket_frame_header &web_socket_process::get_recent_send_frame_header()
{
    return _recent_send_web_header;
}

void web_socket_process::notice_send()
{
    if (_wb_status == WB_HANDSHAKE_OK)
        _p_connect->add_event(EPOLLOUT);
}

const string &web_socket_process::get_recv_header()
{
    return _recv_header;
}

const string &web_socket_process::get_send_header()
{
    return _send_header;
}		

void web_socket_process::handle_timeout(const uint32_t timer_type)
{
    _p_data_process->handle_timeout(timer_type);
}

bool web_socket_process::check_head_finish()
{
    bool ret = false;
    size_t pos =  _recv_header.find("\r\n\r\n");
    if (pos != string::npos)
    {
        _recv_header.erase(pos + 4);
        parse_header();                     
        ret = true;
    }
    else
    {
        if (_recv_header.length() > MAX_HTTP_HEAD_LEN) //http头不要超过10k
        {
            THROW_COMMON_EXCEPT("http head too long (" << _recv_header.length() << ")");
        }
    }			
    return ret;
}


string *web_socket_process::SEND_WB_HANDSHAKE_OK_PROCESS()
{    
    string *p_str = NULL;   
    if (_p_tmp_str.begin() != _p_tmp_str.end())
    {
        p_str = _p_tmp_str.front();
        _p_tmp_str.pop_front();				
        WRITE_TRACE("real send ping to peer");
    }
    else
    {
        if (_recent_send_web_header._wb_body_status == WB_FRAME_HEAD_STAUS)
        {
            int8_t content_type = 0;
            uint64_t len = _p_data_process->get_next_send_len(content_type);
            if (len > 0)
            {
                p_str = new string();					
                string mask_key;					
                if (_if_send_mask)
                {
                    int32_t r = rand();
                    mask_key.assign((char*)&r, 4);
                }
                *p_str = _recent_send_web_header.gen_frame_header(len, mask_key, content_type); //change status
            }
        }
        else//WB_FRAME_BODY_STAUS
        {		
            p_str = _p_data_process->get_send_buf();
            if (p_str != NULL)
            {               
                if (_recent_send_web_header._mask_flag == 1)
                {
                    *p_str = _recent_send_web_header.mask_code(p_str->c_str(), p_str->length());
                }
                _recent_send_web_header.update(p_str->length()); //change status
                if (_recent_send_web_header.if_finish())
                {
                    _recent_send_web_header.clear();
                }
            }
        }
    }
    return p_str;
}


size_t web_socket_data_process::RECV_WB_HANDSHAKE_OK_PROCESS(const char *buf, const size_t len)
{
    WRITE_TRACE("RECV_WB_HANDSHAKE_OK_PROCESS %d", len);
    char *left_buf = (char*)buf;
    uint32_t left_len = len;
    while(left_len > 0)
    {							       
        if (_recent_recv_web_header._wb_body_status == WB_FRAME_HEAD_STAUS)
        {
            _recent_recv_web_header.process(left_buf, left_len);					
        }

        if (_recent_recv_web_header._wb_body_status == WB_FRAME_BODY_STAUS)
        {
            uint32_t tmp_left = _recent_recv_web_header.update(left_len);//状态在里边变换
            int8_t tmp_code = _recent_recv_web_header._op_code;
            if (tmp_code != 0x09 && tmp_code != 0x0a) //ping包,pung包不要到上层处理了
            {	
                if (_recent_recv_web_header._payload_len == 0)//直接取下一条消息
                {
                    _recent_recv_web_header.clear();
                    continue;
                }

                if (left_len > 0)
                {
                    if (_recent_recv_web_header._mask_flag == 1)
                    {
                        string tmp_ret = _recent_recv_web_header.mask_code(left_buf, left_len - tmp_left);						
                        _p_data_process->process_recv_body(tmp_ret.c_str(), tmp_ret.length());
                    }
                    else
                    {
                        _p_data_process->process_recv_body(left_buf, left_len - tmp_left);
                    }

                    left_buf = left_buf + (left_len - tmp_left);
                    left_len = tmp_left;

                    if (_recent_recv_web_header.if_finish())
                    {
                        _p_data_process->msg_recv_finish();
                    }	
                }                                        
            }
            else //ping包, pung包处理
            {
                _ping_data.append(left_buf,  left_len - tmp_left);
                left_buf = left_buf + (left_len - tmp_left);
                left_len = tmp_left; 
                if(_recent_recv_web_header.if_finish()) //更新状态,ping包就在底层处理了
                {				
                    _p_data_process->on_ping(tmp_code, _ping_data);
                    _ping_data.clear();
                }
            }
        }
    }
    return len;
}


#ifndef _BASE_MSG_PROCESS_H_
#define _BASE_MSG_PROCESS_H_

#include "common_epoll.h"
#include "net_obj.h"


enum BASE_MSG_PROCESS_STATUS
{
    RECV_MSG_HEAD,
    RECV_EXTEND_BODY,
    RECV_MSG_BODY	
};


const uint32_t MSG_HEAD_BUF_SIZE = 10;
const uint32_t MAX_EXTEND_BODY_LEN = 64*1024;
#pragma pack( push, 1)
struct msg_base_head
{
    uint32_t _body_len;       
    char  _buf[MSG_HEAD_BUF_SIZE];
    uint16_t _extend_body_len;
    msg_base_head()
    {
        _body_len = 0;
        _extend_body_len = 0;
        memset(_buf, 0, MSG_HEAD_BUF_SIZE);
    }
};
#pragma pack(pop)
const string KU_MSG_HEAD_CHECK_STR = "hello"; //这个不能超过10个字节的


const size_t MSG_HEAD_LEN = sizeof(msg_base_head);
const size_t MAX_KU_MSG_BODY_LEN = 10*1024*1024;


class ku_connect_info:public connect_info
{
};

struct ku_data_obj_msg:public normal_obj_msg
{
    string _s_data;
    string _extend_data;
};


template<class DATA_PROCESS>
class base_msg_process
{
    public:
        base_msg_process(void *p)
        {
            //_p_cur_send_msg = NULL;
            recv_init();
            clear_send_list();
            _data_process = DATA_PROCESS::gen_process((void*)this);
            _p_connect = (NET_OBJ*)p;
        }

        virtual ~base_msg_process()
        {
            if (_data_process != NULL)
                delete _data_process;
            clear_send_list();
        }	

        size_t process_recv_buf(char *buf, size_t len)
        {
            LOG_DEBUG("recv buf %d", len);
            //size_t ret = 0;
            size_t left_len = len;
            while(left_len > 0)
            {
                if (_status == RECV_MSG_HEAD)
                {
                    size_t head_left_len = MSG_HEAD_LEN - _s_head.length();
                    if (left_len >= head_left_len)
                    {
                        _s_head.append(buf + len - left_len, head_left_len);

                        left_len = left_len - head_left_len;
                        memcpy(&_msg_head, _s_head.c_str(), sizeof(_msg_head));
                        _msg_head._body_len = ntohl(_msg_head._body_len);
                        _msg_head._extend_body_len = ntohs(_msg_head._extend_body_len);
                        WRITE_TRACE("extend body len %d, body len %d", _msg_head._extend_body_len, 
                                _msg_head._body_len);
                        if (_msg_head._extend_body_len == 0)
                            _status = RECV_MSG_BODY;
                        else
                            _status = RECV_EXTEND_BODY;
                        if (_msg_head._body_len == 0)
                            WRITE_WARN("body len zero %lld, %s, sHead.length %lld, head_left_len %lld left_len %d", len, _msg_head._buf, _s_head.length(), head_left_len, left_len);

                        if (_msg_head._body_len > MAX_KU_MSG_BODY_LEN)
                        {
                            THROW_SOCKET_EXPECT(-1, "body length is too big, can't greater than " << MAX_KU_MSG_BODY_LEN)
                        }


                        //属于严重错误应该直接断开连接
                        if (strcmp(_msg_head._buf, KU_MSG_HEAD_CHECK_STR.c_str()) != 0)
                        {
                            LOG_WARNING("_msg_head._body_len %d, body buf is not hello %s",_msg_head._body_len, _msg_head._buf);
                            THROW_SOCKET_EXPECT(-1, "body buf is not " << KU_MSG_HEAD_CHECK_STR)
                        }						
                    }
                    else
                    {
                        _s_head.append(buf + len - left_len, left_len);
                        left_len = 0;
                    }
                }

                if (_status == RECV_EXTEND_BODY)
                {                                  
                    size_t body_left =   (_msg_head._extend_body_len - _recv_extend_body_len > left_len)?
                        left_len:_msg_head._extend_body_len - _recv_extend_body_len;
                    _extend_body.append(buf + len - left_len,   body_left);
                    left_len = left_len -  body_left;
                    _recv_extend_body_len += body_left;
                    if (_recv_extend_body_len == _msg_head._extend_body_len)
                        _status = RECV_MSG_BODY;
                    WRITE_TRACE("extend body recv over");
                }

                if (_status == RECV_MSG_BODY)
                {
                    size_t body_left = (_msg_head._body_len - _recv_body_len > left_len)?
                        left_len:_msg_head._body_len - _recv_body_len;
                    size_t tmpret = _data_process->process_recv_buf(buf + len - left_len, body_left);
                    _recv_body_len += tmpret;
                    //ret += tmpret;	
                    left_len = left_len - tmpret;
                    if (tmpret != body_left)
                        break;

                    if (_recv_body_len == _msg_head._body_len)
                    {
                        recv_init();
                    }
                    WRITE_TRACE("extend body recv over");
                }				
            }

            return len - left_len;
        }	

        string *get_send_buf()
        {
            string *ret =  get_send_msg();
            return ret;
        }

        void handle_timeout(const uint32_t timer_type)
        {
        }

        virtual void process_s(normal_obj_msg *p_msg)
        {		
            ku_data_obj_msg *p_k_m = dynamic_cast<ku_data_obj_msg*>(p_msg);
            if (p_k_m != NULL)
            {
                if (p_k_m->_extend_data.length() > MAX_EXTEND_BODY_LEN)
                {
                    THROW_COMMON_EXCEPT(-1, "extend body len  must less than " << MAX_EXTEND_BODY_LEN);
                }
                msg_base_head head;			
                gen_head(p_k_m->_s_data, head, p_k_m->_extend_data);

                string *p = new string();
                p->append((char*)&head, sizeof(head));
                p->append(p_k_m->_extend_data);
                p->append(p_k_m->_s_data);
                put_msg(p);
            }	
        }

        void reset()
        {
            recv_init();
            clear_send_list();
            _data_process->reset();
        }

        virtual void routine()
        {
        }


        uint32_t get_body_len()
        {
            return _msg_head._body_len;
        }

        msg_base_head &get_msg_head()
        {
            return _msg_head;
        }

        const string &get_extend_body()
        {
            return _extend_body;
        }

        NET_OBJ* get_connect()
        {
            return _p_connect;
        }

        connect_info *gen_connect_info()
        {
            connect_info *p_info = new ku_connect_info();
            return p_info;
        }

        void peer_close()
        {
        }

        void set_para()
        {		   
        }

        void on_connect_comming()
        {
            _data_process->on_connect_comming();
        }
    private:
        virtual void gen_head(string &buf, msg_base_head &head, const string &extend_body)
        {
            head._body_len = htonl(buf.length());
            head._extend_body_len = htons(extend_body.length());                   
            strcpy(head._buf, KU_MSG_HEAD_CHECK_STR.c_str());
        }

        void recv_init()
        {		
            _status = RECV_MSG_HEAD;
            memset(&_msg_head, 0, sizeof(_msg_head));
            _recv_body_len = 0;
            _recv_extend_body_len = 0;
            _s_head.clear();
            _extend_body.clear();
        }

        void clear_send_list()
        {			
            for (list<string*>::iterator itr = _send_list.begin(); itr != _send_list.end(); ++itr)
            {
                delete *itr;
            }
            _send_list.clear();
        }

        string *get_send_msg()
        {
            if (_send_list.begin() == _send_list.end())
                return NULL;

            string *p = *(_send_list.begin());
            _send_list.erase(_send_list.begin());
            return p;
        }

        void put_msg(string *p_msg)
        {
            bool if_add = false;
            if (_send_list.begin() == _send_list.end())
                if_add = true;
            _send_list.push_back(p_msg);
            if (if_add)
                _p_connect->add_event(EPOLLOUT);
        }
    private:		
        string _s_head;
        msg_base_head _msg_head;
        uint32_t _recv_body_len;
        list<string*> _send_list;
        BASE_MSG_PROCESS_STATUS _status;
        DATA_PROCESS *_data_process;
        NET_OBJ *_p_connect;
        uint32_t _recv_extend_body_len;
        string _extend_body;
};
#endif


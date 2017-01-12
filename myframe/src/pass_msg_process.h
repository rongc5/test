#ifndef _BASE_MSG_PROCESS_H_
#define _BASE_MSG_PROCESS_H_
#include <string>
#include "common_epoll.h"
#include <list>
#include "net_obj.h"
#include "common_def.h"

using namespace std;

enum KU_MSG_PROCESS_STATUS
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
class pass_msg_process
{
	public:
		pass_msg_process(void *p)
		{
			//_p_cur_send_msg = NULL;
			recv_init();
			clear_send_list();
			_data_process = DATA_PROCESS::gen_process((void*)this);
			_p_connect = (NET_OBJ*)p;
		}
		
		virtual ~pass_msg_process()
		{
			if (_data_process != NULL)
				delete _data_process;
                     clear_send_list();
		}	

		virtual size_t process_recv_buf(char *buf, size_t len) = 0;
		
		string *get_send_buf()
		{
			string *ret =  get_send_msg();
			return ret;
		}

        void handle_timeout(const uint32_t timer_type)
        {
        }

		virtual void process_s() = 0;

		void reset()
		{
			recv_init();
			clear_send_list();
			_data_process->reset();
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

		virtual void peer_close() = 0;

     void set_para()
		{		   
		}
                 
     virtual void on_connect_comming() = 0;
	 virtual void put_msg(string *p_msg) = 0;
	 
	 static gen_connect(int fd);

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

	private:		
		string _s_head;
		msg_base_head _msg_head;
		uint32_t _recv_body_len;
		list<string*> _send_list;
		KU_MSG_PROCESS_STATUS _status;
		DATA_PROCESS *_data_process;
		NET_OBJ *_p_connect;
              uint32_t _recv_extend_body_len;
              string _extend_body;
};
#endif


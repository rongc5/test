#ifndef __WEB_SOCKET_PROCESS_H_
#define __WEB_SOCKET_PROCESS_H_

#include "base_def.h"
#include "http_base_process.h"
#include "web_socket_msg.h"


class web_socket_process;

class ws_data_process
{
	public:
		ws_data_process(web_socket_process *p)
		{
			_p_process = p;
		}

		virtual ~ws_data_process()
		{
		}

		virtual void peer_close() 
		{
		}

		virtual void on_handshake_ok()		
		{
		}

		virtual void on_ping(const char op_code, const string &ping_data)
		{			
		}

		virtual void process_s(normal_obj_msg *p_msg)
		{
		}

		virtual void handle_timeout(const uint32_t timer_type)
		{
		}

		virtual uint64_t get_timeout_len()
		{
			return WS_CONNECT_TIMEOUT;
		}

		virtual uint64_t get_next_send_len(int8_t &content_type) = 0;
		virtual size_t process_recv_body(const char *buf, const size_t len) = 0; 
		virtual string *get_send_buf() = 0;
		virtual void msg_recv_finish() = 0;
	protected:
		web_socket_process *_p_process;
};

class web_socket_process
{
	public:
		web_socket_process(NET_OBJ *p)
		{
			_wb_status = WB_INIT_STAUTS;
			_p_connect = p;
			_if_send_mask = true;
			_p_data_process = NULL;
		}

		virtual ~web_socket_process()
		{
			if (_p_data_process != NULL)
				delete _p_data_process;

			for (list<string*>::iterator itr = _p_tmp_str.begin(); itr != _p_tmp_str.end(); ++itr)
			{
				if (*itr != NULL)
					delete *itr;
			}
		}

		//处理接收的数据
		size_t process_recv_buf(const char *buf, const size_t len)
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
				THROW_COMMON_EXCEPT(-1, "WB_HANDSHAKE_FAIL status, can't recv and send data")
			}
			return len;
		}

		void process_s(normal_obj_msg *p_msg)
		{
			if (_p_data_process != NULL)
				_p_data_process->process_s(p_msg);
		}

		string* get_send_buf()
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
				THROW_COMMON_EXCEPT(-1, "WB_HANDSHAKE_FAIL status, can't recv and send data")
			}
			return p_str;
		}

		virtual void reset()
		{
		}

		void routine()
		{
		}

		void set_para()
		{	
			_p_connect->set_timeout_len(_p_data_process->get_timeout_len());
		}

		virtual void on_connect_comming()
		{
		}

		void peer_close()
		{
			if (_p_data_process != NULL)
				_p_data_process->peer_close();
		}

		void send_ping(const char op_code, const string &ping_data)
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
		string get_web_accept_key(const string &ws_key)
		{
			string accept_key;
			string tmp_key = ws_key + WEB_SOCKET_GUID;			
			unsigned char md[20];
			SHA1((unsigned char*)tmp_key.c_str(), tmp_key.length(), md);
			string aa = string((char*)md, 20);
			accept_key = CBase64::encode(aa);
			return accept_key;
		}


		web_socket_frame_header &get_recent_recv_frame_header()
		{
			return _recent_recv_web_header;
		}

		web_socket_frame_header &get_recent_send_frame_header()
		{
			return _recent_send_web_header;
		}

		void notice_send()
		{
			if (_wb_status == WB_HANDSHAKE_OK)
				_p_connect->add_event(EPOLLOUT);
		}

		const string &get_recv_header()
		{
			return _recv_header;
		}

		const string &get_send_header()
		{
			return _send_header;
		}		

		virtual void handle_timeout(const uint32_t timer_type)
		{
			_p_data_process->handle_timeout(timer_type);
		}
	protected:
		virtual void  parse_header() = 0;        

		virtual bool check_head_finish()
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
					THROW_COMMON_EXCEPT(-1, "http head too long (" << _recv_header.length() << ")")
				}
			}			
			return ret;
		}

		virtual string* SEND_WB_HEAD_FINISH_PROCESS() = 0;
		virtual string* SEND_WB_INIT_STAUTS_PROCESS() = 0;			

		virtual string *SEND_WB_HANDSHAKE_OK_PROCESS()
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


		size_t RECV_WB_HANDSHAKE_OK_PROCESS(const char *buf, const size_t len)
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

		virtual size_t RECV_WB_HEAD_FINISH_PROCESS(const char *buf, const size_t len) = 0;
		virtual size_t RECV_WB_INIT_STAUTS_PROCESS(const char *buf, const size_t len) = 0;
	protected:		
		web_socket_frame_header _recent_recv_web_header;
		web_socket_frame_header _recent_send_web_header;
		string _recv_header;
		string _send_header;
		WEB_SOCKET_STATUS _wb_status;
		ws_data_process*  _p_data_process;
		NET_OBJ *_p_connect;
		bool _if_send_mask;		

		string _ping_data;
		list<string*> _p_tmp_str;
};

template<class PROCESS_GENERATOR>
class web_socket_process_res:public web_socket_process
{
	public:		
		web_socket_process_res(NET_OBJ *p):web_socket_process(p)
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
		      _p_connect->set_timer(WEB_SOCKET_HANDSHAKE_OK_TIMER_TYPE, 
                        WEB_SOCKET_HANDSHAKE_OK_TIMER_LENGTH);
		}

              virtual void handle_timeout(const uint32_t timer_type)
		{
			if (timer_type == WEB_SOCKET_HANDSHAKE_OK_TIMER_TYPE)
			{
				if (_wb_status != WB_HANDSHAKE_OK)
				{
					THROW_COMMON_EXCEPT(-1, "the web socket handshake time out, delete it");
				}
				else
				{
					WRITE_TRACE("web socket handshake timer arrive, status ok");
				}
			}
			else
			{
				web_socket_process::handle_timeout(timer_type);
			}
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
			THROW_COMMON_EXCEPT("web_socket_process_res can't send data in WB_INIT_STATUS)");
		}

		virtual size_t RECV_WB_HEAD_FINISH_PROCESS(const char *buf, const size_t len)
		{
			THROW_COMMON_EXCEPT("web_socket_process_res can't recv data in WB_HEAD_FINISH)");
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


struct ws_req_head_para
{
	string _s_path;
	string _s_host;
	string _s_websocket_key;
	string _origin;
	uint32_t _version;
	ws_req_head_para()
	{
		_version = 13;
	}
};

class web_socket_connect_info:public connect_info
{
	public:
		ws_req_head_para _req_head_para;
};


template<class PROCESS_GENERATOR>
class web_socket_process_req:public web_socket_process
{
	public:
		web_socket_process_req(NET_OBJ *p):web_socket_process(p)
	{			
		_p_data_process = PROCESS_GENERATOR::gen_ws_data_process(this);
	}

		ws_req_head_para &get_req_para()
		{
			return _req_para;
		}		

		connect_info *gen_connect_info()
		{
			web_socket_connect_info *p_info = new web_socket_connect_info();
			p_info->_req_head_para = _req_para;
			return p_info;
		}
	protected:	   
		virtual void ping_process(const int8_t op_code)//客户端不用处理服务器的ping
		{
			WRITE_TRACE("recv server ping res");
		}

		virtual string* SEND_WB_HEAD_FINISH_PROCESS()
		{
			return NULL;
		}

		virtual string* SEND_WB_INIT_STAUTS_PROCESS()
		{
			string *p_str = new string();
			*p_str = gen_send_http_head();
			_wb_status = WB_HEAD_FINISH;
			_send_header = *p_str;
			return p_str;
		}

		virtual size_t RECV_WB_HEAD_FINISH_PROCESS(const char *buf, const size_t len)
		{
			_recv_header.append(buf, len);
			check_head_finish();
			return len; 
		}

		virtual size_t RECV_WB_INIT_STAUTS_PROCESS(const char *buf, const size_t len)
		{
			THROW_COMMON_EXCEPT(-1, "web_socket_process_res can't recv data in WB_INIT_STAUTS");
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
		string gen_send_http_head()
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

		virtual void  parse_header()
		{
			string ret_code;
			CToolKit::GetCaseStringByLabel(_recv_header, "HTTP/1.1 ", " ", ret_code);
			if (ret_code != "101")
			{
				THROW_COMMON_EXCEPT(-1, "ret_code "<< ret_code << " is not right ")
			}

			CToolKit::GetCaseStringByLabel(_recv_header, "Sec-WebSocket-Accept:", "\r\n", _s_accept_key);			
			CToolKit::StringTrim(_s_accept_key);
			//校验下
			string tmp = get_web_accept_key(_req_para._s_websocket_key);			
			if (tmp != _s_accept_key)
			{
				THROW_COMMON_EXCEPT(-1, "parse_header recv _s_accept_key "<< _s_accept_key << " is not right , it should be " 
						<< tmp);
			}
		}


		bool check_head_finish()
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


	protected:
		ws_req_head_para _req_para;
		string _s_accept_key;
};

class normal_ws_data_process:public ws_data_process
{
	public:
		normal_ws_data_process(web_socket_process *p):ws_data_process(p)
	{
	}

		~normal_ws_data_process()
		{
			for(list<ws_msg_type>::iterator itr = _send_list.begin(); itr != _send_list.end(); ++itr)
			{
				delete itr->_p_msg;
			}	
		}

		virtual uint64_t get_next_send_len(int8_t &content_type)
		{
			uint64_t len = 0;
			if (_send_list.begin() != _send_list.end())
			{
				len = _send_list.begin()->_p_msg->length();
				content_type = _send_list.begin()->_con_type;
			}
			return len;
		}	

		virtual string *get_send_buf()
		{	
			string *p_ret = NULL;
			ws_msg_type tmp = get_send_msg();
			p_ret = tmp._p_msg;
			return p_ret;
		}

	protected:
		void put_send_msg(ws_msg_type msg)
		{
			if (_send_list.begin() == _send_list.end())
			{
				_p_process->notice_send();
			}
			_send_list.push_back(msg);
		}

		ws_msg_type get_send_msg()
		{
			ws_msg_type ret;
			if (_send_list.begin() != _send_list.end())
			{
				ret = _send_list.front();
				_send_list.pop_front();
			}
			return ret;
		}

		list<ws_msg_type> _send_list;
};


#endif


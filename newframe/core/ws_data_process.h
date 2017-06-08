#ifndef __WS_DATA_PROCESS_H__
#define __WS_DATA_PROCESS_H__


#include "log_helper.h"
#include "base_connect.h"
#include "base_net_thread.h"
#include "common_def.h"


/*
   websocket 协议字段
   0					1					2					3
   0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
   +-+-+-+-+-------+-+-------------+-------------------------------+
   |M|R|R|R| opcode|R| Payload len |	 Extended payload length	|
   |O|S|S|S|  (4)	|S| 	(7) 	|			  (16/63)			|
   |R|V|V|V|		|V| 			|	(if payload len==126/127)	|
   |E|1|2|3|		|4| 			|								|
   +-+-+-+-+-------+-+-------------+ - - - - - - - - - - - - - - - +
   |	  Extended payload length continued, if payload len == 127	|
   + - - - - - - - - - - - - - - - +-------------------------------+
   |								|		  Extension data		|
   +-------------------------------+ - - - - - - - - - - - - - - - +
   :																:
   +---------------------------+
   :						Application data						:
   +---------------------------+


   0                   1                   2                   3
   0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
   +-+-+-+-+-------+-+-------------+-------------------------------+
   |F|R|R|R| opcode|M| Payload len |    Extended payload length    |
   |I|S|S|S|  (4)  |A|     (7)     |             (16/63)           |
   |N|V|V|V|       |S|             |   (if payload len==126/127)   |
   | |1|2|3|       |K|             |                               |
   +-+-+-+-+-------+-+-------------+ - - - - - - - - - - - - - - - +
   |     Extended payload length continued, if payload len == 127  |
   + - - - - - - - - - - - - - - - +-------------------------------+
   |                               |Masking-key, if MASK set to 1  |
   +-------------------------------+-------------------------------+
   | Masking-key (continued)       |          Payload Data         |
   +-------------------------------- - - - - - - - - - - - - - - - +
   :                     Payload Data continued ...                :
   + - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - +
   |                     Payload Data continued ...                |
   +---------------------------+

*/	
const string WEB_SOCKET_GUID = "258EAFA5-E914-47DA-95CA-C5AB0DC85B11";
const uint64_t MAX_INT32_NUM = 4294967295;
const uint64_t MAX_INT16_NUM = 65535;
const string WEB_SOCKET_NONCE_KEY = "AQIDBAUGBwgJCgsMDQ4PEC==";
const uint32_t WS_CONNECT_TIMEOUT = 20*60*1000; //20分钟
const uint32_t MAX_PAYLOAD_LEN = 100*1024;

const uint32_t WEB_SOCKET_HANDSHAKE_OK_TIMER_TYPE = 5001;
const uint32_t WEB_SOCKET_HANDSHAKE_OK_TIMER_LENGTH = 30*1000;

enum WEB_SOCKET_STATUS
{
	WB_INIT_STAUTS,
	WB_HEAD_FINISH,
	WB_HANDSHAKE_OK,
	WB_HANDSHAKE_FAIL
};


enum WEB_SOCKET_FRAME_STATUS
{
	WB_FRAME_HEAD_STAUS,
	WB_FRAME_BODY_STAUS
};



class web_socket_frame_header
{	
	public:
		web_socket_frame_header()
		{
			clear();
		}

		void clear()
		{
			_s_header.clear();
			_wb_body_status = WB_FRAME_HEAD_STAUS;
			_payload_len = 0;
			_mask_flag = -1;
			_e_p_len = 0;
			_process_body_len = 0;
			_mask_offset = 0;
			_mask_key.clear();
			_op_code = 1;
			_more_flag=0;
		}

		void process(char* &buf,   uint32_t &len)
		{
			char *ret_buf =NULL;
			uint32_t ret_len = 0;
			_s_header.append(buf, len);
			if (_s_header.length() >= 2)
			{
				if (_mask_flag == -1)
				{
					_op_code = _s_header[0]  & 0xF;
					_more_flag = (_s_header[0] >> 7) & 0x01;
					if (_op_code == 0x08) //客户端关闭
					{
						THROW_COMMON_EXCEPT(-1, "websocket client close the connection")
					}					

					_mask_flag = (_s_header[1] >> 7) & 0x01;
					uint8_t tmp_len = (_s_header[1] & 0x7F);
					if (tmp_len < 126)
					{					
					}
					else if (tmp_len == 126)
					{
						_e_p_len = 2;
					}
					else //127
					{
						_e_p_len = 8;
					}
				}

				//计算left_str
				if (_mask_flag == 1)
				{	
					if (_s_header.length() >= (uint32_t)(2 + _e_p_len + 4))
					{
						uint32_t aa_len = _s_header.length() - (2 + _e_p_len + 4);                                         
						ret_buf = buf + (len - aa_len);
						ret_len = aa_len;
						_mask_key.resize(4);
						memcpy((char*)_mask_key.c_str(), _s_header.c_str() + 2 + _e_p_len, 4);
						//left_str = _s_header.substr(6 + _e_p_len);
						_s_header = _s_header.substr(0, 6 + _e_p_len);
						get_payload_length();
						_wb_body_status = WB_FRAME_BODY_STAUS;
					}

				}
				else
				{
					if (_s_header.length() >= (uint32_t)(2 + _e_p_len))
					{
						uint32_t aa_len = _s_header.length() - (2 + _e_p_len);                                       
						ret_buf = buf + (len - aa_len); //_s_header.substr(2 + _e_p_len);
						ret_len = aa_len;
						_s_header = _s_header.substr(0, 2 + _e_p_len);
						get_payload_length();
						_wb_body_status = WB_FRAME_BODY_STAUS;
					}	

				}
			}		
			else
			{
				ret_buf = buf + len;
				ret_len = 0;
			}

			buf = ret_buf;
			len = ret_len;
		}

		static string gen_ping_header(const int8_t op_code/*0x09 or  0x10*/, const string &ping_data)//ping or pung
		{
			char aa = 0;
			aa = aa | (0x01 << 7);
			aa = aa | op_code;
			string frame_header;
			frame_header.append(1, aa);
			char bb = 0;
			bb = bb | (char)ping_data.length();
			frame_header.append(1, bb);
			if (ping_data.length() > 0)
				frame_header.append(ping_data);
			return frame_header;
		}

		string gen_frame_header(const uint64_t data_len, const string &mask_key, const int8_t content_type)
		{	
			if (!mask_key.empty())
			{
				_mask_flag = 1;
				_mask_key = mask_key;
			}
			else
			{
				_mask_flag = 0;
			}

			_op_code = content_type;
			_payload_len = data_len;
			string frame_header;

			char aa = 0;
			aa = aa | (0x01 << 7);
			aa = aa | _op_code;
			frame_header.append(1, aa);
			if (data_len < 126)
			{
				char tmp = data_len;
				tmp = tmp | (_mask_flag << 7);
				frame_header.append(1, tmp);
			}
			else if (data_len <= MAX_INT16_NUM)//16 bit int
			{
				char tmp = 126;
				tmp = tmp | (_mask_flag << 7);
				frame_header.append(1, tmp);
				uint16_t len = data_len;
				len = htons(len);
				frame_header.append((char*)&len, 2);
			}
			else //64 bit int
			{
				char tmp = 127;
				tmp = tmp | (_mask_flag << 7);
				frame_header.append(1, tmp);
				uint64_t tmp_len = CToolKit::htonl64(data_len);
				frame_header.append((char*)&tmp_len, 8);
			}

			if (_mask_flag == 1)
			{
				frame_header.append(mask_key.c_str(), 4);
			}
			_wb_body_status = WB_FRAME_BODY_STAUS;
			return frame_header;
		}

		uint32_t update(const uint32_t len)
		{	
			uint32_t ret = 0;
			if (_process_body_len + len < _payload_len)
			{
				_process_body_len += len;
				ret = 0;
			}
			else
			{	
				ret =_process_body_len + len - _payload_len;              
				_process_body_len = _payload_len;
			}
			return ret;
		}

		bool if_finish()
		{
			bool ret = false;
			if (_op_code == 0x09 || _op_code == 0x0a)
			{
				ret = true;
			}
			else
			{
				ret =  (_payload_len != 0) && (_process_body_len == _payload_len) ;
			}
			if (ret)
				clear();
			return ret && (_more_flag == 0);
		}	

		string mask_code(const char *p_buf, const size_t len)
		{		
			string ret;
			ret.resize(len);
			for (uint32_t ii=0; ii < len; ++ii)
			{
				ret[ii] = p_buf[ii] ^ _mask_key[_mask_offset];
				_mask_offset = (_mask_offset + 1)%4;
			}
			return ret;
		}

		string _s_header;
		WEB_SOCKET_FRAME_STATUS _wb_body_status;
		uint64_t _payload_len;
		uint8_t _e_p_len;
		int8_t _mask_flag;
		int8_t _more_flag;
		string _mask_key;
		uint64_t _process_body_len;		
		uint32_t _mask_offset;
		int8_t _op_code;
	private:
		void get_payload_length()
		{
			if (_e_p_len == 0)	
			{
				_payload_len = (_s_header[1] & 0x7F);			
			}
			else if (_e_p_len == 2)
			{
				uint16_t tmp = 0;
				memcpy(&tmp , _s_header.c_str() + 2, sizeof(tmp));
				_payload_len = ntohs(tmp);
			}
			else 
			{
				memcpy(&_payload_len , _s_header.c_str() + 2, 8);
				_payload_len = CToolKit::ntohl64(_payload_len);
			}
			if (_payload_len >= MAX_PAYLOAD_LEN)
				THROW_COMMON_EXCEPT(-1,  "PAYLOAD_LEN too long " << _payload_len);
		}
};



struct ws_msg_type
{
	string *_p_msg;
	int8_t _con_type;
	ws_msg_type()
	{   
		init();
	}   
	void init()
	{   
		_p_msg = NULL;
		_con_type = 0x01;
	}   
};


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

#endif

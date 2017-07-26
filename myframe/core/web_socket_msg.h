#ifndef __WEB_SOCKET_MSG_H__
#define __WEB_SOCKET_MSG_H__


#include "log_helper.h"
#include "base_def.h"


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
//const uint64_t MAX_INT32_NUM = 4294967295;
const uint64_t MAX_INT16_NUM = 65535;
const string WEB_SOCKET_NONCE_KEY = "AQIDBAUGBwgJCgsMDQ4PEC==";
const uint32_t WS_CONNECT_TIMEOUT = 20*60*1000; //20分钟
const uint32_t MAX_PAYLOAD_LEN = 100*1024;


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
		web_socket_frame_header();

		void clear();

		void process(char* &buf,   uint32_t &len);

		static string gen_ping_header(const int8_t op_code/*0x09 or  0x10*/, const string &ping_data);//ping or pung

		string gen_frame_header(const uint64_t data_len, const string &mask_key, const int8_t content_type);

		uint32_t update(const uint32_t len);

		bool if_finish();

		string mask_code(const char *p_buf, const size_t len);

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
		void get_payload_length();
};


struct ws_msg_type
{
	string *_p_msg;
	int8_t _con_type;
	ws_msg_type();

    void init();
};

struct ws_req_head_para
{
	string _s_path;
	string _s_host;
	string _s_websocket_key;
	string _origin;
	uint32_t _version;
	ws_req_head_para();
};

#endif

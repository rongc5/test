#include "base_data_process.h"
#include "framecommon/obj_collector.h"

size_t base_data_process::process_recv_buf(char *buf, size_t len)
{
	_s_buf.append(buf, len);
    if (_s_buf.length() == _p_msg_process->get_body_len())
    {   
    	//_s_buf 自动回收, 在抛异常的时候也能够析构这个数据
    	obj_collector<string, string_gc> tmp_gc(&_s_buf);
        trans_msg(_s_buf);
    }   
    else if (_s_buf.length() > _p_msg_process->get_body_len())
    {   
        WRITE_WARN("invalid %lld, %lld", _s_buf.length(), _p_msg_process->get_body_len());
    }   
	return len;
}




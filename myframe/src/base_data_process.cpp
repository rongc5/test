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

void base_data_process::trans_msg(const string &buf)
{   		 	
    ku_lhead head;
	memcpy(&head, buf.c_str(), sizeof(ku_lhead));
	head.N_2_H();

	KU_SOCK_TYPE sock_type;
	bool if_create_passive_handle = false;
	if (head._msg_type == DATA_GRAM) //数据报
	{
		sock_type = GRAM;
		if (head._dest_handle == 0)//被动端
		{
			if_create_passive_handle = true;					
		}
        /*
		else //主动端
		{	
			create_type = INITIATIVE_TYPE;
		}
		*/
	}
    else //流方式
    {   
    	sock_type = STREAM;
    	if (head._msg_type == OPEN_HANDLE )
        {        
        	if_create_passive_handle = true;
        }
        /*
		else
		{
			create_type = INITIATIVE_TYPE;	                	
		}
		*/
    }
			
	if (if_create_passive_handle) //被动创建handle并进行消息处理
	{
		obj_id_str obj_id;
        obj_id._obj_id = _p_msg_process->get_connect()->get_id();
        obj_id._thread_index = common_thread_obj::get_thread_index();    
		sockaddr_in addr;
		_p_msg_process->get_connect()->get_peer_addr(addr);
        PASSIVE_NEW_HANDLE(obj_id, buf, addr, sock_type); //放入accept队列
	}
	else //消息处理
	{	
		try
    	{
        	ku_handle handle(head._dest_handle);
        	PUT_MSG_TO_HANDLE(handle, buf, sock_type);
    	}
		catch(CMyCommonException &e)
		{
			WRITE_ERROR("put msg to handle fail");
		}
	}		
    //_s_buf.clear();
}



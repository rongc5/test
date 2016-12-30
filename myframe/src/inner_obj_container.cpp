 #include "net_obj_container.h"
 
void net_obj_container::obj_process()
{   
	uint32_t tmp_num = 0;
	map<uint64_t, base_net_obj*> exp_list;
	for (map<uint64_t, base_net_obj*>::iterator tmp_itr = _obj_net_map.begin();
			tmp_itr != _obj_net_map.end(); )
	{
		int32_t ret = 0;
		try
		{
			ret = tmp_itr->second->real_net_process();            
		}
		catch(CMyCommonException &e)
		{
			exp_list.insert(make_pair(tmp_itr->first,tmp_itr->second));	
		}
		catch(std::exception &e)
		{
			exp_list.insert(make_pair(tmp_itr->first,tmp_itr->second));	
		}

		if (ret == -1) //空的对象删除之
		{
			map<uint64_t, base_net_obj*>::iterator aa_itr = tmp_itr;
			++tmp_itr;
			_obj_net_map.erase(aa_itr);
		}
		else
		{
			tmp_num++;
			++tmp_itr;
		}
	}


	int ret = _p_epoll->epoll_wait(exp_list, tmp_num);
	if (exp_list.size() != 0)
	{
		for (map<uint64_t, base_net_obj*>::iterator itr = exp_list.begin(); itr != exp_list.end(); ++itr)
		{         	
			_obj_map.erase(itr->second->get_id());
			_obj_net_map.erase(itr->second->get_id());
			itr->second->destroy();
		}
	}       
}

void net_obj_container::register_with_thread(pthread_t tid, base_net_obj * p_net_obj)
	{
			pair<map<pthread_t, base_net_obj*>::iterator, bool> tmp_pair = _obj_map.insert(make_pair(tid, p_net_obj));
			if (!tmp_pair.second)
			{
				THROW_COMMON_EXCEPT(-1, "insert p_obj fail");
			}
			
			insert()
	}

base_net_obj* net_obj_container::find(uint64_t obj_id)
{
	base_net_obj *p_obj = NULL;
	map<uint64_t, base_net_obj*>::iterator itr =  _obj_map.find(obj_id);
	if (itr != _obj_map.end())
		p_obj = itr->second;
	return p_obj;
}

void net_obj_container::insert(base_net_obj *p_obj)
{
	pair<map<uint64_t, base_net_obj*>::iterator, bool> tmp_pair = _obj_map.insert(make_pair(p_obj->get_id(), p_obj));
	if (!tmp_pair.second)
	{
		THROW_COMMON_EXCEPT(-1, "insert p_obj fail");
	}

	p_obj->set_net_container(this);
}

void net_obj_container::push_net_obj(base_net_obj *p_obj)
{
	
	_obj_net_map.insert(make_pair(p_obj->get_id(), p_obj));
}

bool net_obj_container::erase(uint64_t obj_id)
{
	bool ret = false;
	base_net_obj *p_obj = NULL;
	map<uint64_t, base_net_obj*>::iterator itr =  _obj_map.find(obj_id);
	if (itr != _obj_map.end())
	{
		p_obj = itr->second;
		if (p_obj != NULL)
		{
			_obj_net_map.erase(p_obj->get_id());
			p_obj->destroy();	    
		}
		_obj_map.erase(itr);

		ret = true;
	}
	return ret;
}



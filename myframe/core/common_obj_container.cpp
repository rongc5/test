#include "common_def.h"
#include "common_epoll.h"
#include "base_net_container.h"
#include "common_obj_container.h"
#include "base_net_thread.h"
#include "base_timer.h"
#include "log_helper.h"


common_obj_container::common_obj_container(base_net_thread * thread):base_net_container(thread)
{
    base_net_container::init();
}

common_obj_container::~common_obj_container()
{
}

void common_obj_container::push_net_obj(base_net_obj *p_obj)
{
    _obj_net_map.insert(make_pair(p_obj->get_id(), p_obj));
}

void common_obj_container::remove_net_obj(base_net_obj *p_obj)
{
    _obj_net_map.erase(p_obj->get_id());
}

void common_obj_container::insert(base_net_obj *p_obj)
{
    p_obj->set_id(gen_id_str());
    _obj_map.insert(make_pair(p_obj->get_id(), p_obj));
}

void common_obj_container::remove(base_net_obj *p_obj)
{
    _obj_net_map.erase(p_obj->get_id());
    _obj_map.erase(p_obj->get_id());
}

base_net_obj* common_obj_container::find(const ObjId * obj_id)
{
    base_net_obj *p_obj = NULL;
    map<ObjId, base_net_obj*>::iterator itr =  _obj_map.find(*obj_id);
    if (itr != _obj_map.end())
        p_obj = itr->second;
    return p_obj;
}

bool common_obj_container::erase(ObjId *obj_id)
{
    if (!obj_id){
        return false;
    }
    bool ret = false;
    base_net_obj *p_obj = NULL;
    map<ObjId, base_net_obj*>::iterator itr =  _obj_map.find(*obj_id);
    if (itr != _obj_map.end())
    {
        p_obj = itr->second;
        if (p_obj != NULL)
        {
            _obj_net_map.erase(p_obj->get_id());
            _obj_map.erase(p_obj->get_id());
            p_obj->destroy();
        }

        ret = true;
    }
    return ret;
}

void common_obj_container::put_msg(ObjId & id, normal_msg * p_msg)
{
    base_net_obj * net_obj = find(&id);
    if (!net_obj) {
        REC_OBJ<normal_msg> rec(p_msg);
        return;
    }

    net_obj->process_recv_msg(id, p_msg);
}

void common_obj_container::obj_process()
{   
    uint32_t tmp_num = 0;
    map<ObjId, base_net_obj*> exp_list;

    for (map<ObjId, base_net_obj*>::iterator tmp_itr = _obj_net_map.begin();tmp_itr != _obj_net_map.end(); )
    {
        map<ObjId, base_net_obj*>::iterator aa_itr = tmp_itr;
        try
        {
            ++tmp_itr;
            aa_itr->second->real_net_process();            
            tmp_num++;
        }
        catch(CMyCommonException &e)
        {
            LOG_DEBUG("step1: _id:%d, _thread_index:%d", aa_itr->second->get_id()._id, aa_itr->second->get_id()._thread_index);            
            //exp_list.insert(make_pair(aa_itr->first,aa_itr->second));
            _obj_net_map.erase(aa_itr);
            _obj_map.erase(aa_itr->second->get_id());
            aa_itr->second->destroy();
        }
        catch(std::exception &e)
        {
            _obj_net_map.erase(aa_itr);
            _obj_map.erase(aa_itr->second->get_id());
            aa_itr->second->destroy();
        }
    }


    _p_epoll->epoll_wait(exp_list, tmp_num);
    if (exp_list.size() != 0)
    {
        for (map<ObjId, base_net_obj*>::iterator itr = exp_list.begin(); itr != exp_list.end(); ++itr)
        {         	
            LOG_DEBUG("step2: _id:%d, _thread_index:%d", itr->second->get_id()._id, itr->second->get_id()._thread_index);            
            _obj_net_map.erase(itr->second->get_id());
            _obj_map.erase(itr->second->get_id());
            itr->second->destroy();
        }
    }       

    get_timer()->check_timer();
}



#include "common_def.h"
#include "common_epoll.h"
#include "base_net_container.h"
#include "common_obj_container.h"
#include "base_net_thread.h"
#include "base_timer.h"
#include "log_helper.h"
#include "common_util.h"


common_obj_container::common_obj_container(base_net_thread * thread):base_net_container(thread)
{
    base_net_container::init();
}

common_obj_container::~common_obj_container()
{
    for (const auto & k :_obj_map)
    {
        k.second->destroy();
    }
}

bool common_obj_container::push_real_net(base_net_obj *p_obj)
{
    uint64_t id = p_obj->get_id()._id | static_cast<uint64_t>(p_obj->get_id()._thread_index) << 32;

    _obj_net_map[id] = p_obj;

    return true;
}

bool common_obj_container::remove_real_net(base_net_obj *p_obj)
{
    uint64_t id = p_obj->get_id()._id | static_cast<uint64_t>(p_obj->get_id()._thread_index) << 32;

    _obj_net_map.erase(id);

    return true;
}


bool common_obj_container::insert(base_net_obj *p_obj)
{
    p_obj->set_id(gen_id_str());

    uint64_t id = p_obj->get_id()._id | static_cast<uint64_t>(p_obj->get_id()._thread_index) << 32;

    _obj_map[id] = p_obj;

    return true;
}

base_net_obj* common_obj_container::find(const ObjId * obj_id)
{
    std::unordered_map<uint64_t, base_net_obj *>::const_iterator it;
    uint64_t id = obj_id->_id | static_cast<uint64_t>(obj_id->_thread_index) << 32;
    it = _obj_map.find(id);
    if (it == _obj_map.end()){
        return NULL;
    }else {
        return it->second;
    }
}

bool common_obj_container::erase(const ObjId *obj_id)
{
    base_net_obj *p_obj = NULL;
    bool ret = false;
    if (!obj_id){
        return ret;
    }

    p_obj = find(obj_id);
    if (p_obj != NULL)
    {
        uint64_t id = obj_id->_id | static_cast<uint64_t>(obj_id->_thread_index) << 32;
        _obj_net_map.erase(id);
        _obj_map.erase(id);
        ret = true;
    }

    return ret;
}

void common_obj_container::put_msg(ObjId & id, std::shared_ptr<normal_msg> & p_msg)
{
    base_net_obj * net_obj = find(&id);
    if (!net_obj) {
        return;
    }

    net_obj->process_recv_msg(id, p_msg);
}

void common_obj_container::obj_process()
{   
    uint32_t tmp_num = 0;

    std::vector<base_net_obj *> exception_vec;
    std::vector<base_net_obj *> real_net_vec;

    for (const auto & u: _obj_net_map)
    {
        try
        {
            //LOG_DEBUG("step1: _id:%d, _thread_index:%d", aa_itr->second->get_id()._id, aa_itr->second->get_id()._thread_index);            
            u.second->real_net_process();            
            if (!u.second->get_real_net()) {
                real_net_vec.push_back(u.second);
            }

            tmp_num++;
        }
        catch(CMyCommonException &e)
        {
            exception_vec.push_back(u.second);
        }
        catch(std::exception &e)
        {
            exception_vec.push_back(u.second);
        }
    }

    for (std::vector<base_net_obj*>::iterator tmp_itr = real_net_vec.begin(); 
            tmp_itr != real_net_vec.end(); tmp_itr++) {
        remove_real_net(*tmp_itr);
    }

    for (std::vector<base_net_obj*>::iterator tmp_itr = exception_vec.begin(); 
            tmp_itr != exception_vec.end(); tmp_itr++) {
        remove_real_net(*tmp_itr);
        erase(&(*tmp_itr)->get_id());
        (*tmp_itr)->destroy();
    }

    std::map<ObjId, base_net_obj*> exp_list;
    std::map<ObjId, base_net_obj*> remove_list;

    _p_epoll->epoll_wait(exp_list, remove_list, tmp_num);
    for (std::map<ObjId, base_net_obj*>::iterator itr = exp_list.begin(); itr != exp_list.end(); ++itr)
    {         	
        LOG_DEBUG("step2: _id:%d, _thread_index:%d", itr->second->get_id()._id, itr->second->get_id()._thread_index);            
        remove_real_net(itr->second);
        erase(&itr->first);
        itr->second->destroy();
    }

    for (std::map<ObjId, base_net_obj*>::iterator itr = remove_list.begin(); itr != remove_list.end(); ++itr)
    {
        itr->second->set_real_net(false);
        remove_real_net(itr->second);
    }


    get_timer()->check_timer();
}



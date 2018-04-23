#include "common_def.h"
#include "common_epoll.h"
#include "common_obj_container.h"
#include "base_net_thread.h"
#include "base_timer.h"
#include "log_helper.h"
#include "common_util.h"


common_obj_container::common_obj_container(uint32_t thread_index, uint32_t epoll_size)
{
    _p_epoll = new common_epoll();
    _p_epoll->init(epoll_size);

    _timer = new base_timer(this);

    _id_str._id = OBJ_ID_BEGIN;
    _id_str._thread_index = thread_index;
}

common_obj_container::~common_obj_container()
{
    for (const auto & k :_obj_map)
    {
        k.second->destroy();
    }

    if (_p_epoll != NULL)
        delete _p_epoll;

    if (_timer) 
        delete _timer;
}


common_epoll * common_obj_container::get_epoll()
{
        return _p_epoll;
}

base_timer * common_obj_container::get_timer()
{
        return _timer;
}

const ObjId & common_obj_container::gen_id_str()
{
    uint32_t obj_id = _id_str._id;
    do {
        obj_id++;
        _id_str._id = obj_id;
    } while (find(&_id_str) || obj_id <= OBJ_ID_BEGIN);

    return _id_str;
}


bool common_obj_container::push_real_net(std::shared_ptr<base_net_obj> & p_obj)
{
    uint64_t id = p_obj->get_id()._id | static_cast<uint64_t>(p_obj->get_id()._thread_index) << 32;

    _obj_net_map[id] = p_obj;

    return true;
}

bool common_obj_container::remove_real_net(std::shared_ptr<base_net_obj> & p_obj)
{
    uint64_t id = p_obj->get_id()._id | static_cast<uint64_t>(p_obj->get_id()._thread_index) << 32;

    _obj_net_map.erase(id);

    return true;
}


bool common_obj_container::insert(std::shared_ptr<base_net_obj> &p_obj)
{
    p_obj->set_id(gen_id_str());

    uint64_t id = p_obj->get_id()._id | static_cast<uint64_t>(p_obj->get_id()._thread_index) << 32;

    _obj_map[id] = p_obj;

    return true;
}

uint32_t common_obj_container::get_thread_index()
{
    return _id_str._thread_index;
}

void common_obj_container::handle_timeout(timer_msg & t_msg)
{
    bool flag = false;
    base_net_thread * get_base_net_thread_obj(uint32_t thread_index);
    if (t_msg._id._id <= OBJ_ID_BEGIN)
    {
        flag = _net_thread->handle_timeout(t_msg);
        return;
    }

    std::shared_ptr<base_net_obj>  net_obj = find(&t_msg._id);
    if (!net_obj)
    {
        return;
    }

    flag = net_obj->handle_timeout(t_msg);
    if (!flag)
    {
        return;
    }
}

void common_obj_container::add_timer(timer_msg & t_msg)
{
    if (_timer)
        _timer->add_timer(t_msg); 
}


std::shared_ptr<base_net_obj> common_obj_container::find(const ObjId * obj_id)
{
    std::unordered_map<uint64_t, std::shared_ptr<base_net_obj> >::const_iterator it;
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
    std::shared_ptr<base_net_obj> p_obj;
    bool ret = false;
    if (!obj_id){
        return ret;
    }

    p_obj = find(obj_id);
    if (p_obj)
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
    std::shared_ptr<base_net_obj>  net_obj = find(&id);
    if (!net_obj) {
        return;
    }

    net_obj->process_recv_msg(id, p_msg);
}

void common_obj_container::obj_process()
{   
    uint32_t tmp_num = 0;

    std::vector<std::shared_ptr<base_net_obj> > exception_vec;
    std::vector<std::shared_ptr<base_net_obj> > real_net_vec;

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

    for (std::vector<std::shared_ptr<base_net_obj> >::iterator tmp_itr = real_net_vec.begin(); 
            tmp_itr != real_net_vec.end(); tmp_itr++) {
        remove_real_net(*tmp_itr);
    }

    for (std::vector<std::shared_ptr<base_net_obj> >::iterator tmp_itr = exception_vec.begin(); 
            tmp_itr != exception_vec.end(); tmp_itr++) {
        remove_real_net(*tmp_itr);
        erase(&(*tmp_itr)->get_id());
    }

    std::map<ObjId, std::shared_ptr<base_net_obj> > exp_list;
    std::map<ObjId, std::shared_ptr<base_net_obj> > remove_list;

    _p_epoll->epoll_wait(exp_list, remove_list, tmp_num);
    for (std::map<ObjId, std::shared_ptr<base_net_obj> >::iterator itr = exp_list.begin(); itr != exp_list.end(); ++itr)
    {         	
        LOG_DEBUG("step2: _id:%d, _thread_index:%d", itr->second->get_id()._id, itr->second->get_id()._thread_index);            
        remove_real_net(itr->second);
        erase(&itr->first);
        itr->second->destroy();
    }

    for (std::map<ObjId, std::shared_ptr<base_net_obj> >::iterator itr = remove_list.begin(); itr != remove_list.end(); ++itr)
    {
        itr->second->set_real_net(false);
        remove_real_net(itr->second);
    }


    get_timer()->check_timer();
}



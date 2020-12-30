#include "common_def.h"
#include "common_epoll.h"
#include "common_obj_container.h"
#include "base_net_thread.h"
#include "base_timer.h"
#include "log_helper.h"
#include "common_util.h"
#include "common_domain.h"

common_obj_container::common_obj_container(uint32_t thread_index, uint32_t epoll_size)
{
    _p_epoll = new common_epoll();
    _p_epoll->init(epoll_size);

    _timer = new base_timer(this);

    _domain = new common_domain(this);

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

    if (_domain)
        delete _domain;
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
    do {
        if (_id_str._id <= OBJ_ID_BEGIN)
            _id_str._id = OBJ_ID_BEGIN;

        _id_str._id++;
    } while (find(_id_str._id));

    return _id_str;
}


bool common_obj_container::push_real_net(std::shared_ptr<base_net_obj> & p_obj)
{
    LOG_DEBUG("base_net_obj:%d, .use_count:%d, _id:%d _thread_index:%d", p_obj, p_obj.use_count(), p_obj->get_id()._id, p_obj->get_id()._thread_index);

    auto it = _obj_net_map.find(p_obj->get_id()._id);
    if (it == _obj_net_map.end())
    {
        _obj_net_map.insert(std::make_pair(p_obj->get_id()._id, p_obj));
    }
    //_obj_net_map[p_obj->get_id()._id] = p_obj;

    LOG_DEBUG("base_net_obj:%d, .use_count:%d, _id:%d _thread_index:%d", p_obj, p_obj.use_count(), p_obj->get_id()._id, p_obj->get_id()._thread_index);

    return true;
}

bool common_obj_container::remove_real_net(std::shared_ptr<base_net_obj> & p_obj)
{
    LOG_DEBUG("base_net_obj:%d, .use_count:%d, _id:%d _thread_index:%d", p_obj, p_obj.use_count(), p_obj->get_id()._id, p_obj->get_id()._thread_index);
    _obj_net_map.erase(p_obj->get_id()._id);
    LOG_DEBUG("base_net_obj:%d, .use_count:%d, _id:%d _thread_index:%d", p_obj, p_obj.use_count(), p_obj->get_id()._id, p_obj->get_id()._thread_index);

    return true;
}

uint32_t common_obj_container::size()
{
    return _obj_map.size();
}

bool common_obj_container::insert(std::shared_ptr<base_net_obj> &p_obj)
{
    p_obj->set_id(gen_id_str());

    _obj_map[p_obj->get_id()._id] = p_obj;

    return true;
}

uint32_t common_obj_container::get_thread_index()
{
    return _id_str._thread_index;
}

void common_obj_container::handle_timeout(std::shared_ptr<timer_msg> & t_msg)
{
    switch (t_msg->_obj_id)
    {
        case OBJ_ID_THREAD:
            {
                base_net_thread * net_thread = base_net_thread::get_base_net_thread_obj(_id_str._thread_index);
                if (net_thread)
                {
                    net_thread->handle_timeout(t_msg);
                }
            }
            break;
        case OBJ_ID_DOMAIN:
            {
                if (_domain)
                    _domain->handle_timeout(t_msg);
            }
            break;
        default:
            {
                std::shared_ptr<base_net_obj>  net_obj = find(t_msg->_obj_id);
                if (net_obj)
                    net_obj->handle_timeout(t_msg);
            }
    }
    //OBJ_ID_DOMAIN

}

void common_obj_container::add_timer(std::shared_ptr<timer_msg> & t_msg)
{
    if (_timer)
    {
        _timer->add_timer(t_msg); 
    }
}

common_domain * common_obj_container::get_domain()
{
    return _domain;
}

std::shared_ptr<base_net_obj> common_obj_container::find(uint32_t obj_id)
{
    auto it = _obj_map.find(obj_id);
    if (it == _obj_map.end()){
        return NULL;
    }else {
        return it->second;
    }
}

void common_obj_container::erase(uint32_t obj_id)
{
    _obj_net_map.erase(obj_id);
    _obj_map.erase(obj_id);

    return;
}

void common_obj_container::handle_msg(uint32_t obj_id, std::shared_ptr<normal_msg> & p_msg)
{
    switch (obj_id)
    {
        case OBJ_ID_THREAD:
            {
                base_net_thread * net_thread = base_net_thread::get_base_net_thread_obj(_id_str._thread_index);
                if (net_thread)
                {
                    net_thread->handle_msg(p_msg);
                }
            }
            break;
        default:
            {

                std::shared_ptr<base_net_obj>  net_obj = find(obj_id);
                if (net_obj)
                    net_obj->handle_msg(p_msg);
            }
    }
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
        LOG_DEBUG("remove_real_net: _id:%d, _thread_index:%d", (*tmp_itr)->get_id()._id, (*tmp_itr)->get_id()._thread_index);            
        remove_real_net(*tmp_itr);
    }

    for (std::vector<std::shared_ptr<base_net_obj> >::iterator tmp_itr = exception_vec.begin(); 
            tmp_itr != exception_vec.end(); tmp_itr++) {
        (*tmp_itr)->destroy();
        erase((*tmp_itr)->get_id()._id);
    }

    std::map<ObjId, std::shared_ptr<base_net_obj> > exp_list;
    std::map<ObjId, std::shared_ptr<base_net_obj> > remove_list;

    _p_epoll->epoll_wait(exp_list, remove_list, tmp_num);
    for (std::map<ObjId, std::shared_ptr<base_net_obj> >::iterator itr = exp_list.begin(); itr != exp_list.end(); ++itr)
    {         	
        LOG_DEBUG("step2: _id:%d, _thread_index:%d", itr->second->get_id()._id, itr->second->get_id()._thread_index);            
        itr->second->destroy();
        erase(itr->first._id);
    }

    for (std::map<ObjId, std::shared_ptr<base_net_obj> >::iterator itr = remove_list.begin(); itr != remove_list.end(); ++itr)
    {
        LOG_DEBUG("remove_real_net: _id:%d, _thread_index:%d", itr->second->get_id()._id, itr->second->get_id()._thread_index);            
        itr->second->set_real_net(false);
        remove_real_net(itr->second);
    }

    std::vector<uint32_t> exp_vec;

    get_timer()->check_timer(exp_vec);

    for (auto it = exp_vec.begin(); it != exp_vec.end(); it++)
    {
        std::shared_ptr<base_net_obj>  net_obj = find(*it);
        if (net_obj)
        {
            net_obj->destroy();
            erase(*it);
        }
    }
}



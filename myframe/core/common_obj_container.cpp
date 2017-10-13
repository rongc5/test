#include "common_def.h"
#include "common_epoll.h"
#include "base_net_container.h"
#include "common_obj_container.h"
#include "base_net_thread.h"
#include "base_timer.h"
#include "log_helper.h"


common_obj_container::common_obj_container(base_net_thread * thread, uint32_t obj_num):base_net_container(thread)
{
    base_net_container::init();

    this->init(obj_num);
}

void common_obj_container::init(uint32_t obj_num)
{
    _obj_map = new(std::nothrow)inc_dict_t<base_net_obj *>();
    if(_obj_map == NULL){
        LOG_WARNING("new _obj_map failed");
        return ;
    }
    if(_obj_map->create(obj_num/2, obj_num/3) < 0) {
        delete _obj_map;
        LOG_WARNING("failed to allocate memory for _obj_map");
        return ;
    }

    _obj_net_map = new(std::nothrow)inc_dict_t<base_net_obj *>();
    if(_obj_net_map == NULL){
        LOG_WARNING("new _obj_net_map failed");
        return ;
    }
    if(_obj_net_map->create(obj_num/2, obj_num/3) < 0) {
        delete _obj_net_map;
        LOG_WARNING("failed to allocate memory for _obj_net_map");
        return ;
    }
}


common_obj_container::~common_obj_container()
{
    base_net_obj ** p_data = NULL;
    inc_dict_t<base_net_obj *>::travel_info_t m_tranverse;
    memset(&m_tranverse, 0, sizeof(m_tranverse));

    uint32_t sign[2];
    for(p_data = _obj_map->get_begin(&m_tranverse, sign);
            p_data != _obj_map->get_end(&m_tranverse);
            p_data = _obj_map->get_next(&m_tranverse, sign)) {

        (*p_data)->destroy();
    }

    if (_obj_net_map) {
        delete _obj_net_map;
        _obj_net_map = NULL;
    }

    if (_obj_map) {
        delete _obj_map;
        _obj_map = NULL;
    }
}

bool common_obj_container::push_real_net(base_net_obj *p_obj)
{
    uint32_t sign[2];
    sign[0] = p_obj->get_id()._id;
    sign[1] = p_obj->get_id()._thread_index;

    _obj_net_map->add_node(sign, &p_obj);

    return true;
}

bool common_obj_container::remove_real_net(base_net_obj *p_obj)
{
    uint32_t sign[2];
    sign[0] = p_obj->get_id()._id;
    sign[1] = p_obj->get_id()._thread_index;

    _obj_net_map->remove_node(sign);

    return true;

}


bool common_obj_container::insert(base_net_obj *p_obj)
{
    p_obj->set_id(gen_id_str());
    uint32_t sign[2];
    sign[0] = p_obj->get_id()._id;
    sign[1] = p_obj->get_id()._thread_index;

    _obj_map->add_node(sign, &p_obj);

    return true;
}

base_net_obj* common_obj_container::find(const ObjId * obj_id)
{

    uint32_t sign[2] = {obj_id->_id, obj_id->_thread_index};
    base_net_obj** p_obj = _obj_map->seek_node(sign);
    if (!p_obj){
        return NULL;
    }else {
        return *p_obj;
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
        uint32_t sign[2] = {obj_id->_id, obj_id->_thread_index};

        _obj_net_map->remove_node(sign);
        _obj_map->remove_node(sign);
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

    base_net_obj ** p_data = NULL;
    inc_dict_t<base_net_obj *>::travel_info_t m_tranverse;
    memset(&m_tranverse, 0, sizeof(m_tranverse));

    vector<base_net_obj *> exception_vec;
    vector<base_net_obj *> real_net_vec;

    uint32_t sign[2];
    for(p_data = _obj_net_map->get_begin(&m_tranverse, sign);
            p_data != _obj_net_map->get_end(&m_tranverse);
            p_data = _obj_net_map->get_next(&m_tranverse, sign)) 
    {

        try
        {
            //LOG_DEBUG("step1: _id:%d, _thread_index:%d", aa_itr->second->get_id()._id, aa_itr->second->get_id()._thread_index);            
            (*p_data)->real_net_process();            
            if (!(*p_data)->get_real_net()) {
                real_net_vec.push_back(*p_data);
            }

            tmp_num++;
        }
        catch(CMyCommonException &e)
        {
            exception_vec.push_back(*p_data);
        }
        catch(std::exception &e)
        {
            exception_vec.push_back(*p_data);
        }
    }

    for (vector<base_net_obj*>::iterator tmp_itr = real_net_vec.begin(); 
            tmp_itr != real_net_vec.end(); tmp_itr++) {
        remove_real_net(*tmp_itr);
    }

    for (vector<base_net_obj*>::iterator tmp_itr = exception_vec.begin(); 
            tmp_itr != exception_vec.end(); tmp_itr++) {
        remove_real_net(*tmp_itr);
        erase(&(*tmp_itr)->get_id());
        (*tmp_itr)->destroy();
    }

    map<ObjId, base_net_obj*> exp_list;
    map<ObjId, base_net_obj*> remove_list;

    _p_epoll->epoll_wait(exp_list, remove_list, tmp_num);
    for (map<ObjId, base_net_obj*>::iterator itr = exp_list.begin(); itr != exp_list.end(); ++itr)
    {         	
        LOG_DEBUG("step2: _id:%d, _thread_index:%d", itr->second->get_id()._id, itr->second->get_id()._thread_index);            
        remove_real_net(itr->second);
        erase(&itr->first);
        itr->second->destroy();
    }

    for (map<ObjId, base_net_obj*>::iterator itr = remove_list.begin(); itr != remove_list.end(); ++itr)
    {
        itr->second->set_real_net(false);
        remove_real_net(itr->second);
    }


    get_timer()->check_timer();
}



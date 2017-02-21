#include "passing_msg_thread.h"
#include "net_obj.h"
#include "common_thread.h"
#include "common_obj_container.h"
#include "passing_msg_process.h"
#include "base_connect.h"
#include "log_helper.h"
#include "base_net_container.h"
#include "base_def.h"
#include "common_def.h"
#include "base_singleton.h"


passing_msg_thread::passing_msg_thread():_net_container(NULL){
    _net_container = new (std::nothrow)common_obj_container();
}

passing_msg_thread::~passing_msg_thread(){
    if (_net_container){
        delete _net_container;
    }
}

int passing_msg_thread::register_thread(common_thread *thread)
{
    if (!thread) {
        return -1;
    }

    passing_msg_thread * pass_thread = base_singleton<passing_msg_thread>::get_instance();
    if (!pass_thread) {
        base_singleton<passing_msg_thread>::set_instance(new passing_msg_thread());

        pass_thread = base_singleton<passing_msg_thread>::get_instance();
        pass_thread->start();
        //PDEBUG("alloc pass_thread\n");
    }

    int fd[2], ret;

    ret = socketpair(AF_UNIX,SOCK_STREAM,0,fd); 
    if (ret < 0) {
        LOG_WARNING("socketpair fail errstr[%s]", strerror(errno));
    }


    NET_OBJ *p_connect = pass_thread->gen_connect(fd[1], EPOLL_LT_TYPE);
    if (p_connect){
        p_connect->set_id(pass_thread->gen_id_str());
        p_connect->set_net_container(pass_thread->get_net_container());
        pass_thread->set_dest_obj(thread, p_connect);
        pass_thread->set_dest_channelid(thread->get_thread_index(), fd[1]);
        //PDEBUG("thread_index[%u], fd[1] %d fd[0] %d\n", thread->get_thread_index(), fd[1], fd[0]);
        thread->set_channelid(fd[0]);
    }

    return 0;
}

const ObjId & passing_msg_thread::gen_id_str()
{
    uint32_t thread_index = get_thread_index();
    _id_str.set_thread_index(thread_index);
    uint32_t  obj_id = _id_str.obj_id();
    obj_id++;
    _id_str.set_obj_id(obj_id);

    return _id_str;
}

void* passing_msg_thread::run()
{
    while (get_run_flag())
    {
        _net_container->obj_process();
    }

    return NULL;
}

NET_OBJ * passing_msg_thread::gen_connect(const int fd, EPOLL_TYPE epoll_type)
{
    NET_OBJ * p_connect = NULL;
    p_connect = new base_connect<passing_msg_process>(fd, epoll_type);
    passing_msg_process *process = new passing_msg_process((NET_OBJ*)p_connect);

    process->set_passing_thread(this);
    base_connect<passing_msg_process> * tmp_con = (base_connect<passing_msg_process> *)p_connect;
    tmp_con->set_process(process);

    return p_connect;
}

base_net_obj * passing_msg_thread::get_dest_obj(uint32_t thread_index)
{
    map<uint32_t,base_net_obj* >::iterator it;

    it = _net_obj_map.find(thread_index);
    if (it != _net_obj_map.end()){
        return it->second;
    }
    return NULL;
}

common_thread * passing_msg_thread::get_dest_thread(uint32_t thread_index)
{
    map<uint32_t, common_thread *>::iterator it;

    it = _thread_obj_map.find(thread_index);
    if (it != _thread_obj_map.end()){
        return it->second;
    }
    return NULL;
}

void passing_msg_thread::set_dest_obj(common_thread *thread, base_net_obj * p_obj)
{
    map<uint32_t, common_thread *>::iterator it;
    it = _thread_obj_map.find(thread->get_thread_index());
    if (it != _thread_obj_map.end()){
        return ;
    }

    _thread_obj_map[thread->get_thread_index()] = thread;
    _net_obj_map[thread->get_thread_index()] = p_obj;

    //PDEBUG("_net_obj_map [%d]\n", _net_obj_map.size());
}

void passing_msg_thread::set_dest_channelid(uint32_t thread_index, uint32_t channelid)
{
    _thread_channelid_map[thread_index] = channelid;
}

uint32_t passing_msg_thread::get_dest_channelid(uint32_t thread_index)
{
    map<uint32_t,uint32_t >::iterator it; 
    it = _thread_channelid_map.find(thread_index);
    if (it != _thread_channelid_map.end()){
        return it->second;
    }

    return 0;
}


base_net_container * passing_msg_thread::get_net_container()
{
    return _net_container;
}


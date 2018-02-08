#include "base_net_thread.h"
#include "channel_data_process.h"
#include "base_net_container.h"
#include "common_obj_container.h"
#include "base_net_obj.h"
#include "base_connect.h"

base_net_thread::base_net_thread(int channel_num, uint32_t obj_num):_channel_num(channel_num), _base_container(NULL){
    _base_container = new common_obj_container(this, obj_num);
    net_thread_init();
}

base_net_thread::~base_net_thread(){
    if (_base_container){
        delete _base_container;
    }
}

void * base_net_thread::run()
{
    while (get_run_flag()) {
        _base_container->obj_process();
    }

    return NULL;
}

void base_net_thread::net_thread_init()
{
    for (int i = 0; i < _channel_num; i++) {

        int fd[2];
        int ret = socketpair(AF_UNIX,SOCK_STREAM,0,fd);
        if (ret < 0) {
            return ;
        }   

        event_channel_msg * msg = new event_channel_msg();
        _channel_msg_vec.push_back(msg);
        msg->_channelid = fd[1];

        base_connect<channel_data_process> * channel_connect = new base_connect<channel_data_process>(fd[0]);
        channel_data_process * data_process = new channel_data_process(channel_connect);
        channel_connect->set_process(data_process);
        channel_connect->set_net_container(_base_container);

        msg->_base_obj = channel_connect;
    }


    _base_net_thread_map[get_thread_index()] = this;
}


void base_net_thread::put_msg(ObjId & id, normal_msg * p_msg)
{
    int index = (unsigned long) p_msg % _channel_msg_vec.size();
    event_channel_msg * msg = _channel_msg_vec[index];
    msg->_base_obj->process_recv_msg(id, p_msg);
    write(msg->_channelid, CHANNEL_MSG_TAG, sizeof(CHANNEL_MSG_TAG));
}

void base_net_thread::handle_msg(normal_msg * p_msg)
{
    REC_OBJ<normal_msg> rec(p_msg);
}

base_net_thread * base_net_thread::get_base_net_thread_obj(uint32_t thread_index)
{
    map<uint32_t, base_net_thread *>::iterator it = _base_net_thread_map.find(thread_index);
    if (it != _base_net_thread_map.end()){
        return it->second;
    }

    return NULL;
}

void base_net_thread::add_timer(timer_msg * t_msg)
{
    _base_container->add_timer(t_msg);
}

void base_net_thread::put_obj_msg(ObjId & id, normal_msg * p_msg)
{
    if (!p_msg) {
        return;
    }

    base_net_thread * net_thread = get_base_net_thread_obj(id._thread_index);
    if (!net_thread) {
        REC_OBJ<normal_msg> rec(p_msg); 
        return;
    }

    net_thread->put_msg(id, p_msg);
}

bool base_net_thread::handle_timeout(timer_msg * t_msg)
{
    return false;
}


map<uint32_t, base_net_thread *> base_net_thread::_base_net_thread_map;


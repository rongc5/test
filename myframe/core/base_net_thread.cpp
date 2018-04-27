#include "base_net_thread.h"
#include "channel_data_process.h"
#include "common_obj_container.h"
#include "base_net_obj.h"
#include "base_connect.h"
#include "common_util.h"

base_net_thread::base_net_thread(int channel_num):_channel_num(channel_num), _base_container(NULL){
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
        run_process();
        _base_container->obj_process();
    }

    return NULL;
}

void base_net_thread::run_process()
{

}

void base_net_thread::net_thread_init()
{
    _base_container = new common_obj_container(get_thread_index());

    for (int i = 0; i < _channel_num; i++) {

        int fd[2];
        int ret = socketpair(AF_UNIX,SOCK_STREAM,0,fd);
        if (ret < 0) {
            return ;
        }   

        std::shared_ptr<base_connect<channel_data_process> >  channel_connect(new base_connect<channel_data_process>(fd[0]));
        channel_data_process * data_process = new channel_data_process(channel_connect, fd[1]);
        channel_connect->set_process(data_process);
        channel_connect->set_net_container(_base_container);

        _channel_msg_vec.push_back(channel_connect);
    }


    _base_net_thread_map[get_thread_index()] = this;
}


void base_net_thread::put_msg(uint32_t obj_id, std::shared_ptr<normal_msg> & p_msg)
{
    int index = (unsigned long) (&p_msg) % _channel_msg_vec.size();
    std::shared_ptr<base_connect<channel_data_process> >  channel_connect = _channel_msg_vec[index];
    channel_data_process * data_process = channel_connect->process();
    if (data_process)
    {
        data_process->put_msg(obj_id, p_msg);
    }
}

void base_net_thread::handle_msg(std::shared_ptr<normal_msg> & p_msg)
{

}

base_net_thread * base_net_thread::get_base_net_thread_obj(uint32_t thread_index)
{
    std::unordered_map<uint32_t, base_net_thread *>::const_iterator it = _base_net_thread_map.find(thread_index);
    if (it != _base_net_thread_map.end()){
        return it->second;
    }

    return NULL;
}

void base_net_thread::add_timer(timer_msg & t_msg)
{
    _base_container->add_timer(t_msg);
}

void base_net_thread::put_obj_msg(ObjId & id, std::shared_ptr<normal_msg> & p_msg)
{
    base_net_thread * net_thread = get_base_net_thread_obj(id._thread_index);
    if (!net_thread) {
        return;
    }

    net_thread->put_msg(id._id, p_msg);
}

void base_net_thread::handle_timeout(timer_msg & t_msg)
{
    return ;
}

common_obj_container * base_net_thread::get_net_container()
{
    return _base_container;
}

std::unordered_map<uint32_t, base_net_thread *> base_net_thread::_base_net_thread_map;


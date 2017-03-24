#include "base_net_thread.h"


void * base_net_thread::run()
{
    while (get_run_flag()) {
        _base_container->obj_process();
    }

    LOG_DEBUG("obj_process over!");

    return NULL;
}

void base_net_thread::init()
{
    int fd[2];
    int ret = socketpair(AF_UNIX,SOCK_STREAM,0,fd);
    if (ret < 0) {
        LOG_WARNING("socketpair fail errstr[%s]", strerror(errno));
        return ;
    }   

    _channelid = fd[0];
    set_channelid(fd[1]);
}


void base_net_thread::put_msg(pass_msg * msg)
{
    thread_lock lock(&_mutex);
    _queue.push_back(msg);
    write(_channelid, "c", sizeof("c"));
}

pass_msg* base_net_thread::get_msg()
{
    pass_msg *pMsg = NULL;
    thread_lock lock(&_mutex);
    dItr tmpItr = _queue.begin();
    if (tmpItr != _queue.end())
    {
        pMsg  = *tmpItr;
        _queue.pop_front();
    }              
    return pMsg;
}

void base_net_thread::deal_msg()
{
    thread_lock lock(&_mutex);
    for (dItr it = _queue.begin(); it != _queue.end();){
        switch ((*it)->_p_op) {
            case PASS_NEW_CONNECT:
                {
                    if ((*it)->p_msg) 
                    {
                        NET_OBJ * p_connect = dynamic_cast<NET_OBJ *> ((*it)->p_msg);
                        if (p_connect) {
                            p_connect->set_id(gen_id_str());
                            p_connect->set_net_container(_base_container);
                        }
                    }
                    delete *it;
                }
                break;
        }

        it = _queue.erase(it);
    }

    _queue.clear();
}


const ObjId & base_net_thread::gen_id_str()
{
    uint32_t obj_id = _id_str._id;
    uint32_t thread_index = get_thread_index();
    _id_str.thread_index = thread_index;
    obj_id++;
    _id_str._id = obj_id;
    return _id_str;
}

void base_net_thread::set_channelid(int fd)
{
    base_connect<channel_data_process> * p_connect = new base_connect< channel_data_process>(fd, EPOLL_LT_TYPE);

    p_connect->set_id(gen_id_str());
    channel_data_process * process = new channel_data_process(p_connect);
    process->set_base_net_thread(this);
    p_connect->set_process(process);
    p_connect->set_net_container(_base_container);
}


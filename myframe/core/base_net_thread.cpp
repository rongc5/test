#include "base_net_thread.h"
#include "channel_data_process.h"


void * base_net_thread::run()
{
    init();

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
    LOG_DEBUG("fd[%d] fd[%d]\n", fd[0], fd[1]);
    _base_net_thread_map[get_thread_index()] = this;
}


void base_net_thread::put_msg(normal_obj_msg * msg)
{
    LOG_DEBUG("_channelid[%d]", _channelid);
    thread_lock lock(&_base_net_mutex);
    _queue.push_back(msg);
    write(_channelid, "c", sizeof("c"));
    LOG_DEBUG("_channelid[%d]", _channelid);
}

void base_net_thread::routine_msg()
{
    thread_lock lock(&_base_net_mutex);
    for (dItr it = _queue.begin(); it != _queue.end();){
        switch ((*it)->_p_op) {
            case PASS_NEW_CONNECT:
                {
                    handle_new_connect(*it);
                }
                break;
            case PASS_NEW_FD:
                {
                    LOG_DEBUG("PASS_NEW_FD");
                    handle_new_fd(*it);
                }
                break;
            case PASS_NEW_MSG:
                {
                    handle_new_msg(*it);
                }
                break;

            default:
                REC_OBJ<pass_msg> rec(*it);
        }

        it = _queue.erase(it);
    }

    _queue.clear();
}

void base_net_thread::handle_new_fd(pass_msg * p_msg)
{
    REC_OBJ<pass_msg> rec(p_msg);
}

void base_net_thread::handle_new_connect(pass_msg * p_msg)
{
    REC_OBJ<pass_msg> rec(p_msg);
    if (p_msg->p_msg) 
    {
        NET_OBJ * p_connect = dynamic_cast<NET_OBJ *> (p_msg->p_msg);
        if (p_connect) {
            p_connect->set_id(gen_id_str());
            p_connect->set_net_container(_base_container);
        }
    }
}

void base_net_thread::handle_new_msg(pass_msg * p_msg)
{
    _base_container->put_msg(p_msg);
}


void base_net_thread::set_channelid(int fd)
{
    base_connect<channel_data_process> * p_connect = new base_connect< channel_data_process>(fd, EPOLL_LT_TYPE);

    p_connect->set_id(gen_id_str());
    channel_data_process * process = new channel_data_process(p_connect);
    p_connect->set_process(process);
    p_connect->set_net_container(_base_container);
}

base_net_thread * base_net_thread::get_base_net_thread_obj(uint32_t thread_index)
{
    bntMapIter it = _base_net_thread_map.find(thread_index);
    if (it != _base_net_thread_map.end()){
        return it->second;
    }

    return NULL;
}

void base_net_thread::put_obj_msg(normal_obj_msg * p_msg)
{
    if (!p_msg) {
        return;
    }

    base_net_thread * net_thread = get_base_net_thread_obj(p_msg->_dest_id._thread_index);
    LOG_DEBUG("_thread_index[%d]", p_msg->_dest_id._thread_index);
    if (!net_thread) {
        REC_OBJ<normal_obj_msg> rec(p_msg); 
        return;
    }

    net_thread->put_msg(p_msg);
}

map<uint32_t, base_net_thread *> base_net_thread::_base_net_thread_map;


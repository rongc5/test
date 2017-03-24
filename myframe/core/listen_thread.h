#ifndef __LISTEN_THREAD_H__
#define __LISTEN_THREAD_H__

#include "base_net_thread.h"
#include "base_net_container.h"
#include "common_obj_container.h"
#include "listen_connect.h"
#include "common_def.h"
#include "listen_data_process.h"


template<class MSG_PROCESS>
class listen_thread: public base_net_thread
{
    public:
        listen_thread():_listen_connect(NULL), _current_indx(0){
        };
        virtual ~listen_thread(){
        };

        void init(const string &ip, unsigned short port)
        {
            _ip = ip;
            _port = port;
            _listen_connect = new listen_connect<listen_data_process>();
            listen_data_process * data_process = new listen_data_process(_listen_connect);
            _listen_connect->set_id(gen_id_str());
            _listen_connect->init(ip, port);
            data_process->set_thread(this);
            _listen_connect->set_process(data_process);
            _listen_connect->set_net_container(_base_container);
        }

        int add_worker_thread(base_net_thread * thread)
        {
            _worker_thrds.push_back(thread);

            return 0;
        }
        
        virtual void put_msg(pass_msg * p_msg)
        {
            REC_OBJ<pass_msg> rec(p_msg);
            if (!p_msg) {
                return ;
            }

            if (p_msg->_p_op == PASS_NEW_FD) {
                recv_msg_fd * r_msg = dynamic_cast<recv_msg_fd *> (p_msg->p_msg);

                NET_OBJ *p_connect = gen_connect(r_msg->fd);

                if (!_worker_thrds.size()) {
                    LOG_DEBUG("recv_fd: %d\n", r_msg->fd);
                    p_connect->set_id(gen_id_str());
                    p_connect->set_net_container(_base_container);
                    return ;
                }

                uint32_t index = _current_indx;
                _current_indx++;

                if (_current_indx >= _worker_thrds.size()){
                    _current_indx = 0;
                }

                pass_msg * p_msg = new pass_msg();

                p_msg->p_msg = p_connect;
                p_msg->_obj_id = _listen_connect->get_id();
                p_msg->_flag = 0;
                p_msg->_p_op = PASS_NEW_CONNECT;

                _worker_thrds[index]->put_msg(p_msg);
            }
        }

    protected:
        const ObjId & gen_id_str()
        {
            uint32_t thread_index = get_thread_index();
            _id_str.thread_index = thread_index;
            uint32_t  obj_id = _id_str._id;
            obj_id++;
            _id_str._id = obj_id;
            return _id_str;
        }

        NET_OBJ * gen_connect(int fd)
        {
            base_connect<MSG_PROCESS> * p_connect = new base_connect<MSG_PROCESS>(fd, EPOLL_LT_TYPE);

            MSG_PROCESS * process = new MSG_PROCESS(p_connect);
            process->set_base_net_thread(this);
            p_connect->set_process(process);

            return p_connect;
        }


    protected:

        string _ip;
        unsigned short _port;
        listen_connect<listen_data_process> *_listen_connect;
        ObjId _id_str;
        vector<base_net_thread * > _worker_thrds;
        uint32_t _current_indx;
};

#endif

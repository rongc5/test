#ifndef __LISTEN_THREAD_H__
#define __LISTEN_THREAD_H__

#include "base_net_thread.h"
#include "base_net_container.h"
#include "common_obj_container.h"
#include "listen_connect.h"
#include "common_def.h"
#include "listen_data_process.h"


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


            struct sockaddr_in address; 
            int reuse_addr = 1;  

            memset((char *) &address, 0, sizeof(address)); 
            address.sin_family = AF_INET;
            address.sin_port = htons(port); 
            int ret = 0;         

            if (ip != "")        
            {
                inet_aton(ip.c_str(), &address.sin_addr);
            }
            else
            {
                address.sin_addr.s_addr = htonl(INADDR_ANY); 
            }

            int fd = socket(AF_INET, SOCK_STREAM, 0);
            if (fd < 0)         
            {
                THROW_COMMON_EXCEPT("socket error " << strerror(errno));     
            }
            setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, (void*)(&(reuse_addr)), sizeof(reuse_addr)); 

            if (::bind(fd, (struct sockaddr *) &address, sizeof(address)) < 0) 
            {        
                THROW_COMMON_EXCEPT("bind error "  << strerror(errno) << " " << ip << ":" << port);
            }        

            ret = listen(fd, 250);
            if (ret == -1)
            {
                THROW_COMMON_EXCEPT("listen error "  << strerror(errno));
            }

            set_unblock(fd); 
            base_listen_connect * listen_connect = new base_listen_connect(fd);
            listen_data_process * data_process = new listen_data_process(_listen_connect);
            _listen_connect->set_process(data_process);
            _listen_connect->set_net_container(_base_container);
        }

        int add_worker_thread(base_net_thread * thread)
        {
            _worker_thrds.push_back(thread);

            return 0;
        }

        virtual void handle_new_fd(pass_msg * p_msg)
        {
            REC_OBJ<pass_msg> rec(p_msg);
            if (!p_msg) {
                return ;
            }

            recv_msg_fd * r_msg = dynamic_cast<recv_msg_fd *> (p_msg->p_msg);

            NET_OBJ *p_connect = gen_connect(r_msg->fd);

            if (!_worker_thrds.size()) {
                LOG_DEBUG("recv_fd: %d\n", r_msg->fd);
                p_connect->set_id(gen_id_str());
                p_connect->set_net_container(_base_container);
                return ;
            }


            {
                uint32_t index = _current_indx;
                _current_indx++;

                if (_current_indx >= _worker_thrds.size()){
                    _current_indx = 0;
                }

                pass_msg * p_msg = new pass_msg();

                p_msg->p_msg = p_connect;
                p_msg->_src_id = _listen_connect->get_id();
                p_msg->_dest_id._thread_index = _worker_thrds[index]->get_thread_index();
                p_msg->_flag = 0;
                p_msg->_p_op = PASS_NEW_CONNECT;

                _worker_thrds[index]->put_msg(p_msg);
            }
        }

    protected:


    protected:

        string _ip;
        unsigned short _port;
        base_listen_connect *_listen_connect;
        vector<base_net_thread * > _worker_thrds;
        uint32_t _current_indx;
};

#endif

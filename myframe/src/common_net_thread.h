#ifndef __COMMON_NET_THREAD_H__
#define __COMMON_NET_THREAD_H__

#include "base_def.h"
#include "channel_msg_process.h"
#include "common_thread.h"
#include "channel_data_process.h"
#include "common_obj_container.h"
#include "net_obj.h"


namespace MZFRAME {

    template<class MSG_PROCESS>
    class common_net_thread:public common_thread
    {
        public:
            common_net_thread():_base_container(NULL){
                _base_container = new common_obj_container();
            };
            virtual ~common_net_thread(){
                if (_base_container){
                    delete _base_container;
                }
            };

            virtual void obj_process()
            {
                _base_container->obj_process();
            }

            virtual void set_channelid(int fd)
            {
                common_thread::set_channelid(fd);
                
                base_connect<channel_msg_process<channel_data_process> > * p_connect = new base_connect<channel_msg_process<channel_data_process> >(fd, EPOLL_LT_TYPE);
                
                p_connect->set_id(gen_id_str());
                channel_msg_process<channel_data_process> * process = new channel_msg_process<channel_data_process>(p_connect);
                process->set_common_thread(this);
                p_connect->set_process(process);
                p_connect->set_net_container(_base_container);
            }


            NET_OBJ * gen_connect(const int fd)
            {
                
                base_connect<MSG_PROCESS> * p_connect = new base_connect<MSG_PROCESS>();
                
                p_connect->set_id(gen_id_str());
                MSG_PROCESS * process = new MSG_PROCESS(p_connect);
                process->set_common_thread(this);
                p_connect->set_process(process);
                p_connect->set_net_container(_base_container);

                return p_connect;
            }

        protected:
            const obj_id_str & gen_id_str()
            {
                _id_str._thread_id = get_thread_id();
                _id_str._obj_id++;
                return _id_str;
            }

        protected:

            base_net_container * _base_container;
            obj_id_str _id_str;
    };

}
#endif

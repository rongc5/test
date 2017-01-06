#ifndef __OBJ_THREAD_H__
#define __OBJ_THREAD_H__

#include "base_def.h"


namespace MZFRAME {

    template<class MSG_PROCESS>
    class obj_thread : public common_thread_obj
    {
        public:
            obj_thread():_base_container(NULL){};
            virtual ~obj_thread(){};

            virtual void* run()
            {
                while (get_run_flag()) {
                    real_run();
                }
            }
            virtual void real_run() = 0;

            NET_OBJ * gen_connect(const int fd) = 0;

            obj_id_str & gen_id_str() = 0;

        protected:
            obj_id_str _id_str;
    };

}
#endif

#ifndef __SKHTTP_RES_THREAD_H_
#define __SKHTTP_RES_THREAD_H_

#include "base_net_thread.h"
#include "common_util.h"
#include "log_helper.h"
#include "base_connect.h"
#include "skhttp_res_data_process.h"

class skhttp_res_thread:public base_net_thread
{
    public:
        skhttp_res_thread();

        void reg_for_date();

        virtual void handle_msg(std::shared_ptr<normal_msg> & p_msg);

        //virtual void run_process();
        //获取日期时间， 检验当前日期用户是否过期, 为了避免重复获取， 故仅仅由req thread 获取后即可
};




#endif

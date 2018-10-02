#include "common_util.h"
#include "base_net_thread.h"

#define TIMER_TYPE_RELOAD_CONF 10000

class test_thread:public base_net_thread
{
    public:
        test_thread():count(0){}

        virtual void handle_timeout(std::shared_ptr<timer_msg> & t_msg)
        {
            if (t_msg->_timer_type == TIMER_TYPE_RELOAD_CONF)
            {
                PDEBUG("count:%d", count);
                if (count > 10000)
                    exit(0);
                reload_timer_start();
            }
        }

        virtual void run_process()
        {
            if (!count)
            {
                reload_timer_start();
            }
        }

        void reload_timer_start()
        {
            std::shared_ptr<timer_msg> t_msg(new timer_msg);

            t_msg->_timer_type = TIMER_TYPE_RELOAD_CONF;
            t_msg->_time_length = 50;
            t_msg->_obj_id = OBJ_ID_THREAD;
            add_timer(t_msg);
            count++;
            PDEBUG("count:%d", count);
        }

    private:
        int count;

};

int main(int argc, char *argv[])
{
    test_thread * master_thread = new test_thread();
    master_thread->start();

    base_thread::join_all_thread();
    
    return 0;
}



#include "listen_thread.h"
#include "base_def.h"
#include "log_helper.h"
#include "base_net_thread.h"
#include "sah_data_process.h"


int main(int argc, char *argv[])
{
    log_conf conf;

    //conf.type = LOGWARNING;
    LOG_INIT(conf);

    listen_thread<sah_data_process> * lthread = new listen_thread<sah_data_process>();
    lthread->init("0.0.0.0", 9090);

    for (int i = 0; i<= 3; i++) {
        base_net_thread * net_thread = new base_net_thread;
        lthread->add_worker_thread(net_thread->get_thread_index());
        net_thread->start();
    }

    lthread->start();

    base_thread::join_all_thread();

    return 0;
}

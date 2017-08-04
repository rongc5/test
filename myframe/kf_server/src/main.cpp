#include "listen_thread.h"
#include "base_def.h"
#include "log_helper.h"
#include "kf_net_thread.h"


int main(int argc, char *argv[])
{
    log_conf conf;
    conf.deal_mode = 3;

    LOG_INIT(conf);

    listen_thread * lthread = new listen_thread();

    for (int i = 0; i<= 3; i++) {
        kf_net_thread * net_thread = new kf_net_thread;
        lthread->add_worker_thread(net_thread->get_thread_index());
        net_thread->start();
    }

    lthread->start();

    return 0;
}

#include "base_thread.h"
#include "ws_ser_thread.h"
#include "base_singleton.h"
#include "listen_thread.h"
#include "log_helper.h"


int main(int argc, char *argv[])
{
    log_conf conf;
    conf.deal_mode = 3;
    //strcpy(conf.prefix_file_name, "asp");

    LOG_INIT(conf); 

    listen_thread * _listen_obj_thread = new listen_thread();
    _listen_obj_thread->init("10.135.136.80", 8080);

    for (int i=1; i <= 3; i++){
        ws_ser_thread * net_thread = new ws_ser_thread();
        _listen_obj_thread->add_worker_thread(net_thread);
        net_thread->start();
    }

    sleep(1);

    _listen_obj_thread->start();

    base_thread::join_all_thread();
    
    return 0;
}

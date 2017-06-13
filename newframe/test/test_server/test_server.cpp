#include "base_thread.h"
#include "job_thread.h"
#include "base_singleton.h"
#include "listen_thread.h"
#include "log_helper.h"
#include "http_client_thread.h"


int main(int argc, char *argv[])
{
    log_conf conf;
    conf.deal_mode = 3;
    //strcpy(conf.prefix_file_name, "asp");

    LOG_INIT(conf); 

    listen_thread * _listen_obj_thread = new listen_thread();
    _listen_obj_thread->init("", 8888);

    for (int i=1; i <= 3; i++){
        job_thread * net_thread = new job_thread();
        net_thread->start();
    }

    for (int i=1; i <= 3; i++){
        http_client_thread * http_thread = new http_client_thread();
        http_thread->start();
    }

    sleep(1);

    _listen_obj_thread->start();

    base_thread::join_all_thread();
    
    return 0;
}

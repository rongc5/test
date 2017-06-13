#include "common_def.h"
#include "client_thread.h"
#include "log_helper.h"
#include "base_thread.h"



int main(int c, char **v)
{
    log_conf conf;
    conf.deal_mode = 3;
    //strcpy(conf.prefix_file_name, "asp");
    LOG_INIT(conf); 

    LOG_DEBUG("c_thread");
    client_thread * c_thread = new client_thread();
    c_thread->init("127.0.0.1", 8888);
    c_thread->start();

    base_thread::join_all_thread();

    return 0;
}


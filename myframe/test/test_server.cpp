#include "server_mgr.h"
#include "test_data_process.h"
#include "base_thread.h"
#include "base_net_thread.h"
#include "test_net_thread.h"
#include "base_singleton.h"


int main(int argc, char *argv[])
{
    log_conf conf;
    conf.deal_mode = 3;
    //strcpy(conf.prefix_file_name, "asp");

    LOG_INIT(conf); 

    test_net_thread * t_thread = new test_net_thread();

    t_thread->start();

    base_singleton<test_net_thread>::set_instance(t_thread);

    server_mgr<test_data_process> * server = new server_mgr<test_data_process>("", 8888);

    server->run();
    
    for (int i=1; i <= 6; i++){
        base_net_thread * net_thread = new base_net_thread();
        server->add_worker_thread(net_thread);
    }

    base_thread::join_all_thread();
    
    return 0;
}

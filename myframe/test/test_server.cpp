#include "server_mgr.h"
#include "common_msg_process.h"
#include "base_data_process.h"
#include "base_thread.h"
#include "base_singleton.h"


int main(int argc, char *argv[])
{
    log_conf conf;
    conf.deal_mode = 3;
    //strcpy(conf.prefix_file_name, "asp");

    LOG_INIT(conf); 

    server_mgr<common_msg_process<base_data_process> > * server = new server_mgr<common_msg_process<base_data_process> >(2, "", 8888);

    server->run();
    
    base_thread::join_all_thread();
    
    return 0;
}

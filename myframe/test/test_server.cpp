#include "server_mgr.h"
#include "common_msg_process.h"
#include "base_data_process.h"
#include "base_thread.h"


int main(int argc, char *argv[])
{

    base_singleton<log_mgr>::set_instance(new log_mgr());
    base_singleton<log_mgr>::get_instance()->init(LOGDEBUG,"test",0, 10000);


    server_mgr<common_msg_process<base_data_process> > * server = new server_mgr<common_msg_process<base_data_process> >(1, "", 8888);

    server->run();
    
    base_thread::join_all_thread();
    
    return 0;
}

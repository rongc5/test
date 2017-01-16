#include "../src/server_mgr.h"
#include "../src/common_msg_process.h"
#include "../src/base_data_process.h"


int main(int argc, char *argv[])
{

    server_mgr<common_msg_process<base_data_process> > * server = new server_mgr<common_msg_process<base_data_process> >(10, "", 0);

    server->run();
    
    return 0;
}

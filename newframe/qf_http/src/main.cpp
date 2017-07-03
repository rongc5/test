#include "base_thread.h"
#include "base_singleton.h"
#include "log_helper.h"
#include "http_server_thread.h"
#include "qf_msg_def.h"


int main(int argc, char *argv[])
{
    log_conf conf;
    conf.deal_mode = 3;
    //strcpy(conf.prefix_file_name, "asp");

    LOG_INIT(conf); 

    qf_msg_mgr<sale_msg> * to_visitor = new qf_msg_mgr<sale_msg>();
    to_visitor->init();
    base_singleton<qf_msg_mgr<sale_msg> >::set_instance(to_visitor);


    qf_msg_mgr<user_msg> * to_sale = new qf_msg_mgr<user_msg>();
    to_sale->init();
    base_singleton<qf_msg_mgr<user_msg> >::set_instance(to_sale);

    int nfd = http_server_thread::bind_port(8888);
    for (uint32_t i=1; i <= 10; i++){
        http_server_thread * net_thread = new http_server_thread();
        net_thread->set_nfd(nfd);
        net_thread->start();
    }


    base_thread::join_all_thread();
    
    return 0;
}

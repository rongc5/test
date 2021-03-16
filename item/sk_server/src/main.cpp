#include "base_def.h"
#include "log_helper.h"
#include "skhttp_req_thread.h"
#include "reload_thread.h"
#include "sk_conf.h"
#include "proc_data.h"
#include "listen_thread.h"
#include "common_util.h"
#include "skhttp_res_thread.h"
#include "sk_util.h"
#include "worker_thread.h"


void do_init()
{
    char conf_path[SIZE_LEN_128];
    char proc_name[SIZE_LEN_128];
    get_proc_name(proc_name, sizeof(proc_name));
    
    snprintf(conf_path, sizeof(conf_path), "./conf/%s.conf", proc_name);

    log_conf lg_conf;
    sk_conf * conf = new (std::nothrow)sk_conf(conf_path);
    conf->load();

    lg_conf.type = conf->log_type;
    LOG_INIT(lg_conf);

    proc_data::instance()->init(conf);
    proc_data::instance()->load();

    for (uint32_t i = 0; i<= conf->worker_thread_num; i++)
    {
        auto * net_thread = new worker_thread();
        net_thread->start();
        proc_data::instance()->add_name_thread("worker_thread", net_thread);
    }

    reload_thread * master_thread = new reload_thread();
    skhttp_req_thread * req_thread = new skhttp_req_thread();

    proc_data::instance()->add_name_thread("reload_thread", master_thread);
    proc_data::instance()->add_name_thread("req_thread", req_thread);

    master_thread->start();
    req_thread->start();

    listen_thread *lthread = new listen_thread();
    lthread->init(conf->http_server_host, conf->http_server_port);

    for (uint32_t i = 0; i<= conf->http_server_thread_num; i++)
    {
        skhttp_res_thread * net_thread = new skhttp_res_thread();
        lthread->add_worker_thread(net_thread->get_thread_index());
        net_thread->start();
        proc_data::instance()->add_name_thread("res_thread", net_thread);
    }


    lthread->start();

    base_thread::join_all_thread();
}


int main(int argc, char *argv[])
{
    signal_process<default_signal_process> sp;
    sp();

    do_init();


    //http_req_msg * req_msg = new http_req_msg();
    //ObjId  id;
    
    //req_msg->sid = "123456789";
    //req_msg->url = "http://web.sqt.gtimg.cn/q=sz300699";
    //req_msg->headers.insert(make_pair("User-Agent", "Mozilla/5.0 (Linux; Android 6.0; Nexus 5 Build/MRA58N) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/63.0.3239.132 Mobile Safari/537.36"));
    //id._thread_index = req_thread->get_thread_index();

    //req_thread->put_msg(id, req_msg);


    return 0;
}

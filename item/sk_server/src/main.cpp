#include "base_def.h"
#include "log_helper.h"
#include "skhttp_req_thread.h"
#include "realod_thread.h"
#include "sk_conf.h"
#include "proc_data.h"
#include "listen_thread.h"
#include "listen_thread.h"


void do_init(char *argv)
{
    log_conf lg_conf;
    sk_conf * conf = new (std::nothrow)sk_conf(argv);
    conf->load();

    lg_conf.type = conf->log_type;
    LOG_INIT(conf);

    proc_data::instance()->init(conf);
    proc_data::instance()->load();

    reload_thread * master_thread = new reload_thread();
    skhttp_req_thread * req_thread = new skhttp_req_thread();

    proc_data::instance()->_http_req_vec.push_back(req_thread);

    master_thread->start();
    req_thread->start();


    base_thread::join_all_thread();
}


int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        return -1;
    }

    do_init(argv);




    //http_req_msg * req_msg = new http_req_msg();
    //ObjId  id;
    
    //req_msg->sid = "123456789";
    //req_msg->url = "http://web.sqt.gtimg.cn/q=sz300699";
    //req_msg->headers.insert(make_pair("User-Agent", "Mozilla/5.0 (Linux; Android 6.0; Nexus 5 Build/MRA58N) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/63.0.3239.132 Mobile Safari/537.36"));
    //id._thread_index = req_thread->get_thread_index();

    //req_thread->put_msg(id, req_msg);


    return 0;
}

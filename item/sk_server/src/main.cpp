#include "base_def.h"
#include "log_helper.h"
#include "skhttp_req_thread.h"


int main(int argc, char *argv[])
{
    log_conf conf;

    //conf.type = LOGWARNING;
    LOG_INIT(conf);


    skhttp_master_thread * master_thread = new skhttp_master_thread();
    master_thread->start();

    for (int i = 0; i < 3; i++)
    {
        skhttp_req_thread * req_thread = new skhttp_req_thread();
        master_thread->add_worker_thread(req_thread->get_thread_index());
        req_thread->start();
    }


    http_req_msg * req_msg = new http_req_msg();
    ObjId  id;
    
    req_msg->sid = "123456789";
    req_msg->url = "http://web.sqt.gtimg.cn/q=sz300699";
    req_msg->headers.insert(make_pair("User-Agent", "Mozilla/5.0 (Linux; Android 6.0; Nexus 5 Build/MRA58N) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/63.0.3239.132 Mobile Safari/537.36"));
    id._thread_index = req_thread->get_thread_index();

    req_thread->put_msg(id, req_msg);

    base_thread::join_all_thread();

    return 0;
}

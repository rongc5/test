#include "base_thread.h"
#include "ws_ser_thread.h"
#include "base_singleton.h"
#include "listen_thread.h"
#include "log_helper.h"
#include "http_server_thread.h"
#include <google/protobuf/text_format.h>
#include <google/protobuf/io/zero_copy_stream_impl.h>
#include "ws_ser.pb.h"
#include "qfws_msg_process.h"

void load_config(WsConf & ws_c)
{
    char buf[SIZE_LEN_1024] = {'\0'};
    get_proc_name(buf, sizeof(buf));
    
    char tmp[SIZE_LEN_1024];
    snprintf(tmp, sizeof(tmp), "%s/%s.conf", "./conf/", buf);
    int fd = open(tmp, O_RDONLY);
    if (fd < 0) {
        LOG_WARNING("open %s failed: %s", tmp, strerror(errno));
        exit(1);
    }

    google::protobuf::io::FileInputStream fileInput(fd);
    fileInput.SetCloseOnDelete( true );
    if (!google::protobuf::TextFormat::Parse(&fileInput, &ws_c)){
        LOG_WARNING("Parse %s failed: %s", tmp, strerror(errno));
        exit(1);
    }

    LOG_DEBUG("parse %s ok:\n"
            "webs_port:%d \n"
            "webs_thread_num:%d\n"
            "http_port:%d\n"
            "http_thread_num:%d\n", tmp, ws_c.webs_port(), ws_c.webs_thread_num(),
            ws_c.http_port(), ws_c.http_thread_num());
}

int main(int argc, char *argv[])
{
    log_conf conf;
    conf.deal_mode = 3;
    //strcpy(conf.prefix_file_name, "asp");

    LOG_INIT(conf); 

    WsConf ws_c;
    load_config(ws_c);

    login_groupid_userid_mgr * login_groupid_mgr = new login_groupid_userid_mgr();
    login_groupid_mgr->init();
    base_singleton<login_groupid_userid_mgr>::set_instance(login_groupid_mgr);

    listen_thread * _listen_obj_thread = new listen_thread();
    _listen_obj_thread->init("0.0.0.0", ws_c.webs_port());

    for (int i=1; i <= ws_c.webs_thread_num(); i++){
        ws_ser_thread * net_thread = new ws_ser_thread();
        _listen_obj_thread->add_worker_thread(net_thread);
        net_thread->start();
    }

    sleep(1);
    _listen_obj_thread->start();

    int nfd = http_server_thread::bind_port(ws_c.http_port());
     for (int i=1; i <= ws_c.http_thread_num(); i++){
         http_server_thread * net_thread = new http_server_thread();
         net_thread->set_nfd(nfd);
         net_thread->start();
     }


    base_thread::join_all_thread();
    
    return 0;
}

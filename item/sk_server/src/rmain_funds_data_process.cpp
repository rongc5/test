#include "rmain_funds_data_process.h"
#include "http_base_process.h"
#include "http_base_data_process.h"
#include "log_helper.h"
#include "out_connect.h"
#include "common_obj_container.h"
#include "base_net_thread.h"

#include "base_def.h"
#include "http_req_process.h"
#include "proc_data.h"
#include "common_domain.h"
#include "sk_conf.h"
#include "strategy_conf.h"
#include "id_dict.h"
#include "history_main_funds_dict.h"
//#include "history_wmain_funds_dict.h"

#include "hquotation_search_index.h"



rmain_funds_data_process::rmain_funds_data_process(http_base_process * _p_process):http_base_data_process(_p_process)
{
    _is_ok = false;
}

std::string * rmain_funds_data_process::get_send_body(int &result)
{
    result = 1;

    return NULL;
}


url_info & rmain_funds_data_process::get_url_info()
{
    return _url_info;
}

void rmain_funds_data_process::header_recv_finish()
{
    //base_net_container * net_container = get_base_net()->get_net_container();
    //http_res_head_para & res_head_para = _base_process->get_res_head_para();
    //std::string * str = new std::string;
    //res_head_para.to_head_str(str);
    //LOG_DEBUG("header: %s", str->c_str());

    //std::shared_ptr<std::string> rc(str);
    _is_ok = true;
}


int rmain_funds_data_process::parse_main_funds1(std::string id, std::string _recv_buf)
{
    LOG_DEBUG("id:%s _recv_buf: %s", id.c_str(), _recv_buf.c_str());
    proc_data * p_data = proc_data::instance();

    std::shared_ptr<main_funds_t> tmp_main_funds(new main_funds_t);

    int ttmp;
    std::vector<std::string> ssVec;
    uint32_t i = 0;
    bool flag = false;
    
    std::vector<std::string> strVec;
    SplitString(_recv_buf.c_str(), "=", &strVec, SPLIT_MODE_ONE);
    if (strVec.size() < 2)
    {
        _recv_buf.clear();
        return -1;
    }

    _recv_buf.clear();


    //不是标准json
    
    SplitString(strVec[1].c_str(), "~", &ssVec, SPLIT_MODE_ALL);

    if (ssVec.size() >= 10)
    {
        tmp_main_funds->force_in = atoi(ssVec[1].c_str());
        tmp_main_funds->force_out = atoi(ssVec[2].c_str());
        tmp_main_funds->force_diff = atoi(ssVec[3].c_str());
        tmp_main_funds->force_dratio = atoi(ssVec[4].c_str());
        tmp_main_funds->small_in = atoi(ssVec[5].c_str());
        tmp_main_funds->small_out = atoi(ssVec[6].c_str());
        tmp_main_funds->small_diff = atoi(ssVec[7].c_str());
        tmp_main_funds->small_dratio = atoi(ssVec[8].c_str());
        tmp_main_funds->all = atoi(ssVec[9].c_str());

        flag = true;
    }

    if (flag)
    {
        gen_response_req_msg(id, tmp_main_funds);
    }

    return 0;
}


int rmain_funds_data_process::parse_main_funds2(std::string id, std::string _recv_buf)
{
    if (_recv_buf.empty()) {
        LOG_WARNING("id:%s _recv_buf: %s", id.c_str(), _recv_buf.c_str());
        return -1;
    }
    proc_data * p_data = proc_data::instance();

    std::shared_ptr<main_funds_t> tmp_main_funds(new main_funds_t);

    int ttmp;
    std::vector<std::string> ssVec;
    uint32_t i = 0;
    bool flag = false;


    Document document;
    if (document.Parse(_recv_buf.c_str()).HasParseError()) {
        LOG_WARNING("id:%s parse _recv_buf: %s size:%d, failed", id.c_str(), _recv_buf.c_str(), _recv_buf.size());
        return -1;
    }

    if (!document.HasMember("code") || !document["code"].IsInt() || document["code"].GetInt()) {
        LOG_WARNING("id:%s parse _recv_buf: %s size:%d, failed", id.c_str(), _recv_buf.c_str(), _recv_buf.size());
        return -1;
    }  

    if (!document.HasMember("data") || !document["data"].IsObject()) {
        LOG_WARNING("id:%s parse _recv_buf: %s size:%d, failed", id.c_str(), _recv_buf.c_str(), _recv_buf.size());
        return -1;
    }

    const Value& data = document["data"];
    
    if (!data.HasMember("todayFundFlow") || !data["todayFundFlow"].IsObject()) {
        LOG_WARNING("id:%s parse _recv_buf: %s size:%d, failed", id.c_str(), _recv_buf.c_str(), _recv_buf.size());
        return -1;
    }

    const Value& fundFlow = data["todayFundFlow"];

    if (!fundFlow.HasMember("mainNetIn") || !fundFlow["mainNetIn"].IsString()) {
        LOG_WARNING("id:%s parse _recv_buf: %s size:%d, failed", id.c_str(), _recv_buf.c_str(), _recv_buf.size());
        return -1;
    }


    tmp_main_funds->force_diff = atoi(fundFlow["mainNetIn"].GetString())/10000;
    flag = true;

    _recv_buf.clear();


    if (flag)
    {
        gen_response_req_msg(id, tmp_main_funds);
    }

    return 0;
}



void rmain_funds_data_process::msg_recv_finish()
{
    LOG_DEBUG("_id: %s recv_buf: %s", _id.c_str(), _recv_buf.c_str());
    //FILE_WRITE("123", _recv_buf.c_str());
    parse_main_funds2(_id, _recv_buf);
    _recv_buf.clear();

over:
    throw CMyCommonException("msg_recv_finish");
}


std::string * rmain_funds_data_process::get_send_head()
{
    http_req_head_para & req_head = _base_process->get_req_head_para();


    std::string * str = new std::string;
    req_head.to_head_str(str);
    LOG_DEBUG("%s", str->c_str());

    return str;
}

size_t rmain_funds_data_process::process_recv_body(const char *buf, size_t len, int& result)
{
    result = 1;

    _recv_buf.append(buf, len);

    //LOG_DEBUG("recv_buf: %s", _recv_buf.c_str());
    return len;
}

void rmain_funds_data_process::set_url_info(url_info & u_info)
{
    _url_info = u_info;
}

void rmain_funds_data_process::set_id(std::string id)
{
    _id = id;
}

int rmain_funds_data_process::load_from_curl(std::string id, common_obj_container * net_container, std::map<std::string, std::string> & headers, curl_req & cur)
{
    proc_data* p_data = proc_data::instance();
    strategy_conf * strategy = p_data->_conf->_strategy->current();
    std::string url = strategy->real_main_funds_api;

    //std::string cmd;


    //std::string path = strategy->req_main_funds_from_file_path;
    //if (!end_with(path, "/"))
        //path.append("/");
    //path.append(id);

    //FILE * fp = NULL;
    //char line[SIZE_LEN_1024];
    //char * ptr = NULL;
    std::string rec_str;

    url.append(id);

    cur.init_url(url, headers, p_data->_conf->_strategy->current()->req_http_timeout);   
    cur.get_data(rec_str);
    parse_main_funds2(id, rec_str);

    //cmd.append("curl ");

    //for (auto ii: headers)
    //{
        //cmd.append(" -H '");
        //cmd.append(ii.first);
        //cmd.append(": ");
        //cmd.append(ii.second);
        //cmd.append("'");
    //}

    //cmd.append(" '");
    //cmd.append(url);
    //cmd.append("'");

    //cmd.append(" > ");
    //cmd.append(path);



    //LOG_DEBUG("cmd: %s", cmd.c_str());

//    exec_shell_cmd(cmd, rec_str);

#if 0
    //system(cmd.c_str());
    //

    fp = fopen(path.c_str(), "r");
    ASSERT_WARNING(fp != NULL,"open file failed. file[%s]", path.c_str());

    while (fgets(line, sizeof(line), fp))
    {
        if('\0' == line[0])
        {
            continue;
        }

        ptr = im_chomp(line); 
        if (ptr == NULL || *ptr == '\0'|| *ptr == '#')
            continue;


        rec_str.assign(ptr);

        parse_main_funds(id, rec_str);
    }

    fclose(fp);
#endif
    //parse_main_funds(id, rec_str);

    gen_destroy_msg(id);

    return 0;
}

void rmain_funds_data_process::gen_net_obj(std::string id, common_obj_container * net_container, std::map<std::string, std::string> & headers)
{
    if (!net_container || id.empty())
        return;

    proc_data* p_data = proc_data::instance();
    strategy_conf * strategy = p_data->_conf->_strategy->current();
    std::string url = strategy->real_main_funds_api;
    url.append(id);

    url_info u_info;
    parse_url(url, u_info);

    std::vector<std::string> VP;
    std::vector<std::string> *vIp = net_container->get_domain()->get_domain(u_info.domain);
    if (!vIp)
    {
        parse_domain(u_info.domain, VP);
        vIp = &VP;
        if (!VP.empty())
        {
            net_container->get_domain()->add_domain(u_info.domain, VP);
        }
    }

    if (!vIp || vIp->empty())
    {
        return;
    }

    srand((unsigned long)vIp);
    int index = rand() % vIp->size();
    u_info.ip = (*vIp)[index];

    std::shared_ptr<out_connect<http_req_process> >  
        connect(new out_connect<http_req_process>(u_info.ip, u_info.port));
    http_req_process * req_process = new http_req_process(connect);
    rmain_funds_data_process * sk_process = new rmain_funds_data_process(req_process);
    req_process->set_process(sk_process);
    connect->set_process(req_process);

    sk_process->set_url_info(u_info);
    sk_process->set_id(id);

    http_req_head_para & req_head = req_process->get_req_head_para();

    req_head._headers.insert(headers.begin(), headers.end());

    req_head._method = "GET";
    req_head._url_path = u_info.full_path;
    req_head._version = "HTTP/1.1";

    req_head._headers.insert(std::make_pair("Host", u_info.domain));
    //req_head._headers.insert(std::make_pair("Accept", "*/*"));

    //req_head._headers["Connection"] = "close";

    connect->connect();
    connect->set_net_container(net_container);

    if (p_data && p_data->_conf && p_data->_conf->_strategy->current()->req_http_timeout)
    {
        std::shared_ptr<timer_msg>  t_msg(new timer_msg);
        t_msg->_time_length = p_data->_conf->_strategy->current()->req_http_timeout;
        t_msg->_timer_type = TIMER_TYPE_HTTP_REQ; 
        t_msg->_obj_id = connect->get_id()._id;
        sk_process->add_timer(t_msg);
        LOG_DEBUG("add_timer TIMER_TYPE_HTTP_REQ");
    }

    return ;
}

void rmain_funds_data_process::handle_timeout(std::shared_ptr<timer_msg> & t_msg)
{
    if (t_msg->_timer_type == TIMER_TYPE_HTTP_REQ)
    {
        LOG_DEBUG("timeout TIMER_TYPE_HTTP_REQ");

        throw CMyCommonException("TIMER_TYPE_HTTP_REQ");
    }
}

void rmain_funds_data_process::destroy()
{
    std::shared_ptr<base_net_obj> connect = get_base_net();

    if (connect)
    {   
        
        common_obj_container * net_container = connect->get_net_container();
        gen_destroy_msg(_id);

        if (!_is_ok) 
        {   
            net_container->get_domain()->delete_domain(_url_info.domain);
        }   
    }
}

void rmain_funds_data_process::gen_destroy_msg(const std::string &sid)
{
        auto id_vec = std::make_shared<std::set<std::string>>();
        id_vec->insert(sid);

        proc_data* p_data = proc_data::instance();
        std::vector<base_net_thread *> * req_thread = p_data->get_thread("req_thread"); 
        if (!req_thread)
            return;

        if (!req_thread->size())
        {   
            return;
        }   
        int index = 0;
        LOG_DEBUG("id_vec.size(): 1");

        std::shared_ptr<destroy_msg>  net_obj(new destroy_msg);
        net_obj->id_vec = id_vec;
        net_obj->_msg_op = NORMAL_MSG_DESTROY_MF;

        ObjId id; 
        id._id = OBJ_ID_THREAD;
        id._thread_index = req_thread->at(index)->get_thread_index();
        std::shared_ptr<normal_msg> ng = std::static_pointer_cast<normal_msg>(net_obj);
        base_net_thread::put_obj_msg(id, ng);
}

void rmain_funds_data_process::gen_response_req_msg(const std::string &sid, std::shared_ptr<main_funds_t> st)
{
    proc_data* p_data = proc_data::instance();
    std::vector<base_net_thread *> * req_thread = p_data->get_thread("req_thread"); 
    if (!req_thread)
        return;

    if (!req_thread->size())
    {   
        return;
    }   
    int index = 0;
    index = index % req_thread->size();
    LOG_DEBUG("id_vec.size(): 1");

    std::shared_ptr<response_req_msg> p_msg = std::make_shared<response_req_msg>(NORMAL_MSG_RESPONSE_MAIN_FUNDS);
    p_msg->_id = sid;
    p_msg->_mft = st;

    ObjId id; 
    id._id = OBJ_ID_THREAD;
    id._thread_index = req_thread->at(index)->get_thread_index();


    std::shared_ptr<normal_msg> ng = std::static_pointer_cast<normal_msg>(p_msg);
    base_net_thread::put_obj_msg(id, ng);
}

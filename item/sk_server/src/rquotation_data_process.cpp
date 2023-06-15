#include "rquotation_data_process.h"
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
#include "id_dict.h"
#include "history_quotation_dict.h"
#include "strategy_conf.h"
//#include "history_wquotation_dict.h"
#include "sk_util.h"

rquotation_data_process::rquotation_data_process(http_base_process * _p_process):http_base_data_process(_p_process)
{
    _is_ok = false;
}

std::string * rquotation_data_process::get_send_body(int &result)
{
    result = 1;

    return NULL;
}


url_info & rquotation_data_process::get_url_info()
{
    return _url_info;
}

void rquotation_data_process::header_recv_finish()
{
    //base_net_container * net_container = get_base_net()->get_net_container();
    //http_res_head_para & res_head_para = _base_process->get_res_head_para();
    //std::string * str = new std::string;
    //res_head_para.to_head_str(str);
    //LOG_DEBUG("header: %s", str->c_str());

    //std::shared_ptr<std::string> rc(str);
    _is_ok = true;
}

int rquotation_data_process::load_from_curl(std::shared_ptr<std::set<std::string>> id_vec, common_obj_container * net_container, std::map<std::string, std::string> & headers, curl_req & cur)
{
    proc_data* p_data = proc_data::instance();
    strategy_conf * strategy = p_data->_conf->_strategy->current();
    std::string url = strategy->real_quotation_api;
    std::vector<std::string> strVec;
    //std::string cmd;

    //std::string path = strategy->req_quotation_from_file_path;
    //if (!end_with(path, "/"))
        //path.append("/");
    //path.append(*(id_vec->begin()));

    //FILE * fp = NULL;
    //char line[SIZE_LEN_1024];
    //char * ptr = NULL;
    std::string rec_str;


    if (id_vec->empty())
        goto over;

    for (auto it = id_vec->begin(); it != id_vec->end();)
    {
        url.append(*it);
        ++it;
        if (it != id_vec->end())
        {
            url.append(",");
        }
    }

    cur.init_url(url, headers, p_data->_conf->_strategy->current()->req_http_timeout, p_data->_conf->_strategy->current()->cafile);
    cur.get_data(rec_str);

    SplitString(rec_str.c_str(), ";", &strVec, SPLIT_MODE_ALL);

    for (auto & it: strVec)
    {   
        int ret = parse_quotation(it);
    }   


#if 0

    cmd.append("curl ");
    
    for (auto ii: headers)
    {
        cmd.append(" -H '");
        cmd.append(ii.first);
        cmd.append(": ");
        cmd.append(ii.second);
        cmd.append("'");
    }

    cmd.append(" '");
    cmd.append(url);
    cmd.append("'");

    cmd.append(" > ");
    cmd.append(path);


    LOG_DEBUG("cmd: %s", cmd.c_str());
    //system(cmd.c_str());
    forksystem(cmd.c_str());

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

        parse_quotation(rec_str);
    }

    fclose(fp);

#endif

over:
    gen_destroy_msg(id_vec);

    return 0;
}

int rquotation_data_process::parse_quotation(std::string rec_str)
{
    LOG_DEBUG("recv_buf: %s", rec_str.c_str());

    char t_buf[SIZE_LEN_512];
    std::shared_ptr<quotation_original> qt(new quotation_original);
    float down_pointer = 0;
    float up_pointer = 0;
    float avg_end = 0;
    float end_avg_end = 0;
    
    std::vector<std::string> strVec;
    SplitString(rec_str.c_str(), "=", &strVec, SPLIT_MODE_ONE);
    if (strVec.size() >= 2)
    {
        std::string value = trim(strVec[1].c_str(), ";\n");
        SplitString(value.c_str(), "~", &strVec, SPLIT_MODE_ALL);
    }

    if (strVec.size() <= 45)
    {
        return -1;
    }

    
    std::string id = strVec[2];
    get_standard_id(id);
    LOG_DEBUG("id : %s", id.c_str());

    proc_data * p_data = proc_data::instance();

    if (id.empty())
    {
        return -1;
    }
    
    snprintf(qt->name, sizeof(qt->name), "%s", strVec[1].c_str());
    qt->start = atof(strVec[5].c_str());
    qt->end = atof(strVec[3].c_str());
    qt->high = atof(strVec[33].c_str());
    qt->low = atof(strVec[34].c_str());
    qt->last_closed = atof(strVec[4].c_str());
    qt->vol = atoi(strVec[36].c_str());
    qt->sell_vol= atoi(strVec[8].c_str());
    qt->buy_vol = atoi(strVec[7].c_str());
    qt->swing = atof(strVec[43].c_str());
    qt->change_rate = atof(strVec[38].c_str());
    qt->range_percent = atof(strVec[32].c_str());
    qt->total_price = atof(strVec[37].c_str());
    qt->quantity_relative_ratio = atof(strVec[49].c_str());
    

    //p_data->_hquoation_dict->update_real_quotation(*p_data->get_trade_date(), id, qt);
    //p_data->_hwquoation_dict->update_real_wquotation(*p_data->get_trade_date(), id, qt);
    gen_response_req_msg(id, qt);

    return 0;
}

void rquotation_data_process::msg_recv_finish()
{
    //FILE_WRITE("123", _recv_buf.c_str());
    std::vector<std::string> strVec;
    SplitString(_recv_buf.c_str(), ";", &strVec, SPLIT_MODE_ALL);

    for (auto & it: strVec)
    {
        int ret = parse_quotation(it);
    }

    _recv_buf.clear();

    //if (ret <0)
     //   throw CMyCommonException("msg_recv_finish");
}

std::string * rquotation_data_process::get_send_head()
{
    http_req_head_para & req_head = _base_process->get_req_head_para();


    std::string * str = new std::string;
    req_head.to_head_str(str);
    LOG_DEBUG("%s", str->c_str());

    return str;
}

size_t rquotation_data_process::process_recv_body(const char *buf, size_t len, int& result)
{
    result = 1;

    _recv_buf.append(buf, len);

    //LOG_DEBUG("recv_buf: %s", _recv_buf.c_str());
    return len;
}

void rquotation_data_process::set_url_info(url_info & u_info)
{
    _url_info = u_info;
}

void rquotation_data_process::set_id(std::shared_ptr<std::set<std::string>> id)
{
    _id_vec = id;
}

void rquotation_data_process::gen_net_obj(std::shared_ptr<std::set<std::string>> id_vec, common_obj_container * net_container, std::map<std::string, std::string> & headers)
{
    if (!net_container || id_vec->empty())
        return;

    proc_data* p_data = proc_data::instance();
    strategy_conf * strategy = p_data->_conf->_strategy->current();
    std::string url = strategy->real_quotation_api;

    for (auto it = id_vec->begin(); it != id_vec->end();)
    {
        url.append(*it);
        ++it;
        if (it != id_vec->end())
        {
            url.append(",");
        }
    }

    std::string refer = "http://gu.qq.com/";
    refer.append(*(id_vec->begin()));
    refer.append("/gp");

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
    rquotation_data_process * sk_process = new rquotation_data_process(req_process);
    req_process->set_process(sk_process);
    connect->set_process(req_process);

    sk_process->set_url_info(u_info);
    sk_process->set_id(id_vec);

    http_req_head_para & req_head = req_process->get_req_head_para();

    req_head._headers.insert(headers.begin(), headers.end());

    req_head._method = "GET";
    req_head._url_path = u_info.full_path;
    req_head._version = "HTTP/1.1";

    req_head._headers.insert(std::make_pair("Host", u_info.domain));
    //req_head._headers.insert(std::make_pair("Accept", "*/*"));

    //req_head._headers["Referer"] = refer;
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

void rquotation_data_process::handle_timeout(std::shared_ptr<timer_msg> & t_msg)
{
    if (t_msg->_timer_type == TIMER_TYPE_HTTP_REQ)
    {
        LOG_DEBUG("timeout TIMER_TYPE_HTTP_REQ:_timer_id %u _timer_type:%u", t_msg->_timer_id, t_msg->_timer_type);

        throw CMyCommonException("TIMER_TYPE_HTTP_REQ");
    }
}


void rquotation_data_process::destroy()
{
    for (auto ii = _id_vec->begin(); ii != _id_vec->end(); ii++)
    {
        LOG_DEBUG("id:%s destroy", ii->c_str());
    }

    std::shared_ptr<base_net_obj> connect = get_base_net();

    if (connect)
    {   

        common_obj_container * net_container = connect->get_net_container();
        gen_destroy_msg(_id_vec);

        if (!_is_ok) 
        {   
            net_container->get_domain()->delete_domain(_url_info.domain);
        }   
    }
}

void rquotation_data_process::gen_destroy_msg(std::shared_ptr<std::set<std::string>> id_vec)
{
    std::shared_ptr<destroy_msg>  net_obj(new destroy_msg);
    net_obj->id_vec = id_vec;
    net_obj->_msg_op = NORMAL_MSG_DESTROY_QT;
    LOG_DEBUG("id_vec.size(): %d", id_vec->size());

    proc_data* p_data = proc_data::instance();
    std::vector<base_net_thread *> * req_thread = p_data->get_thread("req_thread"); 
    if (!req_thread)
        return;

    if (!req_thread->size())
    {   
        return;
    }   
    int index = 0;

    ObjId id; 
    id._id = OBJ_ID_THREAD;
    id._thread_index = req_thread->at(index)->get_thread_index();
    std::shared_ptr<normal_msg> ng = std::static_pointer_cast<normal_msg>(net_obj);

    base_net_thread::put_obj_msg(id, ng);
}

void rquotation_data_process::gen_response_req_msg(const std::string &sid, std::shared_ptr<quotation_original> qt)
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

    std::shared_ptr<response_req_msg> p_msg = std::make_shared<response_req_msg>(NORMAL_MSG_RESPONSE_QUOTATION);
    p_msg->_id = sid;
    p_msg->_qt = qt; 

    ObjId id; 
    id._id = OBJ_ID_THREAD;
    id._thread_index = req_thread->at(index)->get_thread_index();

    std::shared_ptr<normal_msg> ng = std::static_pointer_cast<normal_msg>(p_msg);
    base_net_thread::put_obj_msg(id, ng);
}

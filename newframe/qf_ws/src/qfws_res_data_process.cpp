#include "qfws_res_data_process.h"
#include "log_helper.h"
#include "tcp_connect.h"
#include "ws_res_data_process.h"
#include "qfws_msg_process.h"
#include "rapidjson/document.h"
#include "rapidjson/prettywriter.h"
#include "base_singleton.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/writer.h"

using namespace rapidjson;

qfws_res_data_process::qfws_res_data_process(tcp_connect * t_cn):ws_res_data_process(t_cn), _groupid(0), _userid(0)
{
}

qfws_res_data_process::~qfws_res_data_process()
{
    LOG_DEBUG("destory:_groupid:%d _userid:%d", _groupid, _userid);
    login_groupid_userid_mgr * login_groupid_mgr = base_singleton<login_groupid_userid_mgr>::get_instance();
    login_groupid_mgr->reduce(_groupid, _userid);
}

void qfws_res_data_process::do_login(qf_req_msg & ws_msg)
{
    login_groupid_userid_mgr * login_groupid_mgr = base_singleton<login_groupid_userid_mgr>::get_instance();

    login_groupid_mgr->add(ws_msg._groupid, ws_msg._userid);
    
    vector<uint32_t>  userid_vec;
    login_groupid_mgr->get_userid(ws_msg._groupid, userid_vec);

    StringBuffer s;
    Writer<StringBuffer> writer(s);
    writer.StartObject();

    writer.Key("op");
    writer.Uint(OP_LOGIN);

    writer.Key("res_code");
    writer.Uint(1000);
    
    writer.Key("groupid");
    writer.Uint(ws_msg._groupid);


    writer.Key("userid");
    writer.Uint(ws_msg._userid);
    
    writer.Key("online_userid");
    writer.StartArray();
    for (unsigned i = 0; i < userid_vec.size(); i++) {
        writer.Uint(userid_vec[i]);
    }
    writer.EndArray();
    writer.Key("msg");
    writer.String("login ok");
    writer.EndObject();

    string *tmp = new string(s.GetString());
    //tmp->append("ok, I have recived !!!!!!!!!!!!!!");
    ws_msg_type msg;
    msg._p_msg = tmp;
    msg._con_type = 0x01;

    put_send_msg(msg);
    LOG_DEBUG("send_response:%s", s.GetString());

    _groupid = ws_msg._groupid;
    _userid = ws_msg._userid;

    return ;
}

void qfws_res_data_process::do_online(qf_req_msg & ws_msg)
{
    login_groupid_userid_mgr * login_groupid_mgr = base_singleton<login_groupid_userid_mgr>::get_instance();

    login_groupid_mgr->add(ws_msg._groupid, ws_msg._userid);
    
    vector<uint32_t>  userid_vec;
    login_groupid_mgr->get_userid(ws_msg._groupid, userid_vec);

    StringBuffer s;
    Writer<StringBuffer> writer(s);
    writer.StartObject();

    writer.Key("op");
    writer.Uint(OP_ONLINE);

    writer.Key("res_code");
    writer.Uint(1000);
    
    writer.Key("groupid");
    writer.Uint(ws_msg._groupid);


    writer.Key("userid");
    writer.Uint(ws_msg._userid);
    
    writer.Key("online_userid");
    writer.StartArray();
    for (unsigned i = 0; i < userid_vec.size(); i++) {
        writer.Uint(userid_vec[i]);
    }
    writer.EndArray();
    writer.EndObject();

    string *tmp = new string(s.GetString());
    //tmp->append("ok, I have recived !!!!!!!!!!!!!!");
    ws_msg_type msg;
    msg._p_msg = tmp;
    msg._con_type = 0x01;

    put_send_msg(msg);
    LOG_DEBUG("send_response:%s", s.GetString());
}



void qfws_res_data_process::do_msg(qf_req_msg & ws_msg)
{
    
}

void qfws_res_data_process::msg_recv_finish()
{
   LOG_DEBUG("recv: %s", _recent_msg.c_str());

    qf_req_msg  ws_msg;
    bool is_parse = qf_req_msg::parse_from_json(ws_msg, (char *)(_recent_msg.c_str()));
    if (is_parse) {
        switch (ws_msg._op)
        {
            case OP_LOGIN:
                do_login(ws_msg);
                break;
            case OP_MSG:
                do_msg(ws_msg);
                break;
            case OP_DELIVER:
                break;
            case OP_ONLINE:
                do_online(ws_msg);
                break;
            default:
                break;
        }
    }
}


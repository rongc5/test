#include "common_def.h"
#include "qfws_msg_process.h"
#include "log_helper.h"
#include "rapidjson/document.h"
#include "rapidjson/prettywriter.h"
#include "base_singleton.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/writer.h"

using namespace rapidjson;

void groupid_userid_set::add(const uint32_t &groupid, const uint32_t & userid)
{
    if (_groupid && _groupid != groupid) {
        LOG_WARNING("_groupid:%d != groupid:%d some thing wrong", _groupid, groupid);
        return;
    }

    _userid_set.insert(userid);
    if (!_groupid) {
        _groupid = groupid;
    }
}

void groupid_userid_set::reduce(const uint32_t &groupid, const uint32_t & userid)
{
    if (_groupid != groupid) {
        LOG_WARNING("_groupid:%d != groupid:%d some thing wrong", _groupid, groupid);
        return;
    }

    _userid_set.erase(userid);
    if (_userid_set.empty()) {
        _groupid = 0;
    }
}

bool groupid_userid_set::check(const uint32_t &groupid, const uint32_t &userid)
{
    LOG_DEBUG("_groupid:%d groupid:%d userid:%d", _groupid, groupid, userid);
    if (_groupid != groupid) {
        LOG_WARNING("_groupid:%d != groupid:%d some thing wrong", _groupid, groupid);
        return false;
    }

    set<uint32_t>::const_iterator it = _userid_set.find(userid);
    if (it == _userid_set.end()) {
        return false;
    }

    return true;
}

void groupid_userid_set::get_userid(const uint32_t &groupid, vector<uint32_t> & userid_vec)
{
    LOG_DEBUG("_groupid:%d groupid:%d", _groupid, groupid);
    if (_groupid != groupid) {
        LOG_WARNING("_groupid:%d != groupid:%d some thing wrong", _groupid, groupid);
        return ;
    }

    set<uint32_t>::iterator it;
    for (it = _userid_set.begin(); it != _userid_set.end(); it++) {
        userid_vec.push_back(*it);
    }
}

void login_groupid_userid_mgr::init(uint32_t bucket)
{
    _bucket = get_prime_num(bucket);
    LOG_DEBUG("_bucket:%d", _bucket);
    _mutex = new thread_mutex_t[_bucket]; 
    _groupid_set = new groupid_userid_set[_bucket];
}


bool login_groupid_userid_mgr::check(const uint32_t &groupid, const uint32_t &userid)
{
    uint32_t index = groupid % _bucket;
    thread_lock lock(&_mutex[index]);
    return _groupid_set[index].check(groupid, userid);
}

void login_groupid_userid_mgr::add(const uint32_t & groupid, const uint32_t &userid)
{
    uint32_t index = groupid % _bucket;
    thread_lock lock(&_mutex[index]);
    return _groupid_set[index].add(groupid, userid);
}
void login_groupid_userid_mgr::reduce(const uint32_t & groupid, const uint32_t & userid)
{
    uint32_t index = groupid % _bucket;
    thread_lock lock(&_mutex[index]);
    return _groupid_set[index].reduce(groupid, userid);
}

void login_groupid_userid_mgr::get_userid(const uint32_t &groupid, vector<uint32_t> & userid_vec)
{
    uint32_t index = groupid % _bucket;
    thread_lock lock(&_mutex[index]);
    return _groupid_set[index].get_userid(groupid, userid_vec);
}



bool qf_req_msg::parse_from_json(qf_req_msg & ws_msg, char * json)
{
	if (!json) {
		   LOG_WARNING("json is NULL");
        return false;
    }
	

    Document d;
    if (d.ParseInsitu(json).HasParseError()) {
        LOG_WARNING("_recent_msg parse error:%s", json);
        return false;
    }

    if (!d.IsObject()) {
        LOG_WARNING("_recent_msg parse error:%s", json);
        return false;
    }

    if (d.HasMember("op")&& d["op"].IsInt()) {
        ws_msg._op = (MsgOp)(d["op"].GetInt());
        LOG_DEBUG("op:%d", ws_msg._op);
    }

    if (d.HasMember("groupid")&& d["groupid"].IsInt()) {
        ws_msg._groupid = d["groupid"].GetInt();
        LOG_DEBUG("groupid:%d", ws_msg._groupid);
    }

    if (d.HasMember("userid")&& d["userid"].IsInt()) {
        ws_msg._userid = d["userid"].GetInt();
        LOG_DEBUG("userid:%d", ws_msg._userid);
    }

    if (d.HasMember("passwd")&& d["passwd"].IsString()) {
        ws_msg._passwd = d["passwd"].GetString();
        LOG_DEBUG("passwd:%s", ws_msg._passwd.c_str());
    }

    if (d.HasMember("msg")&& d["msg"].IsString()) {
        ws_msg._msg = d["msg"].GetString();
        LOG_DEBUG("msg:%s", ws_msg._msg.c_str());
    }

    if (d.HasMember("dest_userid")) {
        const Value& a = d["dest_userid"];
        if (a.IsArray()) {
            for (Value::ConstValueIterator itr = a.Begin(); itr != a.End(); ++itr) {
                if (itr->IsInt()) {
                    ws_msg._dest_userid_vec.push_back(itr->GetInt());
                    LOG_DEBUG("dest_userid:%d", itr->GetInt());
                }
            }
        }
    }

    return true;
}


bool qf_res_msg::parse_from_json(qf_res_msg & ws_msg, char * json)
{
		if (!json) {
		   LOG_WARNING("json is NULL");
        return false;
    }
    
    
     Document d;
    if (d.ParseInsitu(json).HasParseError()) {
        LOG_WARNING("_recent_msg parse error:%s", json);
        return false;
    }

    if (!d.IsObject()) {
        LOG_WARNING("_recent_msg parse error:%s", json);
        return false;
    }

    if (d.HasMember("op")&& d["op"].IsInt()) {
        ws_msg._op = (MsgOp)(d["op"].GetInt());
        LOG_DEBUG("op:%d", ws_msg._op);
    }
    
    
     if (d.HasMember("res_code")&& d["res_code"].IsInt()) {
        ws_msg._res_code = d["res_code"].GetInt();
        LOG_DEBUG("res_code:%d", ws_msg._groupid);
    }

    if (d.HasMember("groupid")&& d["groupid"].IsInt()) {
        ws_msg._groupid = d["groupid"].GetInt();
        LOG_DEBUG("groupid:%d", ws_msg._groupid);
    }

    if (d.HasMember("userid")&& d["userid"].IsInt()) {
        ws_msg._userid = d["userid"].GetInt();
        LOG_DEBUG("userid:%d", ws_msg._userid);
    }


    if (d.HasMember("msg")&& d["msg"].IsString()) {
        ws_msg._msg = d["msg"].GetString();
        LOG_DEBUG("msg:%s", ws_msg._msg.c_str());
    }

    if (d.HasMember("online_userid")) {
        const Value& a = d["online_userid"];
        if (a.IsArray()) {
            for (Value::ConstValueIterator itr = a.Begin(); itr != a.End(); ++itr) {
                if (itr->IsInt()) {
                    ws_msg._online_userid_vec.push_back(itr->GetInt());
                    LOG_DEBUG("online_userid:%d", itr->GetInt());
                }
            }
        }
    }

    return true;
    
    
}




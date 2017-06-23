#include "common_def.h"
#include "qfws_msg_process.h"
#include "log_helper.h"


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

uint32_t groupid_userid_set::get_userid(const uint32_t &groupid, const set<uint32_t> &deny_userid_set)
{
    set_difference(_userid_set.begin(), _userid_set.end(), deny_userid_set.begin(), deny_userid_set.end(), inserter(e, e.begin()));

    if (e.begin() != e.end()) {// 此处还需要修改
        LOG_DEBUG("userid:%d", e.begin());
        return e.begin();
    }

    return 0;
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
void login_groupid_userid_mgr::add(const string &server_uid)
{
    uint32_t index = groupid % _bucket;
    thread_lock lock(&_mutex[index]);
    return _groupid_set[index].add(groupid, userid);
}
void login_groupid_userid_mgr::reduce(const string &server_uid)
{
    uint32_t index = groupid % _bucket;
    thread_lock lock(&_mutex[index]);
    return _groupid_set[index].reduce(groupid, userid);
}
uint32_t get_userid(const uint32_t &groupid, const set<uint32_t> &deny_userid_set)
{
    uint32_t index = groupid % _bucket;
    thread_lock lock(&_mutex[index]);
    return _groupid_set[index].get_userid(groupid, userid);
}

void qf_ws_msg::reset()
{
    _op = OP_NON;
    _groupid = 0;
    _userid = 0;
    _passwd.clear();
    _msg.clear();
}



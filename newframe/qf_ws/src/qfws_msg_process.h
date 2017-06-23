#ifndef __QFWS_MSG_PROCESS_H__
#define __QFWS_MSG_PROCESS_H__

#include "common_def.h"
#include "thread_helper.h"

struct groupid_userid_set
{
    void add(const uint32_t &groupid, const uint32_t & userid);
    void reduce(const uint32_t &groupid, const uint32_t & userid);
    bool check(const uint32_t &groupid, const uint32_t &userid);

    uint32_t get_userid(const uint32_t &groupid, const set<uint32_t> &deny_userid_set);

    uint32_t _groupid;
    set<uint32_t> _userid_set;

    groupid_userid_set():groupid(0){}
}


class login_groupid_userid_mgr
{
    public:
        void init(uint32_t bucket=2000000);
        bool check(const uint32_t &groupid, const uint32_t &userid);
        void add(const uint32_t &groupid, const uint32_t & userid);
        void reduce(const uint32_t &groupid, const uint32_t & userid);
        uint32_t get_userid(const uint32_t &groupid, const set<uint32_t> &deny_userid_set);

    private:
        uint32_t _bucket;
        groupid_userid_set * _groupid_set;
        thread_mutex_t * _mutex;
};

enum MsgOp
{
    OP_NON,
    OP_LOGIN,
    OP_MSG,
    OP_DELIVER
};

struct qf_ws_msg
{
    MsgOp _op;
    uint32_t _groupid;
    uint32_t _userid;
    string _passwd;
    string _msg;

    void reset();
};


#endif

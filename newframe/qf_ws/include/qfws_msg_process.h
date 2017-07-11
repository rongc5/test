#ifndef __QFWS_MSG_PROCESS_H__
#define __QFWS_MSG_PROCESS_H__

#include "common_def.h"
#include "thread_helper.h"

struct groupid_userid_set
{
    void add(const uint32_t &groupid, const uint32_t & userid);
    void reduce(const uint32_t &groupid, const uint32_t & userid);
    bool check(const uint32_t &groupid, const uint32_t &userid);
    void get_userid(const uint32_t &groupid, vector<uint32_t> & userid_vec);

    uint32_t _groupid;
    set<uint32_t> _userid_set;

    groupid_userid_set():_groupid(0){}
};


class login_groupid_userid_mgr
{
    public:
        void init(uint32_t bucket=2000000);
        bool check(const uint32_t &groupid, const uint32_t &userid);
        void add(const uint32_t &groupid, const uint32_t & userid);
        void reduce(const uint32_t &groupid, const uint32_t & userid);
        void get_userid(const uint32_t &groupid, vector<uint32_t> & userid_vec);

    private:
        uint32_t _bucket;
        groupid_userid_set * _groupid_set;
        thread_mutex_t * _mutex;
};

enum MsgOp
{
    OP_NON,
    OP_LOGIN,
    OP_ONLINE,
    OP_MSG,
    OP_DELIVER
};

struct qf_req_msg
{
    MsgOp _op;
    uint32_t _groupid;
    uint32_t _userid;
    vector<uint32_t> _dest_userid_vec;
    string _passwd;
    string _msg;

    qf_req_msg()
    {
        _op = OP_NON;
        _groupid = 0;
        _userid = 0;
        _dest_userid_vec.clear();
        _passwd.clear();
        _msg.clear();
    }

    static bool parse_from_json(qf_req_msg & ws_msg, char * json);
};


struct qf_res_msg
{
    MsgOp _op;
    uint32_t _res_code;
    uint32_t _groupid;
    uint32_t _userid;
    vector<uint32_t> _online_userid_vec;
    string _msg;

    qf_res_msg()
    {
        _op = OP_NON;
        _res_code = 0;
        _groupid = 0;
        _userid = 0;
        _online_userid_vec.clear();
        _msg.clear();
    }

    static bool parse_from_json(qf_res_msg & ws_msg, char * json);
};


#endif

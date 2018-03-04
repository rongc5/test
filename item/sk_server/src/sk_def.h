#ifndef __SK_DEF_H__
#define __SK_DEF_H__

#include "common_util.h"

#define MSG_HTTP_REQ 1002
#define MSG_HTTP_RES 1003


class http_req_msg:public normal_msg
{
    public:
        http_req_msg()
        {
            _msg_op = MSG_HTTP_REQ;
        }
        virtual ~http_req_msg(){}

        http_cmd_type cmd_type;

        string sid;
        string  url;
        string post_data;
        map<string, string> headers;
};


class http_res_msg: public normal_msg
{
    public:
        http_res_msg()
        {   
            _msg_op = MSG_HTTP_RES;
        }   

        virtual ~http_res_msg(){}

        string sid;
        string url;
        string response;
};

struct sk_t
{
    string id;
    ToBufferMgr<string> date;
    ToBufferMgr<string> name;

    ToBufferMgr<float> start;
    ToBufferMgr<float> end;
    ToBufferMgr<float> high;
    ToBufferMgr<float> low;
    ToBufferMgr<float> last_closed;

    ToBufferMgr<uint64_t> vol;
    ToBufferMgr<uint64_t> sell_vol;
    ToBufferMgr<uint64_t> buy_vol;

    ToBufferMgr<int> pe;
    ToBufferMgr<int> pb;

    ToBufferMgr<float> swing;

    ToBufferMgr<float> change_rate;
    ToBufferMgr<float> range_percent;

    ToBufferMgr<uint32_t> value;
    ToBufferMgr<uint32_t> cir_value;


    ToBufferMgr<float> mgxj;
    ToBufferMgr<float> mgsy;

    ToBufferMgr<float> mgsygr;
    ToBufferMgr<float> mgxjgr;
    ToBufferMgr<float> zysrgr;
    ToBufferMgr<float> yylrgr;
    ToBufferMgr<float> jlrgr;

    ToBufferMgr<string> lbdata;

    ToBufferMgr<deque<uint32_t> > king_single_in;
    ToBufferMgr<deque<uint32_t> > king_single_out;
    ToBufferMgr<deque<uint32_t> > king_single_inflow;

    ToBufferMgr<deque<uint32_t> > large_single_in;
    ToBufferMgr<deque<uint32_t> > large_single_out;
    ToBufferMgr<define<uint32_t> >large_single_inflow;

    ToBufferMgr<deque<uint32_t> > middle_single_in;
    ToBufferMgr<deque<uint32_t> > middle_single_out;
    ToBufferMgr<deque<uint32_t> > middle_single_inflow;

    ToBufferMgr<deque<uint32_t> > main_force;


    void reset()
    {
        id.clear();
        date.current()->clear();
        name.current()->clear();

        *start.current() = 0;
        *end.current() = 0;
        *high.current() = 0;
        *low.current() = 0;
        *last_closed.current() = 0;

        *vol.current() = 0;
        *sell_vol.current() = 0;
        *buy_vol.current() = 0;

        *pe.current() = 0;
        *pb.current() = 0;

        *swing.current() = 0;
        *change_rate.current() = 0;
        *range_percent.current() = 0;

        *value.current() = 0;
        *cir_value.current() = 0;
        
        *mgxj.current() = 0;
        *mgsy.current() = 0;

        *mgsygr.current() = 0;
        *mgxjgr.current() = 0;
        *zysrgr.current() = 0;
        *yylrgr.current() = 0;
        *jlrgr.current() = 0;

        lbdata.current()->clear();
        king_single_in.current()->clear();
        king_single_out.current()->clear();
        king_single_inflow.current()->clear();

        large_single_in.current()->clear();
        large_single_out.current()->clear();
        large_single_inflow.current()->clear();
        
        middle_single_in.current()->clear();
        middle_single_out.current()->clear();
        middle_single_inflow.current()->clear();

        main_force.current()->clear();
    }
};





#endif

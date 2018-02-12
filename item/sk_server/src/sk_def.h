#ifndef __SK_DEF_H__
#define __SK_DEF_H__


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






#endif

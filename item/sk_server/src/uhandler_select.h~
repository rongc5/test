#ifndef __UHANDLER_SELECT_H__
#define __UHANDLER_SELECT_H__

#include "sk_def.h"

class uhandler_select:public url_handler
{
    public:
        virtual void perform(http_req_head_para * req_head, std::string * recv_body, http_res_head_para * res_head, std::string * send_body);
        
        static int do_parse_request(std::string * _recv_buf, std::map<std::string, std::string> & url_para_map);
        
        static int do_check_select(std::map<std::string, std::string> & url_para_map, std::set<std::string> & res);
};

#endif

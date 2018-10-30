#ifndef __UHANDLER_DEFAULT_H__
#define __UHANDLER_DEFAULT_H__

#include "sk_def.h"
#include "stringbuffer.h"
#include "writer.h"
#include "document.h"
#include "memorystream.h"
#include "prettywriter.h"
#include "rapidjson.h"
#include <algorithm>

using namespace rapidjson;



class uhandler_default:public url_handler
{
    public:
        virtual void perform(http_req_head_para * req_head, std::string * recv_body, http_res_head_para * res_head, std::string * send_body);

};

#endif

#include "uhandler_default.h"
#include "proc_data.h"

void uhandler_default::perform(http_req_head_para * req_head, std::string * recv_body, http_res_head_para * res_head, std::string * send_body)
{
    if (!req_head || !recv_body || !send_body || !res_head)
        return;

    proc_data* p_data = proc_data::instance();
    int recode;
    char t_buf[SIZE_LEN_256];
    t_buf[0] = '\0';

    Document document;
    Document::AllocatorType& allocator = document.GetAllocator(); 
    Value root(kObjectType); 
    Value data_array(kArrayType);

    recode = HTPP_REQ_PATH_ERR;

    Value key(kStringType);    
    Value value(kStringType); 

    key.SetString("recode", allocator);
    root.AddMember(key, recode, allocator);

    key.SetString("data", allocator);

    root.AddMember(key, data_array, allocator);

    StringBuffer buffer;    
    Writer<StringBuffer> writer(buffer);    
    root.Accept(writer);

    send_body->append(buffer.GetString());

    res_head->_headers.insert(std::make_pair("Date", SecToHttpTime(time(NULL))));
    res_head->_headers.insert(std::make_pair("Server", p_data->proc_name));
    res_head->_headers.insert(std::make_pair("Connection", "close"));

    snprintf(t_buf, sizeof(t_buf), "%d", send_body->length());
    res_head->_headers.insert(std::make_pair("Content-Length", t_buf));

}

#include "common_def.h"
#include "log_helper.h"
#include "common_util.h"
#include "common_exception.h"

bool operator<(const ObjId & oj1, const ObjId & oj2)
{
    if (oj1._id != oj2._id){
        return oj1._id < oj2._id;
    } else if (oj1._thread_index != oj2._thread_index) {
        return oj1._thread_index < oj2._thread_index;
    }else 
        return false;
}


bool operator==(const ObjId & oj1, const ObjId & oj2)
{
    return oj1._thread_index ==  oj2._thread_index && oj1._id == oj2._id;
}

http_response_code http_response_code::response;

std::string http_response_code::get_response_str(int status_code)
{
    std::string str;
    std::map<int, std::string>::iterator tmp_itr = _response_list.find(status_code);
    if (tmp_itr == _response_list.end())
    {
        LOG_DEBUG("http response code not found:%d", status_code);
        return str;
    }

    return tmp_itr->second;
}


void http_req_head_para::to_head_str(std::string * head)
{
    if (!head) 
    {
        return;
    }


    head->append(_method);
    head->append(" ");
    head->append(_url_path);
    head->append(" ");
    head->append(_version);
    head->append(CRLF);        


    if (_cookie_list.size() > 0)
    {
        head->append("Cookie");
        head->append(": ");
        int ii = 0;
        for (std::map<std::string, std::string>::iterator itr = _cookie_list.begin(); 
                itr != _cookie_list.end(); ++itr)
        {
            if (ii > 0)
                head->append(";");
            head->append(itr->first);
            head->append("=");
            head->append(itr->second);
            ii++;
        }
        head->append(CRLF);
    }

    //other para
    for (std::map<std::string, std::string>::iterator itr = _headers.begin();itr != _headers.end(); ++itr)
    {
        head->append(itr->first);
        head->append(": ");
        head->append(itr->second);
        head->append(CRLF);
    }

    //最后一个分隔符
    head->append(CRLF);

}

void http_res_head_para::to_head_str(std::string * head)
{
    if (!head) {
        return;
    }
    //返回状态码
    if (!_response_str.empty())
        _response_str = http_response_code::response.get_response_str(_response_code);

    {
        head->append(_version);
        head->append(" ");
        char tmp_buf[SIZE_LEN_32];
        snprintf(tmp_buf, sizeof(tmp_buf), "%d", _response_code);
        head->append(tmp_buf);
        head->append(" ");
        head->append(_response_str);
        head->append(CRLF);
    }

    //cookie
    if (_cookie_list.size() > 0)
    {
        for (std::map<std::string, set_cookie_item>::iterator itr = _cookie_list.begin(); 
                itr != _cookie_list.end(); ++itr)
        {
            head->append("Set-Cookie: ");
            head->append(itr->first);
            head->append("=");
            head->append(itr->second._value);
            if (itr->second._expire != 0)
            {
                head->append(";expires=");
                head->append(SecToHttpTime(itr->second._expire));
            }

            if (itr->second._path != "")
            {
                head->append(";path=");
                head->append(itr->second._path);
            }

            if (itr->second._domain != "")
            {
                head->append(";domain=");
                head->append(itr->second._domain);
            }
            head->append(CRLF);
        }
    }

    //other para
    for (std::map<std::string, std::string>::iterator itr = _headers.begin(); 
            itr != _headers.end(); ++itr)
    {
        head->append(itr->first);
        head->append(": ");
        head->append(itr->second);
        head->append(CRLF);
    }


    //最后一个分隔符
    head->append(CRLF);
}

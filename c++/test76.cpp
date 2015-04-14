#include "msgpack.h"
#include <string>
#include <iostream>

using namespace std;

struct login_switch_res_msg
{
    int result; //注册成功0, 失败1
    string m_type;  
    string m_name; 
    MSGPACK_DEFINE(result, m_type, m_name);
};


struct login_switch_req_msg
{
    string m_name;
    MSGPACK_DEFINE(m_name);
};

int main(int argc, char *argv[])
{
    login_switch_req_msg req, tmp, req1;
    login_switch_res_msg res, res1;

    req.m_name = "hello";

    res.m_type = "qianfeng";
    res.m_name = "zm";
    res.result = 100;

    string str, str1;
    size_t off_set = 0;
    std::cout << req.m_name.c_str() << " "<<res.result << " "<<res.m_type.c_str() << " " << res.m_name.c_str() <<'\n';

    pack_util::pack<login_switch_req_msg, login_switch_res_msg>(str, req, res);

    pack_util::un_pack<login_switch_req_msg>(str, tmp, off_set);
    tmp.m_name = "bilin";
    pack_util::pack<login_switch_req_msg>(str1, tmp);
    
    str1.append(str, off_set, str.size()-off_set);

    //std::cout << str1 << '\n';

    off_set = 0;
    pack_util::un_pack<login_switch_req_msg>(str1, req1, off_set);
    pack_util::un_pack<login_switch_res_msg>(str1, res1, off_set);
    std::cout << req1.m_name.c_str() << " "<<res1.result << " "<<res1.m_type.c_str() << " " << res1.m_name.c_str() <<'\n';

    return 0;
}


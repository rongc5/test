#include <msgpack.hpp>
#include <string>
#include <cstring>
#include <iostream>
using namespace std;


struct Foo
{
    string  host;
    int     port;
    // 原始指针类型，内部封装了pack_raw和pack_raw_body方法
    //msgpack::type::raw_ref  data;

    MSGPACK_DEFINE(host, port); 
};


int main(int argc, char** argv)
{
    Foo  f;
    f.host = "192.168.10.29";
    f.port = 7881;
    //const char* tmp = "msgpack";
    //f.data.ptr = tmp;
    //f.data.size = strlen(tmp) + 1;
    char buf[64];
    int len;

    msgpack::sbuffer sbuf, s_tmp;
    msgpack::pack(sbuf, f);

    std::cout << "packed ok" << '\n';

    sprintf(buf, sbuf.data(), sbuf.size());

    len = strlen(buf);

    std::cout << sbuf.size() << "==== " << len << '\n';
    s_tmp.write(buf, len);

    msgpack::unpacked  unpack;
    //msgpack::unpack(&unpack, s_tmp.data(), len);
    msgpack::unpack(&unpack, buf, len);

    msgpack::object  obj = unpack.get();

    Foo f2;
    obj.convert(&f2);

    cout << f2.host << ", " << f2.port << ", " << '\n';
    //cout << f2.data.ptr << endl;

    return 0;
}

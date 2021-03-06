#include <msgpack.hpp>
#include <iostream>
#include <string>


using namespace std;

class myclass {
    private:
        std::string str1;
        std::string str2;
    public:
        myclass(){};
        myclass(string s1,string s2):str1(s1),str2(s2){};
        MSGPACK_DEFINE(str1,str2);
};

int main(int argc, char **argv)
{
    std::vector<myclass> vec;
    myclass m1("m1","m2");
    vec.push_back(m1);

    // you can serialize myclass directly
    msgpack::sbuffer sbuf;
    msgpack::pack(sbuf, vec);

    msgpack::unpacked msg;
    msgpack::unpack(&msg, sbuf.data(), sbuf.size());

    msgpack::object obj = msg.get();

    // you can convert object to myclass directly
    std::cout << obj << '\n';
    std::vector<myclass> rvec;
    obj.convert(&rvec);

    std::vector<myclass>::iterator it;

    //for (it = rvec.begin(); it != rvec.end(); ++it)
        //std::cout << it << '\n';

    return 0;
}

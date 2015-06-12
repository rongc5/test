#include <map>
#include <string>
#include <iostream>

namespace zm{
    
class A{
public:
    void add(int a, const std::string &b);
    void show();
    private:
        typedef std::map<int, std::string> info;
        info data;
};

}

//namespace zm{

void zm::A::add(int a, const std::string &b)
{
    data.insert(std::make_pair(a, b));
}

void zm::A::show()
{
    //std::map<int, std::string>::iterator it;
    info::iterator it;

    for (it = data.begin(); it != data.end(); it++){
        std::cout<<it->first <<" "<<it->second.c_str()<<std::endl;
    }
}
//}

int main(int argc, char *argv[])
{
    zm::A mm;
    mm.add(1, "hello");
    mm.add(2, "world");

    mm.show();
    return 0;
}


#include <sstream>
#include <iostream>
#include <string>

using namespace std;

int main(int argc, char *argv[])
{
    std::stringstream stream;
    stream<<456;
    std::cout<<stream.str() <<'\n';

    string url= "http://192.168.10.230:8081/getBadgeForUser.html?userId=";
    url.append(stream.str());
    url.append("&debug=1");


    std::cout<<url <<'\n';

    return 0;
}

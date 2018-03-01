#include <string>
#include <stdio.h>
#include <iostream>

using namespace std;

std::string getCmdOutput(const std::string& mStr)
{
    std::string result, file;
    FILE* pipe{popen(mStr.c_str(), "r")};
    char buffer[8192];

    while(fgets(buffer, sizeof(buffer), pipe) != NULL)
    {
        file = buffer;
        result += file.substr(0, file.size() - 1);
    }

    pclose(pipe);
    printf("%s\n", result.c_str());
    return result;
}

int main(int argc, char *argv[])
{
    string cmd;
    cmd = "curl http://10.26.24.86:6011/recomm.json?recommType=207&uid=75191735&udid=8511889F1912F9799DBB760C7E016D39&start=0&count=10&charge=1&cid=nice&modCount=8&appid=10001";
    getCmdOutput(cmd);


    return 0;
}

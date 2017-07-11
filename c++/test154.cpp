#include <string>
#include <stdio.h>

using namespace std;

int main(int argc, char *argv[])
{
    string str = "hello world\r\n\r\n";
    size_t pos =  str.find("\r\n\r\n");
    if (pos != string::npos) {
        string left_str = str.substr(pos + 4);
        str.erase(pos + 4);

        printf("left_str:%d\n", left_str.length());
    }

    printf("str:%s\n", str.c_str());

    return 0;
}

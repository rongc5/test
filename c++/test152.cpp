#include <iostream>
#include <string>
#include <string.h>
#include <stdio.h>

using namespace std;

const char * ptr = "GET / HTTP/1.1\r\nHost:www.inbilin.com\r\nUpgrade: websocket\r\nConnection: Upgrade\r\nSec-WebSocket-Key:zEkljJfDq59X6c+8BZx7yw==\r\nOrigin:inbilin.com\r\nSec-WebSocket-Protocol: chat, superchat\r\nSec-WebSocket-Version: 13\r\n\r\n";

void test(const char * buf, size_t len)
{
    string tstr;
    //tstr.append(string(buf, len));
    tstr.append(buf, len);
    //tstr.assign(buf, len);

    printf("buf:%s tstr:%s %d\n", buf, tstr.c_str(), tstr.size());
    size_t pos = tstr.find("\r\n\r\n");
    if (pos != string::npos)
        printf("%s\n", tstr.c_str());
}

int main(int argc, char *argv[])
{
    string str;
    memcpy(const_cast<char *>(str.c_str()), "hello world", sizeof("hello world"));
    //str.append(ptr);

    string str3;
    str3.copy((char *)str.c_str(), sizeof("hello world"));

    printf("str3:%s len:%d\n", str3.data(), str3.length());

    string str1("hello world");
    string str2("hello zhangming");

    str.swap(str2);

    test((char *)str.data(), str.length());
    printf("str:%s str2:%s strlength:%d str2length:%d\n", str.c_str(), str2.c_str(), str.length(), str2.length());
    
    //printf("%d", str1.size());
    
    return 0;
}

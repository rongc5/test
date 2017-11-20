#include <iostream>
#include <string>
#include <string.h>
#include <stdio.h>
#include <stdarg.h>

#include <vector>
#include <cstdio>
#include <cstdarg>
#include <ctime>
 
void debug_log(const char *fmt, ...)
{
    std::time_t t = std::time(NULL);
    char time_buf[100];
    std::strftime(time_buf, sizeof time_buf, "%D %T", std::gmtime(&t));
    va_list args1;
    va_start(args1, fmt);
    va_list args2;
    va_copy(args2, args1);
    std::vector<char> buf(100+std::vsnprintf(NULL, 0, fmt, args1));
    int ret = snprintf(buf.data(), buf.size(), "%s ", time_buf);
    va_end(args1);
    std::vsnprintf(buf.data() + ret, buf.size() - ret, fmt, args2);
    //std::vsnprintf(buf.data(), buf.size(), fmt, args2);
    va_end(args2);
    //std::printf("%s [debug]: %s\n", time_buf, buf.data());
    std::printf("[debug]: %s\n", buf.data());
}

int main()
{
    debug_log("Logging, %s, %d, %d, %d", "hello, world", 1, 2, 3);
}

#include <stdio.h>
#include <stdarg.h>
#include <vector>
#include <memory>

using namespace std;

void FILE_SYN_WRITE(const char * filename, const char *format, ...)
{

    FILE * fp = NULL;
    fp = fopen(filename, "a");
    if (!fp){
        return;
    }   

    va_list args1, args2;
    va_start(args1, format);
    va_copy(args2, args1);

    auto _buf = make_shared<vector<char>>(vsnprintf(NULL, 0, format, args1) + 2);
    va_end(args1);

    vsnprintf(_buf->data(), _buf->size(), format, args2);
    va_end(args2);

    fprintf(fp, "%s", _buf->data());

    fclose(fp);
}  

int main(int argc, char *argv[])
{
    FILE_SYN_WRITE("20210312", "%s\n", "hello");
    FILE_SYN_WRITE("20210312", "%s\n", "world");
    FILE_SYN_WRITE("20210312", "%d\n", 123456);

    return 0;
}

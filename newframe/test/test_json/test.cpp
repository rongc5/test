#include "common_def.h"
#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"


using namespace rapidjson;

int main(int argc, char *argv[])
{
    //const char* json = "{\"project\":\"rapidjson\",\"stars\":10}";
    const char* json = "{\"project\":\"rapidjson\",\"stars\":1";
    char buf[SIZE_LEN_1024];
    snprintf(buf, sizeof(buf), "%s", json);

    Document d;

    d.Parse(buf);
    if (d.ParseInsitu(buf).HasParseError()) {
           printf("error \n");
        return -1;
    }


    if (d["stars"].IsNumber())
    printf("hello = %d\n", d["stars"].GetInt());

    return 0;
}

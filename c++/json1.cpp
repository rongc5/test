#include <iostream>
#include <string>
#include <json/json.h>

int main(int argc, char *argv[])
{
    Json::Value root;
    Json::Value arrayObj;
    Json::Value item;

    for (int i = 0; i < 10; i ++)
    {
        item["key"] = i;
        arrayObj.append(item);
    }

    root["key1"] = "value1";
    root["key2"] = "value2";
    //root["array"] = arrayObj;

    //std::string out = root.toStyledString();
    std::string out = arrayObj.toStyledString();
    std::cout << out << std::endl;

    return 0;
}

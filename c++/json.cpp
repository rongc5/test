#include <iostream>
#include <string>
#include <json/json.h>

int main(int argc, char *argv[])
{
    std::string strValue = "{\"key1\":\"value1\",\"array\":[{\"key2\":\"value2\"},{\"key2\":\"value3\"},{\"key2\":\"value4\"}]}";

    Json::Reader reader;
    Json::Value value;

    if (reader.parse(strValue, value))
    {
        std::string out = value["key1"].asString();
        std::cout << out << std::endl;
        const Json::Value arrayobj = value["array"];

        for (int i=0; i<arrayobj.size(); i++)
        {
            out = arrayobj[i]["key2"].asString();
            std::cout << out;
            if (i != arrayobj.size() - 1)
                std::cout << std::endl;
        }
    }

    return 0;
}

#include "common_util.h"
#include "base_def.h"

int main(int argc, char *argv[])
{
    string url = "https://www.google.com/search?ei=BRViWsbHK4SGjwOZk5SYDA&q=linux+c+http+url+%E8%A7%A3%E6%9E%90&oq=linux+c+http+url+%E8%A7%A3%E6%9E%90&gs_l=psy-ab.3...8828.13164.0.13484.10.10.0.0.0.0.349.1854.2-3j3.6.0....0...1c.1.64.psy-ab..4.3.964...0j0i13k1.0.sO6yNCKSApI";

    vector<string> strList;

    SplitString(url.c_str(), "://", &strList, SPLIT_MODE_ONE);

    for (int i = 0; i< strList.size(); i++){
        printf("%s\n", strList[i].c_str());
    }
    
    url_info info;
    parse_url(url, info);

    printf("%s %s %d %s %s\n", info.protocol_type.c_str(), info.domain.c_str(), 
            info.port, info.path.c_str(), info.query.c_str());

    string t("hhhheeellllhhh");
    printf("%s\n", t.c_str());
    string dest;
    stringStrip(t.c_str(), "hhh", &dest, STRIP_MODE_LEFT | STRIP_MODE_RIGHT);

    printf("%s\n", dest.c_str());

    return 0;
}

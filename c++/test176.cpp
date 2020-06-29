#include <string>
#include <iostream>
#include <stdio.h>
#include <time.h>
#include <string.h>

using namespace std;

void get_yearweek(const std::string &in, std::string &out)
{
    out.clear();

    struct tm tmin;
    struct tm *tminp=&tmin;

    memset(tminp, 0, sizeof(struct tm));

    char tmp[512];
    memset(tmp,0,sizeof(tmp));
    strptime(in.c_str(),"%Y%m%d %Z",tminp);

    strftime(tmp,sizeof(tmp),"%Y%W",tminp);
    out.append(tmp);
}


int main(int argc, char *argv[])
{
    
    string in ="20200624";
    string out;
    get_yearweek(in, out);

    printf("%s\n", out.c_str());

    return 0;
}

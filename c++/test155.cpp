#include <iostream>
#include <string>
#include <vector>
#include <string.h>
#include <stdio.h>

using namespace std;

#if 1
int SplitString(const string &srcStr,const string &delim, vector<string>& strList)
{
    char *sBuf = new char[srcStr.length() + 1]; 
    char *sTmpBuf = new char[srcStr.length() + 1]; 

    strncpy(sBuf, srcStr.c_str(), srcStr.length());
    sBuf[srcStr.length()] = 0;

    char *tmpSrc = sBuf;
    char *tt = sTmpBuf;

    char *sToken = strtok_r(tmpSrc, delim.c_str(), &tt);
    while (sToken)
    {   
        printf("sToken:%s tt: %s\n", sToken, tt);
        strList.push_back(string(sToken));
        sToken = strtok_r(NULL, delim.c_str(), &tt);
    }   
    delete [] sBuf;
    delete [] sTmpBuf;
    return 0;
}

#endif

int SplitOneString(const string &srcStr,const string &delim, vector<string>& strList)
{
    size_t pos = srcStr.find_first_of(delim);
    if (pos != std::string::npos) {
        string tmp_str = srcStr.substr(0, pos);
        string left_str = srcStr.substr(pos+delim.length());
        strList.push_back(tmp_str);
        printf("%s %s\n", tmp_str.c_str(), left_str.c_str());
        //pos = srcStr.find_first_of(delim);
    }
}

int main(int argc, char *argv[])
{
    string str="hellozmworldzmtest";

    vector<string> tmp_vec;
    SplitOneString(str, "zm", tmp_vec);
    //
    //size_t pos = str.find_first_of("zm");
    //if (pos != std::string::npos) {
        //printf("pos:%d\n", pos);
        //string tmp_str = str.substr(0, pos);
        //printf("%s\n", tmp_str.c_str());
    //}

    return 0;
}

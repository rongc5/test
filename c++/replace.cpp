#include <string>
#include <iostream>
#include <stdio.h>

using namespace std;

void replaceAll(std::string& str, const std::string& from, const std::string& to) {
    if(from.empty())
        return;
    size_t start_pos = 0;
    while((start_pos = str.find(from, start_pos)) != std::string::npos) {
        str.replace(start_pos, from.length(), to);
        start_pos += to.length(); // In case 'to' contains 'from', like replacing 'x' with 'yx'
    }
}

int main(int argc, char *argv[])
{
    string src = "motivate_snippet\":\"\",\"ext\":\"{\\\"sex\\\":0,\\\"coin\\\":0}";
    string dest;

    replaceAll(dest, src, "");

    printf("%s\n", dest.c_str());

    return 0;
}


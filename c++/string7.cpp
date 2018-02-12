#include <iostream>
#include <fstream>
#include <string>
#include <stdio.h>

using namespace std;

std::string trim(std::string const& source, char const* delims = " \t\r\n") {
    std::string result(source);
    std::string::size_type index = result.find_last_not_of(delims);
    if(index != std::string::npos)
        result.erase(++index);

    index = result.find_first_not_of(delims);
    if(index != std::string::npos)
        result.erase(0, index);
    else
        result.erase();
    return result;
}

int main(int argc, char *argv[])
{
    string source = "\n\nhello world\n\n";

    printf("%s\n", source.c_str());

    printf("========\n");
    printf("%s\n", trim(source).c_str());

    return 0;
}

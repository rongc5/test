#include <iostream>
#include <stdio.h>
#include <stdlib.h>

using namespace std;

string GetStdoutFromCommand(string cmd) {

    string data;
    FILE * stream;
    const int max_buffer = 256;
    char buffer[max_buffer];
    cmd.append(" 2>/dev/null");

    stream = popen(cmd.c_str(), "r");
    if (stream) {
        while (!feof(stream))
            if (fgets(buffer, max_buffer, stream) != NULL) data.append(buffer);
        pclose(stream);
    }
    printf("%s\n", data.c_str());
    return data;
}

int main (){

    string ls = GetStdoutFromCommand("curl 'http://10.26.24.86:6011/recomm.json?recommType=207&uid=75191735&udid=8511889F1912F9799DBB760C7E016D39&start=0&count=10&charge=1&cid=nice&modCount=8&appid=10001'");

    //cout << "LS: " << ls << endl;

    return 0;
}

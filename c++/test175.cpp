#include <iostream>
#include <vector>
#include <memory>
#include <string.h>

using namespace std;

int main(int argc, char *argv[])
{
    std::shared_ptr<vector<int> > single = std::make_shared<vector<int> >(5);

    printf("%d %d\n", single->size(), single->at(0));

    return 0;
}


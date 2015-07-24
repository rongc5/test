#include <iostream>
#include <string>

using namespace std;

class Test
{
    public:
        explicit Test(string name);
};

Test::Test(string name)
{
    cout << name << '\n';
}

int main(int argc, char *argv[])
{
    Test t("hello world");

    return 0;
}

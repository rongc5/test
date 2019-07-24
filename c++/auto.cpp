#include <iostream>
#include <string>


using namespace std;

struct test_t
{
    string name;
    int score;

    test_t()
    {
        score = 0;
    }
};

int main(int argc, char *argv[])
{
    int array[] = {2,2,3,4,5};

    for (auto &i:array)
        cout << "==> " << i << '\n';

    test_t * t = new test_t;
    auto f = t;
    f->name = "test";
    f->score = 90;

    
    cout << "name: " << f->name << "score: " << f->score << '\n';

    
    return 0;
}

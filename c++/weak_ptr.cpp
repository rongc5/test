#include <iostream>
#include <memory>

using namespace std;

class parent;
class children;

typedef shared_ptr<parent> parent_ptr;
typedef shared_ptr<children> children_ptr;

class parent
{
    public:
        ~parent(){cout<<" A desroy\n";}
    children_ptr childPerson;
};

class children
{
    public:
        ~children(){cout << "B desroy\n";}
        parent_ptr parentPerson;
};

void test()
{
    parent_ptr father(new parent);
    children_ptr son(new children);

    father->childPerson = son;
    son->parentPerson = father;
}

int main(int argc, char *argv[])
{

    cout << "begin test()\n";

    test();

    cout << "end test()\n";

    return 0;
}

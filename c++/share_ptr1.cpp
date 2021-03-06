#include <iostream>
#include <memory>

struct test
{
    int id;
    test()
    {
        id = 0;
    }
};

void foo(const std::shared_ptr<int> & i)
{
    (*i)++;
}

int main(int argc, char *argv[])
{
    auto sp = std::make_shared<test>();

    //foo(sp);
    sp->id = 15;

    std::cout<< sp->id <<'\n';

    auto xp = std::make_shared<test>(*sp);

    xp->id = 25;
    std::cout<< sp->id <<'\n';
    std::cout<< xp->id <<'\n';

    return 0;
}

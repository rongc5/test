#include <iostream>
#include <string.h>
#include <stdio.h>

typedef int (*test_t)(int a, int b);
int test(int a, int b)
{
    printf("%d\n", a+b);

    return a+b;
}


class A{

    public:
        A():_t(NULL){}
        void set(test_t t)
        {
            _t = t;
        }

        void play(int a, int b)
        {
            if (_t)
                _t(a, b);
        }
    protected:
    test_t _t;
};


int main(int argc, char *argv[])
{
    A a;
    a.set(test);
    a.play(1, 2);

    return 0;
}


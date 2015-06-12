#include <iostream>

using namespace std;

class NUM {
    int b, e, num;
    public:
    NUM(){}
    NUM(int beg, int end)
    {
        b = beg;
        e = end;
        num = 0;
    }
    void prime(void);
};

void NUM::prime(void)
{
    int val, i;

    for (val = b; val <= e; val++) {
        for (i = 2; i < val; i++) {
            if (val %i == 0) {
                flag = 1;
                break;
            }
        }
    }
}

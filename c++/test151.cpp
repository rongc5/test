#include <iostream>
#include <deque>
#include "stdio.h"

using namespace std;

class test
{
    public:
        test()
        {
            index = i;
            //printf("create %d\n", index);
            i++;
        }
        
        unsigned int get()
        {
            return index;
        }

        ~test()
        {
            //printf("destory %d\n", index);
        }

    private:
        static unsigned int i;
        unsigned int index;
};

unsigned int test::i = 0;

int main()
{
    deque<test *> td;

    printf("deque.size:%d\n", td.size());
    for (int i = 1; i <= 10000; i++){
        test * t = new test;
        td.push_back(t);
    }

    printf("deque.size:%d\n", td.size());
    deque<test *>::iterator it;
    for (it = td.begin(); it != td.end();) {
       //if  ((*it)->get() % 3 == 0){
           delete *it;
            it = td.erase(it);
       //} else {
            //++it;
       //}
    }

    printf("deque.size:%d\n", td.size());

    return 0;
}

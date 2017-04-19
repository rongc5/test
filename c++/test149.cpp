#include <iostream>
#include <deque>
#include <stdio.h>



class A
{
    public:
        A()
        {
            printf("create A\n");
        }
        ~A()
        {
            printf("destory ~A\n");
        }
};


int main ()
{
    std::deque<A *> mydeque;

    mydeque.push_back (new A);

    std::cout << "Popping out the elements in mydeque:";
    while (!mydeque.empty())
    {
        //std::cout << ' ' << mydeque.front();
        mydeque.pop_front();
    }

    std::cout << "\nThe final size of mydeque is " << int(mydeque.size()) << '\n';

    return 0;
}

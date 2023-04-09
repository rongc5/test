#include <iostream>
#include <memory>
#include <utility>
#include <array>

struct A {
    A(int&& n) { std::cout << "rvalue overload, n=" << n << "\n"; }
    A(int& n)  { std::cout << "lvalue overload, n=" << n << "\n"; }
};

class B {
    public:
        template<class T1, class T2, class T3>
            B(T1&& t1, T2&& t2, T3&& t3) :
                a1_{std::forward<T1>(t1)},
                a2_{std::forward<T2>(t2)},
                a3_{std::forward<T3>(t3)}
        {
        }

    private:
        A a1_, a2_, a3_;
};

    template<class T, class U>
std::unique_ptr<T> make_unique1(U&& u)
{
    return std::unique_ptr<T>(new T(std::forward<U>(u)));
}

    template<class T, class... U>
std::unique_ptr<T> make_unique(U&&... u)
{
    return std::unique_ptr<T>(new T(std::forward<U>(u)...));
}


int main(int argc, char *argv[])
{

    auto p1 = make_unique1<A>(2); //rvalue

    int i = 1;

    auto p2 = make_unique1<A>(i); // lvalue

    std::cout << "B\n";
    auto t = make_unique<B>(2, i, 3);


    return 0;
}


/**
 *  无论是T&&、左值引用、右值引用，std::forward都会依照原来的类型完美转发
 *
 */

#include <iostream>
#include <initializer_list>
#include <vector>

/*
 *std::initializer_list 的内部并不负责保存初始化列表中元素的拷贝,仅仅
        存储了列表中元素的引用而已，因此需要再持有对象的生存周期之前传递完毕
        使用 std::initializer_list<int> 来初始化任意长度的初始化列表
 *
 * 
 * 
 */

class MagicFoo {
    public:
        std::vector<int> vec;
        MagicFoo(std::initializer_list<int> list) {
            for (auto it = list.begin();
                    it != list.end(); ++it)
                vec.push_back(*it);
        }
};


void foo(std::initializer_list<int> list) {
    for (auto it = list.begin();
            it != list.end(); ++it) 
        std::cout << *it << " ";
    std::cout << "\n";
}


int main(int argc, char *argv[])
{
    
    MagicFoo magicFoo = {1, 2, 3, 4, 5};
    std::cout << "magicFoo: ";
    for (auto it = magicFoo.vec.begin(); it != magicFoo.vec.end(); ++it) 
        std::cout<< *it << " ";
    std::cout<<'\n';


    foo({1,2,3,4,5,6,7,8,9});

    return 0;
}

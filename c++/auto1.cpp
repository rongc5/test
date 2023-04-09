#include <iostream>
#include <initializer_list>
#include <vector>

/*
 *std::initializer_list ���ڲ��������𱣴��ʼ���б���Ԫ�صĿ���,����
        �洢���б���Ԫ�ص����ö��ѣ������Ҫ�ٳ��ж������������֮ǰ�������
        ʹ�� std::initializer_list<int> ����ʼ�����ⳤ�ȵĳ�ʼ���б�
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

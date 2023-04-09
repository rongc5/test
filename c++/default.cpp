#include <iostream>

class Magic {
    public:
        Magic() = default; // 显式声明使用编译器生成的构造
        Magic& operator=(const Magic&) = delete; // 显式声明拒绝编译器生成构造
        Magic(int magic_number){
            _value = magic_number;
        }

        int get_value()
        {
            return _value;
        }

    private:
        int _value;
};

int main(int argc, char *argv[])
{
    Magic m;
    std::cout << m.get_value() << "\n";
    Magic n(100);
    std::cout << n.get_value() << "\n";
    
    return 0;
}

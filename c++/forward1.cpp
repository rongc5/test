#include <iostream>
#include <utility>

void reference(int& v) {
    std::cout << " ��ֵ����" << std::endl;
}

void reference(int&& v) {
    std::cout << " ��ֵ����" << std::endl;
}

template <typename T>
void pass(T&& v) {
    std::cout << " ��ͨ����: ";
    reference(v);
    std::cout << " std::move ����: ";
    reference(std::move(v));
    std::cout << " std::forward ����: ";

    reference(std::forward<T>(v));
    std::cout << "static_cast<T&&> ����: ";
    reference(static_cast<T&&>(v));
}

int main(int argc, char *argv[])
{
    std::cout << " ������ֵ:" << std::endl;
    pass(1);
    std::cout << " ������ֵ:" << std::endl;
    int v = 1;
    pass(v);

    return 0;
}

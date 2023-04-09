#include <iostream>

using namespace std;


/*
 *C++11��һЩ�����ĸ��ƣ����ָ��Ʊ�֤д���Ĵ���������κ�ʱ���ִ��Ч�ʶ�Ҫ�á����ָ���֮һ�������ɳ������ʽ������������ñ���ʱ�ļ���������
 ��Ҫ����һ������ʱ��֪�ĳ����������ض�ֵ��sine��cosin��ȷʵ�������ʹ�ÿ⺯��sin��cos������������뻨������ʱ�Ŀ�����ʹ��constexpr������Դ���һ������ʱ�ĺ���������Ϊ����������Ҫ����ֵ���û��ĵ��Խ�����Ҫ����Щ������
 */

#define LEN 10

int len_foo() {
    int i = 2;
    return i;
}

constexpr int len_foo_constexpr() {
    return 5; 
}

constexpr int fibonacci(const int n) {
    return n == 1 || n == 2 ? 1 : fibonacci(n-1)+fibonacci(n-2);
}

int main(int argc, char *argv[])
{
    int len = 10;

    const int len_2 = len + 1;
    constexpr int len_2_constexpr = 1 + 2 + 3;
    char arr_4[len_2_constexpr];

    char arr_6[len_foo_constexpr() + 1];

    std::cout << fibonacci(10) << std::endl;
    std::cout << fibonacci(10) << std::endl;
    std::cout << len_2_constexpr << std::endl;


    return 0;
}

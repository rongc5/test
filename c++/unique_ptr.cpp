#include <iostream>
#include <memory>


int main(int argc, char *argv[])
{
    std::unique_ptr<int> uptr(new int(10)); 
    std::unique_ptr<int> uptr2 = std::move(uptr);

    if(uptr == nullptr)
        printf("uptr give up *int\n");

    int * p = uptr2.release(); //uptr2�ͷŶ�ָ��Ŀ���Ȩ������ָ�룬����uptr2��Ϊ��

    if(uptr2 == nullptr)
        printf("uptr2 give up *int\n");

    printf("%d\n", *p);
            delete p;

    return 0;
}

#include <iostream>
#include <memory>


int main(int argc, char *argv[])
{
    std::unique_ptr<int> uptr(new int(10)); 
    std::unique_ptr<int> uptr2 = std::move(uptr);

    if(uptr == nullptr)
        printf("uptr give up *int\n");

    int * p = uptr2.release(); //uptr2释放对指针的控制权，返回指针，并将uptr2置为空

    if(uptr2 == nullptr)
        printf("uptr2 give up *int\n");

    printf("%d\n", *p);
            delete p;

    return 0;
}

#include <atomic>
#include <thread>
#include <iostream>

std::atomic<int> count = {0};

int main(int argc, char *argv[])
{
    std::thread t1([](){
            count.fetch_add(1);
            });

    std::thread t2([](){
            count++; // �ȼ��� fetch_add
            count += 1; // �ȼ��� fetch_add

            });

    t1.join();
    t2.join();
    std::cout << count << std::endl;
    
    return 0;
}

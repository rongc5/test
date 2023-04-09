#include <atomic>
#include <thread>
#include <iostream>
#include <vector>



int main(int argc, char *argv[])
{
    std::vector<int> v;
    std::atomic<int> flag = {0};
    std::thread release([&]() {
            v.push_back(42);
            flag.store(1, std::memory_order_release);
            });

    std::thread acqrel([&]() {
            int expected = 1; // must before compare_exchange_strong
            while(!flag.compare_exchange_strong(expected, 2, std::memory_order_acq_rel)) {
            expected = 1; // must after compare_exchange_strong
            }
    });

    std::thread acquire([&]() {
            while(flag.load(std::memory_order_acquire) < 2);
            std::cout << v.at(0) << std::endl; // must be 42
            });

    release.join();
    acqrel.join();
    acquire.join();


    return 0;
}

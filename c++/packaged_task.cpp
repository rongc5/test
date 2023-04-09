#include <iostream>
#include <future>
#include <thread>

int main(int argc, char *argv[])
{
    std::packaged_task<int(int)> task([](int x){return 7 * x;});

    std::future<int> result = task.get_future();
    std::thread(std::move(task), 9).detach();
    
    std::cout << "waiting...";
    result.wait();

    std::cout << "done!" << std:: endl << "future result is " << result.get() << std::endl;
    
    return 0;
}

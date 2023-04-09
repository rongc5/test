#include <iostream>
#include <thread>
#include <mutex>


/**
 *std::lock_guard 不能显式的调用 lock 和 unlock，而 std::unique_lock 可以在声明后的任意位
 置调用，可以缩小锁的作用范围，提供更高的并发度
 *
 * std::unique_lock 的对象会以独占所有权（没有其他的 unique_lock 对象同时拥有某个 mutex
 * 对象的所有权）的方式管理 mutex 对象上的上锁和解锁的操作
 * 
 * std::condition_variable::wait 则必须使用 std::unique_lock 作为参
 * 数
 */

int v = 1;

void critical_section(int change_v) {
    static std::mutex mtx;
    std::unique_lock<std::mutex> lock(mtx);

    v = change_v;
    std::cout << v << std::endl;

    lock.unlock();
    //在此期间，任何人都可以抢夺 v 的持有权
    lock.lock();
    v += 1;
    std::cout << v << std::endl;
}

int main(int argc, char *argv[])
{
    std::thread t1(critical_section, 2), t2(critical_section, 3);
    t1.join();
    t2.join();
    return 0;

}


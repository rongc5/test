#include <iostream>
#include <thread>
#include <mutex>


/**
 *std::lock_guard ������ʽ�ĵ��� lock �� unlock���� std::unique_lock �����������������λ
 �õ��ã�������С�������÷�Χ���ṩ���ߵĲ�����
 *
 * std::unique_lock �Ķ�����Զ�ռ����Ȩ��û�������� unique_lock ����ͬʱӵ��ĳ�� mutex
 * ���������Ȩ���ķ�ʽ���� mutex �����ϵ������ͽ����Ĳ���
 * 
 * std::condition_variable::wait �����ʹ�� std::unique_lock ��Ϊ��
 * ��
 */

int v = 1;

void critical_section(int change_v) {
    static std::mutex mtx;
    std::unique_lock<std::mutex> lock(mtx);

    v = change_v;
    std::cout << v << std::endl;

    lock.unlock();
    //�ڴ��ڼ䣬�κ��˶��������� v �ĳ���Ȩ
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


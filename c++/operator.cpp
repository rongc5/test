#include <iostream>
#include <string>
#include <sstream>

using namespace std;

class Test
{
    public:
        Test()
        {
            cout<<"Constructors\n";
        }
        void operator()()
        {
            cout << "operator\n";
            return;
        }
        void operator()(const string & str)
        {
            cout << "str:" <<str.c_str()<<'\n';
        }

#if 1
        template <typename T>
            Test& operator<<(const T& data) {
                // 将数据写入日志文件和标准输出
            ss << data;
            //cout << data;
            return *this;
        }

#endif
        void show()
        {
            cout << str();
        }
        string str()
        {
            return ss.str();
        }
    private:
        stringstream ss;
};


int main(int argc, char *argv[])
{
    Test t;
    
    t();

    //Test();

    //t("hello world");

    t << "123" << ":" << "hello" << '\n';
    //cout << t.str();
    //cout << t;

    return 0;
}

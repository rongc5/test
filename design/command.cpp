#include <iostream>

using namespace std;

class executor
{
    public:
        void display1()
        {
            cout << "hello:1\n";
        }
        void display2()
        {
            cout << "hello:2\n";
        }
};

class cmd
{
    public:
        virtual void exe()=0;
        virtual ~cmd(){}
};

class cmd1: public cmd
{
    public:
        cmd1(executor *exe):_ex(exe){}
        virtual void exe()
        {
            _ex->display1();
        }
    private:
        executor * _ex;
};

class cmd2: public cmd
{
    public:
        cmd2(executor *exe):_ex(exe){}
        virtual void exe()
        {
            _ex->display2();
        }
    private:
        executor * _ex;
};

class client
{
    public:
        void set(cmd * ptr)
        {
            _cmd = ptr;
        }

        void play()
        {
            _cmd->exe();
        }

    private:
        cmd * _cmd;
};

int main(int argc, char *argv[])
{
    executor * p_exer = new executor;

    cmd * p_cmd1 = new cmd1(p_exer);
    cmd * p_cmd2 = new cmd2(p_exer);

    client * p_client = new client;
    p_client->set(p_cmd1);
    p_client->play();

    p_client->set(p_cmd2);
    p_client->play();

    return 0;
}

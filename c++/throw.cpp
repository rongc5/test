#include <iostream>
#include <string>

using namespace std;

class MyException:public exception
{
    private:
        string _str;
    public:
        MyException (const char * str) throw():_str(str){};
        virtual const char * what() const throw();
        virtual ~MyException()throw(){}
};

const char * MyException::what() const throw()
{
    return _str.c_str();
}

int main(int argc, char *argv[])
{
    try {
        throw MyException("hello world");
    }
    catch (exception & e){
        cout << e.what() <<"\n";
    //catch (...){
        //cout << e.what() <<"\n";
        //cout << "hello world\n";
        throw;
    }

    return 0;
}

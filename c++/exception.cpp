#include <iostream>
#include <string>

using namespace std;

class Test: public std::exception
{
    private:
        string str;
    public:
        Test(const char * cstr);
        virtual ~Test() throw ();
        virtual const char * what() const throw();
};

Test::Test(const char * cstr)
{
    str.append(cstr);
}

Test::~Test() throw ()
{
    cout << "destory\n";
}

const char * Test::what() const throw()
{
    return str.c_str();
}

int main(int argc, char *argv[])
{
   try {
       //Test t("hello");
       //throw t;
       throw Test("this is a Test!"); 
   }catch (std::exception  & e){
       cout << e.what() << '\n';
   }catch (...) {
        cout << "hello world\n";
   }

    return 0;
}

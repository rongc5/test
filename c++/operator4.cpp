#include <iostream>
#include <string>
#include <sstream>

using namespace std;

class MyClass {
    private:
        int myInt;
    public:
        MyClass(int n) : myInt(n) {}
        friend stringstream& operator<<(stringstream& os, const MyClass& obj);
};

stringstream& operator<<(stringstream& os, const MyClass& obj) {
        os << obj.myInt;
            return os;
}


int main(int argc, char *argv[])
{
    MyClass m(1);

    stringstream ss;
    
    ss << m << MyClass(3);

    cout << ss.str();

    return 0;
}

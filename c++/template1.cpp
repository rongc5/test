#include <iostream>

using namespace std;

template <class _T>
class compare
{

};

template <>
class compare<int>
{
    public:

    bool operator()(int arg1, int arg2)
    {
        if (arg1 < arg2){
            cout << "int true\n";
        }else {
            cout << "int false\n";
        }

        return false;
    }


};

template <>
class compare<float>
{
    public:
    bool operator()(float arg1, float arg2)
    {
        if (arg1 -arg2 > 0.000001 || arg1 - arg2 < -0.000001 ){
            cout << "float true\n";
        }else {
            cout << "float false\n";
        }

        return false;
    }

};


int main(int argc, char *argv[])
{
    compare<int> c;

    c(1, 3);

    compare<int>()(3,1);
    

    compare<float>()(1.0f,1.1f);
    return 0;
}

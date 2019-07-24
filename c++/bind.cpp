#include <iostream>
#include <functional>
#include <memory>
#include <string>
#include <map>

using namespace std;

typedef std::function<void(int a,int b, int c)> func;

class Func
{
    public:
        static void  func6(int numa, int numb, int numc)
        {
            std::cout << numa << " " << numb << " " << numc << endl;
        }

        void func2(int numa, int numb, int numc,std::string name)
        //void func2(int numa, int numb, int numc)
        {
            std::cout << numa << " " << numb << " " << numc<<" " << name<< endl ;
            //std::cout << numa << " " << numb << " " << endl ;
        }
};


int main(int argc, char *argv[])
{
    using namespace std::placeholders;

    map<string, func> mm;

    //Func fc;
    std::shared_ptr<Func> fc(new Func);

    func call = bind(&Func::func2, fc, _1, _2, _3, "hello world");
    mm["mingz"] = call;

    mm["mingz"](6, 7, 8);

    return 0;
}

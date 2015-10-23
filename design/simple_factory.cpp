#include <iostream>
#include <memory>
#include <string>

using namespace std;

class tv{
    public:
        tv(){}
        virtual ~tv(){}
        virtual void play()=0;
};

class changhong_tv:public tv
{
    public:
        changhong_tv(){
            cout << "this is changhong_tv!\n";
        }
        virtual ~changhong_tv() {
            cout << "changhong_tv close\n";
        }
        virtual void play(){
            cout << "now, this is changhong_tv playing\n";
        }
};

class haier_tv:public tv
{
    public:
        haier_tv(){
            cout << "this is haier_tv!\n";
        }
        virtual ~haier_tv() {
            cout << "haier_tv close!!!\n";
        }
        virtual void play() {
            cout << "now, this is haier_tv playing\n";
        }
};

class factory
{
    public:
        static shared_ptr<tv> get_instanse(const string & str);
};

shared_ptr<tv> factory::get_instanse(const string & str)
{
    if (str == "changhong"){
        return shared_ptr<tv>(new changhong_tv());
    }else if (str == "haier"){
        return shared_ptr<tv>(new haier_tv());
    }
    
    return NULL;
}


int main(int argc, char *argv[])
{
    shared_ptr<tv>  tv_instanse = factory::get_instanse("haier"); 

    tv_instanse->play();

    return 0;
}

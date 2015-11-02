#include <iostream>

using namespace std;

class Tv
{
    public:
        virtual void play()=0;
        virtual ~Tv(){}
};

class HaierTv:public Tv
{
    public:
        virtual void play()
        {
            cout << "this is haier tv\n";
        }
};

class ChangHongTv:public Tv
{
    public:
        virtual void play()
        {
            cout << "this is changhong tv\n";
        }
};

class Factory
{
    public:
        virtual Tv * create()=0;
        virtual ~Factory(){}
};

class HaierTvFactory:public Factory
{
    public:
        virtual Tv * create()
        {
            return new HaierTv();
        }
        virtual ~HaierTvFactory(){}
};

class ChangHongTvFactory:public Factory
{
    public:
        virtual Tv * create()
        {
            return new ChangHongTv();
        }
        virtual ~ChangHongTvFactory(){}
};

int main(int argc, char *argv[])
{
    
    Factory * tv_factory = new HaierTvFactory();
    tv_factory->create()->play();

    Factory * factory = new ChangHongTvFactory();
    factory->create()->play();

    return 0;
}

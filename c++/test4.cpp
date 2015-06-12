#include <iostream>
#include <typeinfo>

using std::cout;
using std::endl;

class Trouble {
    public:
        Trouble(const char * Str = "There's a Problem");
        virtual ~Trouble();
        virtual const char * what() const;
    private:
        const char *PMessage;
};

class MoreTrouble : public Trouble {
    public:
        MoreTrouble(const char * pStr = "There's more Trouble");
};

class BigTrouble : public MoreTrouble {
    public:
        BigTrouble (const char * pStr = "Really big Trouble");
};

Trouble::Trouble(const char *pStr):PMessage(pStr){}
Trouble::~Trouble(){}
const char *Trouble::what()const{
    return PMessage;
}

MoreTrouble::MoreTrouble(const char *pStr):Trouble(pStr){}

BigTrouble::BigTrouble(const char *pStr):MoreTrouble(pStr){}

int main()
{
    Trouble trouble;
    MoreTrouble moreT;
    BigTrouble bigT;

    cout<<endl;

    for (int i = 0; i < 7; i++){
        try{
            if (i < 3)
                throw trouble;
            if (i < 5)
                throw moreT;
            else
                throw bigT;
        }catch(Trouble & rT){
            cout << typeid(rT).name()<< rT.what() << " i = " << i<< endl;
        }

    }

    return 0;
}

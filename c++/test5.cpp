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

Trouble::Trouble(const char *pStr):PMessage(pStr){cout << " I am here!" << endl;}
Trouble::~Trouble(){cout << "I am not here!" << endl;}
const char *Trouble::what()const{
    return PMessage;
}

MoreTrouble::MoreTrouble(const char *pStr):Trouble(pStr){}

BigTrouble::BigTrouble(const char *pStr):MoreTrouble(pStr){}

int main()
{
    {
        Trouble *ptr = new Trouble();
        delete ptr;
    }
    return 0;
}

#ifndef __NEST__H_
#define __NEST__H_

class A
{
    public:
        A(const char *ptr);
        ~A();
        void show();
    private:
        class B;
        B * m_b;
};


#endif

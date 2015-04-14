#include <iostream>

struct base{
int a, b;
base(){
    a = 0;
    b = 0;
}

base(int m, int n):a(m), b(n){}
void show(){
    std::cout << a << " "<< b << std::endl;
}

void set_a(int m){
    this->a = m;
}

void set_b(int n)
{
    this->b = n;
}

};


int main(int argc, char *argv[])
{
    base mm;
    mm.show();


    base nn(9, 6);
    nn.show();

    return 0;
}

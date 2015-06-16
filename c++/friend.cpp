#include <iostream>

using namespace std;

class Base
{
    public:
        Base():x(0), y(0){}
        Base(int a, int b):x(a), y(b){}
    protected:
        int x, y;
};

class Node:public Base
{
    public:
        Node():Base(){}
        Node(int a, int b):Base(a, b){}
    private:
        friend void show(Node & node);
};

void show(Node &node)
{
    std::cout << "x = " << node.x << " y= "<< node.y <<'\n';
}

int main(int argc, char *argv[])
{
    //Node node(3, 5);
    Node node;

    show(node);
    
    return 0;
}



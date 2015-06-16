#include <iostream>

using namespace std;

class Node
{
    public:
        Node():x(0), y(0){}
        Node(int a, int b):x(a), y(b){}
    private:
        friend void show(Node & node);
        int x, y;
};

void show(Node &node)
{
    std::cout << "x = " << node.x << " y= "<< node.y <<'\n';
}

int main(int argc, char *argv[])
{
    Node node(3, 5);

    show(node);
    
    return 0;
}



#include "nest.h"

int main(int argc, char *argv[])
{
    A * a = new A("hello world");
    a->show();
    return 0;
}

#include <iostream>

auto add(int x, int y) 
{
    return x+y;
}


template<typename T, typename U>
auto add3(T x, U y){
    return x + y;
}

int main(int argc, char *argv[])
{
    
    std::cout << add(1,3) << std::endl;
    auto w = add3<int, double>(1, 2.0);
    if (std::is_same<decltype(w), double>::value) {
        std::cout << "w is double: "; 
    }
    
    return 0;
}

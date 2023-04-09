#include <iostream>
#include <memory>
#include <utility>

void lambda_expression_capture()
{
    auto important = std::make_unique<int>(1);
    auto add = [v1 = 1, v2 = std::move(important)](int x, int y) -> int {
        return x+y+v1+(*v2);
    };
    std::cout << add(3,4) << std::endl;
}

int main(int argc, char *argv[])
{
    lambda_expression_capture();

    return 0;
}

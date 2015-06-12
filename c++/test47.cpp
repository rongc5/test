#include <msgpack.hpp>
#include <vector>
#include <string>
#include <iostream>

int main(int argc, char *argv[])
{
    std::vector<std::string> test;
    test.push_back("hello");
    test.push_back("world");

    msgpack::sbuffer _sbuffer;
    msgpack::pack(_sbuffer, test);
    std::cout << _sbuffer.data() << std::endl;

    msgpack::unpacked msg;
    msgpack::unpack(&msg, _sbuffer.data(), _sbuffer.size());
    msgpack::object obj = msg.get();
    std::cout << obj << std::endl;

    std::vector<std::string> Rtest;
    obj.convert(&Rtest);

    for (size_t i = 0; i < Rtest.size(); ++i)
        std::cout << Rtest[i] << std::endl;

    return 0;
}

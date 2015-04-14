#include <msgpack.hpp>
#include <vector>
#include <string>

class my_class
{
private:
	std::string my_string;
	std::vector<int> vec_int;
	std::vector<std::string> vec_string;
public:
	MSGPACK_DEFINE(my_string, vec_int, vec_string);
};

int main()
{
	std::vector<my_class> my_class_vec;

	// add some data

	msgpack::sbuffer buffer;
	msgpack::pack(buffer, my_class_vec);

	msgpack::unpacked msg;
	msgpack::unpack(&msg, buffer.data(), buffer.size());

	msgpack::object obj = msg.get();
	std::vector<my_class> my_class_vec_r;
	obj.convert(&my_class_vec_r);

	return 0;
}

#include <msgpack.hpp>
#include <vector>
#include <string>
#include <iostream>

int main()
{
	// msgpack stream

	// use msgpack::packer to pack multiple objects.
	msgpack::sbuffer buffer_;
	msgpack::packer pack_(&buffer_);
	pack_.pack(std::string("this is 1st string"));
	pack_.pack(std::string("this is 2nd string"));
	pack_.pack(std::string("this is 3th string"));

	// use msgpack::unpacker to unpack multiple objects.
	msgpack::unpacker unpack_;
	unpack_.reserve_buffer(buffer_.size());
	memcpy(unpack_.buffer(), buffer_.data(), buffer_.size());
	unpack_.buffer_consumed(buffer_.size());

	msgpack::unpacked result_;
	while (unpack_.next(&result_))
	{
		std::cout << result_.get() << std::endl;
	}

	return 0;
}

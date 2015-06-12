#include <msgpack.hpp>

using namespace std;

class pack_util
{
public:
	pack_util()
	{

	};
	~pack_util()
	{

	}

public:
	
	template <typename T,typename S>
	static int pack(string& str, const T& head , const S& body)
	{
	
		//WRITE_TRACE("pack_util::pack;str:%s;",str);
		//write log head and body msg tostring
		msgpack::sbuffer buffer;
		msgpack::pack(buffer, head);
		msgpack::pack(buffer, body);
		str = buffer.data();
		return 0;
	}

	template <typename T>
	static int pack(string& str, const T& struct_obj)
	{
		msgpack::sbuffer buffer;
		msgpack::pack(buffer, struct_obj);
		str = buffer.data();
		return 0;
	}
	
	template <typename T>
	static int un_pack(const string& str, T& struct_obj, size_t& off_set)
	{
		//WRITE_TRACE("pack_util::un_pack;str:%s;off_set:%d",str,off_set);
		//write log unpacked msg tostring
		msgpack::unpacked msg;
		msgpack::unpack(&msg, str.c_str(), str.length(), &off_set);
		msgpack::object obj = msg.get();
		obj.convert(&struct_obj);
		return 0;
		
	}


	template <typename T>
	static int un_pack(const char* buf, int len, T& struct_obj, size_t& off_set)
	{
		msgpack::unpacked msg;
		msgpack::unpack(&msg, buf, len, &off_set);
		msgpack::object obj = msg.get();
		obj.convert(&struct_obj);
		return 0;
	}



};

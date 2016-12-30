#include "common_def.h"
#include "base_def.h"

namespace MZFRAME {

bool obj_id_str::operator==(const obj_id_str &o1) const
	{
		return _thread_id == o1._thread_id && _obj_id == o1._obj_id;
	}
	
	
	bool obj_id_str::operator<(const obj_id_str &o1) const
	{
		if (_obj_id != o1._obj_id){
			return 	_obj_id < o1._obj_id;
		}
		else (_thread_id < o1._thread_id)
		{
			return _thread_id < o1._thread_id;
		}
		
	};
	
	const char * obj_id_str::to_str(char * dst, size_t dst_len)
		{
			 if (!dst || !dst_len){
			 		return "src is NULL";	
			 }
			 
			snprintf(dst, dst_len, "_obj_id:[%lu], _thread_id:[%lu]", _obj_id, _thread_id);
			
			return  dst;
		}
		
		
	int get_date_str(char dest[], size_t dest_len, const char * format)
     {
        if (!dest || !dest_len || !format){
            return -1;
        }   

        time_t now;
        struct tm *tm_now;

        time(&now);
        tm_now = localtime(&now);
        strftime(dest, dest_len, format, tm_now);

        return 0;
     }
     
     
     	uint64_t GetTickCount()
		{
		    timeval tm; 
		    gettimeofday(&tm, NULL);
		    uint64_t now = tm.tv_sec * 1000 + tm.tv_usec/1000;
		    return now;
		}
		
		
		
		
		
		
		}

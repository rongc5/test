#ifndef __UTIL_H__
#define __UTIL_H__



namespace MZFRAME{


    class utils{
    
        public:
            static int get_date_str(char dest[], size_t dest_len, const char * format);
						static uint64_t utils::GetTickCount();	
    };

		// to string ·½·¨
		class to_string {
			public:
				to_string(){}
				virtual ~to_string(){}
				virtual const char * to_str(char * dst, size_t dst_len)=0;				
		};
		
		
		

}


#endif

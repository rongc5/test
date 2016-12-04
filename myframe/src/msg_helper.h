#ifndef __MSG_HELPER_H__
#define __MSG_HELPER_H__



namespace MZFRAME {
	
struct obj_id_str;

typedef struct _nshead_t
{
     struct obj_id_str obj_str;
     unsigned short version; //
     unsigned int reserved; //±£Áô×Ö¶Î
     unsigned int body_len;
} nshead_t;
	
	
	
	
}

#endif
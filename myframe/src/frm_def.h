#ifndef __FRM_DEF_H__
#define __FRM_DEF_H___

#include <stdint.h>

static const uint32_t SIZE_LEN_32 = 32; 
static const uint32_t SIZE_LEN_64 = 64; 

#define ASSERT(valid, PRINT) \
    do{ \
        if (!(valid)) { \
            PRINT; \
            return -1; \
        } \
    }while(0)



#endif

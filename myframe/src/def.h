#ifndef __DEF_H__
#define __DEF_H__

#include <stdint.h>

static const uint32_t SIZE_LEN_16 = 16; 
static const uint32_t SIZE_LEN_32 = 32; 
static const uint32_t SIZE_LEN_64 = 64; 
static const uint32_t SIZE_LEN_128 = 128; 
static const uint32_t SIZE_LEN_256 = 256; 

#define ASSERT(valid, PRINT) \
    do{ \
        if (!(valid)) { \
            PRINT; \
            return -1; \
        } \
    }while(0)



#endif

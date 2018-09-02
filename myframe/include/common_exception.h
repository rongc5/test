#ifndef  __COMMON_EXCEPTION_H
#define  __COMMON_EXCEPTION_H

#include "common_def.h"

class CMyCommonException : public std::exception 
{
    public:

        CMyCommonException(const std::string &err_str)
        {
            _errstr = err_str;
        }
        
        virtual ~CMyCommonException() throw(){};    

    public:
        virtual const char* what() const throw()
        {
            return _errstr.c_str();
        }

    protected:
        std::string _errstr;
};


#define THROW_COMMON_EXCEPT(errorstr) \
do { \
    std::stringstream ss; \
    ss << errorstr; \
    throw CMyCommonException(ss.str());\
} while (0)

#endif


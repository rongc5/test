#ifndef  __COMMON_EXCEPTION_H
#define  __COMMON_EXCEPTION_H

#include "common_def.h"

class CMyCommonException : public std::exception 
{
    public:

        CMyCommonException(const std::string &sErrStr)
        {
            m_sErrStr = sErrStr;
        }

        virtual ~CMyCommonException() throw(){};    

    public:
        virtual const char* what() const throw()
        {
            return m_sErrStr.c_str();
        }

    protected:
        std::string			m_sErrStr;		
};


#define THROW_COMMON_EXCEPT(errorstr) \
do { \
    std::stringstream ss; \
    ss << errorstr; \
    throw CMyCommonException(ss.str());\
} while (0)

#endif


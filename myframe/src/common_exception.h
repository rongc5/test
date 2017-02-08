#ifndef  __COMMON_EXCEPTION_H
#define  __COMMON_EXCEPTION_H

#include "common_def.h"
#include "log_helper.h"


class CMyCommonException : public std::exception 
{
    public:

        CMyCommonException(const string &sErrStr)
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
        string			m_sErrStr;		
};

#define THROW_COMMON_EXCEPT(errorstr) \
do { \
    stringstream ss; \
    ss << errorstr; \
    {\
        LOG_WARNING("throw exception error %s",  ss.str().c_str());\
    }\
    throw CMyCommonException(ss.str());\
} while (0)

#endif


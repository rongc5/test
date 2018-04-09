#ifndef __MY_EXCEPTION_H
#define  __MY_EXCEPTION_H
#include <sys/types.h>
#include <string>
using namespace std;
namespace MYFRAMECOMMON{

/**
* dec:
* ClassName:CMyException
* author: 
* version:
*CreateTime: 2006.11.25
*/
class CMyException:public std::exception
{
public:
        CMyException(const std::string &sErrMsg)
        {
                m_sErrMsg = sErrMsg;
        }
        ~CMyException()throw()
        {
                
        }
        
        std::string &GetErrMsg()
        {
                return m_sErrMsg;
        }

		const char *what() const throw()
		{
			return m_sErrMsg.c_str();
		}
protected:
        std::string m_sErrMsg;
};

class CMyCommonException : public std::exception 
{
public:
	CMyCommonException(const int32_t uiErrCode, const std::string &sErrStr)
	{
		m_uiErrCode = uiErrCode;
		m_sErrStr = sErrStr;
	}
	virtual ~CMyCommonException() throw(){};    

public:
	virtual const char* what() const throw()
	{
		return m_sErrStr.c_str();
	}

	int32_t getErrCode()
	{
		return m_uiErrCode;
	}

protected:
	int32_t			m_uiErrCode;
    std::string			m_sErrStr;		
};

#define THROW_COMMON_EXCEPT(errorid, errorstr) \
	{\
        std::stringstream ss; \
		ss << errorstr; \
		if (errorid < 100) \
		{\
			WRITE_ERROR("throw exception, %s:%d, error %s", __FILE__, __LINE__,  ss.str().c_str());\
		}\
		else\
		{\
			WRITE_DEBUG("throw exception, %s:%d, error %s", __FILE__, __LINE__,  ss.str().c_str());\
		}\
		throw CMyCommonException(errorid, ss.str());\
	}


}
#endif


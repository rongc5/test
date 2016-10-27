#ifndef __SINGLETON_BASE_H_
#define __SINGLETON_BASE_H_

namespace MZFRAME {
template<class T>
class singleton_base
{
public:
	static T *get_instance()
	{		
		return _p_instance;
	}
	static void set_instance(T *instance)
	{     
		_p_instance = instance;
	}
	static void del_instance()
	{
		if(NULL != _p_instance)
		{
			delete _p_instance;
			_p_instance = NULL;
		}
	}
private:
	static T *_p_instance;	
};
template<class T>
T* singleton_base<T>::_p_instance = NULL;

}
#endif


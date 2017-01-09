#ifndef __BASE_SINGLETON_H__
#define __BASE_SINGLETON_H__

namespace MZFRAME {
template<class T>
class base_singleton
{
public:
	static T *get_instance()
	{		
		return _p_instance;
	}
	static void set_instance(T *instance)
	{     
		if(NULL != _p_instance)
		{
			delete _p_instance;
			_p_instance = NULL;
		}

		_p_instance = instance;
	}
private:
	static T *_p_instance;	
};
template<class T>
T* base_singleton<T>::_p_instance = NULL;

}
#endif


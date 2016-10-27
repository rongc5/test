#ifndef __BASE_THREAD_HPP_
#define __BASE_THREAD_HPP_
#include <pthread.h>
#include <vector>

namespace MZFRAME {

using namespace std;

class base_thread
{
	public:
		base_thread():_thread_id(0), _run_flag(true)
		{
		}
		
		virtual ~base_thread()
		{
		}
		
		virtual bool start()
		{
			int ret = pthread_create(&_thread_id, NULL, base_thread_proc, this);
			if (ret != 0)
			{
				WRITE_ERROR("start thread fail");
				return false;
			}
			return true;
		}

		void join_thread()
		{
			pthread_join(_thread_id, NULL);
		}
		
		virtual bool stop() 
		{
			_run_flag = false;
			return true;
		}
		
		virtual void* run() = 0;
		
		static void *base_thread_proc(void *arg)
		{
			base_thread *p = (base_thread*)arg;
			return p->run();
		}
		
		bool get_run_flag()
		{
			return _run_flag;
		}

		void set_run_flag(bool flag)
		{
			_run_flag = flag;
		}

		pthread_t get_thread_id()
		{
			return _thread_id;
		}
	protected:
		pthread_t _thread_id;
		bool _run_flag;
};

}
#endif


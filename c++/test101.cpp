#include <iostream>
#include <pthread.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>

using namespace std;

class base_thread
{
	public:
		virtual bool start()
		{
			int ret = pthread_create(&_thread_id, NULL, base_thread_proc, this);
			if (ret != 0)
			{
				return false;
			}
			return true;
		}
		
		virtual void* run() = 0;
		
		static void *base_thread_proc(void *arg)
		{
			base_thread *p = (base_thread*)arg;
			return p->run();
		}
		protected:
		pthread_t _thread_id;
};

class common_thread_obj: public base_thread
{
	public:
	virtual void * run()
	{
		while (1)
		{
			printf("hello world : %lu\n", _thread_id);
			sleep(3);
		}
	}
};

int main(int argc, char *argv[])
{
   common_thread_obj pid;
   pid.start();
   
   while (1)
		pause();
    
    return 0;
}

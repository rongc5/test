#ifndef __SK_UTIL_H__
#define __SK_UTIL_H__

#include "common_util.h"
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>

void get_standard_id(std::string & id);


class default_signal_process
{
    public:
        static void sighandler(int signum)
        {
            sigset_t block_mask;

            if (signum == SIGCHLD)
            {
                int errno2, result; 
                pid_t deadpid;
                errno2 = errno;
                while((deadpid = waitpid(0, &result, WNOHANG)) > 0)
                {
                }
                errno = errno2;
                signal(SIGCHLD, sighandler);
            }
            else if (signum == SIGUSR1)
            {
                signal(SIGUSR1, sighandler);
            }
            else if ( signum == SIGINT || signum == SIGTERM)
            {
                sigemptyset( &block_mask );
                sigaddset( &block_mask, SIGINT );
                sigaddset( &block_mask, SIGQUIT );
                sigaddset( &block_mask, SIGTERM );
                sigaddset( &block_mask, SIGCHLD );
                sigaddset( &block_mask, SIGIO );
                sigaddset( &block_mask, SIGPIPE );
                sigaddset( &block_mask, SIGHUP );
                sigaddset( &block_mask, SIGSEGV);
                sigprocmask( SIG_BLOCK, &block_mask, NULL );                
            }
        }
};

template<class T>
class signal_process
{
    public: 
        void operator()()
        {
            signal( SIGIO, SIG_IGN);
            signal( SIGPIPE, SIG_IGN);
            signal( SIGTERM, T::sighandler);
            signal( SIGHUP,  T::sighandler);
            signal( SIGCHLD, T::sighandler);
            signal( SIGUSR1, T::sighandler);
            signal( SIGINT,  T::sighandler); 
        }           
};


std::string float_2_str(float a, int decimal = 2);
std::string int_2_str(int a);

void get_union(std::vector<std::set<std::string> > & arr, std::set<std::string> &res);

void get_intersection(std::vector<std::set<std::string> > & arr, std::set<std::string> &res);


#endif

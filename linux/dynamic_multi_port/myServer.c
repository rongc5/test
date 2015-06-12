#include "SrandPort.h"

#define CLIENTPORT 8888
#define SERVERPORT 9999

int main(void)
{
    int i, ret, serverport;
    int pid_num = PORT_NUM;
    pid_t pid;
    int tmp = 0;
    int IsFirst = 1;
	double curtimeval, nexttimeval;
    Prolib_t *lib = create_dynamic_process_libs(pid_num);

    struct timeval CurPval, NextPval;
	
	Init_TimerValue(&curtimeval, &nexttimeval);
	

    while (1)
    {
        serverport = generate_dynamic_RandPort();
        for (i = 0; i < PORT_NUM; i++)
        {	
            tmp = serverport + i;
            assign_job(lib, i, tmp);
        }

#if DEBUG
		printf("task asign over!\n");
#endif

        if (IsFirst == 1)
        {
#if DEBUG
		CurPval.tv_sec = 30;
#else
    	CurPval.tv_sec = curtimeval;
#endif
    	CurPval.tv_usec = 0;
            select(0, NULL, NULL, NULL, &CurPval);
            IsFirst = 0;
        }
        else if (IsFirst == 0)
        {	
#if DEBUG
			NextPval.tv_sec = 60;
#else
    		NextPval.tv_sec = nexttimeval;
#endif   
   	 		NextPval.tv_usec = 0;

            select(0, NULL, NULL, NULL, &NextPval);
        }
    }

    return 0;
}

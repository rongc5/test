#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/eventfd.h>
#include <pthread.h>
#include <unistd.h>

 
int efd;

void *readtid(void * arg)
{
	uint64_t count = 0;
	int ret, i;
	//sleep(1);
	    for (;i < 5; i++){
                ret = read(efd, &count, sizeof(count));
                if (ret == -1){
                        perror("read");
                        return NULL;
                }
                //count++;
                printf("%llu\n", count);
		count = 0;
        }
	
	return NULL;
}

int main(int c, char **v)
{
	efd = eventfd(0, 0);
	uint64_t count = 0;	
	int ret;
	pthread_t tid;

	ret = pthread_create(&tid, NULL,readtid, NULL);
	int i = 0;

	for (;i < 5; i++){
		ret = write(efd, &count, sizeof(count));
		if (ret == -1){
			perror("write");
			return -1;
		}
		count++;
		//printf("%d\n", count);
		if (count > 1000)
			break;
	}
	pthread_join(tid, NULL);	
    
    close(efd);
	return 0;
}

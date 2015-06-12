#include <stdio.h>
#include <string.h>
#include <aio.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <signal.h>

void async_process(sigval_t val)
{
	struct aiocb *cbp = (struct aiocb *)(val.sival_ptr);
	
	printf("read:%s\n", (char *)cbp->aio_buf);	
}

int main(void)
{
	struct aiocb cb;
	char buf[256] = {0};
	int ret;

	memset(&cb, 0, sizeof(cb));
	cb.aio_fildes = 0;
	cb.aio_nbytes = 256;
	cb.aio_buf = buf;
	cb.aio_sigevent.sigev_value.sival_ptr = &cb;
	cb.aio_sigevent.sigev_notify = SIGEV_THREAD;
	cb.aio_sigevent.sigev_notify_attributes = NULL;
	cb.aio_sigevent.sigev_notify_function=async_process;

	ret = aio_read(&cb);
	if (-1 == ret)
	{
		perror("aio_read");
		exit(1);
	}	
	
	printf("Aio read task!\n");

	while (1)
	{
		sleep(1);
		//	printf("I am running!\n");
	}

	return 0;
}

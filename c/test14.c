#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/syscall.h>

int main(int argc, char *argv[])
{
    printf("%lu\n", pthread_self());

    return 0;
}


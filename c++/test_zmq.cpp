#include "asp_tool.h" 
#include <pthread.h>
#include <unistd.h>
#include <time.h>


void * server(void *)
{
    zmq_base * ser = NULL;

    ser = new zmq_server();
//ser->init("/mnt/hgfs/D/tmp");
    ser->init("tcp://*:5555");

    char buf[512];

    while (1)
    {
        int ret = ser->do_recv(buf, sizeof(buf));

        buf[ret] = '\0';

        printf("do_recv: %s\n", buf);
    }
}

void * client(void *)
{
    zmq_base * ser = NULL;

    ser = new zmq_client();
    ser->init("tcp://localhost:5555");

    char buf[512];

    while (1)
    {
        sprintf(buf, "%s", "hello world");
        printf("do_send: %s\n", buf);
        int ret = ser->do_send(buf, strlen(buf));

        sleep(3);
    }
}



int main(int argc, char *argv[])
{
    pthread_t s_id, c_id;
    pthread_create(&s_id, NULL, server, NULL);
    
    pthread_create(&c_id, NULL, client, NULL);

    pthread_join(s_id, NULL), pthread_join(c_id, NULL);


    return 0;
}


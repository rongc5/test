#include <mpg123.h>
#include <linux/soundcard.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <errno.h>
#include <poll.h>

#define SERVERADDR "192.168.1.146"
#define SERVERPORT 8080

#define RATE 44100
#define CHANNELS 1
#define FMTS 16
#define FRAMESIZE RATE*FMTS*(CHANNELS+1)/8

//pthread_mutex_t g_mutex = PTHREAD_MUTEX_INITIALIZER;

typedef struct music_data
{
    char buf[FRAMESIZE];
    struct music_data * prev;
    struct music_data * next;
}M_DATA;

int init_output(void)
{
    int fd, arg, ret;

    fd = open("/dev/dsp", O_RDWR);
    if (-1 == fd)
    {
        perror("open:dsp");
        return -1;
    }

    arg = RATE;
    ret = ioctl(fd, SNDCTL_DSP_SPEED, &arg);
    if (-1 == ret)
    {
        perror("set write bites");
        return -1;
    }

    if (arg != RATE)
    {
        perror("unable to set rate!");
        return -1;
    }

    arg = CHANNELS;
    ret = ioctl(fd, SNDCTL_DSP_STEREO, &arg);
    if (-1 == ret)
    {
        perror("set channels fail!");
        return -1;
    }

    if(arg != CHANNELS)
    {
        perror("unable to  set channels");
        return -1;
    }

    arg = FMTS;
    ret = ioctl(fd, SNDCTL_DSP_SETFMT, &arg);
    if (-1 == ret)
    {
        perror("set fmt fail!\n");
        return -1;
    }
    if (arg != FMTS)
    {
        perror("set format fail\n");
        return -1;
    }

    return fd;
}

mpg123_handle * init_mpg123_lib(void)
{
    unsigned char *audio;
    mpg123_handle *m;
    int ret, state;
    off_t len, num;
    size_t bytes;

    mpg123_init();
    m = mpg123_new(NULL, &ret);
    if (m == NULL)
    {
        fprintf(stderr, "unable to create mpg123 handle: %s\n",mpg123_plain_strerror(ret));
        return NULL;
    }

    ret = mpg123_open_feed(m);
    if (ret != MPG123_OK)
    {
        fprintf(stderr, "unable to create mpg123 feed: %s\n",mpg123_plain_strerror(ret));
        return NULL;
    }

    return m;
}

M_DATA * list_init(void)
{
    M_DATA * phead = (M_DATA *)malloc(sizeof(M_DATA));
    if (phead == NULL)
    {
        perror("malloc");
        exit(1);
    }

    memset(phead, 0, sizeof(*phead));
    phead->next = phead;
    phead->prev = phead;

    return phead;
}

void list_show(M_DATA * phead)
{
    M_DATA * pos;
    for (pos = phead->next; pos != phead; pos = pos->next)
        printf("%s\n", pos->buf);
}

void list_insert(M_DATA * phead, M_DATA * pnode)
{
    M_DATA * e = (M_DATA *)malloc(sizeof(M_DATA));
    if (NULL == e)
    {
        perror("malloc");
        exit(1);
    }
    M_DATA * next = phead->next;
    phead->next = e;
    e->prev = phead;
    e->next = next;
    next->prev = e;
}

void * thread_handler(void *arg)
{
    pthread_detach(pthread_self());
    int sd, fd, ret;
    struct sockaddr_in addr;
    socklen_t len;
    char buf[FRAMESIZE];
    struct pollfd fds;
    
    M_DATA *phead = (M_DATA *)arg;
    M_DATA * music_data;
    len = sizeof(addr);
    addr.sin_family = AF_INET;
    addr.sin_port = htons(SERVERPORT);
    inet_pton(AF_INET, SERVERADDR, &addr.sin_addr);
    
    sd = socket(AF_INET, SOCK_STREAM, 0);
    if (sd == -1)
    {
        perror("socket");
        exit(1);
    }
    
    ret = connect(sd, (struct sockaddr *)&addr, len);
    if (-1 == ret)
    {
        perror("connect");
        exit(1);
    }
    printf("connect success!\n");

    write(sd, buf, strlen(buf));

    fds.fd = sd;
    fds.events = POLLIN;

    while (1)
    {
        ret = poll(&fds, 1, 3000);
        if (0 == ret)
        {
            printf("transport end!\n");
            break;
        }
        ret = read(sd, buf, FRAMESIZE);
        if (0 == ret)
            break;

        strncpy(music_data.buf, buf, sizeof(FRAMESIZE));
        list_insert(phead, &music_data);
        write(1, ".....", 5);
        memset(buf, 0, sizeof(FRAMESIZE));
    }

    close(sd);
    pthread_exit(NULL);
}

int main(int c, char **v)
{
 
    int re;
    M_DATA * list = list_init();
    pthread_t tid;
    
    re = pthread_create(&tid, NULL, thread_handler, (void *)list);
    if (re) {
        fprintf(stderr, "pthread_create:%s\n", strerror(re));
        exit(1);
    }
    
    off_t ret, num;
    int len;
    if (c != 2)
    {
        fprintf(stderr, "usage...\n");
        exit(1);
    }

    size_t bytes;
    int fd, mfd;
    mpg123_handle *m;
    unsigned char *audio;
    char buf[FRAMESIZE];
    fd = init_output();
    if (-1 == fd)
        exit(1);
    m = init_mpg123_lib();
    if (m == NULL)
    {
        close(fd);
        return -1;
    }

   /* mfd = open(v[1], O_RDONLY);
    if (-1 == fd){
        perror("open");
        exit(1);
    }*/

    list = list->next;
    while (1)
    {
       /* ret = read(mfd, buf, FRAMESIZE);
        if (0 == ret)
        {
            printf("play music %s over!\n", v[1]);
            break;
        }*/
        strncpy(buf, list->buf, sizeof(FRAMESIZE));


        ret = mpg123_feed(m, buf, len);
        while (ret != MPG123_ERR && ret != MPG123_NEED_MORE)
        {
            ret = mpg123_decode_frame(m, &num, &audio, &bytes);
            write(fd, audio, bytes);
        }
        list = list->next;
    }

    return 0;
}

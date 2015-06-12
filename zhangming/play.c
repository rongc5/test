#include <mpg123.h>
#include <linux/soundcard.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>

#define RATE 44100
#define CHANNELS 1
#define FMTS 16
#define FRAMESIZE RATE*FMTS*(CHANNELS+1)/8


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

int main(int c, char **v)
{
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

    mfd = open(v[1], O_RDONLY);
    if (-1 == fd){
        perror("open");
        exit(1);
    }

    while (1)
    {
        ret = read(mfd, buf, FRAMESIZE);
        if (0 == ret)
        {
            printf("play music %s over!\n", v[1]);
            break;
        }
        ret = mpg123_feed(m, buf, len);
        while (ret != MPG123_ERR && ret != MPG123_NEED_MORE)
        {
            ret = mpg123_decode_frame(m, &num, &audio, &bytes);
            write(fd, audio, bytes);
        }
    }

    return 0;
}

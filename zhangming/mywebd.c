#include <stdio.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdlib.h>

#define FMT_EOT "\r\n"
#define SERVER_PORT 80
#define HTML_FILE "./myindex.html"
#define BUFSIZE 32

int main(void)
{
    int ret, sd, newsd, optval, cnt = 1;
    struct sockaddr_in myend, hisend;
    socklen_t hisend_len;
    FILE *ipcfp, *fp;
    char *linebuf = NULL, buf[BUFSIZE];
    size_t linelen;

    sd = socket(AF_INET, SOCK_STREAM, 0);
    if (-1 == sd)
    {
        perror("socket()");
        exit(1);
    }

    myend.sin_family = AF_INET;
    myend.sin_port = htons(SERVER_PORT);
    inet_pton(AF_INET, "0.0.0.0", &myend.sin_addr);

    optval = 1;
    ret = setsockopt(sd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval));
    if (-1 == ret)
    {
        perror("setsockopt()");
        fprintf(stderr, "Thus, bind() maybe fail.\n");
    }

    ret = bind(sd, (struct sockaddr *)&myend, sizeof(myend));
    if (-1 == ret)
    {
        perror("bind()");
        exit(1);
    }

    ret = listen(sd, 10);
    if (-1 == ret)
    {
        perror("listen()");
        exit(1);
    }

    hisend_len = sizeof(hisend);

    while (1)
    {
        newsd = accept(sd, (struct sockaddr *)&hisend, &hisend_len);
        if (-1 == newsd)
        {
            perror("accept");
            exit(1);
        }

        ipcfp = fdopen(newsd, "r+b");
        if (ipcfp == NULL)
        {
            perror("fdopen()");
            exit(1);
        }
        printf("accept ok!\n");
        while (1)
        {
            ret = getline(&linebuf, &linelen, ipcfp);
            if (strcmp(linebuf, FMT_EOT) == 0)
                break;
        }
        char * path;
        if (cnt == 1)
        {
            path = HTML_FILE;
        }else if (2 == cnt)
            path = "1.gif";
        else if (cnt == 3)
        {
            path = "1.mp3";
            cnt = 0;
        }
        cnt++;
        fp = fopen(path, "r");
        if (NULL == fp)
        {
            printf("fopen:%s\n", path);
            perror("fopen(HTML_FILE)");
            exit(1);
        }
        printf("open file %s\n", path);
        while (1)
        {
            ret = fread(buf, 1, BUFSIZE, fp);
            if (0 == ret)
            {
                printf("fread\n");
                break;
            }
            fwrite(buf, 1, ret, ipcfp);
            if (strcmp(path, "1.mp3") == 0)
            {
                printf("ret = %d\n", ret);
            }
        }
        fclose(ipcfp);
        fclose(fp);
    }
    close(sd);
    exit(0);
}

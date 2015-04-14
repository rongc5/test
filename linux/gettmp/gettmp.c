#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <errno.h>
#include <poll.h>
#include <sys/epoll.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>


#define SERVER_PORT 80
#define SERVER_IP "61.4.185.35"
#define SERVER_IP_M "113.108.239.107"


static const char *p =  "请按如下格式输入:\n"
                        "例如：./app 北京\n"
                        "或者: ./app -c 北京\n"
                        "或者: ./app -s 北京\n";

typedef struct cmd_parse{
    char cityid[32];
    char *city;
    void (*parse)(char *buf);
    void (*package)(char buf[], char * cityid);
}CMDPAR;

void dfl_parse(char *buf);
void dfl_package(char buf[], char *cityid);
void cin_parse(char *buf);
void cin_package(char buf[], char *cityid);
void sk_parse(char *buf);
void sk_package(char buf[], char * cityid);

void dfl_parse(char *buf)
{
    char *pos;
    int i = 0; 
    if (pos = strstr(buf, "city")) {
        i = 0;
        while (pos[7+i++] != ',');
        pos[7+i-2] = '\0';
        printf("城市:%s\n", pos+7);
        pos[7+i-2] = '\"';
    }
    if (pos = strstr(buf, "date_y")) {
        i = 0;
        while (pos[9+i++] != ',');
        pos[9+i-2] = '\0';
        printf("%s\n", pos+9);
        pos[9+i-2] = '\"';
    }
    if (pos = strstr(buf, "week")) {
        i = 0;
        while (pos[7+i++] != ',');
        pos[7+i-2] = '\0';
        printf("%s\n", pos+7);
        pos[7+i-2] = '\"';
    }
    if (pos = strstr(buf, "temp1")) {
        i = 0;
        while (pos[8+i++] != ',');
        pos[8+i-2] = '\0';
        printf("今天温度范围:%s\n", pos+8);
        pos[8+i-2] = '\"';
    }
    if (pos = strstr(buf, "temp2")) {
        i = 0;
        while (pos[8+i++] != ',');
        pos[8+i-2] = '\0';
        printf("明天温度范围:%s\n", pos+8);
        pos[8+i-2] = '\"';
    }
    if (pos = strstr(buf, "temp3")) {
        i = 0;
        while (pos[8+i++] != ',');
        pos[8+i-2] = '\0';
        printf("后天温度范围:%s\n", pos+8);
        pos[8+i-2] = '\"';
    }
    if (pos = strstr(buf, "weather1")) {
        i = 0;
        while (pos[11+i++] != ',');
        pos[11+i-2] = '\0';
        printf("今天天气:%s\n", pos+11);
        pos[11+i-2] = '\"';
    }
    if (pos = strstr(buf, "weather2")) {
        i = 0;
        while (pos[11+i++] != ',');
        pos[11+i-2] = '\0';
        printf("明天天气:%s\n", pos+11);
        pos[11+i-2] = '\"';
    }
    if (pos = strstr(buf, "weather3")) {
        i = 0;
        while (pos[11+i++] != ',');
        pos[11+i-2] = '\0';
        printf("后天天气:%s\n", pos+11);
        pos[11+i-2] = '\"';
    }
    if (pos = strstr(buf, "img_title1")) {
        i = 0;
        while (pos[13+i++] != ',');
        pos[13+i-2] = '\0';
        printf("今天白天:%s\n", pos+13);
        pos[13+i-2] = '\"';
    }
    if (pos = strstr(buf, "img_title2")) {
        i = 0;
        while (pos[13+i++] != ',');
        pos[13+i-2] = '\0';
        printf("今天夜间:%s\n", pos+13);
        pos[13+i-2] = '\"';
    }
    if (pos = strstr(buf, "img_title3")) {
        i = 0;
        while (pos[13+i++] != ',');
        pos[13+i-2] = '\0';
        printf("明天白天:%s\n", pos+13);
        pos[13+i-2] = '\"';
    }
    if (pos = strstr(buf, "img_title4")) {
        i = 0;
        while (pos[13+i++] != ',');
        pos[13+i-2] = '\0';
        printf("明天夜间:%s\n", pos+13);
        pos[13+i-2] = '\"';
    }
    if (pos = strstr(buf, "img_title5")) {
        i = 0;
        while (pos[13+i++] != ',');
        pos[13+i-2] = '\0';
        printf("后天白天:%s\n", pos+13);
        pos[13+i-2] = '\"';
    }
    if (pos = strstr(buf, "img_title6")) {
        i = 0;
        while (pos[13+i++] != ',');
        pos[13+i-2] = '\0';
        printf("后天夜间:%s\n", pos+13);
        pos[13+i-2] = '\"';
    }
}

void cin_parse(char *buf)
{
    char *pos;
    int i = 0;
    if (pos = strstr(buf, "city")) {
        while (pos[7+i++] != ',');
        pos[7+i-2] = '\0';
        printf("城市:%s\n", pos+7);
        pos[7+i-2] = '\"';
    }
    if (pos = strstr(buf, "temp1")) {
        i = 0;
        while (pos[8+i++] != ',');
        pos[8+i-2] = '\0';
        printf("最高温度:%s\n", pos+8);
        pos[8+i-2] = '\"';
    }
    if (pos = strstr(buf, "temp2")) {
        i = 0;
        while (pos[8+i++] != ',');
        pos[8+i-2] = '\0';
        printf("最低温度:%s\n", pos+8);
        pos[8+i-2] = '\"';
    }
    if (pos = strstr(buf, "weather\"")) {
        i = 0;
        while (pos[10+i++] != ',');
        pos[10+i-2] = '\0';
        printf("天气:%s\n", pos+10);
        pos[10+i-2] = '\"';
    }
    if (pos = strstr(buf, "ptime")) {
        i = 0;
        while (pos[8+i++] != '\"');
        pos[8+i-1] = '\0';
        printf("更新时间:%s\n", pos+8);
        pos[8+i-1] = '\"';
    }
}

void sk_parse(char *buf)
{
    char *pos;
    int i = 0;
    if (pos = strstr(buf, "city")) {
        while (pos[7+i++] != '\"');
        pos[7+i-1] = '\0';
        printf("城市:%s\n", pos+7);
        pos[7+i-1] = '\"';
    }
    if (pos = strstr(buf, "temp")) {
        i = 0;
        while (pos[7+i++] != '\"');
        pos[7+i-1] = '\0';
        printf("温度:%s\n", pos+7);
        pos[7+i-1] = '\"';
    }
    if (pos = strstr(buf, "WD")) {
        i = 0;
        while (pos[5+i++] != '\"');
        pos[5+i-1] = '\0';
        printf("风向:%s\n", pos+5);
        pos[5+i-1] = '\"';
    }
    if (pos = strstr(buf, "WS")) {
        i = 0;
        while (pos[5+i++] != ',');
        pos[5+i-2] = '\0';
        printf("风力:%s\n", pos + 5);
        pos[5+i-2] = '\"';
    }
    if (pos = strstr(buf, "SD")) {
        i = 0;
        while (pos[5+i++] != ',');
        pos[5+i-2] = '\0';
        printf("相对湿度:%s\n", pos + 5);
        pos[5+i-2] = '\"';
    }

}

int init_sfd(char *ip)
{
    struct sockaddr_in sin;
    sin.sin_family = AF_INET;
    sin.sin_port = htons(SERVER_PORT);
    inet_pton(AF_INET, ip, &sin.sin_addr);

    int sfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sfd == -1)
    {
        printf("%s\n", strerror(errno));
        return -1;
    }

    int ret = connect(sfd, (struct sockaddr *)&sin, sizeof(struct sockaddr_in));
    if (ret == -1) {
        printf("%s\n", strerror(errno));
        return -1; 
    }

    struct timeval timeout;
    timeout.tv_sec=3; // 2 second
    timeout.tv_usec=0;
    setsockopt(sfd, SOL_SOCKET, SO_RCVTIMEO, (char *)&timeout, sizeof(struct timeval));

    return sfd;
}

int getcityid(CMDPAR *par)
{
    int len;
    char *peq, buf[512];
    
    FILE *fp = fopen("API.txt", "r+");
    if (fp == NULL) {
        printf("%s\n", strerror(errno));
        return -1;
    }

    while (1){
        char *pbuf = fgets(buf, sizeof(buf), fp);
        if (pbuf == NULL) {
            if (!feof(fp)) {
                continue;
            }else{
                break;
            }
        }

        if ((peq = strstr(buf, "=")) && strstr(buf, par->city)) {
            *peq = '\0';
            strcpy(par->cityid, buf);
            printf("%s:%s\n", par->city, par->cityid);
            break;
        }
    }

    fclose(fp);
    return 0;
}

void sk_package(char buf[], char *cityid)
{
    sprintf(buf, "%s%s%s%s\r\n", "GET /data/sk/", cityid,
    ".html HTTP/1.1\r\n", "Host: www.weather.com.cn\r\n");
}

void cin_package(char buf[], char *cityid)
{
    sprintf(buf, "%s%s%s%s\r\n", "GET /data/cityinfo/", cityid,
    ".html HTTP/1.1\r\n", "Host: www.weather.com.cn\r\n");
}

void dfl_package(char buf[], char *cityid)
{
    sprintf(buf, "%s%s%s%s\r\n", "GET /data/", cityid,
    ".html HTTP/1.1\r\n", "Host: m.weather.com.cn\r\n");
}

ssize_t recvn(int fd, void *vptr, size_t n){
    size_t  nleft = 0;
    ssize_t nrecv = 0;
    char    *ptr;

    ptr = (char*)vptr;
    nleft = n;
    while (nleft > 0) {
        if((nrecv = recv(fd, ptr, nleft, 0)) < 0){
            if(errno == EINTR){
                nrecv = 0;
            }else{
                return -1;
            }
        }else if(nrecv == 0){
            break;
        }
        nleft -= nrecv;
        if(nleft == 0){
            return (n - nleft);
        }
        ptr   += nrecv;     
    }//while
    return(n - nleft);
}

void askdata(char buf[], size_t size, CMDPAR *par, int sfd)
{
    int ret;
    par->package(buf, par->cityid);
    write(sfd, buf, strlen(buf));
    memset(buf, 0, size);

    recvn(sfd, (void *)buf, size);

    par->parse(buf);
    return ;
}

int parse_command(int c, char **v, char **ip, int *bufsize, CMDPAR *par)
{
    int opt;
    //while ((opt = getopt(c, v, "c:s:")) != -1){
        opt = getopt(c, v, "c:s:");
        switch (opt){
            case 'c':
                par->city = optarg;
                par->parse = cin_parse;
                par->package = cin_package;
                *ip = SERVER_IP;
                *bufsize = 512;
                break;
            case 's':
                par->city = optarg;
                par->parse = sk_parse;
                par->package = sk_package;
                *ip = SERVER_IP;
                *bufsize = 512;
                break;
            default:
                //printf("%s\n", v[1]);
                par->city = v[1];
                par->parse = dfl_parse;
                par->package = dfl_package;
                *ip = SERVER_IP_M;
                *bufsize = 4096;
                break;
        }
    //}
   
    return 0;
}

int main(int c, char **v)
{
    int sfd, ret, bufsize;
    char *ip; 
    CMDPAR par;

    if (c < 2){
        printf("help:%s\n", p);
        return 1;
    }
    memset(&par, 0, sizeof(par));
    parse_command(c, v, &ip, &bufsize, &par);
    
    char buf[bufsize];
    
    //printf("1111%s\n", par.city);
    getcityid(&par);

    sfd = init_sfd(ip);
    askdata(buf, bufsize, &par, sfd);
    
    close(sfd);

    return 0;
}

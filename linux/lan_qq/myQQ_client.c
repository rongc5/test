#include "myQQ.h"
#define LOCALIPADDR "127.0.0.2"

void from_server_handler(MYQQ_LDATA ld, struct sockaddr_in hisend, int sd)
{
    time_t cur;
    struct tm * res;
    char buf[12];
    int i = 0;
    int ret;
    memset(buf, 0, 12);
    FILE * fp;
    time(&cur);
    res = localtime(&cur);
    if (res == NULL) {
        perror("localtime");
        exit(1);
    }
    
    sprintf(ld.mylog_msgbox, "%s  %d-%d-%d %d:%d", ld.mylog_msgbox, res->tm_year+1900, res->tm_mon+1, res->tm_mday, res->tm_hour, res->tm_min); 
    if (strcmp(ld.mylog_msgbox, "esc") == 0)
    {
        printf("\nserver to client: %s\n", ld.mylog_msgbox);
        exit(0);
    }
    else if (strncmp(ld.mylog_msgbox, "chat", 4) == 0)
    {
        fp = fopen(buf_logname, "a+");
        fprintf(fp, "%s\n", ld.mylog_msgbox);
        printf("\nserver to client: %s\n", ld.mylog_msgbox);
        fclose(fp);
    }
    else if (strncmp(ld.mylog_msgbox, "online you", 10) == 0)
    {
        char *p = buf_logname;
        memset(&ld, 0, sizeof(ld));
        while (*p != '.')
        buf[i++] = *p++;
        buf[i] = '\0';
        sprintf(ld.mylog_msgbox, "online yes %s", buf);
        ret = sendto(sd, &ld, sizeof(ld), 0, (struct sockaddr *)&hisend, sizeof(hisend));
        if (ret == -1) 
        {
            perror("sendto");
            exit(1);
        }        
    }
    else
    {
        printf("\nserver to client: %s\n", ld.mylog_msgbox);
    }
}

void to_server_handler(MYQQ_LDATA ld, struct sockaddr_in hisend, int sd)
{
    int ret;

    if (strncmp(ld.mylog_msgbox, "chat", 4) == 0)
    {
        time_t cur;
        struct tm * res;
        FILE * fp;
        time(&cur);
        char buf[MAXSIZE];
        memset(buf, 0, MAXSIZE);
        res = localtime(&cur);
        if (res == NULL) 
        {
            perror("localtime");
            exit(1);
        }
        sprintf(buf, "%s  %d-%d-%d %d:%d", ld.mylog_msgbox, res->tm_year+1900, res->tm_mon+1, res->tm_mday, res->tm_hour, res->tm_min); 
        fp = fopen(buf_logname, "a+");
        fprintf(fp, "%s\n", buf);
        fclose(fp);
        ret = sendto(sd, &ld, sizeof(ld), 0, (struct sockaddr *)&hisend, sizeof(hisend));
        if (ret == -1) 
        {
            perror("sendto");
            exit(1);
        }
    }
    else
    {
        ret = sendto(sd, &ld, sizeof(ld), 0, (struct sockaddr *)&hisend, sizeof(hisend));
        if (ret == -1) 
        {
            perror("sendto");
            exit(1);
        }
    }
}

void chat_to_server(int sd)
{
    MYQQ_LDATA ld;
    char buf[256];
    struct pollfd fds[2];
    struct sockaddr_in hisend;
    hisend.sin_family = AF_INET;
    hisend.sin_port = htons(SERVERPORT);
    inet_pton(AF_INET, SERVERIPADDR, &hisend.sin_addr);
    int ret;

    fds[0].fd = sd;
    fds[0].events = POLLIN;
    fds[1].fd = 0;
    fds[1].events = POLLIN;

    printf("请输入你的选择: (提示输入list + 状态可查询所在状态的用户，\n"
	    "输入set status +状态为设置自己的状态，输入set nickname + \n"
            "名称为设置自己的别名，输入“ESC”表示退出，输入chat + 用户名\n"
            "表示聊天)\n");
    while (1)
    {
        ret = poll(fds, 2, -1);
        if (ret == -1) 
        {
            perror("poll");
            exit(1);
        }

        memset(&ld, 0, sizeof(ld));
        if (fds[0].revents & POLLIN) 
        {
            ret = recvfrom(sd, &ld, sizeof(ld), 0, NULL, NULL);
            if (ret == -1) 
            {
                perror("recvfrom");
                exit(1);
            }

           from_server_handler(ld, hisend, sd);
        }

        memset(&ld, 0, sizeof(ld));

        if (fds[1].revents & POLLIN)
        {
            memset(buf, 0, 256);
            ret = read(0, buf, sizeof(buf));
            if (ret == -1) {
                perror("read");
                exit(1);
            }
            buf[ret-1] = '\0';

            strncpy(ld.mylog_msgbox, buf, strlen(buf));
            if (ret == 1)
                continue;
            else if (ret > 1)
            {
                to_server_handler(ld, hisend, sd);
            }
        }
    }
}

int initial_udp_socket(void)
{
    int sd, ret;
    struct sockaddr_in myend;
    socklen_t len;

    len = sizeof(myend);
    myend.sin_family = AF_INET;
    myend.sin_port = htons(CHATPORT);
    inet_pton(AF_INET, LOCALIPADDR, &myend.sin_addr);

    sd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sd == -1) {
        perror("socket");
        exit(1);
    }

    ret = bind(sd, (struct sockaddr *)&myend, len);
    if (ret == -1) {
        perror("bind");
        exit(1);
    }

    return sd;   
}

int judge_from_server(int sd)
{
    int ret;
    MYQQ_LDATA ld;
    memset(&ld, 0, sizeof(ld));

    ret = read(sd, &ld, sizeof(ld));
    if (ret == -1) {
        perror("read");
        exit(1);
    }
    if (strcmp(ld.mylog_msgbox, "yes") == 0)
        return 0;
    if (strcmp(ld.mylog_msgbox, "no") == 0)
        return 1;
}

MYQQ_LDATA mylogin_and_regester(char ch)
{   
    MYQQ_LDATA ld;
    char name[12];
    char passwd[256];
    memset(&ld, 0, sizeof(ld));
    memset(name, 0, 12);
    memset(passwd, 0, 256);


    printf("输入用户名(请输入在12个字符以内)：\n");
    scanf("%s", name);
    printf("输入密码:\n");
    scanf("%s", passwd);

    strcpy(ld.mylog_passwd, (char *)crypt(passwd, JAR));
    strcpy(ld.mylog_name, name);
    if (ch == 'l' || ch == 'L')
        strcpy(ld.mylog_msgbox, "login");
    else
        strcpy(ld.mylog_msgbox, "regester");

    return ld;
}

void regester_login_result(char ch, int ret)
{
    if (ch == 'l' || ch == 'L'){
        if (!ret)
            printf("登录成功！\n");
        else
            printf("用户名或者密码错误，登录失败!重新登录\n");
    }
    else  if (ch == 'r' || ch == 'R'){
        if (!ret)
            printf("注册成功!\n");
        else
            printf("注册失败，用户名已经存在!\n");
    }
}

void usersel_1(int sd)
{
    char ch;
    MYQQ_LDATA ld;
    int ret;
    int i = 0;
    struct sockaddr_in hisend;

    hisend.sin_family = AF_INET;
    hisend.sin_port = htons(SERVERPORT);
    inet_pton(AF_INET, SERVERIPADDR, &hisend.sin_addr);
    memset(&ld, 0, sizeof(0));

    while (1)
    {
        printf("请选择注册(r | R)or登录(l | L)!\n");
        scanf(" %c", &ch);

        if (ch == 'l' || ch == 'L')
            break;
        else if (ch == 'r' || ch == 'R')
            break;
        else 
            printf("输入有误, 重新输入!\n");
        i++;
        if (i >= 3){
            printf("输入超过三次退出!\n");
            exit(0);
        }
    }

    while (1)
    {
        ld = mylogin_and_regester(ch);
        ret = sendto(sd, &ld, sizeof(ld), 0, (struct sockaddr *)&hisend, sizeof(hisend));
        if (ret == -1) {
            perror("sendto");
            exit(1);
        }
        ret = 1;
        ret = judge_from_server(sd);
        regester_login_result(ch, ret);
        i++;
        if (i >= 3 && ret == 1){
            printf("错误输入超过三次退出!\n");
            exit(0);
        }
        if (ret == 0){
            sprintf(buf_logname, "%s.msg_log", ld.mylog_name);
            break;
        }
        memset(&ld, 0, sizeof(0));
    }
}

int main(void)
{
    int sd;
    sd = initial_udp_socket();
    usersel_1(sd);
    chat_to_server(sd);

    return 0;
}

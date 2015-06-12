#include "myQQ.h"
#define LOCALIPADDR "127.0.0.2"
MYQQ_LDATA * list_init(void)
{
    MYQQ_LDATA *phead = (MYQQ_LDATA *)malloc(sizeof(MYQQ_LDATA));
    memset(phead, 0, sizeof(MYQQ_LDATA));
    if (NULL == phead)
    {
        perror("in list_init, nalloc error");
    }
    phead->next = NULL;
    return phead;
}

void insert_node(MYQQ_LDATA * phead, MYQQ_LDATA *new)
{
    while (phead->next != NULL)
    {
        phead = phead->next;
    }
    if (new == NULL) 
    {
        printf("new == NULL\n");
    }
    phead->next = new;
    new->next = NULL;
    return;
}

void list_delete(MYQQ_LDATA * phead)
{
    MYQQ_LDATA * tmp = NULL;
    while (phead!= NULL)
    {
        tmp = phead;
        phead = phead->next;
        free(tmp);
    }
}

void add_ldata(MYQQ_LDATA * phead, MYQQ_LDATA *ld)
{
    MYQQ_LDATA * new;
    int ret;
    new = (MYQQ_LDATA *)malloc(sizeof(MYQQ_LDATA));
    if (new == NULL) 
    {
        perror("malloc");
        DEBUG;
        return;
    }  
    memcpy(new, ld, sizeof(MYQQ_LDATA));
    new->next = NULL;
    insert_node(phead, new);
    return;
}

void set_qq_status(MYQQ_LDATA * worker, MYQQ_Status status)
{
    if (worker == NULL) 
    {
        return;
    }
    worker->mylog_status = status;
    return;
}

void set_qq_qd(MYQQ_LDATA * worker, MYQQ_QD qd)
{
    if (worker == NULL) 
    {
        return;
    }
    worker->mylog_qd = qd;
    return;
}

void set_worker_nickname(MYQQ_LDATA *worker, const char *name)
{
    strncpy(worker->mylog_nickname, name, sizeof(worker->mylog_nickname));
}

MYQQ_LDATA * find_node_name(const char * name, MYQQ_LDATA *phead)
{
    while (phead != NULL)
    { 	
        if (strcmp(phead->mylog_name, name) == 0) 
        {       	
            return phead;
        }         
        phead = phead->next;
    }
    return NULL;
}

MYQQ_LDATA * find_node_ipbuf(const char *ipbuf, MYQQ_LDATA *phead)
{
    while (phead != NULL)
    {
        if (strcmp(phead->mylog_ipbuf, ipbuf) == 0)
            return phead;
        phead = phead->next;
    }
    return NULL;
}

MYQQ_LDATA * find_node_passwd(const char * passwd, MYQQ_LDATA *phead)
{
    while (phead != NULL)
    {
        if (strcmp(phead->mylog_passwd, passwd) == 0)
            return phead;
        phead = phead->next;
    }
    return NULL;
}

int find_num_status(MYQQ_Status status, MYQQ_LDATA *phead)
{
    int i = 0;
    while (phead != NULL)
    {
        if (phead->mylog_status == status)
            i++;
        phead = phead->next;
    }
    return i;
}

MYQQ_LDATA * find_node_status(MYQQ_Status status, MYQQ_LDATA *phead)
{
    while (phead != NULL)
    {
        if (phead->mylog_status == status)
            return phead;
        phead = phead->next;
    }
    return NULL;
}

void display_meet_status(MYQQ_LDATA * phead, MYQQ_Status status, struct sockaddr_in userend, int sd)
{
    int i, j = 1;
    i = find_num_status(status, phead);
    int ret;
    MYQQ_LDATA ld;
    memset(&ld, 0, sizeof(ld));
    MYQQ_LDATA * list = phead;
    char buf[16];

    for (j; j <= i; j++)
    {
        memset(buf, 0, 16);
        list = find_node_status(status, list);
        if (!list)
            return;
        if (list->mylog_status == OFFLINE)
            strcpy(buf, "OFFLINE");
        else if (list->mylog_status == ONLINE)
            strcpy(buf, "ONLINE");
        else if (list->mylog_status == HIDDEN)
            strcpy(buf, "HIDDEN");
        else if (list->mylog_status == AWAY)
            strcpy(buf, "AWAY");
        sprintf(ld.mylog_msgbox, "login_name = %s, nickname = %s, log_status = %s", list->mylog_name, list->mylog_nickname, buf);
        ret = sendto(sd, &ld, sizeof(MYQQ_LDATA), 0, (struct sockaddr *)&userend, sizeof(userend));
        if (ret == -1) 
        {
            perror("sendto");
            exit(1);
        }
        memset(ld.mylog_msgbox, 0, MAXSIZE);
    }
}

void display_all_users(MYQQ_LDATA * phead, struct sockaddr_in userend, int sd)
{
    phead = phead->next;
    MYQQ_LDATA ld;
    memset(&ld, 0, sizeof(ld));
    int ret;
    char buf[16];

    while (phead != NULL)
    {    
        memset(buf, 0, 16);
        printf("%s\n", phead->mylog_name);
        if (phead->mylog_status == OFFLINE)
            strcpy(buf, "OFFLINE");
        else if (phead->mylog_status == ONLINE)
            strcpy(buf, "ONLINE");
        else if (phead->mylog_status == HIDDEN)
            strcpy(buf, "HIDDEN");
        else if (phead->mylog_status == AWAY)
            strcpy(buf, "AWAY");
        sprintf(ld.mylog_msgbox, "login_name = %s, nickname = %s, log_status = %s", phead->mylog_name, phead->mylog_nickname, buf);
        ret = sendto(sd, &ld, sizeof(ld), 0, (struct sockaddr *)&userend, sizeof(userend));
        if (ret == -1) 
        {
            perror("sendto");
            exit(1);
        }
        memset(ld.mylog_msgbox, 0, MAXSIZE);
        phead = phead->next;
    }
}

void display_chat_result(MYQQ_LDATA * phead, struct sockaddr_in userend, const char * user, const char *msg, int sd)
{
    MYQQ_LDATA *p, *ip, ld;
    char ipbuf[16];
    int ret;
    struct sockaddr_in hisend;
    memset(&ld, 0, sizeof(ld));
    char buf[22];

    memset(buf, 0, 22);
    p = find_node_name(user, phead);
    if (!p)
        return;
    if (p->mylog_status != OFFLINE)
    {
        inet_ntop(AF_INET, &userend.sin_addr, ipbuf, 16);
        ip = find_node_ipbuf(ipbuf, phead);

        hisend.sin_family = AF_INET;
        hisend.sin_port = htons(CHATPORT);
        inet_pton(AF_INET, p->mylog_ipbuf, &hisend.sin_addr);

        sprintf(ld.mylog_msgbox, "chat from: %s ,msg: %s", ip->mylog_name, msg);
        ret = sendto(sd, &ld, sizeof(ld), 0, (struct sockaddr *)&hisend, sizeof(hisend));
        if (ret == -1)
        {
            perror("sendto");
            exit(1);
        }
    }
    else
    {
        inet_ntop(AF_INET, &userend.sin_addr, ipbuf, 16);
        ip = find_node_ipbuf(ipbuf, phead);
        sprintf(buf, "%s.off_msg", user);
        FILE *fp;
        fp = fopen(buf, "a+");
        fprintf(fp, "chat from: %s ,msg: %s\n", ip->mylog_name, msg);
        memset(&ld, 0, sizeof(ld));
        sprintf(ld.mylog_msgbox, "chat %s ,msg: %s 发送失败, 对方未登录", user, msg);//写的文件中
        ret = sendto(sd, &ld, sizeof(ld), 0, (struct sockaddr *)&userend, sizeof(userend));
        if (ret == -1)
        {
            perror("sendto");
            exit(1);
        }
        fclose(fp);
    }

}

void display_esc_result(MYQQ_LDATA * phead, struct sockaddr_in userend, int sd)
{

    MYQQ_LDATA *p, ld;
    char ipbuf[16];
    int ret;

    memset(&ld, 0, sizeof(ld));
    inet_ntop(AF_INET, &userend.sin_addr, ipbuf, 16);
    p = find_node_ipbuf(ipbuf, phead);
    if (!p)
        return;
    set_qq_status(p, OFFLINE);
    sprintf(ld.mylog_msgbox, "ESC success!");
    ret = sendto(sd, &ld, sizeof(ld), 0, (struct sockaddr *)&userend, sizeof(userend));
    if (ret == -1) 
    {
        perror("sendto");
        exit(1);
    }
}

void display_set_nickname(MYQQ_LDATA * phead, struct sockaddr_in userend, const char * nickname, int sd)
{
    MYQQ_LDATA *p, ld;
    char ipbuf[16];
    int ret;

    memset(&ld, 0, sizeof(ld));
    inet_ntop(AF_INET, &userend.sin_addr, ipbuf, 16);
    p = find_node_ipbuf(ipbuf, phead);
    if (!p)
        return;
    set_worker_nickname(p, nickname);
    sprintf(ld.mylog_msgbox, "nickname set success!");
    ret = sendto(sd, &ld, sizeof(ld), 0, (struct sockaddr *)&userend, sizeof(userend));
    if (ret == -1) 
    {
        perror("sendto");
        exit(1);
    }
}

void display_set_status(MYQQ_LDATA * phead, struct sockaddr_in userend, MYQQ_Status mystatus, int sd)
{
    MYQQ_LDATA *p, ld;
    char ipbuf[16];
    int ret;
    memset(&ld, 0, sizeof(ld));

    inet_ntop(AF_INET, &userend.sin_addr, ipbuf, 16);
    p = find_node_ipbuf(ipbuf, phead);
    if (!p)
        return;

    set_qq_status(p, mystatus);
    sprintf(ld.mylog_msgbox, "status set success!");
    ret = sendto(sd, &ld, sizeof(ld), 0, (struct sockaddr *)&userend, sizeof(userend));
    if (ret == -1) 
    {
        perror("sendto");
        exit(1);
    }
}

void display_status_detection(MYQQ_LDATA * phead, const char * name, int sd)
{
    FILE * fp;
    char buf_t[512];
    char * retptr;
    char buf[22];
    int ret;

    struct stat bf;
    MYQQ_LDATA ld;
    struct sockaddr_in hisend;
    hisend.sin_family = AF_INET;
    hisend.sin_port = htons(CHATPORT);
    while (phead != NULL)
    {
        if (strcmp(phead->mylog_ipbuf, " ") != 0)
        {
            if (strcmp(phead->mylog_name, name) == 0)
            {    
                phead->flag = 0;
                memset(buf, 0, 22);
                sprintf(buf, "%s.off_msg", phead->mylog_name);
                phead->mylog_qd = IN;//判断文件是否为0长

                if (!access(buf, F_OK))
                {
                    ret = lstat(buf, &bf);
                    if (ret == -1) 
                    {
                        perror("lstat");
                        return ;
                    }

                    if (bf.st_size)
                    {     
                        memset(&ld, 0, sizeof(ld));
                        fp = fopen(buf, "r");
                        if (NULL == fp) 
                        {
                            perror("fopen");
                            exit(1);
                        }
                        while (1) 
                        {
                            retptr = fgets(buf_t, 512, fp);
                            if (retptr == NULL)
                            {
                                if (feof(fp))
                                    break;
                                else 
                                {
                                    perror("fgets");
                                    exit(1);
                                }
                            }

                            if (buf_t[strlen(buf_t)-1] == '\n')
                                buf_t[strlen(buf_t) - 1] = '\0';
                            inet_pton(AF_INET, phead->mylog_ipbuf, &hisend.sin_addr);

                            sprintf(ld.mylog_msgbox, "%s", buf_t);
                            ret = sendto(sd, &ld, sizeof(ld), 0, (struct sockaddr *)&hisend, sizeof(hisend));
                            if (ret == -1) 
                            {
                                perror("sendto");
                                exit(1);
                            }
                        }
                        ret = truncate(buf, 0);
                        if (ret == -1) 
                        {
                            perror("truncate");
                            return;
                        }
                    }
                }
            }
        }
        phead = phead->next;
    }
}

void dilplay_list_error(struct sockaddr_in userend, int sd)
{
    int ret;
    MYQQ_LDATA ld;
    memset(&ld, 0, sizeof(ld));
    sprintf(ld.mylog_msgbox, "list input error");
    ret = sendto(sd, &ld, sizeof(ld), 0, (struct sockaddr *)&userend, sizeof(userend));
    if (ret == -1) 
    {
        perror("sendto");
        exit(1);
    }
}

FILE * get_file(void)
{
    FILE * fp = NULL;
    fp = fopen(MYQQ_LOG_DB, "a+");
    if (!fp) 
    {
        perror("fopen");
        DEBUG;
    }
    return fp;
}

MYQQ_LDATA * read_file(FILE * fp)
{
    MYQQ_LDATA * ptr = (MYQQ_LDATA *)malloc(sizeof(MYQQ_LDATA));
    if (!ptr) 
    {
        perror("malloc");
        return NULL;
    }
    int ret;
    ret = fread(ptr, sizeof(MYQQ_LDATA), 1, fp);
    if (ret < 1)
        return NULL;
    return ptr;
}

void construct_list(FILE * fp, MYQQ_LDATA *phead)
{
    MYQQ_LDATA * new = NULL;

    while (1) 
    {
        new = read_file(fp);    
        if (new == NULL)
            break;
        insert_node(phead, new);
    }
}

MYQQ_LDATA * initial_file(void)
{
    MYQQ_LDATA * list = list_init();    
    FILE * fp = NULL;     
    fp = get_file();
    if (fp == NULL)
        return NULL;

    construct_list(fp, list);
    return list;
}

void client_status_detection(MYQQ_LDATA * phead, int sd)
{
    struct sockaddr_in hisend;
    MYQQ_LDATA ld, *plist;
    memset(&ld, 0, sizeof(ld));

    int ret;
    plist = phead;
    hisend.sin_family = AF_INET;
    hisend.sin_port = htons(CHATPORT);
    while (1)
    {
        phead = plist;
        while (phead != NULL)
        {
            if (strcmp(phead->mylog_ipbuf, " ") != 0)
            {    
                inet_pton(AF_INET, phead->mylog_ipbuf, &hisend.sin_addr);
                sprintf(ld.mylog_msgbox, "online you ?");
                ret = sendto(sd, &ld, sizeof(ld), 0, (struct sockaddr *)&hisend, sizeof(hisend));
                if (ret == -1) 
                {
                    perror("sendto");
                    exit(1);
                }
                phead->mylog_qd = OUT;
            }
            phead = phead->next;
        }

        usleep(1000000);
        phead = plist;
        while (phead != NULL)
        {
            if (strcmp(phead->mylog_ipbuf, " ") != 0)
            {
                if (phead->mylog_qd == OUT)
                {
                    phead->flag++;
                    if (phead->flag >= 3)
                    {
                        set_qq_status(phead, OFFLINE);
                        memset(phead->mylog_ipbuf, 0, 16);
                    }
                }
            }
            phead = phead->next;
        }
    }
}

void recive_handle(MYQQ_LDATA rd, MYQQ_LDATA * phead, struct sockaddr_in userend, int sd)
{
    char v[3][200];
    char *tmp = rd.mylog_msgbox;
    int i = 0, j;
    MYQQ_Status mystatus;

    memset(v, 0, 600);
    while (*tmp != '\0')
    {
        j = 0;
        while (*tmp != ' ' && *tmp != '\0')
            v[i][j++] = *tmp++;
        v[i][j] = '\0';
        i++;
        if (*tmp == '\0')
            break;
        while (*tmp == ' ' && *tmp != '\0')
            tmp++;
        j = 0;
        while (*tmp != ' ' && *tmp != '\0')
            v[i][j++] = *tmp++;
        v[i][j] = '\0';
        i++;
        if (*tmp == '\0')
            break;
        while (*tmp == ' ' && *tmp != '\0')
            tmp++;
        j = 0;
        while (*tmp != '\0')
            v[i][j++] = *tmp++;
        v[i][j] = '\0';
        i++;
        if ( i == 3)
            break;       
    }  

    if (strcmp(v[0], "list") == 0)
    {
        if (i == 1)
        {
            display_all_users(phead, userend, sd);
        }
        else {
            if (strcmp(v[1], "online") == 0)
            {
                mystatus = ONLINE;
                display_meet_status(phead, mystatus, userend, sd);
            }
            else if (strcmp(v[1], "offline") == 0)
            {
                mystatus = OFFLINE;
                display_meet_status(phead, mystatus, userend, sd);
            }
            else if (strcmp(v[1], "away") == 0)
            {
                mystatus = AWAY;
                display_meet_status(phead, mystatus, userend, sd);
            }
            else if (strcmp(v[1], "hidden") == 0)
            {
                mystatus = HIDDEN;
                display_meet_status(phead, mystatus, userend, sd);
            }
        }
    }
    else if (strcmp(v[0], "set") == 0)
    {
        if (strcmp(v[1], "status") == 0 && i == 3)
        {
            if (strcmp(v[2], "online") == 0)
            {
                mystatus = ONLINE;
                display_set_status(phead, userend, mystatus, sd);
            }
            else if (strcmp(v[2], "offline") == 0)
            {
                mystatus = OFFLINE;
                display_set_status(phead, userend, mystatus, sd);
            }
            else if (strcmp(v[2], "away") == 0)
            {   
                mystatus = AWAY;
                display_set_status(phead, userend, mystatus, sd);
            }
            else if (strcmp(v[2], "hidden") == 0)
            {
                mystatus = HIDDEN;
                display_set_status(phead, userend, mystatus, sd);
            }
        } 
        else if (strcmp(v[1], "nickname") == 0 && i == 3)
        {
            display_set_nickname(phead, userend, v[2], sd);
        }
    }
    else if (strcmp(v[0], "esc") == 0 || strcmp(v[0], "ESC") == 0)
    {
        display_esc_result(phead, userend, sd);
    }
    else if (strcmp(v[0], "chat") == 0 && i == 3)
    { 
        display_chat_result(phead, userend, v[1], v[2], sd);
    }
    else if (strcmp(v[0], "login") == 0 && i == 1)
    {
       // printf("login\n");
        display_myQQ_login(phead, rd, sd, userend);
    }
    else if (strcmp(v[0], "regester") == 0)
    {   
        //printf("regester\n");
        display_myQQ_regester(phead, rd, sd, userend);
    }
    else if (strcmp(v[0], "online") == 0 && strcmp(v[1], "yes") == 0)
    {
        display_status_detection(phead, v[2], sd);
    }
    else
    {    
        dilplay_list_error(userend, sd);
    }  
}

int initial_udp_socket(void)
{
    int sd, ret;
    struct sockaddr_in myend;
    socklen_t len;

    len = sizeof(myend);
    myend.sin_family = AF_INET;
    myend.sin_port = htons(SERVERPORT);
    inet_pton(AF_INET, SERVERIPADDR, &myend.sin_addr);

    sd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sd == -1) 
    {
        perror("socket");
        exit(1);
    }

    ret = bind(sd, (struct sockaddr *)&myend, len);
    if (ret == -1) 
    {
        perror("bind");
        exit(1);
    }

    return sd;    
}


void chat_to_client(MYQQ_LDATA * phead, int sd)
{
    struct sockaddr_in userend;
    MYQQ_LDATA ld;
    int ret;
    int len = sizeof(userend);

    while (1)
    {
        memset(&ld, 0, sizeof(ld));
        ret = recvfrom(sd, &ld, sizeof(ld), 0, (struct sockaddr *)&userend, &len);
        if (ret == -1) 
        {
            perror("recvfrom");
            exit(1);
        }
        recive_handle(ld, phead, userend, sd);  
    }
}


int check_regester_name(const char *name)
{
    char buf[MAXSIZE];
    char * ptr;
    int flag = 0;

    FILE * fp;
    fp = fopen(MYQQ_LOG_DB, "a+");
    if (!fp)
    {
        perror("fopen");
        exit(1);
    }

    while (1)
    {
        ptr = fgets(buf, MAXSIZE, fp);
        if (NULL == ptr)
            break;
        if (buf[strlen(buf) - 1] == '\n')
            buf[strlen(buf) - 1] = '\0';
        if (strcmp(buf, name) == 0)
        {
            flag = 1;
            break;
        }
    }

    fclose(fp);
    return flag;
}

void display_myQQ_regester(MYQQ_LDATA * phead, MYQQ_LDATA rd, int sd, struct sockaddr_in userend)
{
    MYQQ_LDATA ld, *list;
    char ret;
    int fd;
    memset(&ld, 0, sizeof(ld));

    ret = check_regester_name(rd.mylog_name);
    if (0 == ret)
    { 
        set_qq_status(&rd, OFFLINE);
        set_worker_nickname(&rd, rd.mylog_name);
        set_qq_qd(list, OUT);
        fd = open(MYQQ_LOG_DB, O_WRONLY);
        lseek(fd, 0, SEEK_END);
        write(fd, &rd, sizeof(rd));
        sprintf(ld.mylog_msgbox, "yes");
        sendto(sd, &ld, sizeof(ld), 0, (struct sockaddr *)&userend, sizeof(userend));
        if (ret == -1) 
        {
            perror("sendto");
            exit(1);
        }
        add_ldata(phead, &rd);
        list = find_node_name(rd.mylog_name, phead);
        if (list == NULL) 
        {
            return;
        }

        set_qq_status(list, ONLINE);
        set_qq_qd(list, IN);
        inet_ntop(AF_INET, &userend.sin_addr, list->mylog_ipbuf, 16);
       // printf("用户名不存在，注册成功\n");
    } 
    else
    {
        //printf("用户名已存在，注册失败\n");
        sprintf(ld.mylog_msgbox, "no");
        sendto(sd, &ld, sizeof(ld), 0, (struct sockaddr *)&userend, sizeof(userend));
    }
}

int check_login(MYQQ_LDATA * phead, const char * name, const char * passwd)
{
    int flag = 0;
    MYQQ_LDATA * p = NULL;

    p = find_node_name(name, phead);
    if (!p)
    {
        flag = 1;
       // printf("用户名不正确\n");
    }

    p = find_node_passwd(passwd, phead);
    if (!p)
    {
        flag = 1;
      //  printf("密码不正确\n");
    }

    return flag;
}

void display_myQQ_login(MYQQ_LDATA * phead, MYQQ_LDATA rd, int sd, struct sockaddr_in userend)
{
    MYQQ_LDATA ld, *list;
    char ret;
    int fd;

    ret = check_login(phead, rd.mylog_name, rd.mylog_passwd);
    if (0 == ret)
    { 
        sprintf(ld.mylog_msgbox, "yes");
        sendto(sd, &ld, sizeof(ld), 0, (struct sockaddr *)&userend, sizeof(userend));
        //printf("用户名密码存在，登录成功\n");
        list = find_node_name(rd.mylog_name, phead);
        set_qq_status(list, ONLINE);
        set_qq_qd(list, IN);
        rd.flag = 0;
        inet_ntop(AF_INET, &userend.sin_addr, list->mylog_ipbuf, 16);
    }
    else 
    {
        sprintf(ld.mylog_msgbox, "no");
        sendto(sd, &ld, sizeof(ld), 0, (struct sockaddr *)&userend, sizeof(userend));
       // printf("用户名或者密码错误，登录失败\n");
    }   
}

void * thread_handler_1(void *arg)
{
    MYQQ_SD_INFO * sd_info;
    sd_info = (MYQQ_SD_INFO *)arg;
    client_status_detection(sd_info->phead, sd_info->sd);
    return NULL;
}

int main(void)
{   
    int sd;
    MYQQ_LDATA * list = initial_file();
    int ret;
    pthread_t tid;
    MYQQ_SD_INFO  sd_info;
    sd = initial_udp_socket();
    sd_info.sd = sd;
    sd_info.phead = list;

    ret = pthread_create(&tid, NULL, thread_handler_1, (void *)&sd_info);
    if (ret) 
    {
        fprintf(stderr, "pthread_create:%s\n", strerror(ret));
        exit(1);
    }

    chat_to_client(list, sd);
     
    list_delete(list);
    return 0;
}

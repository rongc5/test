#include "data_struct.h"
#include "baseinfo_thread.h"
#include <QtDebug>
#include <iostream>
#include <unistd.h>
#include <poll.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>

#include <net/if.h>
#include <netinet/in.h>
#include <netinet/if_ether.h>
#include <sys/socket.h>
#include <linux/sockios.h>
#include <sys/ioctl.h>
#include <arpa/inet.h>
#include <errno.h>

#include <ifaddrs.h>

#include<sys/time.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <signal.h>

#include <stdio.h>
#include <string.h>

int BaseThread::prase_str(char *buf, const char *name, char *save)
{
    char *p;
    int i = 0;
    //std::cout << "buf = " << buf << "name =" << name << "save = " << save << "\n";
    if ((p = strstr(buf, name))){
        while (*p != '=')
            p++;
        p++;

        while (*p != '\n' && *p != '\0'&& *p != ' ')
            save[i++] = *p++;
        save[i] = '\0';
        std::cout << "return 0\n";
        return 0;
    }
    std::cout << "return -1\n";
    return -1;
}


void BaseThread::getLocalIp()
{
    int s;
    struct ifconf conf;
    struct ifreq *ifr;
    char buff[BUFSIZ];
    char ipbuf[16];
    int num;
    int i;

    s = socket(PF_INET, SOCK_DGRAM, 0);
    conf.ifc_len = BUFSIZ;
    conf.ifc_buf = buff;

    ioctl(s, SIOCGIFCONF, &conf);
    num = conf.ifc_len / sizeof(struct ifreq);
    ifr = conf.ifc_req;

    for(i=0;i < num;i++)
    {
        struct sockaddr_in *sin = (struct sockaddr_in *)(&ifr->ifr_addr);

        ioctl(s, SIOCGIFFLAGS, ifr);
        if(((ifr->ifr_flags & IFF_LOOPBACK) == 0) && (ifr->ifr_flags & IFF_UP))
            //if(((ifr->ifr_flags & IFF_LOOPBACK) == 0))
        {
            inet_ntop(AF_INET, &sin->sin_addr, ipbuf, 16);
            printf("ipbuf:(%s)\n", ipbuf);

            pthread_mutex_lock(&Local_lock);
            if (ipbuf)
                base.local_Ip = ipbuf;
            pthread_mutex_unlock(&Local_lock);
            break;
        }
        ifr++;
    } 

    close(s);

}

void BaseThread::getNetworkName()
{
    struct ifaddrs *addrs,*tmp;

    getifaddrs(&addrs);
    tmp = addrs;

    while (tmp)
    {
        if (tmp->ifa_addr && tmp->ifa_addr->sa_family == AF_PACKET && strstr(tmp->ifa_name, "eth")){                   

            pthread_mutex_lock(&Local_lock);
            if (tmp->ifa_name)
                base.net_name = tmp->ifa_name;
            pthread_mutex_unlock(&Local_lock);

            printf("%s\n", tmp->ifa_name);
            break;	
        }

        tmp = tmp->ifa_next;
    }

    freeifaddrs(addrs);	 
}

void BaseThread::prase_haier_info(char *buf)
{
    char tmp[60];
    memset(tmp, 0, sizeof(tmp));

    if (prase_str(buf, "haierIp", tmp))
        return;

    printf("buf:%s, base.tv_ip:%s\n", buf, tmp);
    if (tmp)
        base.tv_ip = tmp;

    if (prase_str(buf, "haierMac", tmp))
        return;

    if (tmp)
        base.tv_Mac = tmp;
    printf("buf:%s, base.tv_Mac:%s\n", buf, tmp);
}

void BaseThread::getTvMacIp()
{
    char buf[1024];
    char name[32];
    memset(name, 0, sizeof(name));

    pthread_mutex_lock(&Local_lock);
    strncpy(name, base.net_name.c_str(), sizeof(name) - 1);
    pthread_mutex_unlock(&Local_lock);

    getHaierTvInfobuf(buf, sizeof(buf), name);

    pthread_mutex_lock(&Tv_lock);
    prase_haier_info(buf);
    pthread_mutex_unlock(&Tv_lock);
}

void * BaseThread::getHaierTvInfo(void *arg)
{
    pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
    BaseThread *ptr = (BaseThread *)arg;

    while (1)
    {
        ptr->getTvMacIp();
        sleep(1);
        pthread_testcancel();
    }

    return NULL;
}

int BaseThread::getHaierTvInfobuf(char *buf, size_t size, char *net_name)
{	
    FILE * fp;

    sprintf(buf, "tcpdump -i %s  -c 1 'tcp[13]==2' -nne 2>/dev/null | sed  -nr 's/.* ([0-9a-f]{2}:[0-9a-f]{2}:[0-9a-f]{2}:[0-9a-f]{2}:[0-9a-f]{2}:[0-9a-f]{2}) > ([0-9a-f]{2}:[0-9a-f]{2}:[0-9a-f]{2}:[0-9a-f]{2}:[0-9a-f]{2}:[0-9a-f]{2}).* ([0-9]+.[0-9]+.[0-9]+.[0-9]+).* > ([0-9]+.[0-9]+.[0-9]+.[0-9]+).*/haierIp=\\3  haierMac=\\1/p'", net_name);

    fp = popen(buf, "r");
    if (!fp){
        perror("popen");
        return -1;
    }
    memset(buf, 0, size);

    fread(buf, size,1,fp);
    printf("%s\n", buf);
    pclose(fp);
}

void * BaseThread::getNetworkInfo(void * arg)
{
    pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);

    BaseThread *ptr = (BaseThread *)arg;

    while (1)
    {
        ptr->getNetworkName();

        ptr->getLocalIp();

        sleep(3);
        pthread_testcancel();
    }

    return NULL;
}

BaseThread::BaseThread()
{
    pthread_mutex_init(&Local_lock, NULL);
    pthread_mutex_init(&Tv_lock, NULL);
}

void BaseThread::start()
{
	
	signal(SIGPIPE,SIG_IGN);

    int ret;
    ret = pthread_create(&Local_tid, NULL, &BaseThread::getNetworkInfo, (void *)this);
    if (ret){
        perror("pthread_create");
        return ;
    }

    pthread_create(&Tv_tid, NULL, &BaseThread::getHaierTvInfo, (void *)this);
    if (ret){
        perror("pthread_create");
        return ;
    }
}

BaseThread::~BaseThread()
{
    pthread_cancel(Local_tid);
    pthread_cancel(Tv_tid);

    pthread_mutex_destroy(&Local_lock);
    pthread_mutex_destroy(&Tv_lock);
}

void BaseThread::getBasic(BasicInfo * info)
{
    pthread_mutex_lock(&Local_lock);

    info->net_name = base.net_name;
    info->local_Ip = base.local_Ip;

    pthread_mutex_unlock(&Local_lock);


    pthread_mutex_lock(&Tv_lock);

    info->tv_ip = base.tv_ip;
    info->tv_Mac = base.tv_Mac;

    pthread_mutex_unlock(&Tv_lock);
}


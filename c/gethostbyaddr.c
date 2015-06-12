#include <stdlib.h>

#include <stdio.h>

#include <errno.h>

#include <string.h>

#include <unistd.h>

#include <netdb.h>

#include <sys/socket.h>

#include <netinet/in.h>

#include <sys/types.h>

#include <arpa/inet.h>


int main(int argc ,char **argv)
{

    struct sockaddr_in addr;
    struct hostent *host;
    char **alias;

    if(argc<2)
    {
        fprintf(stderr,"Usage:%s hostname|ip..\n\a",argv[0]);
        exit(1);
    }

    argv++;
    for(;*argv!=NULL;argv++)
    {
        /* 这里我们假设是IP*/  
        if(inet_aton(*argv,&addr.sin_addr)!=0)
        {
            host=gethostbyaddr((char   *)&addr.sin_addr,4,AF_INET);
            printf("Address information of Ip %s\n",*argv);
        }
        else
        {
            /* 失败,难道是域名?*/
            host=gethostbyname(*argv); printf("Address informationof host %s\n",*argv);
        }
        if(host==NULL)
        {
            /* 都不是 ,算了不找了*/
            fprintf(stderr,"No address information of %s\n",*argv);
            continue;
        }
        printf("Official host name %s\n",host->h_name);
        printf("Name aliases:");
        for(alias=host->h_aliases;*alias!=NULL;alias++)
            printf("%s ,",*alias);
        printf("\nIp address:");
        for(alias=host->h_addr_list;*alias!=NULL;alias++)
            printf("%s ,",inet_ntoa(*(struct in_addr *)(*alias)));
    }

}


#include<sys/types.h>
#include<asm/types.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<sys/socket.h>
#include <arpa/inet.h>
#include<netinet/in.h>
#include<linux/rtnetlink.h>
#include<linux/netlink.h>

struct req {
    struct nlmsghdr nlmsg;
    struct rtmsg rtm;
};

static struct rtattr* get_gw_attr(struct nlmsghdr *nlmsghdr);
int print_route(struct rtattr *rtp);

int get_gateway_ip(struct sockaddr *gwp);

int main(void)
{
    int n;
    struct sockaddr gw;
    if (get_gateway_ip(&gw)); {
        perror("inet_ntop failed !");
        exit (1);
    }
}

int get_gateway_ip(struct sockaddr *gwp){
    int fd;
    int n;
    char *c;
    char buff[BUFSIZ];
    char str[22];
    struct req req;
    struct rtattr *rtp;
    struct nlmsghdr nlmsg, *nlp;
    struct rtmsg rtm;
    struct sockaddr so;
    struct sockaddr_nl nl;
    struct in_addr addr;

    fd = socket(AF_NETLINK, SOCK_RAW, NETLINK_ROUTE);
    if (fd < 0) {
        perror("open error");
        exit (1);
    }
    nl.nl_family = AF_NETLINK;
    nl.nl_pad = 0;
    nl.nl_pid = getpid();
    nl.nl_groups = 0;

    n = bind(fd, (struct sockaddr*)&nl, sizeof(nl));
    if (n < 0) {
        perror(" bind error");
        exit (1);
    }

    memset(&nlmsg, 0, sizeof(struct nlmsghdr));
    req.nlmsg.nlmsg_len = NLMSG_LENGTH(sizeof(struct rtmsg));
    req.nlmsg.nlmsg_type = RTM_GETROUTE;

    /** In kernel header comments NLM_F_ROOT
      "specify tree root", but On somebooks
      said this means return the entire table
      not just one entry **/
    req.nlmsg.nlmsg_flags = NLM_F_ROOT|NLM_F_REQUEST;

    /** this is not required! **/
    // req.nlmsg.nlmsg_pid = nl.pid;

    memset(&rtm, 0, sizeof(struct rtmsg));
    req.rtm.rtm_family = AF_NETLINK;

    /** we need change here **/
    req.rtm.rtm_dst_len = 4;
    req.rtm.rtm_src_len = 4;

    req.rtm.rtm_table = RT_TABLE_MAIN;

    /** Route installed during boot **/
    req.rtm.rtm_protocol = RTPROT_BOOT;
    /** located on directly LAN **/
    req.rtm.rtm_scope = RT_SCOPE_LINK;

    /** Gateway or direct route **/
    req.rtm.rtm_type = RTN_UNICAST;

    n = send(fd, &req, sizeof(req), 0);
    if (n < 0)
    {
        perror("send error!");
        exit (-1);
    }
    else
        printf("%d bytes send!\n",n);

    n = recv(fd, buff, BUFSIZ, 0);
    if (n < 0)
        perror("received failed!");

    printf("%d bytes received!\n", n);
    for (nlp = (struct nlmsghdr*)buff; \
            (nlp->nlmsg_type != NLMSG_DONE)&& NLMSG_OK(nlp, n);
            nlp = NLMSG_NEXT(nlp, n)) {
        if ((rtp = get_gw_attr(nlp)) != NULL) {
            memcpy((void* )(gwp->sa_data), RTA_DATA(rtp),\
                    sizeof(struct in_addr));
            print_route(rtp);
            return 0;
        }
    }
    return -1;
}
static struct rtattr* get_gw_attr(struct nlmsghdr *nlmsghdr)
{
    struct rtattr *rta;
    int len;
    int gw;
    char str[16];

    len = nlmsghdr->nlmsg_len - NLMSG_LENGTH(sizeof(struct rtmsg));

    /** NLMSG_DATA(nlmsghdr) return the rtmsg
      pointer following, and RTM_RTA return
      the rtattr pointer following the rtmsg. **/
    rta = RTM_RTA(NLMSG_DATA(nlmsghdr));
    while (RTA_OK(rta, len)) {
        if (rta->rta_type >= RTA_MAX)
            break;
        /** We check if the address is INADDR_ANY.I
          don't know whethher this is needed **/
        if(rta->rta_type == RTA_GATEWAY &&\
                *(int *)RTA_DATA(rta) != INADDR_ANY)
            return rta;
        rta = RTA_NEXT(rta, len);
    }
    return NULL;
};
int print_route(struct rtattr *rtp)
{
    char str[16];
    const char *c;
    char buff[16];
    c = inet_ntop (AF_INET, RTA_DATA(rtp), buff, INET_ADDRSTRLEN);
    if (!c) {
        perror("inet_ntop failed !");
        exit (1);
    }
    printf("The gateway IP address is %s\n",c);
}


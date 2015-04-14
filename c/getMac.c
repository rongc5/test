#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <net/if.h>
#include <netinet/in.h>
#include <netinet/if_ether.h>
#include <sys/socket.h>
#include <linux/sockios.h>
#include <sys/ioctl.h>
#include <arpa/inet.h>
#include <errno.h>

char *mac_ntoa ( const unsigned char *mac )
{
    /* Linux 下有 ether_ntoa(),不过我们重新写一个也很简单 */
    static char buffer[18];
    memset ( buffer,0,sizeof ( buffer ) );
    sprintf ( buffer,"%02X:%02X:%02X:%02X:%02X:%02X",
              mac[0],mac[1],mac[2],mac[3],mac[4],mac[5] );
    //printf("mac:%s\n", buffer);
    return ( buffer );
}

int localInfo(void)
{
    int s;
    struct ifconf conf;
    struct ifreq *ifr = NULL;
    char buff[BUFSIZ];
    int num;
    int i;

    //ifr = (struct ifreq *)malloc(sizeof(struct ifreq));
    s = socket(PF_INET, SOCK_DGRAM, 0);
    struct sockaddr_in *sin;

    conf.ifc_len = BUFSIZ;
    conf.ifc_buf = buff;

    ioctl(s, SIOCGIFCONF, &conf);
    num = conf.ifc_len / sizeof(struct ifreq);
    ifr = conf.ifc_req;

    for(i=0;i < num;i++)
    {
    /*memset(ifr, 0, sizeof(struct ifreq));*/
	//strcpy(ifr->ifr_name, device);
        if (ioctl(s, SIOCGIFHWADDR, (char *)ifr) == -1)
	{
		fprintf(stderr, "ioctl SIOCGIFHWADDR:%s\n\a", strerror(errno));
	}
	//(ifr->ifr_hwaddr).sa_data[6] = '\0';
	printf("111:%s\t\n", ifr->ifr_name);
	/*if (strstr(ifr->ifr_name, "eth"))*/
	/*printf("Link encap:%s	", "Ethernet");*/
	/*else */
	/*printf("Link encap:%s	", "Local Loopback");*/
	/*printf("HWaddr:%s\n", mac_ntoa(ifr->ifr_hwaddr.sa_data));*/
	/*if (ioctl(s, SIOCGIFADDR, (char *)ifr) == -1)*/
	/*{*/
		/*fprintf(stderr, "ioctl SIOCGIFADDR:%s\n\a", strerror(errno));*/
	/*}*/
	/*sin = (struct sockaddr_in *)&(ifr->ifr_addr);*/
	/*printf("inet addr:%s	", inet_ntoa(sin->sin_addr));*/
	/*if (ioctl(s, SIOCGIFBRDADDR, (char *)ifr) == -1)*/
	/*{*/
		/*fprintf(stderr, "ioctl SIOCGIFBRDADDR:%s\n\a", strerror(errno));*/
	/*}*/
	/*sin = (struct sockaddr_in *)&(ifr->ifr_broadaddr);*/
	/*printf("Bcast:%s    ", inet_ntoa(sin->sin_addr));*/
	/*if (ioctl(s, SIOCGIFNETMASK, (char *)ifr) == -1)*/
	/*{*/
		/*fprintf(stderr, "ioctl SIOCGIFNETMASK:%s\n\a", strerror(errno));*/
	/*}*/
	/*sin = (struct sockaddr_in *)&(ifr->ifr_netmask);*/
	/*printf("Mask:%s\n", inet_ntoa(sin->sin_addr));*/
	
		
	
        ifr++;
	/*if (i < num - 1)*/
	/*printf("\n");*/
    }
} 

int main(void)
{
    localInfo();

    return 0;
}

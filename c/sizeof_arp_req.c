#include <stdlib.h>
#include <sys/param.h>
#include <sys/socket.h>
#include <linux/sockios.h>
#include <sys/file.h>
#include <sys/time.h>
#include <sys/signal.h>
#include <sys/ioctl.h>
#include <linux/if.h>
#include <linux/if_packet.h>
#include <linux/if_ether.h>
#include <net/if_arp.h>
#include <sys/uio.h>

#include <netdb.h>
#include <unistd.h>
#include <stdio.h>
#include <ctype.h>
#include <errno.h>
#include <string.h>
#include <netinet/in.h>
#include <arpa/inet.h>

typedef struct  send_arping{
    unsigned char arp_dst_hw[6];
    unsigned char arp_src_hw[6];
    unsigned short arp_type;
    unsigned short hw_type;
    unsigned short pro_type;
    unsigned char hw_len;
    unsigned char pro_len;
    unsigned short op;
    unsigned char src_hw[6];
    unsigned char src_ip[4];
    unsigned char dst_hw[6];
    unsigned char dst_ip[4];
} send_arping_t;





int main(void)
{
    send_arping_t sbuf;
    printf("%d\n", sizeof(sbuf));
    return 0;
}

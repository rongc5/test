#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <netinet/ip_icmp.h>
#include <sys/time.h>
#include <signal.h>

typedef struct send_ping {
	struct icmphdr icmp_hdr;
	char data[56];
} __attribute__((packed)) send_icmp_t;

typedef struct recv_ping {
	struct iphdr ip_hdr;	
	struct icmphdr icmp_hdr;
	char data[56];
} __attribute__((packed)) recv_icmp_t;

char *dstip;
int sendpacketnum;
int recvpacketnum;

unsigned short csum(char *buf, int size)
{
	unsigned short *base = (unsigned short *)buf;
	unsigned long sum = 0;

	while (size > 1) {
		sum += *(base++);
		if (sum >> 16)
			sum = (sum >> 16) + (sum & 0xffff);
		size -= 2;
	}

	if (size == 1) 
		sum += *((unsigned char *)base);
	while (sum >> 16)
		sum = (sum >> 16) + (sum & 0xffff);

	return ~sum;
}

void siginthandler(int s)
{
	printf("--- %s ping statistics ---\n", dstip);
	printf("%d packets transmitted, %d received, %d%% packet loss, "
			"\n", sendpacketnum, recvpacketnum, 
			(sendpacketnum - recvpacketnum )*100 / sendpacketnum);
	exit(1);
}

int send_packet(int sd, struct sockaddr_in *dst, int seq)
{
	int ret;
	send_icmp_t sbuf;
	memset(&sbuf, 0, sizeof(sbuf));	

	sbuf.icmp_hdr.type = 8;
	sbuf.icmp_hdr.code = 0;
	sbuf.icmp_hdr.checksum = 0;
	sbuf.icmp_hdr.un.echo.id = getpid();
	sbuf.icmp_hdr.un.echo.sequence = seq;
	strcpy(sbuf.data, "hello world");
	sbuf.icmp_hdr.checksum = csum((char *)&sbuf, sizeof(sbuf));

	sendpacketnum++;
	return sendto(sd, &sbuf, sizeof(sbuf), 0,
			(struct sockaddr *)dst, sizeof(*dst));

}

int recv_packet(int sd, struct timeval *prev, struct timeval *next)
{
	int ret;
	recv_icmp_t rbuf;
	memset(&rbuf, 0, sizeof(rbuf));
	unsigned long long p, n;
	double res;

	ret = recvfrom(sd, &rbuf, sizeof(rbuf), 0,
			NULL, NULL);
	if (ret == -1) {
		perror("recvfrom");
		return ret;
	} else {
		gettimeofday(next, NULL);
		if ((rbuf.icmp_hdr.type == 0) &&
				(rbuf.icmp_hdr.code == 0) &&
				(rbuf.icmp_hdr.un.echo.id == getpid())) {
			n = next->tv_sec * 1000000ull + next->tv_usec;
			p = prev->tv_sec * 1000000ull + prev->tv_usec;
			res = ((double)(n-p)) / 1000;
			printf("64 bytes from %s: icmp_seq = %d ttl=%d "
					"time=%.3f ms\n", 
					(char *)inet_ntoa(rbuf.ip_hdr.saddr),
					rbuf.icmp_hdr.un.echo.sequence,
					rbuf.ip_hdr.ttl, res);

		} else if ((rbuf.icmp_hdr.type == 3) &&
				(rbuf.icmp_hdr.un.echo.id == getpid())) { 
			if (rbuf.icmp_hdr.code == 0) {
				printf("net unreachable!\n");
			} else if (rbuf.icmp_hdr.code == 1) {
				printf("host unreachable!\n");
			} else if (rbuf.icmp_hdr.code == 2) {
				printf("protocol unreachable!\n");
			} else if (rbuf.icmp_hdr.code == 3) {
				printf("port unreachable!\n");
			} else {
				printf("other error!\n");
			}
		}else {
			printf("%s is not live!\n", 
					(char *)inet_ntoa(rbuf.ip_hdr.saddr));
		}
	}
	return 0;
}

int main(int c, char **v)
{
	//setuid(0);
	if (c != 2) {
		fprintf(stderr, "Usage...\n");
		exit(1);
	}

	setuid(0);
    //int uid = getuid();
    //setuid(uid);
	int sd, ret, seq = 0;
	struct sockaddr_in dst;
	socklen_t len;
	int opt;
	struct timeval prev, next;
	struct sigaction act;

	act.sa_handler = siginthandler;
	sigemptyset(&act.sa_mask);
	act.sa_flags = SA_RESTART;

	sigaction(SIGINT, &act, NULL);

	dstip = v[1];
	len = sizeof(dst);
	dst.sin_family = AF_INET;
	dst.sin_port = htons(0);
	inet_pton(AF_INET, v[1], &dst.sin_addr);

	sd = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);
	if (sd == -1) {
		perror("socket");
		exit(1);
	}


	while (1) {
		seq++;
		gettimeofday(&prev, NULL);
		send_packet(sd, &dst, seq);
		recv_packet(sd, &prev, &next);
		sleep(1);
	}
	close(sd);
	exit(0);
}

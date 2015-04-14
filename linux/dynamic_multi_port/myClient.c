#include "SrandPort.h"

#define CLIENTPORT 8888
#define SERVERPORT 9980


int main(int c, char **v)
{
    int len, ret, epfd, SockFd, serverport, tmpport;
    char buf[512];
    struct epoll_event revents;
    struct sockaddr_in sa;

    len = sizeof(struct sockaddr_in);
    
    if (c != 2)
    {
    	return 1;
    }

    while (1)
    {	
        SockFd = InitSockFd(CLIENTPORT, v[1], AF_INET, SOCK_STREAM, 0);

        epfd = InitEpoll(SockFd); 

        serverport = generate_dynamic_RandPort();   
        srand(getpid());
        do
        {
            tmpport = rand()%2;

        }while (tmpport < 0 && tmpport >= PORT_NUM);
        
        serverport += tmpport;
        Connect(SockFd, SERVERADDR, AF_INET, serverport);
        sprintf(buf, "I am a client from %s", v[1]);
		write(SockFd, buf, strlen(buf));
		
        while (1)
        {	
            ret = epoll_wait(epfd, &revents, 1, -1);

            if (ret >= 0)
            {
                if (revents.events & EPOLLIN)
                {
                    ret = read(revents.data.fd, buf, 256);
                    buf[ret] = '\0';
                    if (revents.data.fd == SockFd)
                    {
                        if (strlen(buf) == 0)
                        {
                            close(epfd);
                            close(SockFd);
                            break;
                        }
#if DEBUG                
                        if (!getpeername(SockFd, (struct sockaddr *)&sa, &len))
                            printf("server IP: %s and PORT: %d\n", inet_ntoa(sa.sin_addr), ntohs(sa.sin_port));
#endif
                        printf("from server:%s\n", buf);
                    }
                    else
                    {
                        write(SockFd, buf, strlen(buf));
#if DEBUG
                        printf("client myself:%s, pid = %u\n", buf, getpid());
#endif
                    }
                }
            }
        }
    }
}

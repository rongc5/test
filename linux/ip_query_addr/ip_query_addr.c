#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>
#include <mysql/mysql.h>
#include <sys/mman.h>

#include <fcntl.h>
#include <sys/types.h>
#include <math.h>

#include <sys/stat.h>
#include <netinet/in.h>
#include <errno.h>

#include <iconv.h>

typedef enum {
    UTF_t = 1,
    GBK_t
}Code_t;

typedef enum {
    COUNTRY = 1,
    PROVINCE,
    CITY,
    TOWN
}Area_L;

typedef struct{
    int id;
    char name[32];
}NameInfo;

typedef struct{
    Area_L level;
    int zipcode;
    NameInfo info[4]; //info[0]代表国家、info[1]代表省、info[2]代表城市、info[3]代表县、区
}IdInfo;

typedef struct{
    char *countrybuf;
    char *areabuf;
}AddrInfo;

typedef struct
{	
    //IdInfo id;
    AddrInfo addr;
    unsigned int ip[2];// ip[0]、ip[1]用户IP所在范围的开始地址、结束地址
}IpInfo;

typedef struct
{	
    //IdInfo id;
    char countrybuf[128];
    char areabuf[128];
    char ip[2][16];// ip[0]、ip[1]用户IP所在范围的开始地址、结束地址
}IpInfoTmp;


int ipsum; //第一条记录与最后一条记录的绝对地址偏移量
IpInfo *p_ip_info = NULL;
Code_t codetype;

//共享内存指针
char *p_share;
//第一条记录指针
char *p_begin;
char *p_end;


#define UNKNOWN ""
#define RLEN 7 //索引区记录长度

unsigned int getlong4(char *pos) //将读取的4个字节转化为长整型数
{
    unsigned int result=(((unsigned char )(*(pos+3)))<<24)
        +(((unsigned char )(*(pos+2)))<<16)
        +(((unsigned char )(*(pos+1)))<<8)
        +((unsigned char )(*(pos)));
    return result;
}

//把3字节转为整数
unsigned int getlong3(char *pos) //将读取的3个字节转化为长整型数
{
    unsigned int result=(((unsigned char )(*(pos+2)))<<16)
        +(((unsigned char )(*(pos+1)))<<8)
        +((unsigned char )(*(pos)));
    return result;
}

void openshare(char *path, off_t *filesize) // map a normal file as shared mem:
{
    int fd, err;
    struct stat statbuf;

    fd=open(path,O_RDWR,00777);
    if (-1 == fd){
        perror("open...");
        exit(1);
    }

    err = stat(path, &statbuf); 
    if (err == -1){
        perror("stat...");
        exit(1);
    }
    if  (S_ISREG(statbuf.st_mode)){
        p_share = (char*)mmap(NULL,statbuf.st_size, PROT_READ, MAP_PRIVATE, fd, 0);
        if(p_share==MAP_FAILED){
            perror("mmap...");
            exit(1);
        }
    }else {
        printf("%s id not a reg file!\n", path);
        exit(1);
    }

    *filesize = statbuf.st_size;

    close(fd);
    //第一条记录位置
    p_begin=p_share+getlong4(p_share);
    //最后一条记录位置
    p_end=p_share+getlong4(p_share+4);
    //记录总数
    ipsum=(getlong4(p_share+4)-getlong4(p_share))/RLEN + 1;
    // printf("total:%d\n", total_record);
}

void closeshare(off_t filesize)
{
    munmap( p_share, filesize);    
}

char *getarea(char *pos) {
    char *byte=pos; // 标志字节
    pos++;
    switch (*byte) {
        case 0: // 没有区域信息
            return UNKNOWN;
        case 1:
        case 2: // 标志字节为1或2，表示区域信息被重定向
            return p_share+getlong3(pos);
        default: // 否则，表示区域信息没有被重定向
            return byte;
    }
}

int my_iconv(char *charset_to, char *charset_from, char *buff_out, size_t len_out, char *buff_in, size_t len_in)
{
    int n = len_out;
    iconv_t convd;

    convd = iconv_open (charset_to, charset_from);
    if (convd == (iconv_t)(-1))
    {
        return 1;
    }
    iconv (convd, &buff_in, &len_in, &buff_out, &len_out);
    iconv_close(convd);

    return 0;
}

void getipaddr(char *start, char **countrybuf, char **areabuf)
{
    char *pos, *p_country = NULL, *p_area = NULL, buf[2][128];
		
		memset(buf, 0, sizeof(buf));
    unsigned int offset;
    int len[2];

    offset = getlong3(start);
    pos = p_share + offset + 4;
    char *byte = pos; // 标志字节
    pos++;
    switch (*byte) {
        case 1:{ // 标志字节为1，表示国家和区域信息都被同时重定向
                   int countryOffset = getlong3(pos); // 重定向地址
                   pos+=3;
                   pos=p_share+countryOffset;
                   byte = pos; // 标志字节
                   pos++;
                   switch (*byte) {
                       case 2: // 标志字节为2，表示国家信息又被重定向
                           {
                               p_country=p_share+getlong3(pos);
                               pos=p_share+countryOffset+4;
                               p_area = getarea(pos);
                           }
                           break;
                       default: // 否则，表示国家信息没有被重定向
                           {
                               p_country=byte;
                               p_area = getarea(p_country+strlen(p_country)+1);
                           }
                           break;
                   }
               }
               break;
        case 2: // 标志字节为2，表示国家信息被重定向
               {
                   p_country=p_share+getlong3(pos);// 重定向地址
                   pos = p_share + offset + 8;
                   p_area = getarea(pos);
               }
               break;
        default:{ // 否则，表示国家信息没有被重定向
                    p_country=byte;
                    p_area=getarea(p_country+strlen(p_country)+1);
                }
                break;
    }

    if (codetype == GBK_t){
        my_iconv("utf8", "gbk", buf[0], sizeof(buf[0]), p_country, strlen(p_country));
        my_iconv("utf8", "gbk", buf[1], sizeof(buf[1]), p_area, strlen(p_area));
        //printf("buf:%s	%s,	%d	%d\n", buf[0], buf[1], strlen(buf[0]), strlen(buf[1]));
        
        len[0] = strlen(buf[0]), len[1] = strlen(buf[1]);
              
        if (len[0]){
        						*countrybuf = (char *)malloc(len[0] + 1);
										if (!(*countrybuf)){
												perror("malloc");
												exit(1);
										}										
										memcpy(*countrybuf, buf[0], len[0]+1); 	
        	}else {
        		*countrybuf = NULL;
        	}

          if (len[1]){
									 *areabuf = (char *)malloc(len[1] + 1);
										if (!(*areabuf)){
												perror("malloc");
												exit(1);
										}										
										memcpy(*areabuf, buf[1], len[1]+1);
									
        	}else {
        		*areabuf = NULL;
        	}  
    }else if (codetype == UTF_t){
    		len[0] = strlen(p_country), len[1] = strlen(p_area);
    		
    		 if (len[0]){
    		 				     *countrybuf = (char *)malloc(len[0] + 1);
										if (!(*countrybuf)){
												perror("malloc");
												exit(1);
										}										
										memcpy(*countrybuf, p_country, len[0]+1); 	
        			  
        	}else {
        		*countrybuf = NULL;
        	}
        
          if (len[1]){			    			
				    				*areabuf = (char *)malloc(len[1] + 1);
										if (!(*areabuf)){
												perror("malloc");
												exit(1);
										}										
										memcpy(*areabuf, p_area, len[1]+1);
				    			
        	}else {
        		*areabuf = NULL;
        	}  		
   	}
		
    //printf("areabuf:%s	%s\n", *countrybuf, *areabuf);
}

int init_cache_ipinfo(char *path)
{
    int i = 0;
    unsigned int tmp, endip;
    char ip[2][16];
		off_t filesize = 0;
    openshare(path, &filesize);

    //char * pos = p_share + getlong4(p_share);

    char * pos = p_begin;
    endip = getlong4(p_end);
    //printf("1111%u\n", getlong4(pos));
    IpInfo * buf;
		
		buf = (IpInfo *)malloc(sizeof(IpInfo)*ipsum);
    if (buf == NULL){
        printf("%s  %s  %d\n", strerror(errno), __func__, __LINE__);
        exit(1);	
    }

    memset(buf, 0, sizeof(IpInfo)*ipsum);
		

    while (i < ipsum){	
    		//memset(ip, 0, sizeof(ip));
    		pos = p_begin + i*RLEN;
    				
        buf[i].ip[0] = getlong4(pos);
        buf[i].ip[1] = getlong4(p_share+getlong3(pos + 4));
        getipaddr(pos+4, &buf[i].addr.countrybuf, &buf[i].addr.areabuf);		
    		#if 0    
        tmp = htonl(buf[i].ip[0]);
        inet_ntop(AF_INET, &tmp, ip[0], sizeof(ip[0]));
        tmp = htonl(buf[i].ip[1]);
        inet_ntop(AF_INET, &tmp, ip[1], sizeof(ip[1]));
        printf("i:%d	%s	%s	%s	%s\n", i, ip[0], ip[1], buf[i].addr.countrybuf, buf[i].addr.areabuf);
        #endif
        i++;	
    }
    closeshare(filesize);
    
    p_ip_info = buf;
    return 0;
}

int show_cache_ipinfo(void)
{
	int i;
	unsigned int tmp;
	char ip[2][16];
	IpInfo * buf = p_ip_info;
	
	for (i = 0; i < ipsum; i++){
		 memset(ip, 0, sizeof(ip));
		 tmp = htonl(buf[i].ip[0]);
     inet_ntop(AF_INET, &tmp, ip[0], sizeof(ip[0]));
     tmp = htonl(buf[i].ip[1]);
     inet_ntop(AF_INET, &tmp, ip[1], sizeof(ip[1]));
     printf("i:%d	%s	%s	%s	%s\n", i + 1, ip[0], ip[1], buf[i].addr.countrybuf, buf[i].addr.areabuf);
	}
}

int export_all_records(void)
{
	int i;
	unsigned int tmp;
	char ip[2][16];
	IpInfo * buf = p_ip_info;


    FILE * fp = fopen("ip.txt", "w");
    if (fp == NULL){
        perror("fopen...");
        exit(1);
    }

	
	for (i = 0; i < ipsum; i++){
		 memset(ip, 0, sizeof(ip));
		 tmp = htonl(buf[i].ip[0]);
     inet_ntop(AF_INET, &tmp, ip[0], sizeof(ip[0]));
     tmp = htonl(buf[i].ip[1]);
     inet_ntop(AF_INET, &tmp, ip[1], sizeof(ip[1]));
     fprintf(fp, "i:%d	%s	%s	%s	%s\n", i + 1, ip[0], ip[1], buf[i].addr.countrybuf, buf[i].addr.areabuf);
	}

    fclose(fp);

    return 0;
}

int getIpByAddr(char *v)
{
	int i, n = 0, flag = 0;
	unsigned int tmp;
	char ip[2][16];
	IpInfo * buf = p_ip_info;
	
	if (!v)
		return -1;
	
	for (i = 0; i < ipsum; i++){
		
		if (buf[i].addr.areabuf){
			if (strstr(buf[i].addr.areabuf, v))
				flag = 1;
		}
		
		if (buf[i].addr.countrybuf){
				if (strstr(buf[i].addr.countrybuf, v))
					flag = 1;
			}
		if (!flag)
			continue;
			flag = 0;
			n++;
		 memset(ip, 0, sizeof(ip));
		 tmp = htonl(buf[i].ip[0]);
     inet_ntop(AF_INET, &tmp, ip[0], sizeof(ip[0]));
     tmp = htonl(buf[i].ip[1]);
     inet_ntop(AF_INET, &tmp, ip[1], sizeof(ip[1]));
     printf("%d:	%s	%s	%s	%s\n", n, ip[0], ip[1], buf[i].addr.countrybuf, buf[i].addr.areabuf);
	}
	
	return 0;	
	
}

int getIpInfo(char *ipstr, IpInfoTmp *tmp)
{
    unsigned int ip, n;
    int high, low, mid;

    IpInfo * buf = p_ip_info;

    ip = ntohl(inet_addr(ipstr));
    //htonl(inet_addr(ipstr));
    low = 0, high = ipsum - 1;
    while(low <= high)
    {
        mid = (high + low)/2;
        if (ip < buf[mid].ip[0]){
            high = mid - 1;
        } else if (ip > buf[mid].ip[1]){
            low = mid + 1;
        } else if (ip >= buf[mid].ip[0] && ip <= buf[mid].ip[1]){ 
            n = htonl(buf[mid].ip[0]);
            inet_ntop(AF_INET, &n, tmp->ip[0], sizeof(tmp->ip[0]));
            n = htonl(buf[mid].ip[1]);
            inet_ntop(AF_INET, &n, tmp->ip[1], sizeof(tmp->ip[1]));
						memcpy(tmp->countrybuf, buf[mid].addr.countrybuf, strlen(buf[mid].addr.countrybuf));
						memcpy(tmp->areabuf, buf[mid].addr.areabuf, strlen(buf[mid].addr.areabuf));
            return 0;
        }
    }

    return -1;
}

int prse_buf(char *buf, char *path, char *type)
{
    char *p;
    int i = 0;
    if ((p = strstr(buf, "qqwrydat_path"))){
        while (*p != '=')
            p++;
        p++;
        while (*p != '\n' && *p != '\0')
            path[i++] = *p++;
        path[i] = '\0';
        //printf("%s\n", qqwrydat_path);
        return 0;
    }

    if ((p = strstr(buf, "qqwrydat_type"))){
        while (*p != '=')
            p++;
        p++;
        while (*p != '\n' && *p != '\0')
            type[i++] = *p++;
        type[i] = '\0';
        //printf("%s\n", qqwrydat_type);
        return 0;
    }
}

int init_from_file(char *v, char *path)
{
    char buf[512];
    char type[32];
    char *retptr;
    int i = 0;
    FILE * fp = fopen(v, "r");
    if (fp == NULL){
        perror("fopen...");
        exit(1);
    }

    while (1){
        retptr = fgets(buf, 512, fp);
        if (NULL == retptr) {
            if (feof(fp))
                break;
            else {
                perror("fgets...");
                continue;
            }
        }

        if (!strstr(buf, "#")){
            prse_buf(buf, path, type);
        }
    }

    if (strstr(type, "UTF8") || strstr(type, "utf8") || strstr(type, "UTF-8") || strstr(type, "utf-8"))
        codetype = UTF_t;
    else
        codetype = GBK_t;

    fclose(fp);

    return 0;
}

const char * help = "               0 means search by ip\n"
                    "               1 means search by addr\n"
                    "               2 means show all info\n"
                    "               3 means export all records\n"
                    "               4 means ESC the application!!!\n";
int main(int c, char **v)
{
    if (c != 2){
        perror("usage...");
        return 0;
    }

    char buf[256], path[32];
    IpInfoTmp tmp;
    int ret, type;

    init_from_file(v[1], path);	
    init_cache_ipinfo(path);		
		
			
    while (1){
            printf("%s\n", help);
    		printf("please input your Choose: ");
    					fflush(stdout);
    					ret = read(0, buf, sizeof(buf));
    					buf[ret - 1] = '\0';
    					type = atoi(buf);
    					
    	switch (type)
    	{
    		case 0:
    			    memset(&tmp, 0, sizeof(IpInfoTmp));
			        printf("please input ipstr:");
			        fflush(stdout);
			        ret = read(0, buf, sizeof(buf));
			        buf[ret - 1] = '\0';
			        if (!getIpInfo(buf, &tmp)){	
			            printf("\n%s  %s  %s  %s\n", tmp.countrybuf, tmp.areabuf, tmp.ip[0], tmp.ip[1]);
			        }
			        break;
			   case 1:
                    printf("please input city name or addr name:");
                    fflush(stdout);
			        ret = read(0, buf, sizeof(buf));
			        buf[ret - 1] = '\0';
			        getIpByAddr(buf);
			        break;
			    case 2:
			    		show_cache_ipinfo();
			    		break;
                case 3:
                        export_all_records();
                        break;
                case 4:
                    goto OUT;
			   	default:
			   			printf("\n your input is err!\n");
    	}
    }
    	
OUT:

    return 0;
}

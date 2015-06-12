
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


#define UNKNOWN "unknow"
#define INET6_ADDRSTRLEN 46
#define RECORD_LEN 7 //单条记录长度

typedef enum {
	UTF_t = 1,
	GBK_t
}CodeTypes_t;

//结果集
typedef struct
{
	char *p_country;
	char *p_area;
	char countrybuf[32];
	char areabuf[32];
	char beginip[INET6_ADDRSTRLEN]; // 用户IP所在范围的开始地址
	char endip[INET6_ADDRSTRLEN]; // 用户IP所在范围的结束地址
}location;


static CodeTypes_t codetype;

static char qqwrydat_path[30];
static char qqwrydat_type[10];

//共享内存指针
char *p_share;
//第一条记录指针
char *p_begin;
char *p_end;
//总记录数
int total_record;

off_t filesize = 0;

//把4字节转为整数
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

/**
 * 打开共享内存指针
 *
 * @return void
 */
void openshare(char *qqwrydat_path) // map a normal file as shared mem:
{
	int fd, err;
	struct stat statbuf;

	fd=open(qqwrydat_path,O_RDWR,00777);
	if (-1 == fd){
		perror("open...");
		exit(1);
	}

	err = stat(qqwrydat_path, &statbuf); 
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
		printf("%s id not a reg file!\n", qqwrydat_path);
		exit(1);
	}

	filesize = statbuf.st_size;

	close(fd);
	//第一条记录位置
	p_begin=p_share+getlong4(p_share);
	//最后一条记录位置
	p_end=p_share+getlong4(p_share+4);
	//记录总数
	total_record=(getlong4(p_share+4)-getlong4(p_share))/RECORD_LEN;
   // printf("total:%d\n", total_record);
}

/**
 * 关闭共享内存指针
 *
 * @return void
 */
void closeshare()
{
	munmap( p_share, filesize);    
}

/**
 * 返回地区信息
 *
 * @char *pos 地区的指针
 * @return char *
 */
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
//获得ip所属地理信息,isp
void getipinfo(char *ipstr,location *p_loc)
{
	char *pos = p_share;
	int record_len=10;
	char *firstip=0; // first record position
	//把ip转为整数
	unsigned int ip=htonl(inet_addr(ipstr));
	firstip=p_begin;

	int l=0;
	int u=total_record;
	int i=0;
	char* findip=firstip;
	unsigned int beginip=0;
	unsigned int endip=0;
	//二分法查找
	while(l <= u)
	{
		i=(l+u)/2;
		pos=firstip+i*RECORD_LEN;
		beginip = getlong4(pos);
		pos+=4;
		if(ip<beginip)
		{
			u=i-1;    
		}
		else
		{
			endip=getlong4(p_share+getlong3(pos));
			if(ip>endip)
			{
				l=i+1;        
			}
			else
			{
				findip=firstip+i*RECORD_LEN;
				break;    
			}
		}
	}
	int offset = getlong3(findip+4);
	pos=p_share+offset;
	endip= getlong4(pos); // 用户IP所在范围的结束地址
	pos+=4;

	unsigned int j=ntohl(beginip);
	inet_ntop(AF_INET,&j,p_loc->beginip, INET6_ADDRSTRLEN);// 获得开始地址的IP字符串类型
	j=ntohl(endip);
	inet_ntop(AF_INET,&j,p_loc->endip, INET6_ADDRSTRLEN);// 获得结束地址的IP字符串类型

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
						       p_loc->p_country=p_share+getlong3(pos);
						       pos=p_share+countryOffset+4;
						       p_loc->p_area = getarea(pos);
					       }
					       break;
				       default: // 否则，表示国家信息没有被重定向
					       {
						       p_loc->p_country=byte;
						       p_loc->p_area = getarea(p_loc->p_country+strlen(p_loc->p_country)+1);
					       }
					       break;
			       }
		       }
		       break;
		case 2: // 标志字节为2，表示国家信息被重定向
		       {
			       p_loc->p_country=p_share+getlong3(pos);// 重定向地址
			       pos = p_share + offset + 8;
			       p_loc->p_area = getarea(pos);
		       }
		       break;
		default:{ // 否则，表示国家信息没有被重定向
				p_loc->p_country=byte;
				p_loc->p_area=getarea(p_loc->p_country+strlen(p_loc->p_country)+1);
			}
			break;
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

static int prse_buf_cityid(char *buf)
{
	char *p;
	int i = 0;
	if ((p = strstr(buf, "qqwrydat_path"))){
		while (*p != '=')
			p++;
		p++;
		while (*p != '\n' && *p != '\0')
			qqwrydat_path[i++] = *p++;
		qqwrydat_path[i] = '\0';
		//printf("%s\n", qqwrydat_path);
		return 0;
	}

	if ((p = strstr(buf, "qqwrydat_type"))){
		while (*p != '=')
			p++;
		p++;
		while (*p != '\n' && *p != '\0')
			qqwrydat_type[i++] = *p++;
		qqwrydat_type[i] = '\0';
		//printf("%s\n", qqwrydat_type);
		return 0;
	}

}



int init_from_file(char *v)
{
	char buf[512];
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
			prse_buf_cityid(buf);
		}
	}


	if (strstr(qqwrydat_type, "UTF8") || strstr(qqwrydat_type, "utf8") || strstr(qqwrydat_type, "UTF-8") || strstr(qqwrydat_type, "utf-8"))
		codetype = UTF_t;
	else
		codetype = GBK_t;

	return 0;
}

void check_ipinfo(location * locInfo)
{
	int ret;
	if (codetype == GBK_t){
		ret = my_iconv("utf8", "gbk", locInfo->countrybuf, sizeof(locInfo->countrybuf), locInfo->p_country, strlen(locInfo->p_country));
		if (ret == 1)
			return ;
		ret = my_iconv("utf8", "gbk", locInfo->areabuf, sizeof(locInfo->areabuf), locInfo->p_area, strlen(locInfo->p_area));
		if (ret == 1)
			return ;

		locInfo->p_country = locInfo->countrybuf;
		locInfo->p_area= locInfo->areabuf;
	} else if (codetype == UTF_t){
		memcpy(locInfo->countrybuf, locInfo->p_country, strlen(locInfo->p_country));
		memcpy(locInfo->areabuf, locInfo->p_area, strlen(locInfo->p_area));
		locInfo->p_country = locInfo->countrybuf;
		locInfo->p_area= locInfo->areabuf;

	}

}


int main(int c, char **v)
{
	if (c != 2){
		perror("usage...");
		return 0;
	}
	char buf[256];
	int ret;
	location loc;
	init_from_file(v[1]);
	openshare(qqwrydat_path);
	while (1){
		memset(&loc, 0, sizeof(location));
		printf("please input ipstr:");
		fflush(stdout);
		ret = read(0, buf, 256);
		buf[ret - 1] = '\0';
		getipinfo(buf,&loc);
		check_ipinfo(&loc);
		printf("%s  %s  %s  %s\n", loc.p_country, loc.p_area, loc.beginip, loc.endip);
	}
	return 0;
}
 

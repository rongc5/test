#include <stdio.h>  
#include <string.h>  
#include <unistd.h>  
#include <sys/stat.h>  
#include <sys/types.h>  
#include <fcntl.h>  
#include <dirent.h>  
#ifndef DEBUG  
    #define PDEBUG(fmt, args...)  
#else  
    #define PDEBUG(fmt, args...) \  
    printf("[%s:%d]"fmt, __func__, __LINE__, ##args)  
#endif  
#ifndef ERROR  
    #define PERROR(err)  
#else  
    #define PERROR(err) \  
    printf("[%s:%d]\n", __func__, __LINE__);\  
    perror(err)  
#endif  
#define BUFFER_SIZE 1024  
/* 
 * 判断是否是目录 
 * @ 是目录返回1，是普通文件返回0，出错返回-1 
 * */  
int IsDir(const char *path)  
{  
    struct stat buf;  
    if (stat(path, &buf)==-1)  
    {  
        PERROR("stat");
        PDEBUG("path = %s", path);  
        return -1;  
    }  
    return S_ISDIR(buf.st_mode);  
}  
/* 
 * 创建目录 
 * @ 可以创建多级目录，失败返回-1 
 * */  
int CreateDir(const char *path)  
{  
    char pathname[256];  
    strcpy(pathname, path);  
    int i, len=strlen(pathname);  
    if (pathname[len-1]!='/')  
    {  
        strcat(pathname, "/");  
        len++;  
    }  
    for (i=0; i<len; i++)  
    {  
        if (pathname[i]=='/')  
        {  
            pathname[i]=0;  
            if (access(pathname, F_OK)) //判断路径是否存在  
            {   //不存在则创建  
                if (mkdir(pathname, 0755)==-1)  
                {  
                    PERROR("mkdir");  
                    return -1;  
                }  
            }  
            pathname[i]='/';  
        }  
    }  
    return 0;  
}  
/* 
 * 拷贝文件 
 * @ @dstpath -- 可以是文件名也可以是目录名 
 * */  
int FileCopy(const char *srcpath, const char *dstpath)  
{  
    int srcfd, dstfd, file_len, ret=1;  
    char buffer[BUFFER_SIZE];  
    char dstfn[256];  
    if (access(srcpath, R_OK))  
    {  
        PDEBUG("不能拷贝不存在或不可读文件: %s/n", srcpath);  
        return -1;  
    }  
    strcpy(dstfn, dstpath);  
    //如果@dstpath存在且是目录则在其后加上srcpath的文件名  
    if (access(dstpath, F_OK)==0 && IsDir(dstpath)==1)  
    {  
        if (dstfn[strlen(dstfn)-1]!='/')  
        {  
            strcat(dstfn, "/");  
        }  
        if (strchr(srcpath, '/'))  
        {  
            strcat(dstfn, strrchr(srcpath, '/'));  
        }  
        else  
        {  
            strcat(dstfn, srcpath);  
        }  
    }  
    srcfd=open(srcpath, O_RDONLY);  
    dstfd=open(dstfn, O_WRONLY|O_CREAT);      
    if (srcfd==-1 || dstfd==-1)  
    {  
        if (srcfd!=-1)  
        {  
            close(srcfd);  
        }  
        PERROR("open");  
        return -1;  
    }  
      
    file_len= lseek(srcfd, 0L, SEEK_END);  
    lseek(srcfd, 0L, SEEK_SET);  
    while(ret)  
    {  
        ret= read(srcfd, buffer, BUFFER_SIZE);  
        if (ret==-1)  
        {  
            PERROR("read");  
            close(srcfd);  
            close(dstfd);  
            return -1;  
        }  
        write(dstfd, buffer, ret);  
        file_len-=ret;  
        bzero(buffer, BUFFER_SIZE);  
    }  
    close(srcfd);  
    close(dstfd);  
    if (ret)  
    {  
        PDEBUG("文件: %s, 没有拷贝完/n", srcpath);  
        return -1;  
    }  
    return 0;  
}  
/* 
 * 目录拷贝 
 * */  
int DirCopy(const char *srcpath, const char *dstpath)  
{  
    int ret;  
    DIR * dir;  
    struct dirent *ptr;  
    char frompath[256];  
    char topath[256];  
    if (!(ret=IsDir(srcpath))) //如果@srcpath 是文件，直接进行文件拷贝  
    {  
        FileCopy(srcpath, dstpath);  
        return 0;  
    }  
    else if(ret!=1)  //目录或文件不存在  
    {  
        return -1;  
    }  
    dir = opendir(srcpath);  
    CreateDir(dstpath);  
    while((ptr=readdir(dir))!=NULL)  
    {  
        bzero(frompath, 256);  
        bzero(topath, 256);  
        strcpy(frompath, srcpath);  
        strcpy(topath, dstpath);  
        if (frompath[strlen(frompath)-1]!='/')  
        {  
            strcat(frompath, "/");  
        }  
        if (topath[strlen(topath)-1]!='/')  
        {  
            strcat(topath, "/");  
        }  
        strcat(frompath, ptr->d_name);  
        printf("%s/n", frompath);  
        strcat(topath, ptr->d_name);  
        if ((ret=IsDir(frompath))==1)  
        {  
            if (strcmp(strrchr(frompath, '/'), "/.")==0  
                || strcmp(strrchr(frompath, '/'), "/..")==0)  
            {  
                PDEBUG(". or ..目录不用复制/n");  
            }  
            else  
            {  
                DirCopy(frompath, topath);  
            }  
        }  
        else if (ret!=-1)  
        {  
            FileCopy(frompath, topath);  
        }  
    }  
    closedir(dir);  
    return 0;  
}  
int main(int argc, char *argv[])  
{  
    int ret;  
    if ((ret=IsDir(argv[1]))==1)  
    {  
        PDEBUG("Is a dir/n");  
    }  
    else if (ret!=-1)  
    {  
        PDEBUG("Is a file/n");  
    }  
    DirCopy(argv[1], argv[2]);  
    return 0;  
}  

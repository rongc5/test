#include <stdio.h>
#include <stdlib.h>
#include <iconv.h>
#include <string.h>


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




int main()
{
        iconv_t cd;
        char instr[]="房山";
        char *inbuf;
        char *outbuf;
        char *outptr;
        unsigned int insize=strlen(instr);
        unsigned int outputbufsize=10;
        unsigned int avail=outputbufsize;
        unsigned int nconv;

        inbuf=instr;
        outbuf=(char *)malloc(outputbufsize);
        outptr=outbuf;    //使用outptr作为空闲空间指针以避免outbuf被改变
          memset(outbuf,'\0',outputbufsize);

        //cd=iconv_open("gbk","utf8");    //将字符串编码由utf-8转换为gbk
		cd=iconv_open("utf8","gbk"); 
        if(cd==(iconv_t)-1)
        {
                printf("fail.\n");
        }
        nconv=iconv(cd,&inbuf,&insize,&outptr,&avail);

        printf("%s\n",outbuf);

        return 0;
}



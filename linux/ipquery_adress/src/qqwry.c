/**
 * ????写??stringIpToLong??????????sscanf?????????卸?ip??址????
 *
 *
 * @author _hhh_
 * @version 0.8
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>

#include "qqwry.h"
#include "localIp.h"

extern FILE *ipFile;
extern unsigned long ipBegin, ipEnd, ipSum;

/**
 * Ip to string conversion unsigned long
 * @param char *ipString
 * @return unsigned long ret
 */
unsigned long stringIpToLong(char *ipString)
{
    int num[4];
    char ch =0;
    unsigned long ret = 0;
    if(sscanf(ipString, "%3d.%3d.%3d.%3d%c",&num[0], &num[1], &num[2], &num[3], &ch) != 4
          //  || ch != 0  //sscanf???? !=4??括??ch??为0
            || num[0] > 0xFF 
            || num[1] > 0xFF
            || num[2] > 0xFF
            || num[3] > 0xFF
            || num[0] < 0x00 
            || num[1] < 0x00
            || num[2] < 0x00
            || num[3] < 0x00){
        puts("ip address error");
        exit(0);
    }

    ret = ret | (num[0] << 24) | (num[1] << 16) | (num[2] << 8) | (num[3]);
    /*  
        char tmp[16];
        unsigned long ret = 0;
        strcpy(tmp, ipString);
        char *str = strtok(tmp, ".");
        ret = ret | (atol(str) << 24);
        str = strtok(NULL, ".");
        ret = ret | (atol(str) << 16);
        str = strtok(NULL, ".");
        ret = ret | (atol(str) << 8);
        str = strtok(NULL, ".");
        ret = ret | (atol(str));
        */
    printf("ret = %d\n", ret);
    return ret;
}

/**
 * ??ip??????? ?????
 * @param char *ipString
 * @return unsigned long 
 */
unsigned long seekIpBinarySearch(char *ipString)
{
    unsigned long ipNum = stringIpToLong(ipString);
    unsigned long start = 0L;
    unsigned long end = ipSum;
    unsigned long mid = 0L;
    unsigned long addr[2] = {0L};

    while(start != end) {
        mid = (start + end) / 2;
        fseek(ipFile, ipBegin + mid * RECORD_LENGTH, SEEK_SET);
        fread(addr, 1, 7, ipFile);
        if(ipNum == addr[0]) {
            return addr[1];
        } else if(end == start +1) {
            return addr[1];
        } else if(ipNum > addr[0]){
            start = mid;
        } else/* if(ipNum > addr[0])*/{
            end = mid;
        }
    }

}

/**
 * ???offset????????string, ???????
 * @param char *string 
 * @param unsigned long offset
 * @return void
 */
void readString(char *string, unsigned long offset)
{
    fseek(ipFile, offset, SEEK_SET);
    while(*string++ = fgetc(ipFile))
        ;
}

/**
 * ?????????????string, ???????
 * @param char *string 
 * @param unsigned long offset
 * @return void
 */
void readStringCur(char *string)
{
    while(*string++ = fgetc(ipFile))
        ;
}
/**
 * ???offset?????????string
 * @param char *string 
 * @param unsigned long offset
 * @return void
 */
void seekCountryArea(char *string, unsigned long offset)
{
    fseek(ipFile, offset+4, SEEK_SET);
    char flag = fgetc(ipFile);
    unsigned long countryOffset[1] = {0L};
    if (AREA_FOLLOWED == flag) {        
        fread(countryOffset, 1, 3, ipFile);
        fseek(ipFile, countryOffset[0], SEEK_SET);
        flag = fgetc(ipFile);
        if(NO_AREA == flag) {
            unsigned long tmp = countryOffset[0];
            fread(countryOffset, 1, 3, ipFile);
            fseek(ipFile, countryOffset[0], SEEK_SET);
            readStringCur(string);
            string = strcat(string, " ");
            fseek(ipFile, tmp+4, SEEK_SET);
        } else {
            fseek(ipFile, -1, SEEK_CUR);
            readStringCur(string);
            string = strcat(string, " ");

        }

    } else if (NO_AREA == flag){
        fread(countryOffset, 1, 3, ipFile);
        readString(string, countryOffset[0]);
        string = strcat(string, " ");
        fseek(ipFile, offset+8, SEEK_SET);
    } else {
        readString(string, offset+4);
        string = strcat(string, " ");
    }
    readArea(string);
}

/**
 * ????????string??? ????????????
 * @param char *string
 * @return void
 */
void readArea(char *string)
{

    unsigned long offset[1] = {0L};
    char area[255] = "";
    int  flag = fgetc(ipFile);
    if (AREA_FOLLOWED == flag || NO_AREA == flag) {
        fread(offset, 1, 3, ipFile);
        readString(area, offset[0]);
        string = strcat(string, area);
    } else {
        fseek(ipFile, -1, SEEK_CUR);
        readStringCur(area);
        string = strcat(string, area);
    }
}

/**
 * ?????????
 */
void checkCmdLine(int argc, char **argv){
    if(argc == 1) {
        //?????
        unsigned long ll = seekIpBinarySearch("255.255.255.0");
        char string[255]="";
        seekCountryArea(string,ll);
        printf("%s %lu条\n", string, ipSum);

        //????ip??
        localIp(string);
        printf("\n本机IP%s\n",string);
        ll = seekIpBinarySearch(string);
        seekCountryArea(string,ll);
        printf("IP adress:%s\n",string);

        //??
        puts("----------------------------------------");
        puts("Check ip address\n");
        printf("\n 用法\%s ip address\n 例如: %s 127.0.0.1\n", argv[0], argv[0]);

        puts("\nExtracting database\n");
        printf("\n 用法\%s -o filename\n 例如:%s -o qqwry.txt\n", argv[0], argv[0]);

        puts("\nSearch string\n");
        printf("\n 用法%s -s string\n 例如:%s -s 北京\n", argv[0], argv[0]);
        puts("----------------------------------------");
        exit(1);
    //??ip??
    }else if(argc==2){
        puts(argv[1]);
        char string[255]="";
        unsigned long ll = seekIpBinarySearch(argv[1]);//?????ip????
        seekCountryArea(string, ll);//?||????ip????
        puts(string);  
    }else if(argc == 3 ){
        //????
        if(strcmp(argv[1],"-o")==0){
            puts("准备导出");
            //???????(????????? ????)
            if(access(argv[2],F_OK) == 0){
               printf("是否覆盖%s(Y/N)",argv[2]);
               char ch = 0;
               while( (ch = tolower(getchar())) != 'y' && ch !='n') {
                   printf("是否覆盖%s(Y/N)",argv[2]);
               }
               if(ch =='n')
                   exit(1);
            }           
            exportData(argv[2]);
        }else if(strcmp(argv[1],"-s")==0){
            searchIpByName(argv[2]);
        }else{
            puts("请联系...........");
        }
    //????
    }else{
        puts("请联系...........");
    }
}

/**
 * ?拥?前位?枚?取4???纸?转??为ip?址???
 * @param char *string ????ip?址???
 */
void readIpString(char *string)
{
// if(sizeof(string) < 16)
//     realloc(string, 16);
sprintf(string, "%d.%d.%d.%d", fgetc(ipFile), fgetc(ipFile), fgetc(ipFile), fgetc(ipFile));
//  free(string);
}

/**
 * ??????录
 * @param char *filename ?募???
 *
 */
void exportData(char *fileName)
{
    FILE *fp = fopen(fileName, "wb");
    unsigned long i=0;
    unsigned long data[1]={0};
    char string[255]="";
    unsigned long di = (ipSum/100);
    int j=0;
    while(!feof(ipFile) && i != ipSum){
        //??始ip
        fseek(ipFile, ipBegin + RECORD_LENGTH * i, SEEK_SET);
        readIpString(string);
        fprintf(fp, "%-16s", string);
        //????ip
        fread(data, 1, 3, ipFile);
        fseek(ipFile, data[0], SEEK_SET);
        readIpString(string);
        fprintf(fp, "%-16s", string);
        //???业???
        seekCountryArea(string, data[0]);
        fputs(string, fp);
        fputs("\r\n",fp); // \r\n for windows
        //????????
        if(i%di==0){
            if(j>9)
                putchar('\b');
            putchar('\b');
            putchar('\b');
            if(j%2==0)
                putchar('#');
            printf("%d%%",j++);
        }
        ++i;
    }   
    puts("\n????????");
}

/**
 * ???????业????邪???某?址?????ip??
 * @param char *name 要???????址???
 */
void searchIpByName(char *name)
{
    unsigned long i=0;
    unsigned long data[1]={0};
    char string[255]="";
    unsigned long j=0;
    int k=0;
    while(!feof(ipFile) && i != ipSum){

        fseek(ipFile, ipBegin + RECORD_LENGTH * i +4, SEEK_SET);
        fread(data, 1, 3, ipFile);
        seekCountryArea(string, data[0]);
        if(strstr(string, name) != NULL) {
            while(k>0){
                putchar('\b');
                --k;
            }
            char ipStr[16]="";
            fseek(ipFile, ipBegin + RECORD_LENGTH * i, SEEK_SET);
            readIpString(ipStr);
            printf("%-16s", ipStr);

            fread(data, 1, 3, ipFile);
            fseek(ipFile, data[0], SEEK_SET);
            readIpString(ipStr);
            printf("%-16s", ipStr);
            
            //printf("string: %s\n", string);
            puts(string);
            ++j;
        }else{
            //?却?
             if(i%(ipSum/50)==0){
                 putchar('>');
                 ++k;
             }
        }
        ++i;
    }
    
    while(k>0){
        putchar('\b');
        --k;
    }
    printf("总共搜索到%lu条记录 ",j);
}

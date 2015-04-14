#ifndef QQWRY_H
#define QQWRY_H

#define RECORD_LENGTH 0x07  //ip记录长度
#define AREA_FOLLOWED 0X01  //模式1
#define NO_AREA       0x02  //模式2

typedef struct ipInfo
{
    char ipBegin[15];            //ip段开始地址
    char ipEnd[15];              //ip段结束地址
    char countryAndArea[255];   //ip段对应的国家和地区
} ipInfo;

unsigned long stringIpToLong(char *ipString);
unsigned long seekIpBinarySearch(char *ipString);
void readString(char *string, unsigned long offset);
void readStringCur(char *string);
void seekCountryArea(char *string, unsigned long offset);
void readArea(char *string);
void checkCmdLine(int argc, char **argv);
void readIpString(char *string);
void exportData(char *fileName);
void searchIpByName(char *name);

#endif


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "qqwry.h"

FILE *ipFile         = NULL; 
unsigned long ipBegin  = 0; 
unsigned long ipEnd    = 0; 
unsigned long ipSum    = 0; 


int main(int argc, char **argv)
{
    ipFile = fopen("./qqwry.dat", "rb");
    if(!ipFile){
        puts("qqwry.dat open failed\n");
        exit(1);
    }
    unsigned long data[2];
    fread(data, 1, 8, ipFile);   
    ipBegin = data[0];
    ipEnd = data[1];
    ipSum = ((data[1]-data[0])/RECORD_LENGTH +1);
    
    printf("ipBegin:%d, ipEnd:%d, ipSum:%d\n", ipBegin, ipEnd, ipSum);


    checkCmdLine(argc, argv);

    fclose(ipFile);
    return 0;
}

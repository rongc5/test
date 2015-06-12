#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

using namespace std;

typedef struct udevice_cache_info{
    udevice_cache_info():valid(-1),isactiv(0),uptime(0){
        memset(udevno, 0, 12);
        memset(sid, 0, 50);
        memset(mac, 0, 33);
        memset(model, 0, 34);
        memset(postcode, 0, 9);
        memset(vip, 0, 50);
        memset(mcode, 0, 25);
		cout<< "the construct fun is used!" << endl;
        printf("the construcet fun is used!\n");
    };
    char sid[50];
    char udevno[12];
    char mac[33];
    char model[34];
    char postcode[9];
    char vip[50];
    char mcode[25];
    unsigned long type_id;
    int isactiv;
    u_int8_t   valid;
    time_t     uptime;
}udevice_cache_info;


int main(void)
{
		udevice_cache_info * p, tmp;
		printf("hello!\n");
		//p = (udevice_cache_info *)malloc(sizeof(udevice_cache_info));
		p = new udevice_cache_info;

        tmp = p;
        p = NULL;
        printf("new start!\n");
        p = tmp;
		free(p);
		return 0;
}

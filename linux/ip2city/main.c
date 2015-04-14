
#include "cityidquery.h"


int main(int c, char **v)
{
    if (c != 2){
        perror("usage...");
        return 0;
    }
    char buf[256];
    int ret;
    ALLINFO allinfo;
    construct_cityid_cache(v[1]);
//list_show_zip();
    
    while (1){
        printf("please input ipstr:");
        fflush(stdout);
        ret = read(0, buf, 256);
        buf[ret - 1] = '\0';
        if (strlen(buf) == 0)
            continue;
        if (strncmp(buf, "exit", 4) == 0)
            break;

        if (!ip_get_cityid(buf, &allinfo)){
            printf("countryname:%s  proname:%s  provinceid:%s cname:%s cityid:%s tname:%s townid:%s zipcode:%s\n",allinfo.cityidInfo.countryname, allinfo.cityidInfo.proname, allinfo.cityidInfo.provinceid, allinfo.cityidInfo.cname, allinfo.cityidInfo.cityid, allinfo.cityidInfo.tname, allinfo.cityidInfo.townid, allinfo.cityidInfo.zipcode);
            printf("%s  %s  %s  %s\n", allinfo.locInfo.countrybuf, allinfo.locInfo.areabuf, allinfo.locInfo.beginip, allinfo.locInfo.endip);
        }

        printf("please input zipstr:");
        fflush(stdout);
        ret = read(0, buf, 256);
        buf[ret - 1] = '\0';

        if (!zipcode_get_cityid(buf, &allinfo.cityidInfo)){
            printf("countryname:%s  proname:%s  provinceid:%s cname:%s cityid:%s tname:%s townid:%s zipcode:%s\n",allinfo.cityidInfo.countryname, allinfo.cityidInfo.proname, allinfo.cityidInfo.provinceid, allinfo.cityidInfo.cname, allinfo.cityidInfo.cityid, allinfo.cityidInfo.tname, allinfo.cityidInfo.townid, allinfo.cityidInfo.zipcode);
        }

    }
    destroy_cityid_cache();

    return 0;
}

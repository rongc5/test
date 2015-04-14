#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[])
{
    FILE * fp;
    char buf[1024];
    /*sprintf(buf, "%s", "ifconfig 2>/dev/null | xargs -L 1 | sed -nr  's/.*(eth[0-9]) .* addr:([0-9]+\\.[0-9]+\\.[0-9]+\\.[0-9]+) .*///NAME=\\1 IP=\\2/p'");*/
    /*[>printf("buf:%s\n", buf);<]*/
    /*fp = popen(buf, "r");*/
    /*memset(buf, 0, sizeof(buf));*/
    /*fread(buf, sizeof(buf),1,fp);*/
    /*printf("%s", buf);*/

    //sprintf(buf, "%s", "tcpdump -i eth1 -c 1 'tcp[13]==2' -nne 2>/dev/null | sed  -r 's/.* ([0-9a-f]{2}:[0-9a-f]{2}:[0-9a-f]{2}:[0-9a-f]{2}:[0-9a-f]{2}:[0-9a-f]{2}) > ([0-9a-f]{2}:[0-9a-f]{2}:[0-9a-f]{2}:[0-9a-f]{2}:[0-9a-f]{2}:[0-9a-f]{2}).* ([0-9]+\\.[0-9]+\\.[0-9]+\\.[0-9]+).* > ([0-9]+\\.[0-9]+\\.[0-9]+\\.[0-9]+).*/\\3 : \\1 > \\4 : \\2/'");
    sprintf(buf, "tcpdump -i eth1  -c 1 'tcp[13]==2' -nne 2>/dev/null | sed  -nr 's/.* ([0-9a-f]{2}:[0-9a-f]{2}:[0-9a-f]{2}:[0-9a-f]{2}:[0-9a-f]{2}:[0-9a-f]{2}) > ([0-9a-f]{2}:[0-9a-f]{2}:[0-9a-f]{2}:[0-9a-f]{2}:[0-9a-f]{2}:[0-9a-f]{2}).* ([0-9]+.[0-9]+.[0-9]+.[0-9]+).* > ([0-9]+.[0-9]+.[0-9]+.[0-9]+).*/haierIp=\\3  haierMac=\\1/p'");

    fp = popen(buf, "r");
    if (!fp){
        perror("popen");
        return -1;
    }
    memset(buf, 0, sizeof(buf));
     /*fflush(fp);*/
    fread(buf, sizeof(buf),1,fp);
    printf("%s", buf);
    pclose(fp);

    return 0;
}

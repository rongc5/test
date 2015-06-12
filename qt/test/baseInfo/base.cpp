#include "base.h"

int Basic::prase_str(char *buf, const char *name, char *save)
{
    char *p;
    int i = 0;

    if ((p = strstr(buf, name))){
        while (*p != '=')
            p++;
        p++;

        while (*p != '\n' && *p != '\0'&& *p != ' ')
            save[i++] = *p++;
        save[i] = '\0';

        return 0;
    }

    return -1;
}

void Basic::parse_network_info(char *buf)
{
    char tmp[60];

    if (prase_str(buf, "NAME", tmp))
        return;

    base.snifferNetface = tmp;
    //printf("%s\n", tmp);
    if (prase_str(buf, "IP", tmp))
        return;
    base.snifferIp = tmp;
    //printf("%s\n", tmp);
}

void Basic::getNetworkInterface(char *buf, size_t size)
{
    FILE * fp;
    int fd;
    memset(buf, 0, size);
    sprintf(buf, "%s", "ifconfig 2>/dev/null | xargs -L 1 | sed -nr  's/.*(eth[0-9]) .* addr:([0-9]+\\.[0-9]+\\.[0-9]+\\.[0-9]+) .*/NAME=\\1 IP=\\2/p'");
    //printf("buf:%s\n", buf);
    fp = popen(buf, "r");
    //if (!fp)
        //printf("failed!\n");
    fd = fileno(fp);
    memset(buf, 0, size);
    read(fd, buf, size);
    //fread(buf, sizeof(buf),1,fp);
    //printf("111%s\n", buf);
 
    pclose(fp);
}

void Basic::prase_haier_info(char *buf)
{
    char tmp[60];

    if (prase_str(buf, "haierIp", tmp))
        return;

    //printf("%s\n", tmp);
    base.haierIp = tmp;
    if (prase_str(buf, "haierMac", tmp))
        return;
    base.haierMac = tmp;
    //printf("%s\n", tmp);
}

void Basic::getHaierInfo(char *buf, size_t size)
{
	FILE * fp;
    int fd;
    memset(buf, 0, size);
    sprintf(buf, "%s", "tcpdump -i eth1 -c 1 'tcp[13]==2' -nne 2>/dev/null | sed  -nr 's/.* ([0-9a-f]{2}:[0-9a-f]{2}:[0-9a-f]{2}:[0-9a-f]{2}:[0-9a-f]{2}:[0-9a-f]{2}) > ([0-9a-f]{2}:[0-9a-f]{2}:[0-9a-f]{2}:[0-9a-f]{2}:[0-9a-f]{2}:[0-9a-f]{2}).* ([0-9]+\\.[0-9]+\\.[0-9]+\\.[0-9]+).* > ([0-9]+\\.[0-9]+\\.[0-9]+\\.[0-9]+).*/haierIp=\\3  haierMac=\\1/p'");
    //printf("%s\n", buf);
    fp = popen(buf, "r");
    //if (!fp)
        //printf("failed!\n");
    fd = fileno(fp);
    memset(buf, 0, size);
    read(fd, buf, size);
    //fread(buf, sizeof(buf),1,fp);
    //printf("%s", buf);
	
	pclose(fp);
}

void Basic::getBasic(Binfo * info)
{
    mutex.lock();
    info->haierIp = base.haierIp;
    info->haierMac = base.haierMac;
    info->snifferNetface = base.snifferNetface;
    info->snifferIp = base.snifferIp;
    mutex.unlock();
}

void Basic::setBasic()
{
    char buf[1024];
    mutex.lock();
    getNetworkInterface(buf, sizeof(buf));
    parse_network_info(buf);
    getHaierInfo(buf, sizeof(buf));
    prase_haier_info(buf);
    mutex.unlock();
}

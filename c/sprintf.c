#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main(void)
{
	char buf[1024];

    sprintf(buf, "tcpdump -i eth1  -c 1 'tcp[13]==2' -nne 2>/dev/null | sed  -nr 's/.* ([0-9a-f]{2}:[0-9a-f]{2}:[0-9a-f]{2}:[0-9a-f]{2}:[0-9a-f]{2}:[0-9a-f]{2}) > ([0-9a-f]{2}:[0-9a-f]{2}:[0-9a-f]{2}:[0-9a-f]{2}:[0-9a-f]{2}:[0-9a-f]{2}).* ([0-9]+.[0-9]+.[0-9]+.[0-9]+).* > ([0-9]+.[0-9]+.[0-9]+.[0-9]+).*/haierIp=\\3  haierMac=\\1/p'");
	
    /*sprintf(buf, "ifconfig | grep -v 127.0.0 | awk -F \":\"  '/inet addr/{split([,a," ");print a[1]}' | head -1]");*/

	printf("%s\n", buf);
	return 0;
}

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int main(int argc, char *argv[])
{
    printf("hello\n");
    system("/mnt/hgfs/D/code/linux-virtual/myproj/shell/tcpdump_syn.sh");
    printf("world\n");

    return 0;
}

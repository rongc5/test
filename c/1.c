#include<stdio.h>
#include<string.h>


struct file_data
{
    unsigned char day:5;
    unsigned char year:4;

};

void show_ban(short* p)
{
    int len=sizeof(*p)*8;
    int i;
    int tmp=*p;
    for(i=0; i<len; i++)
    {
        if(tmp & 1<<(len-1))
        {
            printf("1");
        }
        else
        {
            printf("0");
        }
        tmp=tmp<<1;
    }

    printf("\n");
}

int main(int argc, char *argv[])
{
    struct file_data d;
    printf("size:%d\n", sizeof(d));
    memset(&d, 0, sizeof(d));
    d.day=0b11111;
    d.year=0b1111;
    show_ban((short*)&d);

    return 0;
}

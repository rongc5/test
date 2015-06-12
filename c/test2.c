#include<stdio.h>
#include<stdlib.h>
#include<time.h>

int main(int argc, char *argv[])
{   
    srand((unsigned int)time(NULL));

    int a[4];
    int i=0;
    int j;
    int flag=0;
    int ret;

    while(i<4)
    {
        flag=0;
        ret=rand()%10;

        for(j=0; j<i; j++)
        {
            if(ret==a[j]){
                flag=1;
                break;
                /*flag=1;*/
            }
        }

        if(flag!=1)
        {
            a[i]=ret;
            printf("%d\n",ret);
            i++;
        }
    }

    return 0;
}


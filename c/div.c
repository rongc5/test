#include<stdio.h>
int main()
{
long int x,y,u,n;
printf("请输入两个整数:\n");
scanf("%d%d",&x,&y);
printf("您想输出小数点后几位？(如果可以整除，只需要输出小数点后一个0)\n");
scanf("%d",&n);
printf("结果：\n");
printf("%d/%d=\n%d.",x,y,x/y);
   while(n--)
   {
    u=x%y;
    x=u*10;
    printf("%d",x/y);
    if(u==0)
     break;
    if(n%10==0)
     printf("\n");
   }
   printf("\n");
   return 0;
}

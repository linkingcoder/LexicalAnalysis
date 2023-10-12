#include <stdio.h>

int main()
{
    int x,y;//定义变量//
    scanf("%d%d",&x,&y);
    int i=1,sum=1;
    while(i<=y)
    {
        sum=sum*x;/*阶乘*/
        i=i+1;
    }
    printf("%d\n",sum);
    return 0;
}
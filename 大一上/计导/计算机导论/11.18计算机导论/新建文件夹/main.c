#include <stdio.h>
int main()
{
	int i=1,s=0,n;
	float r=0,t=0;
	printf("请输入一个非零整数:");
	scanf("%d",&n);
	
	if(i<n)
	{
		s+=i;
		t=1/s;
		r=r+t;
		i++;
	}
	else
	{
	printf("结果为：%f",r);
    }
    return 0;
}


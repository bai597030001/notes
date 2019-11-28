//C语言中的错误处理
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

//返回1～10之间随机数的函数
int randNum(void)
{
	//设置随机种子
	srand(time(0));
	//生成1~10之间的随机数
	int num = rand()%10+1;
	return 5 == num?-1:num;
}

//返回两个整数的最大值,通过返回值来表示成功或者失败，而最大值通过指针类型的参数带出去
int max(int ia,int ib,int* pi)
{
	// int* pi = &res;
	/*if(ia == ib) 
	{
		return -1;
	}
	// *pi = *(&res) = res;
	*pi = ia>ib?ia:ib;
	return 0;*/
	return ia==ib?-1:(*pi=ia>ib?ia:ib),0;
}

#include <string.h>
//传入的字符串是"error",返回错误
char* stringDeal(const char* ps)
{
	return !strcmp(ps,"error")?NULL:"ok";
}

//打印字符串
void print(const char* ps)
{
	printf("传入的字符串是：%s\n",ps);
}

int main(void)
{
	printf("生成的随机数是：%d\n",randNum());

	int res = 0;
	if(-1 != max(-1,-2,&res))
	{
		printf("最大值是：%d\n",res);
	}

	//printf("返回的内容是：%s\n",stringDeal("hello"));
	printf("返回的内容是：%s\n",stringDeal("error"));

	print("let me give you some color to see see");
	return 0;
}





//动态使用共享库文件
#include <stdio.h>
#include <dlfcn.h>

int main(void)
{
	//1.打开共享库文件
	void* handler = dlopen("./shared/libadd.so",RTLD_NOW);
	//2.判断打开是否成功
	char* error = dlerror();
	if(NULL != error)
	{
		printf("打开共享库文件失败\n");
		return -1;
	}
	//3.获取共享库文件中指定函数名的地址
	int (*padd)(int ia,int ib);
	padd = dlsym(handler,"add_int");
	//4.判断是否获取成功
	error = dlerror();
	if(error)
	{
		printf("获取函数地址失败\n");
		return -1;
	}
	//5.使用函数名进行调用
	printf("两个数的和是：%d\n",padd(20,30));
	//6.关闭共享库文件
	dlclose(handler);
	return 0;
}



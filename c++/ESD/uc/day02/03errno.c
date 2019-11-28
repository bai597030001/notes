//全局变量errno的使用
#include <stdio.h>
#include <errno.h>

// extern int errno; 声明全局变量
#include <string.h>

int main(void)
{
	FILE* fp = fopen("/etc/paswd","r");
	if(NULL == fp)
	{
		printf("打开文件失败\n");
		printf("errno = %d\n",errno);
		printf("%s\n",strerror(errno));
		perror("fopen2343");
		printf("fopen = %m\n");
		return -1;
	}
	printf("打开文件成功\n");
	fclose(fp);
	return 0;
}

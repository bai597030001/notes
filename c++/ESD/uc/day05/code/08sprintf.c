//使用标C函数实现 数据的写入
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(void)
{
	//1.创建/打开文件
	FILE* fp = fopen("num.dat","wb+");
	if(NULL == fp)
	{
		perror("fopen"),exit(-1);
	}
	printf("创建文件成功\n");
	//2.写入数据
	int i = 0;
	while(i < 1000000)
	{
		//将整型数据格式化转换为字符串写入即可
		char buf[100] = {0};
		sprintf(buf,"%d\n",i);
		//fwrite(&i,sizeof(int),1,fp);
		fwrite(buf,strlen(buf),1,fp);
		i++;
	}
	//3.关闭文件
	fclose(fp);
	//练习：vi 07write.c使用UC函数实现此功能
	return 0;
}





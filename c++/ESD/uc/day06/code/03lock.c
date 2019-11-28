//使用fcntl函数进行加锁和解锁
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int main(void)
{
	//1.打开文件
	int fd = open("a.txt",O_RDWR);
	if(-1 == fd)
	{
		perror("open"),exit(-1);
	}
	printf("打开文件成功\n");
	//2.准备一把锁
	struct flock lock;
	lock.l_type = F_RDLCK;
	lock.l_whence = SEEK_SET;
	lock.l_start = 0;
	lock.l_len = 10;
	lock.l_pid = -1;
	//3.给文件加锁
	int res = fcntl(fd,F_SETLK,&lock);
	if(-1 == res)
	{
		perror("fcntl"),exit(-1);
	}
	//4.占用文件
	printf("加锁成功，开始使用文件...\n");
	sleep(20);
	printf("使用文件完毕，开始解锁...\n");
	//5.给文件解锁
	lock.l_type = F_UNLCK;
	res = fcntl(fd,F_SETLK,&lock);
	if(-1 == res)
	{
		perror("解锁失败"),exit(-1);
	}
	printf("加锁成功，允许其他进程加锁，10秒后进程结束\n");
	sleep(10);
	//6.关闭文件
	close(fd);
	return 0;
}


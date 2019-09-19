#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string>
#include <iostream>
using namespace std;
int main(void)
{
	int fd = open("/home/gsmrlab/baijd/CFile.cpp", 
			O_RDWR|O_APPEND, 0664);
	if(-1 == fd)
	{
		cout << "open file failed" << endl;
		return -1;
	}
	cout << "fd  = " << fd << endl;
	struct flock lck;
	lck.l_type = F_WRLCK;//F_RDLCK
	lck.l_whence = SEEK_SET;
	lck.l_start = 0;
	lck.l_len = 0;//锁定整个文件
	lck.l_pid = getpid();

	int ret = fcntl(fd, F_SETLK, &lck);
	if(ret == -1)
	{
		cout << "lock file failed" << endl;
		return -1;
	}
	if(ret == 0)
	{
		cout << "lock file successful" << endl;
	}
	sleep(30);
	cout << "unlock file" << endl;
	close(fd);

	getchar();
}

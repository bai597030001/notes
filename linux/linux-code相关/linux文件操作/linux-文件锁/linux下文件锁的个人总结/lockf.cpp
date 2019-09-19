#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/file.h>
#include <string>
#include <iostream>
using namespace std;
int main(void)
{
	int fd = open("/home/gsmrlab/baijd/CFile.cpp", 
			O_RDWR|O_APPEND, 0664);
	if(-1 == fd)
	{
		cout << "lockf : open file failed" << endl;
		return -1;
	}
	cout << "lockf : fd = " << fd << endl;
	int rs = lockf(fd, F_LOCK, 0);
	if(rs == -1)
	{
		cout << "lockf : lock file failed" << endl;
		return -1;
	}
	if(rs == 0)
	{
		cout << "lockf : lock file success" << endl;
	}
	sleep(50);
	cout << "lockf : unlock file" << endl;
	close(fd);

	getchar();
}

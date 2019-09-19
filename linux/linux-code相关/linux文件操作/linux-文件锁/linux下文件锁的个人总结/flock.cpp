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
		cout << "flock : open file failed" << endl;
		return -1;
	}
	cout << "flock : fd = " << fd << endl;
	int rs = flock(fd, LOCK_EX);
	if(rs == -1)
	{
		cout << "flock : lock file failed" << endl;
		return -1;
	}
	if(rs == 0)
	{
		cout << "flock : lock file success" << endl;
	}
	sleep(50);
	cout << "flock : unlock file" << endl;
	close(fd);

	getchar();
}

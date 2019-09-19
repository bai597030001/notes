#include<cstdio>
#include<cstdlib>
#include<unistd.h>
#include<string>
#include<cstring>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<sys/time.h>
#include<time.h>
#include<iostream>
using namespace std;

class CFileLock
{
	public:
		CFileLock(){}
		CFileLock(int fd)
		{
			m_fd = fd;
			m_lock.l_type = F_WRLCK;
			m_lock.l_whence = SEEK_SET;
			m_lock.l_start = 0;
			m_lock.l_len = 0;//锁定整个文件
			m_lock.l_pid = getpid();
		}
		~CFileLock()
		{
			clearlock();
			close(m_fd);
			m_fd = -1;
		}
	public:
		void lockfile()
		{
			/*
			int flags = fcntl(m_fd, F_GETFL);
			if(flags == -1)
				printf("lockfile : flags == -1");
			flags |= O_NONBLOCK;//设置锁为阻塞
			*/
			if(fcntl(m_fd, F_SETLKW, &m_lock) == -1)
				printf("lockfile function failed\n");
		}
		void clearlock()
		{
			m_lock.l_type = F_UNLCK;

			if(fcntl(m_fd, F_SETLKW, &m_lock) == -1)
				printf("clear lock file failed\n");
		}
	private:
		int m_fd;
		struct flock m_lock;
};

int main()
{
	int fd = open("test.txt", O_RDWR|O_CREAT, 0664);
	if(fd == -1)
	{
		printf("open file failed\n");
		return -1;
	}
	for(int i = 0; i < 10; i++)
	{
		write(fd, "HelloWorld! ", strlen("HelloWorld! "));
	}
	CFileLock g_lock(fd);
	g_lock.lockfile();
	sleep(20);
	g_lock.clearlock();


	return 0;
}

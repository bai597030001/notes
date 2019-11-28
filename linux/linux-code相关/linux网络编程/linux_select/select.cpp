/*
 this process is just for test select module
*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define MAX_CONN_COUNT	2
#define	BUFFER_SIZE		1024
#define	MYPORT			8888

static int	g_cur_conn_count = 0;//当前连接数
int g_fd_accept[MAX_CONN_COUNT];//accept连接的fd

// 从count个fd中删除第num个fd
void delUnusedFd(int fd[], int count, int num)
{
	for(int i = num; i < count; i++)
	{
		fd[i] = fd[i + 1];
	}
}

int main(int argc, char * argv[])
{
	//socket
	int sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if(-1 == sockfd)
	{
		printf("socket func error\n");
		return -1;
	}
	/*
	 需要设置socket属性为地址重用,否则在客户端不断开连接，
	 服务端断开连接再重启时，会出现bind地址失败的情况
	 */
	// set socket attribute
	socklen_t opt = 1;
	if(-1 == setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(socklen_t)))
	{
		printf("setsockopt func failed\n");
		perror("setsockopt");
		return -1;
	}
	//prepare address
	struct sockaddr_in addrSrv;
	addrSrv.sin_family = AF_INET;
	addrSrv.sin_port = htons(MYPORT);
	addrSrv.sin_addr.s_addr = INADDR_ANY;
	//bind
	if(-1 == bind(sockfd, (struct sockaddr *)&addrSrv, sizeof(addrSrv)))
	{
		printf("bind sockfd failed\n");
		perror("bind");
		printf("errno = %d\n", errno);
		return -1;
	}
	//listen
	//在进程正理一个一个连接请求的时候，可能还存在其它的连接请求。因为TCP连接是一个过程，所以可能存在一种半连接的状态，
	//有时由于同时尝试连接的用户过多，使得服务器进程无法快速地完成连接请求。如果这个情况出现了，服务器进程希望内核如何
	//处理呢？内核会在自己的进程空间里维护一个队列以跟踪这些完成的连接但服务器进程还没有接手处理或正在进行的连接，这样
	//的一个队列内核不可能让其任意大，所以必须有一个大小的上限。这个backlog告诉内核使用这个数值作为上限。
	//具体详见 https://blog.csdn.net/yangbodong22011/article/details/60399728
	if(-1 == listen(sockfd, MAX_CONN_COUNT))
	{
		printf("listen sockfd failed\n");
		return -1;
	}
	//prepare select
	fd_set rfds;
	int max_sockfd = sockfd;

	while(1)
	{
		/*
		每次循环开始，需要将监听socket加入到rfds
		*/
		FD_ZERO(&rfds);
		FD_SET(sockfd, &rfds);
		
		for(int i = 0; i < g_cur_conn_count; i++)
		{
			FD_SET(g_fd_accept[i], &rfds);
		}

		struct timeval tv = {20, 0};
		//select
		int ret = select(max_sockfd + 1, &rfds, NULL, NULL, &tv);
		if(-1 == ret)
		{
			printf("sockfd select error\n");
			break;
		}
		else if(0 == ret)
		{
			printf("select time out\n");
			continue;
		}
		else
		{
			//accept
			if(FD_ISSET(sockfd, &rfds))
			{
				struct sockaddr_in addrCli;
				socklen_t addrlen = sizeof(addrCli);

				int connfd = accept(sockfd, (struct sockaddr *)&addrCli, &addrlen);
				if(-1 == connfd)
				{
					printf("accept error\n");
					continue;
				}

				if(g_cur_conn_count < MAX_CONN_COUNT)
				{
					printf("new connfd is connect, connfd = %d\n", connfd);

					g_fd_accept[g_cur_conn_count++] = connfd;
					FD_SET(connfd, &rfds);

					if(connfd > max_sockfd)
					{
						max_sockfd = connfd;
					}
				}
				else
				{
					printf("current socket connect count is max, exit\n");
					send(connfd, "max connect count now, bye!", 28, 0);
					close(connfd);
					continue;
				}
			}
			else
			{
				for(int i = 0; i < g_cur_conn_count; i++)
				{
					//recv
					if(FD_ISSET(g_fd_accept[i], &rfds))
					{
						char buf[BUFFER_SIZE] = {0};
						size_t rc = recv(g_fd_accept[i], buf, BUFFER_SIZE, 0);
						if(rc < 0)
						{
							printf("recv func err, g_fd_accept[%d] failed\n", i);

							delUnusedFd(g_fd_accept, g_cur_conn_count, i);
							g_cur_conn_count--;

							FD_CLR(g_fd_accept[i], &rfds);
							close(g_fd_accept[i]);

							continue;
						}
						else if(0 == rc)//客户端断开连接
						{
							printf("recv return 0, client shutdown the connect\n");

							delUnusedFd(g_fd_accept, g_cur_conn_count, i);
							g_cur_conn_count--;

							FD_CLR(g_fd_accept[i], &rfds);
							close(g_fd_accept[i]);

							continue;
						}
						else
						{
							//send
							printf("recv data from fd = %d:%s\n", g_fd_accept[i], buf);
							send(g_fd_accept[i], buf, strlen(buf) + 1, 0);
						}
					}
				}
			}
		}
	}
	return 0;
}

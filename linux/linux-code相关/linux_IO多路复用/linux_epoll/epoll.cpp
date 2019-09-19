#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>   
#include <sys/socket.h>   
#include <netinet/in.h>   
#include <arpa/inet.h> 
#include <stdlib.h>
#include <string.h>
#include <sys/epoll.h>

#define BUFFER_SIZE 1024
#define MAX_EVENTS 2
#define MAX_CONN_COUNT	2

int main(int argc, char * argv[])   
{
	//socket
	int sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if(-1 == sockfd)
	{
		perror("socket");
		exit(-1);
	}
	//setsockopt
	socklen_t opt = 1;
	if(-1 == setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(socklen_t)))
	{
		perror("setsockopt");
		exit(-1);
	}
	//prepare address
	struct sockaddr_in addrSvr;
	addrSvr.sin_family = AF_INET;
	addrSvr.sin_port = htons(8000);
	addrSvr.sin_addr.s_addr = INADDR_ANY;
	//bind
	if(-1 == bind(sockfd, (struct sockaddr *)&addrSvr, sizeof(addrSvr)))
	{
		perror("bind");
		exit(-1);
	}
	//listen
	/* SOMAXCONN 为系统默认的backlog, 128*/
	//if(-1 == listen (sfd, SOMAXCONN));
	if(-1 == listen(sockfd, MAX_CONN_COUNT))
	{
		perror("listen");
		exit(-1);
	}
	//epoll
	/*
	   	struct epoll_event
	   	{
	   		__uint32_t events;
			epoll_data_t data;
		};
		typedef union epoll_data
		{
			void * 	ptr;
			int		fd;
			uint32_t	u32;
			uint64_t	u64;
		}epoll_data_t;
	 */
	int epoll_fd = epoll_create(MAX_EVENTS);
	if(-1 == epoll_fd)
	{
		perror("epoll_create");
		exit(-1);
	}

	struct epoll_event ev;
	struct epoll_event event[MAX_EVENTS];//事件监听队列
	ev.events = EPOLLIN;
	ev.data.fd = sockfd;
	if(-1 == epoll_ctl(epoll_fd, EPOLL_CTL_ADD, sockfd, &ev))
	{
		perror("epoll_ctl");
		exit(-1);
	}
	int cur_fd_cnt = 1;

	while(1)
	{
		int nfds = epoll_wait(epoll_fd, event, MAX_EVENTS, -1);
		if(-1 == nfds)
		{
			perror("epoll_wait");
			exit(-1);
		}

		for(int i = 0; i < nfds; i++)
		{
			if(event[i].data.fd == sockfd)//有新的连接
			{
				struct sockaddr_in addrCli;
				socklen_t addrlen = sizeof(struct sockaddr_in);

				int connfd = accept(sockfd, (struct sockaddr *)&addrCli, &addrlen);
				if(-1 == connfd)
				{
					perror("accept");
					exit(-1);
				}
				//将新的连接加入到epoll监听事件
				ev.events = EPOLLIN;
				ev.data.fd = connfd;
				if(-1 == epoll_ctl(epoll_fd, EPOLL_CTL_ADD, connfd, &ev))
				{
					printf("now, epoll_ctl add new fd failed\n");
					perror("epoll_ctl");
					exit(-1);
				}
				cur_fd_cnt++;
				printf("new connect is establishd, connfd = %d\n", connfd);
			}
			//recv
			//send
			else if(event[i].events & EPOLLIN)//收到已经存在的连接发来的数据
			{
				char buf[BUFFER_SIZE] = {0};

				int recv_fd = event[i].data.fd;
				size_t rc = recv(recv_fd, buf, BUFFER_SIZE, 0);
				if(rc < 0)
				{
					perror("recv");
					cur_fd_cnt--;
					exit(-1);
				}
				else if(0 == rc)//客户端关闭
				{
					cur_fd_cnt--;
					printf("client fd = %d, is closed\n", recv_fd);
					close(recv_fd);
					epoll_ctl(recv_fd, EPOLL_CTL_DEL, event[i].data.fd, &event[i]);
				}
				printf("recv data form fd = %d, data:%s\n", recv_fd, buf);
				send(recv_fd, buf, strlen(buf) + 1, 0);

				//ev.events = EPOLLOUT;
				//epoll_ctl(recv_fd, EPOLL_CTL_MOD, event[i].data.fd, &ev);
			}
		}
	}
	return 0;   
}  

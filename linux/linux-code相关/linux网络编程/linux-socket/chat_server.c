#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <pthread.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>
//一些准备工作
int sockfd;//socket描述符
char *IP = "127.0.0.1";//本机IP
short PORT = 10222;//端口
typedef struct sockaddr SA;//用作通信地址类型转换
struct client{//客户端信息结构
	char name[20];//昵称
	int fds;//socket描述符
};
#define max_client_size 1024
struct client c[max_client_size];//最多记录max_client_size个socket
int size;//数组下标，记录客户端个数

//1、初始化服务器的网络，创建socket
void init(){
	//创建socket
	sockfd = socket(PF_INET,SOCK_STREAM,0);
	if(sockfd == -1){
		perror("创建socket失败");
		exit(-1);
	}

	int yes = 1;
	if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1) 
	{
		perror("setsockopt");
		exit(1);
	}

	//准备网络通信地址
	struct sockaddr_in addr;
	addr.sin_family = PF_INET;//协议簇
	addr.sin_port = htons(PORT);//端口
	addr.sin_addr.s_addr = inet_addr(IP);//IP
	//绑定
	if(bind(sockfd,(SA*)&addr,sizeof(addr))==-1){
		perror("绑定失败");
		exit(-1);
	}
	//设置监听
	// backlog参数的行为在linux2.2中发生了变化。现在用来表示完成连接的处于established状态等待被accept的socket的队列的大小,
	//		而不是等待连接的请求队列的大小。
	// 未完成连接的队列大小可以在/proc/sys/net/ipv4/tcp_max_syn_backlog中被设置。当syncookies生效时，没有逻辑上的该值的最大值，因此该设置无效。
	// 如果backlog的值比/proc/sys/net/core/somaxconn中的还要大,则会被截断,文件中默认的值的大小是124。
	// if(listen(sockfd,max_client_size)==-1){
	if(listen(sockfd, 2)==-1){
		perror("设置监听失败");
		exit(-1);
	}
	//等待客户端连接(这部分放到service中)
}
//用于把消息分发给所有在聊天室内的客户端
void sendMsgToAll(char * msg){
	int i = 0;
	for(;i<size;i++){
		// printf("sendto%d\n",c[i].fds);//测试
		send(c[i].fds,msg,strlen(msg),0);
	}
}
//线程函数，用于通信服务
void* service_thread(void* p){
	int fd = *(int*)p;//拿到客户端的socket
	printf("pthread socket fd = %d\n",fd);//测试察看
	//记录客户端的socket
	c[size].fds = fd;
	size++;//数据索引值增1
	printf("当前连接客户端个数: %d\n", size);
	
	//正式收发聊天内容
	while(1){
		char buf[100] = {};
		if(recv(fd,buf,sizeof(buf),0)==0){
			//recv函数返回0表示对方断开连接
			printf("fd(%d) is quit\n",fd);//测试
			//处理结构体数组中的客户端信息，用数组
		//的最后一个元素信息，覆盖目标元素信息。
		//最后让size的值减一
			int i = 0;
			char name[20]={};
			for(i=0;i<size;i++){
				if(c[i].fds == fd){
					strcpy(name,c[i].name);//备份名字
					c[i].fds = c[size-1].fds;//
					strcpy(c[i].name,c[size-1].name);
				}
			}
			size--;
			close(fd);//关闭描述符
			return;//客户端退出，结束线程
		}
		// sendMsgToAll(buf);//如果接收成功，把内容发给其他人
		printf("server recv msg: %s\n", buf);
	}
}
//2、启动服务器的服务，接发消息
void service(){
	while(1){
		struct sockaddr_in fromaddr;//存储客户端通信地址的结构体
		socklen_t len = sizeof(fromaddr);
		int fd = accept(sockfd,(SA*)&fromaddr,&len);
		if(fd == -1){
			printf("accept 失败, continue");
			sleep(1);
			continue;//继续下一次循环，处理连接
		}
		//如果有客户端成功连接
		//开启一个线程，为该客户端进行通信服务
		pthread_t pid;
		pthread_create(&pid,0,service_thread,&fd);

	}
}
int main(){
	init();//搭建网络
	service();//启动服务
	return 0;
}

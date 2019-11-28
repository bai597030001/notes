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
char *IP = "127.0.0.1";//本机IP
short PORT = 10222;//端口
typedef struct sockaddr SA;//用作通信地址类型转换
char name[20];//存储客户昵称
//分两部分，1启动客户端，连接服务器
int init(){
	int sockfd = socket(PF_INET,SOCK_STREAM,0);
	struct sockaddr_in addr;
	addr.sin_family = PF_INET;
	addr.sin_port = htons(PORT);
	addr.sin_addr.s_addr = inet_addr(IP);
	//客户端不需要绑定，需要用connect连接
	if(connect(sockfd,(SA*)&addr,sizeof(addr))==-1){
		perror("无法连接到服务器");
		printf("客户端启动失败\n");
		exit(-1);
	}
	return sockfd;
}
//2开始通信
void start(int sockfd){
	//该函数用于发送消息，发之前启动一个线程
	//该线程用于接受服务器发过来的消息
	pthread_t pid;
	void* send_pthread(void*);//函数声明
	pthread_create(&pid,0,send_pthread,&sockfd);
}
//收消息线程函数
void* send_pthread(void *p)
{
	int sockfd = *(int*)p;
	while(1){
		char msg[256] = {};
		sprintf(msg, "%d", sockfd);	
		send(sockfd, msg, strlen(msg), 0);
		usleep(10);
	}
	/*
	int sockfd = *(int*)p;
	while(1){
		char buf[100] = {};
		if(recv(sockfd,buf,sizeof(buf),0)<=0){
			return;//接收出错，结束线程
		}
		printf("客户端接收到消息: %s\n",buf);
	}
	*/
}
#define max_client_size 1024
int main(){
	int i = 0;
	for(; i < max_client_size; i++){
		int sockfd = init();//启动客户端
		start(sockfd);//通信
		usleep(10);
	}
	printf("%d 个客户端连接完毕", max_client_size);
	return 0;
}


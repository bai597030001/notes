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
char name[20];//存储客户昵称
//分两部分，1启动客户端，连接服务器
void init(){
	printf("聊天室客户端开始启动..\n");
	sockfd = socket(PF_INET,SOCK_STREAM,0);
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
	printf("客户端启动成功\n");
}
//2开始通信
void start(){
	//该函数用于发送消息，发之前启动一个线程
	//该线程用于接受服务器发过来的消息
	pthread_t pid;
	void* recv_pthread(void*);//函数声明
	pthread_create(&pid,0,recv_pthread,0);
	while(1){
		char buf[100] = {};
		scanf("%s",buf);//接收客户端说的话
		char msg[200] = {};
		sprintf(msg,"%s 说 %s",name,buf);
		send(sockfd,msg,strlen(msg),0);//发给服务器
	}
}
//收消息线程函数
void* recv_pthread(void *p){
	while(1){
		char buf[100] = {};
		if(recv(sockfd,buf,sizeof(buf),0)<=0){
			return;//接收出错，结束线程
		}
		printf("%s\n",buf);
	}
}
/*
int main(){
	printf("请输入您的名字:");
	scanf("%s",name);
	init();//启动客户端
	send(sockfd,name,strlen(name),0);
	start();//通信
	return 0;
}
*/
int main(){
	printf("请输入您的名字:");
	scanf("%s",name);
	for(int i = 0; i < 10240; i++){
		init();//启动客户端
		send(sockfd,name,strlen(name),0);
		start();//通信
	}
	return 0;
}



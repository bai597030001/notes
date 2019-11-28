//聊天室 服务器
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
struct client c[100];//最多记录100个socket
int size;//数组下标，记录客户端个数

//1、初始化服务器的网络，创建socket
void init(){
	printf("聊天室服务器开始启动..\n");
	//创建socket
	sockfd = socket(PF_INET,SOCK_STREAM,0);
	if(sockfd == -1){
		perror("创建socket失败");
		printf("服务器启动失败\n");
		exit(-1);
	}
	//准备网络通信地址
	struct sockaddr_in addr;
	addr.sin_family = PF_INET;//协议簇
	addr.sin_port = htons(PORT);//端口
	addr.sin_addr.s_addr = inet_addr(IP);//IP
	//绑定
	if(bind(sockfd,(SA*)&addr,sizeof(addr))==-1){
		perror("绑定失败");
		printf("服务器启动失败\n");
		exit(-1);
	}
	printf("绑定成功\n");
	//设置监听
	if(listen(sockfd,100)==-1){//100表示同一时间能处理的最大连接请求
		perror("设置监听失败");
		printf("服务器启动失败\n");
		exit(-1);
	}
	printf("设置监听成功\n");
	printf("初始化服务器网络成功\n");
	//等待客户端连接(这部分放到service中)
}
//用于把消息分发给所有在聊天室内的客户端
void sendMsgToAll(char * msg){
	int i = 0;
	for(;i<size;i++){
		printf("sendto%d\n",c[i].fds);//测试
		send(c[i].fds,msg,strlen(msg),0);
	}
}
//线程函数，用于通信服务
void* service_thread(void* p){
	int fd = *(int*)p;//拿到客户端的socket
	printf("pthread=%d\n",fd);//测试察看
	//记录客户端的socket
	c[size].fds = fd;
	//接收到客户端的昵称
	char name[20] = {};
	if(recv(fd,name,sizeof(name),0)>0){
		//TCP通信用的两个函数，无论客户端/服务器
			//都是用recv接受消息,用send发送消息
		strcpy(c[size].name,name);
	}
	size++;//数据索引值增1
	char tishi[100] = {};
	sprintf(tishi,"热烈欢迎 %s 登陆本聊天室..",name);
	//发送给所有在聊天室内的客户
	sendMsgToAll(tishi);//分发消息
	//正式收发聊天内容
	while(1){
		char buf[100] = {};
		if(recv(fd,buf,sizeof(buf),0)==0){
			//recv函数返回0表示对方断开连接
			printf("fd=%dquit\n",fd);//测试
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
			printf("quit->fd=%dquit\n",fd);//测试
			char msg[100] = {};
			sprintf(msg,"欢送 %s 离开聊天室，再见!",name);
			sendMsgToAll(msg);//将退出提示发给其他人
			close(fd);//关闭描述符
			return;//客户端退出，结束线程
		}
	sendMsgToAll(buf);//如果接收成功，把内容发给其他人
	}
}
//2、启动服务器的服务，接发消息
void service(){
	printf("服务器开始服务\n");
	while(1){
		struct sockaddr_in fromaddr;//存储客户端通信地址的结构体
		socklen_t len = sizeof(fromaddr);
		int fd = accept(sockfd,(SA*)&fromaddr,&len);
		if(fd == -1){
			printf("客户端连接出错\n");
			continue;//继续下一次循环，处理连接
		}
		//如果有客户端成功连接
		//开启一个线程，为该客户端进行通信服务
		printf("fd=%d\n",fd);//用于测试察看
		pthread_t pid;
		pthread_create(&pid,0,service_thread,&fd);

	}
}
int main(){
	init();//搭建网络
	service();//启动服务
	return 0;
}




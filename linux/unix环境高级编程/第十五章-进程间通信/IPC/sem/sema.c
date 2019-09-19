   //使用信号量集进行进程间的通信      01sema.c
   #include<stdio.h>
   #include<stdlib.h>
   #include<unistd.h>
   #include<sys/types.h>
   #include<sys/ipc.h>
   #include<sys/sem.h>
   #include<signal.h>
   int semid;
  void fa(int signo) {
      printf("正在删除信号量集，请稍后\n");
      sleep(3);
      int res = semctl(semid,0,IPC_RMID);
      if (-1 == res) {
          perror("semctl"),exit(-1);
      }
      printf("删除信号量集成功\n");
      exit(0);
  }
  int main () {
      //获取key值，ftok
      key_t key = ftok(".",150);
      if (-1 == key) {
          perror("ftok"),exit(-1);
      }
      printf("key = %#x\n",key);
      //2创建信号量集,semget
      semid = semget(key,1,IPC_CREAT|IPC_EXCL|0666);
      if (-1 == semid) {
          perror("semget"),exit(-1);
      }
      printf("semid = %d\n",semid);
      //3初始化信号量集,semctl
      int res = semctl(semid,0,SETVAL,5);
      if (-1 == res) {
          perror("semctl"),exit(-1);
      }
      printf("靠靠靠靠\n");
      //4使用信号来删除信号量集
      printf("靠靠靠靠ctrl c\n");
      signal(SIGINT,fa);
      while (1);
      return 0;
  }  

   //ʹ���ź��������н��̼��ͨ��      01sema.c
   #include<stdio.h>
   #include<stdlib.h>
   #include<unistd.h>
   #include<sys/types.h>
   #include<sys/ipc.h>
   #include<sys/sem.h>
   #include<signal.h>
   int semid;
  void fa(int signo) {
      printf("����ɾ���ź����������Ժ�\n");
      sleep(3);
      int res = semctl(semid,0,IPC_RMID);
      if (-1 == res) {
          perror("semctl"),exit(-1);
      }
      printf("ɾ���ź������ɹ�\n");
      exit(0);
  }
  int main () {
      //��ȡkeyֵ��ftok
      key_t key = ftok(".",150);
      if (-1 == key) {
          perror("ftok"),exit(-1);
      }
      printf("key = %#x\n",key);
      //2�����ź�����,semget
      semid = semget(key,1,IPC_CREAT|IPC_EXCL|0666);
      if (-1 == semid) {
          perror("semget"),exit(-1);
      }
      printf("semid = %d\n",semid);
      //3��ʼ���ź�����,semctl
      int res = semctl(semid,0,SETVAL,5);
      if (-1 == res) {
          perror("semctl"),exit(-1);
      }
      printf("��������\n");
      //4ʹ���ź���ɾ���ź�����
      printf("��������ctrl c\n");
      signal(SIGINT,fa);
      while (1);
      return 0;
  }  

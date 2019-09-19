   //ʹ���ź���������ͨ��         01semb.c
   #include<stdio.h>
   #include<stdlib.h>
   #include<unistd.h>
   #include<sys/types.h>
   #include<sys/ipc.h>
   #include<sys/sem.h>
   int main () {
       //��ȡkeyֵ
      key_t key = ftok(".",150);
      if (-1 == key) {
          perror("ftok"),exit(-1);
      }
      printf("key = %#x\n",key);
      //2��ȡ�ź�������ID
      int semid = semget(key,0,0);
      if (-1 == semid) {
          perror("semget"),exit(-1);
      }
      printf("semid = %d\n",semid);
      //3ʹ���ź����������ƽ�����
      int i = 0;
          for (i = 0;i < 10;i++) {
              pid_t pid = fork();
              if (0 == pid) {//�ӽ���
                  //׼���ṹ��
                  struct sembuf buf;
                  buf.sem_num = 0;//�±�
                  buf.sem_op = -1;//������1
                  buf.sem_flg = 0;//������־
                  //ʹ��semop���������ź���
                  int res = semop(semid,&buf,1);
                  if (-1 == res) {
                      perror("semop"),exit(-1);
                  }
                  printf("���빲����Դ�ɹ�\n");
                  sleep(20);
                  printf("ʹ����Դ���\n");
                  buf.sem_op = 1;
                  semop(semid,&buf,1);
                  exit(0);//��ֹ�ӽ���
              }
          }
      return 0;
  }  

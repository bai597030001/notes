#include<unistd.h>
#include<signal.h>
#include<stdio.h>
#include<stdlib.h>
#include<sys/param.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<time.h>

void init_daemon()
{
	int pid;
	int i;

	pid=fork();
	if(pid<0)    
	    exit(1);  //���������˳�
	else if(pid>0) //�������˳�
	    exit(0);
	    
	setsid(); //ʹ�ӽ��̳�Ϊ�鳤

	pid=fork();
	if(pid>0)
	    exit(0); //�ٴ��˳���ʹ���̲����鳤���������̾Ͳ���򿪿����ն�
	else if(pid<0)    
	    exit(1);
	
	//�رս��̴򿪵��ļ����
	for(i=0;i<NOFILE;i++)
	    close(i);
	chdir("/tmp");  //�ı�Ŀ¼
	umask(0);//�����ļ�����������
	return;
}

void main()
{
    FILE *fp;
    time_t t;
    init_daemon();
    while(1)
    {
        sleep(10); //�ȴ�һ������д��
        fp=fopen("testfork2.log","a");
        if(fp>=0)
        {
            time(&t);
            fprintf(fp,"current time is:%s\n",asctime(localtime(&t)));  //ת��Ϊ����ʱ�����
            fclose(fp);
        }
    }
    return;
}

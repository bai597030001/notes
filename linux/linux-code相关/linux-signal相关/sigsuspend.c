#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<time.h>
#include<signal.h>
void func(int signo)
{
	printf("catch signal %d\n", signo);
}
int main(int argc, char * argv[])
{
	signal(SIGINT, func);
	signal(SIGQUIT, func);

	sigset_t setnew, setold;
	sigemptyset(&setnew);
	sigemptyset(&setold);
	sigaddset(&setnew, SIGQUIT);
	sigaddset(&setold, SIGINT);

	sigprocmask(SIG_BLOCK, &setold, NULL);
	int i = 0;
	for(i = 0; i < 10; i++)
	{
		printf("i = %d\n", i);
		sleep(1);
	}

	sigsuspend(&setnew);

	for(i = 0; i < 10; i++)
	{
		sleep(1);
		printf("a\n");
	}
	//printf("now sigset_t is %lld\n", setold);
	return 0;
}



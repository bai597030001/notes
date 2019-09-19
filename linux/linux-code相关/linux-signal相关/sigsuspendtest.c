#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<time.h>
#include<signal.h>
void func (int signo)
{
	printf("0\n");
}
int main(int argc, char * argv[])
{
	sigset_t set, empty;
	sigemptyset(&set);
	sigemptyset(&empty);
	sigaddset(&set, SIGINT);
	signal(SIGINT, func);

	while(1)
	{
		sigprocmask(SIG_BLOCK, &set, NULL);
		int i = 0;
		for(i  = 0; i < 5; i++)
		{
			printf("**\n");
			sleep(1);
		}

		//sigsuspend(&empty);
		sigprocmask(SIG_UNBLOCK, &set, NULL);
		pause();
	}
	return 0;
}



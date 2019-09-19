#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <termios.h>
#include <semaphore.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <signal.h>
#include <time.h>
#include <string>
#include <mutex>
using std::mutex;
using namespace std;

class CTimer {
public:
	CTimer()
	{
		this->init();
	}
	~CTimer(){}
	void init()
	{
		memset(&act, 0, sizeof(act));
		sigemptyset(&act.sa_mask);
		act.sa_flags = SA_SIGINFO;
		act.sa_sigaction = sig_handler;

		sigemptyset(&act.sa_mask);

		if (sigaction(SIGUSR1, &act, NULL) == -1)
		{
			perror("fail to sigaction");
			exit(-1);
		}

		memset(&evp, 0, sizeof(struct sigevent));
		evp.sigev_signo = SIGUSR1;
		evp.sigev_notify = SIGEV_SIGNAL;

		if (timer_create(CLOCK_REALTIME, &evp, &timerid) == -1)
		{
			perror("fail to timer_create");
			exit(-1);
		}
	}
	void setTimer(time_t sec)
	{
		printf("setTimer::sec = %lu\n", sec);
		it.it_interval.tv_sec = sec;
		it.it_interval.tv_nsec = 0;
		it.it_value.tv_sec = sec;
		it.it_value.tv_nsec = 0;
	}
	void start(time_t sec)
	{
		printf("start::sec = %lu\n", sec);
		this->setTimer(sec);
		
		struct itimerspec ovalue;
		
		if (timer_settime(timerid, 0, &it, &ovalue) == -1)
		{
			perror("fail to timer_settime");
			exit(-1);
		}
	}
	void stop()
	{
		timer_delete(timerid);
	}
	void handler(){}
public:
	static void sig_handler(int signo, siginfo_t* info, void* context)
	{
		printf("Start Timer Task now !!!\n");
		//fflush(stdout);
		//CTimer *pTimer = (CTimer*)(info->si_ptr);
		//pTimer->setTimer(1);
	}
private:
	timer_t timerid;
	struct sigevent evp;
	struct itimerspec it;
	struct sigaction act;
};
int main() {
	CTimer tm;
	tm.start(2);
	while(true);
	return 0;
}

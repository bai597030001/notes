#include <stdio.h>  
#include <time.h>  
#include <stdlib.h>  
#include <signal.h>  
#include <string.h>  
#include <unistd.h>  
#include <cstring>
#include <functional>
using namespace std;

#define CLOCKID CLOCK_REALTIME
/*
class CTimer {
public:
	CTimer(){}
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
	void start()
	{
		struct itimerspec it;
		it.it_interval.tv_sec = 2;
		it.it_interval.tv_nsec = 0;
		it.it_value.tv_sec = 1;
		it.it_value.tv_nsec = 0;
		if (timer_settime(timerid, 0, &it, 0) == -1)
		{
			perror("fail to timer_settime");
			exit(-1);
		}

	}
	void stop()
	{
		timer_delete(timerid);
	}
	void handler()
	{
		//printf("Hello World!");
	}
private:
	static void sig_handler(int signo, siginfo_t* info, void* context)
	{
		printf("timer_signal function! %d\n", signo);
		CTimer *pTimer = (CTimer*)(info->si_ptr);
		pTimer->handler();
	}
private:
	timer_t timerid;
	struct sigevent evp;
	struct sigaction act;
};
*/
/*
class CTimer {
public:
	CTimer(){}
	void init()
	{
		memset(&evp, 0, sizeof(struct sigevent));
		
		//evp.sigev_value.sival_int = 111;			//也是标识定时器的，这和timerid有什么区别？回调函数可以获得
		evp.sigev_value.sival_ptr = this;

		evp.sigev_notify = SIGEV_THREAD;			//线程通知的方式，派驻新线程
		evp.sigev_notify_function = timer_thread;		//线程函数地址
		
		it.it_interval.tv_sec = 1;
		it.it_interval.tv_nsec = 0;
		it.it_value.tv_sec = 1;
		it.it_value.tv_nsec = 0;
		
		if (timer_create(CLOCK_REALTIME, &evp, &timerid) == -1)
		{
			perror("fail to timer_create");
			exit(-1);
		}


	}
	void setTimer(time_t sec)
	{
		it.it_interval.tv_sec = sec;
		it.it_interval.tv_nsec = 0;
		it.it_value.tv_sec = sec;
		it.it_value.tv_nsec = 0;
	}
	void start()
	{
		if (timer_settime(timerid, 0, &it, 0) == -1)
		{
			perror("fail to timer_settime");
			exit(-1);
		}

	}
	void stop()
	{
		timer_delete(timerid);
	}
private:
	static void timer_thread(union sigval v)
	{
		printf("timer_thread function! %d\n", v.sival_int);
	}
private:
	timer_t timerid;
	struct sigevent evp;
	struct itimerspec it;
};
*/
/*
class CTimer {
public:
	CTimer(int hour)
	{
		this->init();
		// this->setTimer(hour * 60 * 60);
		this->setTimer(hour);
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
		it.it_interval.tv_sec = sec;
		it.it_interval.tv_nsec = 0;
		it.it_value.tv_sec = sec;
		it.it_value.tv_nsec = 0;
	}
	void start()
	{
		if (timer_settime(timerid, 0, &it, 0) == -1)
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
private:
	static void sig_handler(int signo, siginfo_t* info, void* context)
	{
		printf("Start Timer Task now !!!");
		CTimer *pTimer = (CTimer*)(info->si_ptr);
		pTimer->setTimer(3);

	}
private:
	timer_t timerid;
	struct sigevent evp;
	struct itimerspec it;
	struct sigaction act;
};
*/
/*
truct sigevent{

    union sigval sigev_value;

    int sigev_signo;

    int sigev_notify;

    void (*sigev_notify_function)(union sigval);

    pthread_attr_t *sigev_notify_attributes;

};
union sigval{

    int sival_int;

    void *sival_ptr;

};
*/
class CTimer {
public:
	CTimer(int hour)
	{
		this->init();
		// this->setTimer(hour * 60 * 60);
		this->setTimer(hour);
	}
	void init()
	{
		memset(&evp, 0, sizeof(struct sigevent));
		
		//evp.sigev_value.sival_int = 111;//也是标识定时器的，这和timerid有什么区别？回调函数可以获得
		evp.sigev_value.sival_ptr = this;

		evp.sigev_notify = SIGEV_THREAD;//线程通知的方式，派驻新线程
		evp.sigev_notify_function = timer_thread;//线程函数地址
		
		if (timer_create(CLOCK_REALTIME, &evp, &timerid) == -1)
		{
			perror("fail to timer_create");
			exit(-1);
		}


	}
	void setTimer(time_t sec)
	{
		it.it_interval.tv_sec = sec;
		it.it_interval.tv_nsec = 0;
		it.it_value.tv_sec = sec;
		it.it_value.tv_nsec = 0;
	}
	void start()
	{
		if (timer_settime(timerid, 0, &it, 0) == -1)
		{
			perror("fail to timer_settime");
			exit(-1);
		}

	}
	void stop()
	{
		timer_delete(timerid);
	}
private:
	static void timer_thread(union sigval v)
	{
		printf("timer_thread function! %d\n", v.sival_int);
		CTimer *pTimer = (CTimer*)(v.sival_ptr);
		pTimer->stop();
	}
private:
	timer_t timerid;
	struct sigevent evp;
	struct itimerspec it;
};

int main()
{
	CTimer tm(3);
	tm.start();
	while(1);
	return 0;
}

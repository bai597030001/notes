#include <deque>
#include <string>
#include <stdio.h>
#include <unistd.h>
#include <assert.h>
#include <signal.h>
#include <pthread.h>
#include <stdlib.h>
using std::string;
using std::deque;

class Task
{
public:
    Task(){}
    virtual ~Task(){}
	virtual int run()
	{ 
		printf("This is Base Class of Task\n");
		return 0; 
	};
};

class ThreadPool
{
public:
    ThreadPool(int threadNum = 10);
    ~ThreadPool();
public:
    size_t AddTask(Task * task);
    void   stop();
    int    size();
    void   start();
    Task*  take();
private:
    int CreateThread();
    static void* threadFunc(void * threadData);
private:
    ThreadPool& operator=(const ThreadPool&);
    ThreadPool(const ThreadPool&);
private:
    volatile bool		m_IsRunning;
    int					m_ThreadsNum;
    pthread_t*			m_pThreadID;

    deque<Task*>		m_TaskQueue;
    pthread_mutex_t		m_Mutex;
    pthread_cond_t		m_Condition;
};

ThreadPool::ThreadPool(int threadNum)
{
    m_ThreadsNum = threadNum;
    //m_IsRunning = true;
}
void ThreadPool::start()
{
	m_IsRunning = true;

    CreateThread();
}
ThreadPool::~ThreadPool()
{
    stop();

    m_TaskQueue.clear();
}
int ThreadPool::CreateThread()
{
	pthread_attr_t attr;
	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
		
    pthread_mutex_init(&m_Mutex, NULL);
    pthread_cond_init(&m_Condition, NULL);
    
	m_pThreadID = new pthread_t[m_ThreadsNum];
    for (int i = 0; i < m_ThreadsNum; i++)
    {
        pthread_create(&m_pThreadID[i], &attr, threadFunc, this);
    }

    return 0;
}
size_t ThreadPool::AddTask(Task *task)
{
    pthread_mutex_lock(&m_Mutex);

    m_TaskQueue.push_back(task);
	size_t size = m_TaskQueue.size();

    pthread_mutex_unlock(&m_Mutex);
    pthread_cond_signal(&m_Condition);

    return size;
}
void ThreadPool::stop()
{
    if (!m_IsRunning)
    {
        return;
    }

    m_IsRunning = false;

    pthread_cond_broadcast(&m_Condition);

	delete[] m_pThreadID;
    m_pThreadID = NULL;

    pthread_mutex_destroy(&m_Mutex);
    pthread_cond_destroy(&m_Condition);
}
int ThreadPool::size()
{
    pthread_mutex_lock(&m_Mutex);

    int size = m_TaskQueue.size();

    pthread_mutex_unlock(&m_Mutex);

    return size;
}
Task* ThreadPool::take()
{
	printf("start prepare for take a Task from m_TaskQueue\n");
    Task* task = NULL;
    while (!task)
    {
        pthread_mutex_lock(&m_Mutex);

        while (m_TaskQueue.empty() && m_IsRunning)
        {
            pthread_cond_wait(&m_Condition, &m_Mutex);
        }

        if (!m_IsRunning)
        {
            pthread_mutex_unlock(&m_Mutex);
			continue;
        }
        else if (m_TaskQueue.empty())
        {
            pthread_mutex_unlock(&m_Mutex);
            continue;
        }

        task = m_TaskQueue.front();
        m_TaskQueue.pop_front();

		printf("take a task from m_TaskQueue\n");

        pthread_mutex_unlock(&m_Mutex);
    }
    return task;
}
void* ThreadPool::threadFunc(void* arg)
{
	printf("thread %lu start;\n", pthread_self());

    ThreadPool* pool = static_cast<ThreadPool*>(arg);
    while (pool->m_IsRunning)
    {
        Task* task = pool->take();

		printf("get a task form pool now\n");

        if (!task)
        {
            printf("thread %lu get a NULL task, continue;\n", pthread_self());
            continue;
        }

		task->run();

		if(task)
		{
			delete task;
			task = NULL;
		}
    }

    return 0;
}
//debug
class MyTask : public Task
{
public:
	MyTask(){}
	~MyTask(){}
	virtual int run()
	{
		usleep(100);
		printf("This is Derive Class of Task\n");
		return 0;
	}
private:
	char m_mem[1024 * 1024];
};

ThreadPool g_ThreadPool(2);

void signalHandler(int signo)
{
	if(signo == SIGINT)
	{
		//MyTask task;
		//g_ThreadPool.AddTask(&task);

		//Task * pT(new MyTask);
		Task * pT = new MyTask;
		g_ThreadPool.AddTask(pT);
	}
}

int main()
{
	signal(SIGINT, signalHandler);
	
	g_ThreadPool.start();

	//MyTask task;
	//g_ThreadPool.AddTask(&task);
	for(size_t i = 0; i < 10000000000; i++)
	{
		usleep(50);
		Task * pT(new MyTask);
		size_t sz = g_ThreadPool.AddTask(pT);
		//printf("TaskDeque count is %lu\n", sz);
	}
	
	while(1);
	return 0;
}

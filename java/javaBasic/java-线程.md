[TOC]



# 相关概念

## 进程与线程

- 并发与并行
  - 并行：真正意义上的同时运行，一般需要多台机器或多个CPU同时执行一个任务。
  - 并发：CPU通过时间片轮转方式执行多个任务。
- 进程与线程介绍
  - 进程：系统进行资源分配和调度的最小单位。
  - 线程：线程是进程的一个实体,是CPU调度和分派的最小单位。
- 进程与线程区别
  - 进程有独立的地址空间，一个进程崩溃后，在保护模式下不会对其它进程产生影响。
  - 线程只是一个进程中的不同执行路径。线程有自己的堆栈和局部变量，但线程之间没有单独的地址空间，一个线程死掉就等于整个进程死掉。
  - 一个程序至少有一个进程,一个进程至少有一个线程。

## 线程状态

```java
public enum State {
    NEW,

    RUNNABLE,

    BLOCKED,

    WAITING,

    TIMED_WAITING,

    TERMINATED;
}
```



![](img/java-thread1.jpeg)



![](img/thread1.png)



- 新建状态（new）:

  使用 **new** 关键字和 **Thread** 类或其子类建立一个线程对象后，该线程对象就处于新建状态。它保持这个状态直到程序 **start()** 这个线程。

- 运行状态（runable）:

  包括了操作系统线程状态中的Running和Ready，也就是处于此状态的线程有可能正在执行，也有可能正在等待着CPU为它分配执行时间。

- 无限期等待（Waiting）：

  处于这种状态的线程不会被分配CPU执行时间，它们要等待被其他线程显式地唤醒。以下方法会让线程陷入无限期的等待状态：

  - 没有设置timeout参数的Object.wait()方法；
  - 没有设置timeout参数的Thread.join()方法；
  - LockSupport.park()方法；

- 限期等待（Timed Waiting）：

  处于这种状态的线程也不会被分配CPU执行时间，不过无须等待被其他线程显式地唤醒，在一定时间之后它们会由操作系统自动唤醒。以下方法会让线程进入限期等待状态：

  - Thread.sleep()方法；
  - 设置了timeout参数的Object.wait()方法；
  - 设置了timeout参数的Thread.join()方法；
  - LockSupport.parkNanos()方法；
  - LockSupport.parkUntil()方法；

- 阻塞状态:

  “阻塞状态”与“等待状态”的区别是：“阻塞状态”在等待着获取到一个排它锁，这个事件将在另外一个线程放弃这个锁的时候发生；而“等待状态”则是在等待一段时间，或者唤醒动作的发生。在程序等待进入同步区域（synchronized）的时候，线程将进入这种状态。

  可以分为三种：

  - 等待阻塞：运行状态中的线程执行 wait() 方法，使线程进入到等待阻塞状态。
  - 同步阻塞：线程在获取 synchronized 同步锁失败(因为同步锁被其他线程占用)。
  - 其他阻塞：通过调用线程的 sleep() 或 join() 发出了 I/O 请求时，线程就会进入到阻塞状态。当sleep() 状态超时，join() 等待线程终止或超时，或者 I/O 处理完毕，线程重新转入就绪状态。

- 死亡状态:

  一个运行状态的线程完成任务或者其他终止条件发生时，该线程就切换到终止状态。

# 创建线程

- 通过实现 Runnable 接口；
- 通过继承 Thread 类本身；
- 通过 Callable 和 Future 创建线程。

## Runnable 接口

- 最简单的方法是创建一个实现 Runnable 接口的类。

```java
class RunnableDemo implements Runnable {
   
   private Thread t;
   private String threadName;
   
   RunnableDemo( String name) {
      threadName = name;
      System.out.println("Creating " +  threadName );
   }
   
   public void run() {
      System.out.println("Running " +  threadName );
      try {
         for(int i = 4; i > 0; i--) {
            System.out.println("Thread: " + threadName + ", " + i);
            // 让线程睡眠一会
            Thread.sleep(50);
         }
      }catch (InterruptedException e) {
         System.out.println("Thread " +  threadName + " interrupted.");
      }
      System.out.println("Thread " +  threadName + " exiting.");
   }
   
   public void start () {
      System.out.println("Starting " +  threadName );
      if (t == null) {
         t = new Thread (this, threadName);
         t.start ();
      }
   }
}

public class TestThread {
 
   public static void main(String args[]) {
      RunnableDemo R1 = new RunnableDemo( "Thread-1");
      R1.start();
      
      RunnableDemo R2 = new RunnableDemo( "Thread-2");
      R2.start();
   }   
}
```

## Thread 类

- 第二种方法是创建一个新的类，该类继承 Thread 类，然后创建一个该类的实例。

  继承类必须重写 run() 方法，该方法是新线程的入口点。它也必须调用 start() 方法才能执行。

  该方法尽管被列为一种多线程实现方式，但是本质上也是实现了 Runnable 接口的一个实例。

```java
class ThreadDemo extends Thread {
   private Thread t;
   private String threadName;
   
   ThreadDemo( String name) {
      threadName = name;
      System.out.println("Creating " +  threadName );
   }
   
   public void run() {
      System.out.println("Running " +  threadName );
      try {
         for(int i = 4; i > 0; i--) {
            System.out.println("Thread: " + threadName + ", " + i);
            // 让线程睡眠一会
            Thread.sleep(50);
         }
      }catch (InterruptedException e) {
         System.out.println("Thread " +  threadName + " interrupted.");
      }
      System.out.println("Thread " +  threadName + " exiting.");
   }
   
   public void start () {
      System.out.println("Starting " +  threadName );
      if (t == null) {
         t = new Thread (this, threadName);
         t.start ();
      }
   }
}
 
public class TestThread {
 
   public static void main(String args[]) {
      ThreadDemo T1 = new ThreadDemo( "Thread-1");
      T1.start();
      
      ThreadDemo T2 = new ThreadDemo( "Thread-2");
      T2.start();
   }   
}
```

- 常用方法

```java
构造：
public Thread() {
        init(null, null, "Thread-" + nextThreadNum(), 0);
    }
    
public Thread(Runnable target) {
        init(null, target, "Thread-" + nextThreadNum(), 0);
    }

public Thread(ThreadGroup group, Runnable target) {
        init(group, target, "Thread-" + nextThreadNum(), 0);
    }

......


public static native Thread currentThread();

public static native void yield();

public static native void sleep(long millis) throws InterruptedException;

public static void sleep(long millis, int nanos) throws InterruptedException;
    
public synchronized void start();

@Deprecated
public final void stop();
    
@Deprecated
public final synchronized void stop(Throwable obj);

public void interrupt();

public static boolean interrupted();

public boolean isInterrupted();

@Deprecated
public void destroy();

public final native boolean isAlive();

@Deprecated
public final void suspend();

@Deprecated
public final void resume();

public final void setPriority(int newPriority);

public final int getPriority();

public final synchronized void setName(String name);

public final String getName();

public final ThreadGroup getThreadGroup();

public static int activeCount();	//程序中活跃的线程数。

public static int enumerate(Thread tarray[]);	//枚举程序中的线程。 

@Deprecated
public native int countStackFrames();

public final synchronized void join(long millis);//等待线程终止。 
public final synchronized void join(long millis, int nanos);
public final void join();

public final void setDaemon(boolean on);//设置一个线程为守护线程。(用户线程和守护线程的区别在于，是否等待主线程依赖于主线程结束而结束) 
    
public final boolean isDaemon();

public final void checkAccess();


public long getId();

public State getState();
```

## Callable 和 Future

- 1. 创建 Callable 接口的实现类，并实现 call() 方法，该 call() 方法将作为线程执行体，并且有返回值。
- 2. 创建 Callable 实现类的实例，使用 FutureTask 类来包装 Callable 对象，该 FutureTask 对象封装了该 Callable 对象的 call() 方法的返回值。
- 3. 使用 FutureTask 对象作为 Thread 对象的 target 创建并启动新线程。
- 4. 调用 FutureTask 对象的 get() 方法来获得子线程执行结束后的返回值。

```java
public class CallableThreadTest implements Callable<Integer> {
    public static void main(String[] args)  
    {  
        CallableThreadTest ctt = new CallableThreadTest();  
        FutureTask<Integer> ft = new FutureTask<>(ctt);  
        for(int i = 0;i < 100;i++)  
        {  
            System.out.println(Thread.currentThread().getName()+" 的循环变量i的值"+i);  
            if(i==20)  
            {  
                new Thread(ft,"有返回值的线程").start();  
            }  
        }  
        try  
        {  
            System.out.println("子线程的返回值："+ft.get());  
        } catch (InterruptedException e)  
        {  
            e.printStackTrace();  
        } catch (ExecutionException e)  
        {  
            e.printStackTrace();  
        }  
  
    }
    @Override  
    public Integer call() throws Exception  
    {  
        int i = 0;  
        for(;i<100;i++)  
        {  
            System.out.println(Thread.currentThread().getName()+" "+i);  
        }  
        return i;  
    }  
}
```

- 注意：

  ```java
  future.isDone() //return true,false 无阻塞
  future.get() // return 返回值，阻塞直到该线程运行结束
  ```

# 停止线程

- `stop() destroy() resume()`等方法已经被废弃，那么如何停止一个线程呢？

## 终止线程(标志位)

- 用修改标志位的方式来结束 run() 方法。

```java
public class ServerThread extends Thread {
    //volatile修饰符用来保证其它线程读取的总是该变量的最新的值
    public volatile boolean exit = false; 

    @Override
    public void run() {
        ServerSocket serverSocket = new ServerSocket(8080);
        while(!exit){
            serverSocket.accept(); //阻塞等待客户端消息
            ...
        }
    }
    
    public static void main(String[] args) {
        ServerThread t = new ServerThread();
        t.start();
        ...
        t.exit = true; //修改标志位，退出线程
    }
}
```



##  中断线程( interrupt)

线程收到中断时，通常**表示应该**停止当前的工作，比如退出循环、停止监听、从`wait/sleep/join`状态立即结束等。具体处理方式完全由程序决定。



```java
public class Thread{
	public void interrupt(){}    //中断目标线程，实际上就是设置中断状态为true
	public boolean isInterrupted(){} //返回目标线程的中断状态
	public static boolean interrupted(){} //清除当前线程的中断状态，并返回它之前的值
}
```



### 中断的用途



- 一个正常运行的线程，收到中断后，不会有任何问题，可以不需要做任何处理，当然程序可以通过显式的判断`isInterrupted`，来决定下一步动作，比如下面这样的：

```java
while (!Thread.currentThread().isInterrupted()) {
	// 循环做某件事情
}
```

- 当线程被`Object.wait、Thread.join和Thread.sleep`三种方法之一阻塞时，这时如果收到中断请求，则会抛出`InterruptedException`**并清除中断标志**。这是中断非常有用的一点，可以提前从阻塞状态返回

- 做为一种约定，`Java API`里任何声明为抛出`InterruptedException`的方法，在抛出异常之前，都会先清除掉中断标志。



`Thread.interrupt()` 方法并不像在 for 循环语句中使用 break 语句那样干脆，马上就停止循环。调用 `interrupt`() 方法仅仅是在当前线程中打一个停止的标记（**中断标记**），并不是真的停止线程。

也就是说，线程中断并不会立即终止线程，而是通知目标线程，有人希望你终止。至于目标线程收到通知后会如何处理，则完全由目标线程自行决定。



### 使用示例



```java
public class InterruptThread1 extends Thread{

    public static void main(String[] args) {
        try {
            InterruptThread1 t = new InterruptThread1();
            t.start();
            Thread.sleep(200);
            t.interrupt();
        } catch (InterruptedException e) {
            e.printStackTrace();
        }
    }

    @Override
    public void run() {
        super.run();
        for(int i = 0; i <= 200000; i++) {
            //判断是否被中断
            if(Thread.currentThread().isInterrupted()){
                //处理中断逻辑
                break;
            }
            System.out.println("i=" + i);
        }
    }
}
```



例如，线程通过wait()进入阻塞状态，此时通过`interrupt()`中断该线程；调用`interrupt`()会立即将线程的中断标记设为“`true`”，但是由于线程处于阻塞状态，所以该“中断标记”会立即被清除为“`false`”，同时，会产生一个`InterruptedException`的异常。



# 线程调度

## 线程优先级

- 1 （Thread.MIN_PRIORITY ） 

- 10 （Thread.MAX_PRIORITY ）
- 5（ Thread.NORM_PRIORITY）默认情况下的优先级

线程的优先级有继承关系，比如A线程中创建了B线程，那么B将和A具有相同的优先级。

Thread类的setPriority()和getPriority()方法分别用来设置和获取线程的优先级。

## 线程睡眠

- Thread.sleep(long millis)方法，使线程转到阻塞状态。（millis参数设定睡眠的时间，以毫秒为单位）。
- 当睡眠结束后，就转为就绪（Runnable）状态。sleep()平台移植性好。

## 线程等待

- Object类中的wait()方法，导致当前的线程等待，直到其他线程调用此对象的 notify() 方法或 notifyAll() 唤醒方法。这个两个唤醒方法也是Object类中的方法，行为等价于调用 wait(0) 一样。

## 线程唤醒

- Object类中的notify()方法，唤醒在此对象监视器上等待的单个线程。如果所有线程都在此对象上等待，则会选择唤醒其中一个线程。选择是任意性的，并在对实现做出决定时发生。线程通过调用其中一个 wait 方法，在对象的监视器上等待。 直到当前的线程放弃此对象上的锁定，才能继续执行被唤醒的线程。被唤醒的线程将以常规方式与在该对象上主动同步的其他所有线程进行竞争；例如，唤醒的线程在作为锁定此对象的下一个线程方面没有可靠的特权或劣势。类似的方法还有一个notifyAll()，唤醒在此对象监视器上等待的所有线程。

-  注意：Thread中suspend()和resume()两个方法在JDK1.5中已经废除，不再介绍。因为有死锁倾向。

## 线程让步

- Thread.yield() 
-  **yield()应该做的是让当前运行线程回到可运行状态，以允许具有相同优先级的其他线程获得运行机会。**因此，使用yield()的目的是让相同优先级的线程之间能适当的轮转执行。但是，实际中无法保证yield()达到让步目的，因为让步的线程还有可能被线程调度程序再次选中。

## 线程加入

- join()方法，等待其他线程终止。在当前线程中调用另一个线程的join()方法，则当前线程转入阻塞状态，直到另一个进程运行结束，当前线程再由阻塞转为就绪状态。

# 线程间通信

## wait/notify

- `wait()`与`notify()`的成对使用， 是一种**线程间通信**的手段。（进一步分析， `wait()` 操作的调用必然是在等待某种条件的成立， 而条件的成立必然是由其他的线程来完成的。）
- 在Java中，可以通过配合调用`Object`对象的`wait`方法和`notify`方法或`notifyAll`方法来实现**线程间的协作通信**。在线程中调用wait方法，将阻塞等待其他线程的通知(其他线程调用`notify`方法或`notifyAll`方法)，在线程中调用`notify`方法或`notifyAll`方法，将通知其他线程从`wait`方法处返回。
- 在JDK1.4之后出现了一个**Condition类**，这个类也能够实现**线程间的协作通信**，并且一般建议使用Condition替代`wait,notify,notifyAll`家族，实现更安全的线程间协作通信功能，比如`ArrayBlockingQueue`就是使用`Condition`实现阻塞队列的。
- 注意
  - `Object.wait()`与`Object.notify()`**必须要与**同步块或同步方法(**synchronized**块或者`synchronized`方法)一起使用，也就是`wait`与`notify`是针对已经获取了`Object`锁进行操作，从语法角度来说就是说`Object.wait(),Object.notify`必须在同步块或同步方法内。
  - `wait`：线程在获取对象锁后，主动释放对象锁，同时本线程休眠。直到有其它线程调用对象的`notify()`唤醒该线程，才能继续获取对象锁，并继续执行。
  - `notify`：对对象锁的唤醒操作。有一点需要注意的是`notify()`调用后，并**不是马上就释放对象锁**的，而是在相应的同步块或同步方法中执行结束，自动释放锁后，JVM会在`wait`()对象锁的线程中随机选取一线程，赋予其对象锁，唤醒线程，继续执行。这样就提供了在线程间同步、唤醒的操作。

### 为什么需要和synchronized一起使用

**每个对象都可以被认为是一个"监视器monitor"，这个监视器由三部分组成（一个独占锁，一个入口队列，一个等待队列）(ps:和AQS的state，同步队列，等待队列好相似)。**注意是一个对象只能有一个独占锁，但是任意线程线程都可以拥有这个独占锁。

- 对于对象的<font color="#dd0000">非同步方法</font>而言，任意时刻可以有任意个线程调用该方法。（即普通方法同一时刻可以有多个线程调用）

- 对于对象的<font color="#dd000">同步方法</font>而言，只有拥有这个对象的独占锁才能调用这个同步方法。如果这个独占锁被其他线程占用，那么另外一个调用该同步方法的线程就会处于阻塞状态，**此线程进入入口队列**。

- 若一个拥有该独占锁的线程调用该对象同步方法的`wait`()方法，则该线程会释放独占锁，并**加入对象的等待队列**。只要该线程在该对象的等待队列中， 就会一直处于闲置状态， 不会被调度执行。（要注意`wait()`方法会强迫线程先进行释放锁操作，所以在调用`wait()`时， 该线程必须已经获得锁，否则会抛出异常。由于`wait()`在`synchonized`的方法内部被执行， 锁一定已经获得， 就不会抛出异常了。）

- 某个线程调用`notify`(),`notifyAll`()方法是**将等待队列的线程转移到入口队列，然后在入口队列中的多个线程就会竞争该对象的锁**，所以这个<font color="00dd00">调用线程本身必须拥有锁</font>。
- `wait()`与`notify()`的成对使用， 是一种**线程间通信**的手段。（进一步分析， `wait()` 操作的调用必然是在等待某种条件的成立， 而条件的成立必然是由其他的线程来完成的。）所以实际上， 我们调用 wait() 的时候， 实际上希望达到如下的效果

```java
// 线程A 的代码
while(!condition){ // 不能使用 if , 因为存在一些特殊情况， 使得线程没有收到 notify 时也能退出等待状态
    wait();
}
// do something
```

```java
// 线程 B 的代码
if(!condition){ 
	// do something ...
    condition = true;
    notify();
}
```

现在考虑， 如果`wait()` 和 `notify()` 的操作没有相应的同步机制， 则会发生如下情况

1.【线程A】 进入了 while 循环后（通过了 !condition 判断条件， 但尚未执行 wait 方法）, CPU 时间片耗尽， CPU 开始执行线程B的代码
2.【线程B】 执行完毕了 condition = true; notify(); 的操作， 此时【线程A】的 wait() 操作尚未被执行， notify() 操作没有产生任何效果
3.【线程A】执行wait() 操作， 进入等待状态，如果没有额外的 notify() 操作， 该线程将持续在 condition = true 的情形下， 持续处于等待状态得不到执行。

由此看出， 在使用 wait() 和 notify() 这种会挂起线程的操作时， 我们**需要一种同步机制保证， `condition` 的检查与 `wait()` 操作， 以及 `condition` 的更新与 `notify()` 是互斥的。**

那是否简单的将之前的代码包裹在一个 synchronized 代码块中就可以满足需求呢？ 像下面这样。

```java
// 线程A 的代码
synchronized(obj_A)
{
	while(!condition){ 
	    wait();
	}
	// do something 
}
```

```java
// 线程 B 的代码
synchronized(obj_A)
{
	if(!condition){ 
		// do something ...
	    condition = true;
	    notify();
	}
}
```

乍一看， 上述的代码可以解决问题， 但是仔细分析一下， 由于wait() 操作会挂起当前线程， 那么必然需要在挂起前释放掉 obj_A 的锁， 但如果 obj_A 允许是任意对象， wait() 函数作为一个没有参数输入的方法，无从得知应该释放哪个对象的锁 。于是很自然的， 语法就会被设计成 java 现在的样子。即基于对象的 wait() 与 notify() 的调用， 必须先获得该对象的锁。

```java
// 线程 A 的代码
synchronized(obj_A)
{
	while(!condition){ 
	    obj_A.wait();
	}
	// do something 
}
```

```java
// 线程 B 的代码
synchronized(obj_A)
{
	if(!condition){ 
		// do something ...
	    condition = true;
	    obj_A.notify();
	}
}
```



- 使用示例

```java
public class WaitNotifyTest {


    @Test
    public void test() throws InterruptedException {

        String s = "abc";

        for(int i = 1; i <= 10; i ++){
            new Thread(new Waiter(s),"waiter" + i).start();
        }

        // new Thread(new Notifier(s), "notifier").start();

        for(int i = 1; i <= 10; i ++){
            new Thread(new Notifier(s), "notifier" + i).start();
        }

        Thread.sleep(3000);
    }
}

class Notifier implements Runnable {

    private String msg;

    public Notifier(String msg) {
        this.msg = msg;
    }

    @Override
    public void run() {
        try {
            Thread.sleep(200);
            synchronized (msg) {
                msg.notify();
                // msg.notifyAll();
            }
        } catch (InterruptedException e) {
            e.printStackTrace();
        }

    }

}

class Waiter implements Runnable{

    private String msg;

    public Waiter(String m){
        this.msg=m;
    }

    @Override
    public void run() {
        String threadName = Thread.currentThread().getName();
        synchronized (msg) {
            try{
                System.out.println("thread: " + threadName + " start waiting!");
                msg.wait();
            }catch(InterruptedException e){
                e.printStackTrace();
            }
        }
        System.out.println("thread: " + threadName + " get the notifier!");
    }

}
```



## Condition类

与`synchronized`关键字与`wait`()和`notify`/`notifyAll`()方法相结合相比，`Lock`与`Condition`结合的优点是：可以实现多路通知功能，也就是在一个`Lock`对象中可以创建多个`Condition`实例（即对象监视器），线程对象可以注册在指定的`Condition`中，从而可以有选择性的进行线程通知，在调度线程上更加灵活。



在使用`notify/notifyAll()`方法进行通知时，被通知的线程是有`JVM`选择的，使用`ReentrantLock`类结合`Condition`实例可以实现“选择性通知”，这个功能非常重要，而且是`Condition`接口默认提供的。



`synchronized`关键字就相当于整个`Lock`对象中只有一个`Condition`实例，所有的线程都注册在它一个身上。如果执行`notifyAll`()方法的话就会通知所有处于等待状态的线程这样会造成很大的效率问题，而`Condition`实例的`signalAll`()方法 只会唤醒注册在该`Condition`实例中的所有等待线程。



### 使用单个Condition实例线程等待/通知机制

```java
public class UseSingleConditionWaitNotify {

	public static void main(String[] args) throws InterruptedException {

		MyService service = new MyService();

		ThreadA a = new ThreadA(service);
		a.start();

		Thread.sleep(3000);

		service.signal();

	}

	static public class MyService {

		private Lock lock = new ReentrantLock();
		public Condition condition = lock.newCondition();

		public void await() {
			lock.lock();
			try {
				System.out.println(" await时间为" + System.currentTimeMillis());
                //必须在condition.await()方法调用之前调用lock.lock()代码获得同步监视器，不然会报错。
				condition.await();
				System.out.println("这是condition.await()方法之后的语句，condition.signal()方法之后我才被执行");
			} catch (InterruptedException e) {
				e.printStackTrace();
			} finally {
				lock.unlock();
			}
		}

		public void signal() throws InterruptedException {
			lock.lock();
			try {				
				System.out.println("signal时间为" + System.currentTimeMillis());
				condition.signal();
				Thread.sleep(3000);
				System.out.println("这是condition.signal()方法之后的语句");
			} finally {
				lock.unlock();
			}
		}
	}

	static public class ThreadA extends Thread {

		private MyService service;

		public ThreadA(MyService service) {
			super();
			this.service = service;
		}

		@Override
		public void run() {
			service.await();
		}
	}
}
```

![](img/java-condition)



<font color="#dd00">在使用`wait/notify`实现等待通知机制的时候，我们知道必须执行完`notify`()方法所在的`synchronized`代码块后才释放锁。在这里也差不多，必须执行完`signal`所在的`try`语句块之后才释放锁，`condition.await()`后的语句才能被执行。</font>



### 使用多个Condition实例线程等待/通知机制

```java
public class UseMoreConditionWaitNotify {
	public static void main(String[] args) throws InterruptedException {

		MyserviceMoreCondition service = new MyserviceMoreCondition();

		ThreadA a = new ThreadA(service);
		a.setName("A");
		a.start();

		ThreadB b = new ThreadB(service);
		b.setName("B");
		b.start();

		Thread.sleep(3000);

		service.signalAll_A();

	}
	static public class ThreadA extends Thread {

		private MyserviceMoreCondition service;

		public ThreadA(MyserviceMoreCondition service) {
			super();
			this.service = service;
		}

		@Override
		public void run() {
			service.awaitA();
		}
	}
	static public class ThreadB extends Thread {

		private MyserviceMoreCondition service;

		public ThreadB(MyserviceMoreCondition service) {
			super();
			this.service = service;
		}

		@Override
		public void run() {
			service.awaitB();
		}
	}
	
}
```





```java
public class MyserviceMoreCondition {

	private Lock lock = new ReentrantLock();
	public Condition conditionA = lock.newCondition();
	public Condition conditionB = lock.newCondition();

	public void awaitA() {
		lock.lock();
		try {
			System.out.println("begin awaitA时间为" + System.currentTimeMillis()
					+ " ThreadName=" + Thread.currentThread().getName());
			conditionA.await();
			System.out.println("  end awaitA时间为" + System.currentTimeMillis()
					+ " ThreadName=" + Thread.currentThread().getName());
		} catch (InterruptedException e) {
			e.printStackTrace();
		} finally {
			lock.unlock();
		}
	}

	public void awaitB() {
		lock.lock();
		try {			
			System.out.println("begin awaitB时间为" + System.currentTimeMillis()
					+ " ThreadName=" + Thread.currentThread().getName());
			conditionB.await();
			System.out.println("  end awaitB时间为" + System.currentTimeMillis()
					+ " ThreadName=" + Thread.currentThread().getName());
		} catch (InterruptedException e) {
			e.printStackTrace();
		} finally {
			lock.unlock();
		}
	}

	public void signalAll_A() {
		lock.lock();
		try {			
			System.out.println("  signalAll_A时间为" + System.currentTimeMillis()
					+ " ThreadName=" + Thread.currentThread().getName());
			conditionA.signalAll();
		} finally {
			lock.unlock();
		}
	}

	public void signalAll_B() {
		lock.lock();
		try {		
			System.out.println("  signalAll_B时间为" + System.currentTimeMillis()
					+ " ThreadName=" + Thread.currentThread().getName());
			conditionB.signalAll();
		} finally {
			lock.unlock();
		}
	}
}
```

![](img/java-condition1.)

只有A线程被唤醒了。



### 使用Condition实现线程顺序执行



```java
public class ConditionSeqExec {

	volatile private static int nextPrintWho = 1;
	private static ReentrantLock lock = new ReentrantLock();
	final private static Condition conditionA = lock.newCondition();
	final private static Condition conditionB = lock.newCondition();
	final private static Condition conditionC = lock.newCondition();

	public static void main(String[] args) {

		Thread threadA = new Thread() {
			public void run() {
				try {
					lock.lock();
					while (nextPrintWho != 1) {
						conditionA.await();
					}
					for (int i = 0; i < 3; i++) {
						System.out.println("ThreadA " + (i + 1));
					}
					nextPrintWho = 2;
					//通知conditionB实例的线程运行
					conditionB.signalAll();
				} catch (InterruptedException e) {
					e.printStackTrace();
				} finally {
					lock.unlock();
				}
			}
		};

		Thread threadB = new Thread() {
			public void run() {
				try {
					lock.lock();
					while (nextPrintWho != 2) {
						conditionB.await();
					}
					for (int i = 0; i < 3; i++) {
						System.out.println("ThreadB " + (i + 1));
					}
					nextPrintWho = 3;
					//通知conditionC实例的线程运行
					conditionC.signalAll();
				} catch (InterruptedException e) {
					e.printStackTrace();
				} finally {
					lock.unlock();
				}
			}
		};

		Thread threadC = new Thread() {
			public void run() {
				try {
					lock.lock();
					while (nextPrintWho != 3) {
						conditionC.await();
					}
					for (int i = 0; i < 3; i++) {
						System.out.println("ThreadC " + (i + 1));
					}
					nextPrintWho = 1;
					//通知conditionA实例的线程运行
					conditionA.signalAll();
				} catch (InterruptedException e) {
					e.printStackTrace();
				} finally {
					lock.unlock();
				}
			}
		};
		Thread[] aArray = new Thread[5];
		Thread[] bArray = new Thread[5];
		Thread[] cArray = new Thread[5];

		for (int i = 0; i < 5; i++) {
			aArray[i] = new Thread(threadA);
			bArray[i] = new Thread(threadB);
			cArray[i] = new Thread(threadC);

			aArray[i].start();
			bArray[i].start();
			cArray[i].start();
		}

	}
}
```





# 线程同步

## volatile

- 保证线程可见性
- 防止指令重排（`synchronized`，`final`关键字也可以）

示例代码：

```java
public class VolatileTest {

    private int a = 1;
    private int b = 2;

//    private volatile int a = 1;
//    private volatile int b = 2;

    public void change(){
        // 这两步操作均为原子操作（赋值操作）
        a = 3;
        b = a;
    }

    public void print(){
        System.out.println("b=" + b + ";a=" + a);
    }

    @Test
    public void volatileTest1(){
        while (true){
            final VolatileTest test = new VolatileTest();
            new Thread(() -> {
                try {
                    Thread.sleep(10);
                } catch (InterruptedException e) {
                    e.printStackTrace();
                }
                test.change();
            }).start();

            new Thread(() -> {
                try {
                    Thread.sleep(10);
                } catch (InterruptedException e) {
                    e.printStackTrace();
                }
                test.print();
            }).start();
        }
    }
}
```

为什么会出现b=3;a=1这种结果呢？正常情况下，如果先执行change方法，再执行print方法，输出结果应该为b=3;a=3。相反，如果先执行的print方法，再执行change方法，结果应该是 b=2;a=1。那b=3;a=1的结果是怎么出来的？原因就是第一个线程将值a=3修改后，但是对第二个线程是不可见的，所以才出现这一结果。

如果将a和b都改成volatile类型的变量再执行，则再也不会出现b=3;a=1的结果了。

## synchronized

- 保证线程执行代码块/方法的原子性
- 修饰的对象

```java
// 1. 修饰一个代码块，被修饰的代码块称为同步语句块，其作用的范围是大括号{}括起来的代码，作用的对象是调用这个代码块的对象；
	public void run() {
        synchronized(this) {
			//......
        }
    }

// 2. 修饰一个方法，被修饰的方法称为同步方法，其作用的范围是整个方法，作用的对象是调用这个方法的对象；
	private synchronized void  add(){
        //......
    }

// 3. 修饰一个静态的方法，其作用的范围是整个静态方法，作用的对象是这个类的所有对象；
	private static synchronized void  add(){
        count++;//该操作为非原子操作
    }

// 4. 修饰一个类，其作用的范围是synchronized后面括号括起来的部分，作用主的对象是这个类的所有对象。
    class ClassName {
       public void method() {
          synchronized(ClassName.class) {
             // todo
          }
       }
    }
```

- Java中的每个对象都有一个锁（lock），或者叫做监视器（monitor），当一个线程访问某个对象的synchronized方法时，**将该对象上锁**，<font color="#STCAIYUN">其他任何线程都无法再去访问该对象的synchronized方法了（这里是指所有的同步方法，而不仅仅是同一个方法）</font>，直到之前的那个线程执行方法完毕后（或者是抛出了异常），才将该对象的锁释放掉，其他线程才有可能再去访问该对象的synchronized方法。

代码示例

```java
public class Counter{
     long count = 0;

     public synchronized void add(long value){
       this.count += value;
     }
}

public class CounterThread extends Thread{

     protected Counter counter = null;

     public CounterThread(Counter counter){
        this.counter = counter;
     }

    @Override
	public void run() {
    	for(int i=0; i<10; i++){
           counter.add(i);
        }
     }
}
public class Example {

    public static void main(String[] args){
      Counter counter = new Counter();
      Thread  threadA = new CounterThread(counter);
      Thread  threadB = new CounterThread(counter);

      threadA.start();
      threadB.start();
    }
}
```

创建了两个线程。他们的构造器引用同一个 Counter 实例。Counter.add 方法是同步在实例上，是因为 add 方法是实例方法并且被标记上 synchronized 关键字。因此每次只允许一个线程调用该方法。另外一个线程必须要等到第一个线程退出 add()方法时，才能继续执行方法。

如果两个线程引用了两个不同的 Counter 实例，那么他们可以同时调用 add()方法。这些方法调用了不同的对象，因此这些方法也就同步在不同的对象上。这些方法调用将不会被阻塞。如下面这个例子所示：

```java
public class Example {

    public static void main(String[] args){
      Counter counterA = new Counter();
      Counter counterB = new Counter();
      Thread  threadA = new CounterThread(counterA);
      Thread  threadB = new CounterThread(counterB);

      threadA.start();
      threadB.start();
    }
  }
```

这两个线程，threadA 和 threadB，不再引用同一个 counter 实例。CounterA 和 counterB 的 add 方法同步在他们所属的对象上。调用 counterA 的 add 方法将不会阻塞调用 counterB 的 add 方法。





```java
public class DemoApplicationTests {

    private static int count = 0;

    private static void  add(){
        try {
            Thread.sleep(1);
        } catch (InterruptedException e) {
            e.printStackTrace();
        }
        count++;//该操作为非原子操作
    }

    @Test
    public void threadTest1() throws InterruptedException {
        for (int i = 0; i < 100; i++) {
            new Thread(DemoApplicationTests::add).start();
        }
        Thread.sleep(1000);
        System.out.println("count: " + count);
    }
}
```

示例中当100个线程去执行add()方法对count进行操作时，在无法保证线程原子性的前提下，最终结果肯定小于100



## ThreadLocal

**作用**：

- 同时满足变量在**线程间的隔离**与**方法间的共享**。（如果单看其中某一点，替代方法很多。比如可通过在线程内创建局部变量可实现每个线程有自己的实例，使用静态变量可实现变量在方法间的共享。但是如果同时满足，ThreadLocal是首选）

**实现**：

- 静态内部类 `ThreadLocalMap`
- 静态内部类 `Entry`



```java
public class ThreadLocal<T> {

	static class ThreadLocalMap {

        static class Entry extends WeakReference<ThreadLocal<?>> {

            Object value;

            Entry(ThreadLocal<?> k, Object v) {
                super(k);
                value = v;
            }
        }

        private static final int INITIAL_CAPACITY = 16;

        private Entry[] table;

        private int size = 0;

        private int threshold; // Default to 0

        private void setThreshold(int len) {
            threshold = len * 2 / 3;
        }

        private static int nextIndex(int i, int len) {
            return ((i + 1 < len) ? i + 1 : 0);
        }

        private static int prevIndex(int i, int len) {
            return ((i - 1 >= 0) ? i - 1 : len - 1);
        }

        ThreadLocalMap(ThreadLocal<?> firstKey, Object firstValue) {
            table = new Entry[INITIAL_CAPACITY];
            int i = firstKey.threadLocalHashCode & (INITIAL_CAPACITY - 1);
            table[i] = new Entry(firstKey, firstValue);
            size = 1;
            setThreshold(INITIAL_CAPACITY);
        }

        private ThreadLocalMap(ThreadLocalMap parentMap) {
            Entry[] parentTable = parentMap.table;
            int len = parentTable.length;
            setThreshold(len);
            table = new Entry[len];

            for (int j = 0; j < len; j++) {
                Entry e = parentTable[j];
                if (e != null) {
                    @SuppressWarnings("unchecked")
                    ThreadLocal<Object> key = (ThreadLocal<Object>) e.get();
                    if (key != null) {
                        Object value = key.childValue(e.value);
                        Entry c = new Entry(key, value);
                        int h = key.threadLocalHashCode & (len - 1);
                        while (table[h] != null)
                            h = nextIndex(h, len);
                        table[h] = c;
                        size++;
                    }
                }
            }
        }

        private Entry getEntry(ThreadLocal<?> key) {
            int i = key.threadLocalHashCode & (table.length - 1);
            Entry e = table[i];
            if (e != null && e.get() == key)
                return e;
            else
                return getEntryAfterMiss(key, i, e);
        }

        private void set(ThreadLocal<?> key, Object value) {

            Entry[] tab = table;
            int len = tab.length;
            int i = key.threadLocalHashCode & (len-1);

            for (Entry e = tab[i];
                 e != null;
                 e = tab[i = nextIndex(i, len)]) {
                ThreadLocal<?> k = e.get();

                if (k == key) {
                    e.value = value;
                    return;
                }

                if (k == null) {
                    replaceStaleEntry(key, value, i);
                    return;
                }
            }

            tab[i] = new Entry(key, value);
            int sz = ++size;
            if (!cleanSomeSlots(i, sz) && sz >= threshold)
                rehash();
        }

        private void remove(ThreadLocal<?> key) {
            Entry[] tab = table;
            int len = tab.length;
            int i = key.threadLocalHashCode & (len-1);
            for (Entry e = tab[i];
                 e != null;
                 e = tab[i = nextIndex(i, len)]) {
                if (e.get() == key) {
                    e.clear();
                    expungeStaleEntry(i);
                    return;
                }
            }
        }

        private void rehash() {
            expungeStaleEntries();

            // Use lower threshold for doubling to avoid hysteresis
            if (size >= threshold - threshold / 4)
                resize();
        }
    }
    //......
}
```



**使用示例**：ThreadLocal解决SimpleDataFormat线程不安全问题。

```java
public class SimpleDateFormatTest {

    //(1)创建单例实例
    static SimpleDateFormat sdf = new SimpleDateFormat("yyyy-MM-dd HH:mm:ss");

    static ThreadLocal<DateFormat> tsdf = new ThreadLocal<DateFormat>(){
        @Override
        protected DateFormat initialValue() {
            return new SimpleDateFormat("yyyy-MM-dd HH:mm:ss");
        }
    };

    @Test
    public void test1() {
        //(2)创建多个线程，并启动
        for (int i = 0; i <100; ++i) {
            Thread thread = new Thread(() -> {
                try {
                    //(3)使用单例日期实例解析文本
                    System.out.println(sdf.parse("2017-12-13 15:17:27"));
                } catch (ParseException e) {
                    e.printStackTrace();
                }
            });
            thread.start();//(4)启动线程
        }
        try {
            Thread.sleep(2000);
        } catch (InterruptedException e) {
            e.printStackTrace();
        }
    }

    @Test
    public void test2() {
        //(2)创建多个线程，并启动
        for (int i = 0; i <100; ++i) {
            Thread thread = new Thread(() -> {
                try {
                    //(3)使用单例日期实例解析文本
                    System.out.println(tsdf.get().parse("2017-12-13 15:17:27"));
                } catch (ParseException e) {
                    e.printStackTrace();
                }
            });
            thread.start();//(4)启动线程
        }
        try {
            Thread.sleep(2000);
        } catch (InterruptedException e) {
            e.printStackTrace();
        }
    }
}
```



**常见问题**：

​	**1.内存泄漏问题**：

​	**2.hash冲突如何处理的**



## juc-Atomic

## juc-Locks

lock: 在java.util.concurrent包内。共有三个实现：

- ReentrantLock
- ReentrantReadWriteLock.ReadLock
- ReentrantReadWriteLock.WriteLock

与synchronized区别如下：

1. lock更灵活，可以自由定义多把锁的枷锁解锁顺序（synchronized要按照先加的后解顺序）
2. 提供多种加锁方案，lock 阻塞式, trylock 无阻塞式, lockInterruptily 可打断式， 还有trylock的带超时时间版本。
3. 本质上和监视器锁（即synchronized是一样的）
4. 能力越大，责任越大，必须控制好加锁和解锁，否则会导致灾难。
5. 和Condition类的结合。
6. 性能更高，对比如下图：

![](img/java-thread2.webp)

# 并发容器

- BlockingQueue

  阻塞队列。单向队列。

  BlockingQueue在队列的基础上添加了多线程协作的功能：

  ![](img/java-thread3.webp)

  除了传统的queue功能（表格左边的两列）之外，还提供了阻塞接口put和take，带超时功能的阻塞接口offer和poll。put会在队列满的时候阻塞，直到有空间时被唤醒；take在队　列空的时候阻塞，直到有东西拿的时候才被唤醒。用于生产者-消费者模型尤其好用，堪称神器。

  常见的阻塞队列有：

  - ArrayListBlockingQueue
  - LinkedListBlockingQueue
  - DelayQueue
  - SynchronousQueue

- ConcurrentHashMap

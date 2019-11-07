## juc-sync 同步器框架

这里的juc-sync同步器框架，是指`java.util.concurrent`包下一些**辅助同步器类**，每个类都有自己适合的使用场景：

| 同步器名称     | 作用                                                         |
| :------------- | :----------------------------------------------------------- |
| CountDownLatch | 倒数计数器，构造时设定计数值，当计数值归零后，所有阻塞线程恢复执行；其内部实现了AQS框架 |
| CyclicBarrier  | 循环栅栏，构造时设定等待线程数，当所有线程都到达栅栏后，栅栏放行；其内部通过ReentrantLock和Condition实现同步 |
| Semaphore      | 信号量，类似于“令牌”，用于控制共享资源的访问数量；其内部实现了AQS框架 |
| Exchanger      | 交换器，类似于双向栅栏，用于线程之间的配对和数据交换；其内部根据并发情况有“单槽交换”和“多槽交换”之分 |
| Phaser         | 多阶段栅栏，相当于CyclicBarrier的升级版，可用于分阶段任务的并发控制执行；其内部比较复杂，支持树形结构，以减少并发带来的竞争 |



### CountDownLatch

- 利用它可以实现类似计数器的功能。比如有一个（也可以是多个）任务A，它要等待其他4个任务执行完毕之后才能执行，此时就可以利用CountDownLatch来实现这种功能了。



```java
//调用await()方法的线程会被挂起，它会等待直到count值为0才继续执行
public void await() throws InterruptedException { };

//和await()类似，只不过等待一定的时间后count值还没变为0的话就会继续执行
public boolean await(long timeout, TimeUnit unit) throws InterruptedException { };

public void countDown() { };  //将count值减1
```



```java
public class CountDownLatchTest {

    CountDownLatch latch = new CountDownLatch(2);

    @Test
    public void test(){
        for(int i = 0; i < 2; i++){
            new Thread(() -> {
                System.out.println("子线程"+Thread.currentThread().getName()+"正在执行");
                try {
                    Thread.sleep(3000);
                } catch (InterruptedException e) {
                    e.printStackTrace();
                }
                System.out.println("子线程"+Thread.currentThread().getName()+"执行完毕");
                latch.countDown();
            }).start();
        }

        System.out.println("等待2个子线程执行完毕...");
        try {
            latch.await();
        } catch (InterruptedException e) {
            e.printStackTrace();
        }
        System.out.println("2个子线程已经执行完毕");
        System.out.println("继续执行主线程");
    }
}
```

执行结果

```java
等待2个子线程执行完毕...
子线程Thread-0正在执行
子线程Thread-1正在执行
子线程Thread-0执行完毕
子线程Thread-1执行完毕
2个子线程已经执行完毕
继续执行主线程
```



### CyclicBarrier

- 回环栅栏，通过它可以实现让**一组线程等待至某个状态之后再全部同时执行**。叫做回环是因为当所有等待线程都被释放以后，CyclicBarrier可以被重用。我们暂且把这个状态就叫做barrier，当调用await()方法之后，线程就处于barrier了。

```java
// 参数parties指让多少个线程或者任务等待至barrier状态；参数barrierAction为当这些线程都达到barrier状态时会执行的内容。
public CyclicBarrier(int parties, Runnable barrierAction) {
}
 
public CyclicBarrier(int parties) {
}
```



```java
// 第一个版本比较常用，用来挂起当前线程，直至所有线程都到达barrier状态再同时执行后续任务；
// 第二个版本是让这些线程等待至一定的时间，如果还有线程没有到达barrier状态就直接让到达barrier的线程执行后续任务。
public int await() throws InterruptedException, BrokenBarrierException { };

public int await(long timeout, TimeUnit unit)throws InterruptedException,BrokenBarrierException,TimeoutException { };
```



假若有若干个线程都要进行写数据操作，并且只有所有线程都完成写数据操作之后，这些线程才能继续做后面的事情，此时就可以利用CyclicBarrier了：

```java
public class Test {
    public static void main(String[] args) {
        int N = 4;
        CyclicBarrier barrier  = new CyclicBarrier(N,new Runnable() {
            @Override
            public void run() {
                System.out.println("当前线程"+Thread.currentThread().getName());   
            }
        });
         
        for(int i=0;i<N;i++)
            new Writer(barrier).start();
    }
    static class Writer extends Thread{
        private CyclicBarrier cyclicBarrier;
        public Writer(CyclicBarrier cyclicBarrier) {
            this.cyclicBarrier = cyclicBarrier;
        }
 
        @Override
        public void run() {
            System.out.println("线程"+Thread.currentThread().getName()+"正在写入数据...");
            try {
                Thread.sleep(5000);      //以睡眠来模拟写入数据操作
                System.out.println("线程"+Thread.currentThread().getName()+"写入数据完毕，等待其他线程写入完毕");
                cyclicBarrier.await();
            } catch (InterruptedException e) {
                e.printStackTrace();
            }catch(BrokenBarrierException e){
                e.printStackTrace();
            }
            System.out.println("所有线程写入完毕，继续处理其他任务...");
        }
    }
}
```

运行结果：

```java
线程Thread-0正在写入数据...
线程Thread-1正在写入数据...
线程Thread-2正在写入数据...
线程Thread-3正在写入数据...
线程Thread-0写入数据完毕，等待其他线程写入完毕
线程Thread-1写入数据完毕，等待其他线程写入完毕
线程Thread-2写入数据完毕，等待其他线程写入完毕
线程Thread-3写入数据完毕，等待其他线程写入完毕
当前线程Thread-3
所有线程写入完毕，继续处理其他任务...
所有线程写入完毕，继续处理其他任务...
所有线程写入完毕，继续处理其他任务...
所有线程写入完毕，继续处理其他任务...
```

从结果可以看出，当四个线程都到达barrier状态后，会从四个线程中选择一个线程去执行Runnable。



CyclicBarrier是可以重用的:

```java
public class Test {
    public static void main(String[] args) {
        int N = 4;
        CyclicBarrier barrier  = new CyclicBarrier(N);
         
        for(int i=0;i<N;i++) {
            new Writer(barrier).start();
        }
         
        try {
            Thread.sleep(25000);
        } catch (InterruptedException e) {
            e.printStackTrace();
        }
         
        System.out.println("CyclicBarrier重用");
         
        for(int i=0;i<N;i++) {
            new Writer(barrier).start();
        }
    }
    static class Writer extends Thread{
        private CyclicBarrier cyclicBarrier;
        public Writer(CyclicBarrier cyclicBarrier) {
            this.cyclicBarrier = cyclicBarrier;
        }
 
        @Override
        public void run() {
            System.out.println("线程"+Thread.currentThread().getName()+"正在写入数据...");
            try {
                Thread.sleep(5000);      //以睡眠来模拟写入数据操作
                System.out.println("线程"+Thread.currentThread().getName()+"写入数据完毕，等待其他线程写入完毕");
             
                cyclicBarrier.await();
            } catch (InterruptedException e) {
                e.printStackTrace();
            }catch(BrokenBarrierException e){
                e.printStackTrace();
            }
            System.out.println(Thread.currentThread().getName()+"所有线程写入完毕，继续处理其他任务...");
        }
    }
}
```



```java
线程Thread-0正在写入数据...
线程Thread-1正在写入数据...
线程Thread-3正在写入数据...
线程Thread-2正在写入数据...
线程Thread-1写入数据完毕，等待其他线程写入完毕
线程Thread-3写入数据完毕，等待其他线程写入完毕
线程Thread-2写入数据完毕，等待其他线程写入完毕
线程Thread-0写入数据完毕，等待其他线程写入完毕
Thread-0所有线程写入完毕，继续处理其他任务...
Thread-3所有线程写入完毕，继续处理其他任务...
Thread-1所有线程写入完毕，继续处理其他任务...
Thread-2所有线程写入完毕，继续处理其他任务...
CyclicBarrier重用
线程Thread-4正在写入数据...
线程Thread-5正在写入数据...
线程Thread-6正在写入数据...
线程Thread-7正在写入数据...
线程Thread-7写入数据完毕，等待其他线程写入完毕
线程Thread-5写入数据完毕，等待其他线程写入完毕
线程Thread-6写入数据完毕，等待其他线程写入完毕
线程Thread-4写入数据完毕，等待其他线程写入完毕
Thread-4所有线程写入完毕，继续处理其他任务...
Thread-5所有线程写入完毕，继续处理其他任务...
Thread-6所有线程写入完毕，继续处理其他任务...
Thread-7所有线程写入完毕，继续处理其他任务...
```

从执行结果可以看出，在初次的4个线程越过barrier状态后，又可以用来进行新一轮的使用。而**CountDownLatch无法进行重复使用。**



### Semaphore

- 信号量，可以控同时访问的线程个数，通过 `acquire`() 获取一个许可，如果没有就等待，而 `release`() 释放一个许可。

```java
//参数permits表示许可数目，即同时可以允许多少线程进行访问
public Semaphore(int permits) {
    sync = new NonfairSync(permits);
}
//这个多了一个参数fair表示是否是公平的，即等待时间越久的越先获取许可
public Semaphore(int permits, boolean fair) {
    sync = (fair)? new FairSync(permits) : new NonfairSync(permits);
}
```

比较重要的几个方法

```java
public void acquire() throws InterruptedException {  }     //获取一个许可
public void acquire(int permits) throws InterruptedException { }    //获取permits个许可
public void release() { }          //释放一个许可
public void release(int permits) { }    //释放permits个许可
```

`acquire`()用来获取一个许可，若无许可能够获得，则会一直等待，直到获得许可。

`release`()用来释放许可。注意，在释放许可之前，必须先获获得许可。



上述方法都会被阻塞，如果想立即得到执行结果，可以使用下面几个方法：

```java
//尝试获取一个许可，若获取成功，则立即返回true，若获取失败，则立即返回false
public boolean tryAcquire() { };

//尝试获取一个许可，若在指定的时间内获取成功，则立即返回true，否则则立即返回false
public boolean tryAcquire(long timeout, TimeUnit unit) throws InterruptedException { };

//尝试获取permits个许可，若获取成功，则立即返回true，若获取失败，则立即返回false
public boolean tryAcquire(int permits) { }; 

//尝试获取permits个许可，若在指定的时间内获取成功，则立即返回true，否则则立即返回false
public boolean tryAcquire(int permits, long timeout, TimeUnit unit) throws InterruptedException { };
```



```java
// 得到可用的许可数目。
public int availablePermits() { return sync.getPermits();}
```



示例：假若一个工厂有5台机器，但是有8个工人，一台机器同时只能被一个工人使用，只有使用完了，其他工人才能继续使用。那么我们就可以通过Semaphore来实现：

```java
public class Test {
    public static void main(String[] args) {
        int N = 8;            //工人数
        Semaphore semaphore = new Semaphore(5); //机器数目
        for(int i=0;i<N;i++)
            new Worker(i,semaphore).start();
    }
     
    static class Worker extends Thread{
        private int num;
        private Semaphore semaphore;
        public Worker(int num,Semaphore semaphore){
            this.num = num;
            this.semaphore = semaphore;
        }
         
        @Override
        public void run() {
            try {
                semaphore.acquire();
                System.out.println("工人"+this.num+"占用一个机器在生产...");
                Thread.sleep(2000);
                System.out.println("工人"+this.num+"释放出机器");
                semaphore.release();           
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
        }
    }
}
```





### Exchanger





### Phaser



### 总结

1. `CountDownLatch`和`CyclicBarrier`都能够实现线程之间的等待，只不过它们侧重点不同：

　　　　`CountDownLatch`一般用于某个线程A等待若干个其他线程执行完任务之后，它才执行；

　　　　而`CyclicBarrier`一般用于一组线程互相等待至某个状态，然后这一组线程再同时执行；

　　　　另外，`CountDownLatch`是不能够重用的，而`CyclicBarrier`是可以重用的。

2. `Semaphore`其实和锁有点类似，它一般用于控制对某组资源的访问权限。


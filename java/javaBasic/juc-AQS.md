[TOC]

参考链接

[http://www.iocoder.cn/](http://www.iocoder.cn/)



# AQS

AQS(`AbstractQueuedSynchronizer`，抽象队列同步器) 是构建锁或者其他同步组件的基础框架，位于 `java.util.concurrent.locks` 下。

JUC(`java.util.concurrent`)里所有的锁机制都是基于AQS框架上构建的。

![](E:/notes/java/javaBasic/img/juc1.png)

首先通过上面我画的结构图（只是一个大致的框架，很多类并未列出），可以大致的了解到，JUC当中，锁、条件变量和一些并发工具类都围绕AQS进行构建。同时，线程池、阻塞队列等又依赖于锁和条件变量实现并发。所以说，<font color="#STCAIYUN">AQS是JUC并发包中的核心基础组件。</font>



## 引言

`ReentrantLock`来加锁和释放锁：

```java
Lock lock = new ReentrantLock();

lock.lock();
//业务逻辑

lock.unlock();
```

上述`ReentrantLock`、还有`ReentrantReadWriteLock`底层都是基于AQS来实现的。



给大家画一个图，看一下ReentrantLock和AQS之间的关系。

![](img/aqs1.jpg)

我们看上图，说白了，ReentrantLock内部包含了一个AQS对象，也就是AbstractQueuedSynchronizer类型的对象。



这个AQS对象就是ReentrantLock可以实现加锁和释放锁的关键性的**核心组件**。



## ReentrantLock加锁和释放锁的底层原理

好了，现在如果有一个线程过来尝试用ReentrantLock的lock()方法进行加锁，会发生什么事情？



很简单，这个AQS对象内部有一个核心的变量叫做**state**，是int类型的，代表了**加锁的状态**。



初始状态下，这个state的值是0。



另外，这个AQS内部还有一个**关键变量**，用来记录**当前加锁的是哪个线程**，初始化状态下，这个变量是null。

![](img/aqs2.jpg)

接着线程1跑过来调用ReentrantLock的lock()方法尝试进行加锁，这个加锁的过程，直接就是用CAS操作将state值从0变为1。

（关于CAS，之前专门有文章做过详细阐述，大家可以自行阅读了解）



如果之前没人加过锁，那么state的值肯定是0，此时线程1就可以加锁成功。



一旦线程1加锁成功了之后，就可以设置当前加锁线程是自己。所以大家看下面的图，就是线程1跑过来加锁的一个过程。

![](img/aqs3.jpg)

其实看到这儿，大家应该对所谓的AQS有感觉了。说白了，就是并发包里的一个核心组件，里面有state变量、加锁线程变量等核心的东西，维护了加锁状态。



你会发现，ReentrantLock这种东西只是一个外层的API，**内核中的锁机制实现都是依赖AQS组件的**。



这个ReentrantLock之所以用Reentrant打头，意思就是他是一个可重入锁。



可重入锁的意思，就是你可以对一个ReentrantLock对象多次执行lock()加锁和unlock()释放锁，也就是可以对一个锁加多次，叫做可重入加锁。



大家看明白了那个state变量之后，就知道了如何进行可重入加锁！



其实每次线程1可重入加锁一次，会判断一下当前加锁线程就是自己，那么他自己就可以可重入多次加锁，每次加锁就是把state的值给累加1，别的没啥变化。



接着，如果线程1加锁了之后，线程2跑过来加锁会怎么样呢？



**我们来看看锁的互斥是如何实现的？**



线程2跑过来一下看到，哎呀！state的值不是0啊？所以CAS操作将state从0变为1的过程会失败，因为state的值当前为1，说明已经有人加锁了！



接着线程2会看一下，是不是自己之前加的锁啊？当然不是了，**“加锁线程”**这个变量明确记录了是线程1占用了这个锁，所以线程2此时就是加锁失败。



给大家来一张图，一起来感受一下这个过程：

![](img/aqs4.jpg)

接着，线程2会将自己放入AQS中的一个等待队列，因为自己尝试加锁失败了，此时就要将自己放入队列中来等待，等待线程1释放锁之后，自己就可以重新尝试加锁了



所以大家可以看到，AQS是如此的核心！AQS内部还有一个等待队列，专门放那些加锁失败的线程！



同样，给大家来一张图，一起感受一下：

![](img/aqs5.jpg)

接着，线程1在执行完自己的业务逻辑代码之后，就会释放锁！**他释放锁的过程非常的简单**，就是将AQS内的state变量的值递减1，如果state值为0，则彻底释放锁，会将“加锁线程”变量也设置为null！



整个过程，参见下图：

![](img/aqs6.jpg)

接下来，会从**等待队列的队头唤醒线程2重新尝试加锁。**



好！线程2现在就重新尝试加锁，这时还是用CAS操作将state从0变为1，此时就会成功，成功之后代表加锁成功，就会将state设置为1。



此外，还要把**“加锁线程”**设置为线程2自己，同时线程2自己就从等待队列中出队了。



最后再来一张图，大家来看看这个过程。

![](img/aqs7.jpg)



## CLH 锁

- CLH锁是一种**自旋锁**，能确保无饥饿性，提供先来先服务的公平性。（所谓的自旋是指：当线程试图去拿已经被其它线程占有的锁时，当前线程不会进入阻塞态，而是进入一个死循环去自旋的获取锁，获取到锁之后退出死循环。）
- 同时CLH锁也是一种**基于链表的可扩展，高性能，公平的自旋锁**，申请线程只在本地变量上自旋轮询前驱的状态，如果发现前驱释放了锁就结束自旋。





## 一.AQS简介

`java.util.concurrent`包的核心。，该包中的大多数同步器都是基于AQS来构建的。AQS框架提供了一套通用的机制来管理<u>同步状态</u>（`synchronization state`）、<u>阻塞/唤醒线程</u>、<u>管理等待队列</u>。

我们所熟知的`ReentrantLock`、`CountDownLatch`、`CyclicBarrier`等同步器，其实都是通过内部类实现了AQS框架暴露的API，以此实现各类同步器功能。这些同步器的主要区别其实就是对同步状态（`synchronization` `state`）的定义不同。



AQS框架，分离了构建同步器时的一系列关注点，它的所有操作都围绕着资源——同步状态（synchronization state）来展开，并替用户解决了如下问题：

1. 资源是可以被同时访问？还是在同一时间只能被一个线程访问？（**共享/独占功能**）
2. 访问资源的线程如何进行并发管理？（**等待队列**）
3. 如果线程等不及资源了，如何从等待队列退出？（**超时/中断**）



这其实是一种典型的<u>模板方法设计模式</u>：父类（AQS框架）定义好骨架和内部操作细节，具体规则由子类去实现。
AQS框架将剩下的一个问题留给用户：**什么是资源？如何定义资源是否可以被访问？**



我们来看下几个常见的同步器对这一问题的定义：

| 同步器                 | 资源的定义                                                   |
| :--------------------- | :----------------------------------------------------------- |
| ReentrantLock          | 资源表示独占锁。State为0表示锁可用；为1表示被占用；为N表示重入的次数 |
| CountDownLatch         | 资源表示倒数计数器。State为0表示计数器归零，所有线程都可以访问资源；为N表示计数器未归零，所有线程都需要阻塞。 |
| Semaphore              | 资源表示信号量或者令牌。State≤0表示没有令牌可用，所有线程都需要阻塞；大于0表示由令牌可用，线程每获取一个令牌，State减1，线程没释放一个令牌，State加1。 |
| ReentrantReadWriteLock | 资源表示共享的读锁和独占的写锁。state逻辑上被分成两个16位的unsigned short，分别记录读锁被多少线程使用和写锁被重入的次数。 |

综上所述，AQS框架提供了以下功能：

### 1.1 提供一套模板框架

由于并发的存在，需要考虑的情况非常多，因此能否以一种相对简单的方法来完成这两个目标就非常重要，因为对于用户（AQS框架的使用者来说），很多时候并不关心内部复杂的细节。而AQS其实就是利用模板方法模式来实现这一点，AQS中大多数方法都是final或是private的，也就是说Doug Lea并不希望用户直接使用这些方法，而是只覆写部分模板规定的方法。
AQS通过暴露以下API来让让用户自己解决上面提到的“**如何定义资源是否可以被访问**”的问题：



| 钩子方法          | 描述               |
| :---------------- | :----------------- |
| tryAcquire        | 排它获取（资源数） |
| tryRelease        | 排它释放（资源数） |
| tryAcquireShared  | 共享获取（资源数） |
| tryReleaseShared  | 共享获取（资源数） |
| isHeldExclusively | 是否排它状态       |



### 1.2 支持中断、超时

还记得Lock接口中的那些锁中断、限时等待、锁尝试的方法吗？这些方法的实现其实AQS都内置提供了。
使用了AQS框架的同步器，都支持下面的操作：

- 阻塞和非阻塞（例如tryLock）同步；
- 可选的超时设置，让调用者可以放弃等待；
- 可中断的阻塞操作。



### 1.3 支持独占模式和共享模式



### 1.4 支持Condition条件等待

AQS框架内部通过一个内部类`ConditionObject`，实现了`Condition`接口，以此来为子类提供条件等待的功能。



## 二.AQS方法说明

AQS利用了模板方法模式，其中大多数方法都是final或是private的，我们把这类方法称为**Skeleton Method**，也就是说这些方法是AQS框架自身定义好的骨架，子类是不能覆写的。

下面会按类别简述一些比较重要的方法

### 2.1 CAS操作

CAS，即CompareAndSet，在Java中CAS操作的实现都委托给一个名为UnSafe类，关于`Unsafe`类，以后会专门详细介绍该类，目前只要知道，通过该类可以实现对字段的原子操作。

| 方法名                  | 修饰符               | 描述                    |
| :---------------------- | :------------------- | :---------------------- |
| compareAndSetState      | protected final      | CAS修改同步状态值       |
| compareAndSetHead       | private final        | CAS修改等待队列的头指针 |
| compareAndSetTail       | private final        | CAS修改等待队列的尾指针 |
| compareAndSetWaitStatus | private static final | CAS修改结点的等待状态   |
| compareAndSetNext       | private static final | CAS修改结点的next指针   |

### 2.2 等待队列的核心操作

| 方法名              | 修饰符  | 描述                 |
| :------------------ | :------ | :------------------- |
| enq                 | private | 入队操作             |
| addWaiter           | private | 入队操作             |
| setHead             | private | 设置头结点           |
| unparkSuccessor     | private | 唤醒后继结点         |
| doReleaseShared     | private | 释放共享结点         |
| setHeadAndPropagate | private | 设置头结点并传播唤醒 |

### 2.3 资源的获取操作

| 方法名                       | 修饰符         | 描述                              |
| :--------------------------- | :------------- | :-------------------------------- |
| cancelAcquire                | private        | 取消获取资源                      |
| shouldParkAfterFailedAcquire | private static | 判断是否阻塞当前调用线程          |
| acquireQueued                | final          | 尝试获取资源,获取失败尝试阻塞线程 |
| doAcquireInterruptibly       | private        | 独占地获取资源（响应中断）        |
| doAcquireNanos               | private        | 独占地获取资源（限时等待）        |
| doAcquireShared              | private        | 共享地获取资源                    |
| doAcquireSharedInterruptibly | private        | 共享地获取资源（响应中断）        |
| doAcquireSharedNanos         | private        | 共享地获取资源（限时等待）        |

| 方法名                     | 修饰符       | 描述                       |
| :------------------------- | :----------- | :------------------------- |
| acquire                    | public final | 独占地获取资源             |
| acquireInterruptibly       | public final | 独占地获取资源（响应中断） |
| acquireInterruptibly       | public final | 独占地获取资源（限时等待） |
| acquireShared              | public final | 共享地获取资源             |
| acquireSharedInterruptibly | public final | 共享地获取资源（响应中断） |
| tryAcquireSharedNanos      | public final | 共享地获取资源（限时等待） |

### 2.4 资源的释放操作

| 方法名        | 修饰符       | 描述         |
| :------------ | :----------- | :----------- |
| release       | public final | 释放独占资源 |
| releaseShared | public final | 释放共享资源 |



## 三.AQS原理简述

我们在第一节中讲到，AQS框架分离了构建同步器时的一系列关注点，它的所有操作都围绕着资源——同步状态（synchronization state）来展开因此，围绕着资源，衍生出三个基本问题：

1. 同步状态（synchronization state）的管理
2. 阻塞/唤醒线程的操作
3. 线程等待队列的管理

### 3.1 同步状态

**同步状态的定义**
同步状态，其实就是资源。AQS使用单个int（32位）来保存同步状态，并暴露出`getState`、`setState`以及`compareAndSetState`操作来读取和更新这个状态。

```java
/**
 * 同步状态.
 */
private volatile int state;

protected final int getState() {
    return state;
}

protected final void setState(int newState) {
    state = newState;
}
/**
 * 以原子的方式更新同步状态.
 * 利用Unsafe类实现
 */
protected final boolean compareAndSetState(int expect, int update) {
    return unsafe.compareAndSwapInt(this, stateOffset, expect, update);
}
```

### 3.2 线程的阻塞/唤醒

在JDK1.5之前，除了内置的监视器机制外，没有其它方法可以安全且便捷得阻塞和唤醒当前线程。
JDK1.5以后，`java.util.concurrent.locks`包提供了`LockSupport`类来作为线程阻塞和唤醒的工具。

### 3.3 等待队列

等待队列，是AQS框架的核心，**整个框架的关键其实就是如何在并发状态下管理被阻塞的线程**。
等待队列是严格的FIFO队列，是Craig，Landin和Hagersten锁（CLH锁）的一种变种，采用双向链表实现，因此也叫CLH队列。

**1. 结点定义**
CLH队列中的结点是对线程的包装，结点一共有两种类型：独占（EXCLUSIVE）和共享（SHARED）。
每种类型的结点都有一些状态，其中独占结点使用其中的CANCELLED(1)、SIGNAL(-1)、CONDITION(-2)，共享结点使用其中的CANCELLED(1)、SIGNAL(-1)、PROPAGATE(-3)。

| 结点状态  | 值   | 描述                                                         |
| :-------- | :--- | :----------------------------------------------------------- |
| CANCELLED | 1    | 取消。表示后驱结点被中断或超时，需要移出队列                 |
| SIGNAL    | -1   | 发信号。表示后驱结点被阻塞了（当前结点在入队后、阻塞前，应确保将其prev结点类型改为SIGNAL，以便prev结点取消或释放时将当前结点唤醒。） |
| CONDITION | -2   | Condition专用。表示当前结点在Condition队列中，因为等待某个条件而被阻塞了 |
| PROPAGATE | -3   | 传播。适用于共享模式（比如连续的读操作结点可以依次进入临界区，设为PROPAGATE有助于实现这种迭代操作。） |
| INITIAL   | 0    | 默认。新结点会处于这种状态                                   |

> AQS使用CLH队列实现线程的结构管理，而CLH结构正是用前一结点某一属性表示当前结点的状态，之所以这么做是因为在双向链表的结构下，这样更容易实现取消和超时功能。
>
> next指针：用于维护队列顺序，当临界区的资源被释放时，头结点通过next指针找到队首结点。
> prev指针：用于在结点（线程）被取消时，让当前结点的前驱直接指向当前结点的后驱完成出队动作。



```java
static final class Node {
    
    // 共享模式结点
    static final Node SHARED = new Node();
    
    // 独占模式结点
    static final Node EXCLUSIVE = null;

    static final int CANCELLED =  1;

    static final int SIGNAL    = -1;

    static final int CONDITION = -2;

    static final int PROPAGATE = -3;

    /**
    * INITAL：      0 - 默认，新结点会处于这种状态。
    * CANCELLED：   1 - 取消，表示后续结点被中断或超时，需要移出队列；
    * SIGNAL：      -1- 发信号，表示后续结点被阻塞了；（当前结点在入队后、阻塞前，应确保将其prev结点类型改为SIGNAL，以便prev结点取消或释放时将当前结点唤醒。）
    * CONDITION：   -2- Condition专用，表示当前结点在Condition队列中，因为等待某个条件而被阻塞了；
    * PROPAGATE：   -3- 传播，适用于共享模式。（比如连续的读操作结点可以依次进入临界区，设为PROPAGATE有助于实现这种迭代操作。）
    * 
    * waitStatus表示的是后续结点状态，这是因为AQS中使用CLH队列实现线程的结构管理，而CLH结构正是用前一结点某一属性表示当前结点的状态，这样更容易实现取消和超时功能。
    */
    volatile int waitStatus;

    // 前驱指针
    volatile Node prev;

    // 后驱指针
    volatile Node next;

    // 结点所包装的线程
    volatile Thread thread;

    /**
     * 作用分成两种:
     *  1. 在 Sync Queue 里面, nextWaiter用来判断节点是 共享模式, 还是独占模式
     *  2. 在 Condition queue 里面, 节点主要是链接后继节点 (Condition queue是一个单向的, 不支持并发的 list)
     */
    Node nextWaiter;

    Node() {
    }

    Node(Thread thread, Node mode) { 
        this.nextWaiter = mode;
        this.thread = thread;
    }
}
```

**2. 队列定义**
对于CLH队列，当线程请求资源时，如果请求不到，会将线程包装成结点，将其挂载在队列尾部。
CLH队列的示意图如下：

①初始状态，队列head和tail都指向空

![](img/aqs1.png)



②首个线程入队，先创建一个空的头结点，然后以自旋的方式不断尝试插入一个包含当前线程的新结点

​	![](img/aqs2.png)


![](img/aqs3.png)

```java
/**
 * 以自旋的方式不断尝试插入结点至队列尾部
 *
 * @return 当前结点的前驱结点
 */
private Node enq(final Node node) {
    for (; ; ) {
        Node t = tail;
        if (t == null) { // 如果队列为空，则创建一个空的head结点
            if (compareAndSetHead(new Node()))
                tail = head;
        } else {
            node.prev = t;
            if (compareAndSetTail(t, node)) {
                t.next = node;
                return t;
            }
        }
    }
}
```



## 利用AQS实现自定义同步器

AQS使用一个int成员变量来表示同步状态，通过内置的FIFO队列来完成获取资源线程的排队工作。

```java
private volatile int state;//共享变量，使用volatile修饰保证线程可见性
```



AQS支持两种同步方式：

**1.独占式**

**2.共享式**

这样方便使用者实现不同类型的同步组件，独占式如`ReentrantLock`，共享式如Semaphore，`CountDownLatch`，组合式的如`ReentrantReadWriteLock`。总之，`AQS`为使用提供了底层支撑，如何组装实现，使用者可以自由发挥。

同步器的设计是基于**模板方法模式**的，一般的使用方式是这样：

　**1.使用者继承AbstractQueuedSynchronizer并重写指定的方法。（这些重写方法很简单，无非是对于共享资源state的获取和释放）**

​	**2.将AQS组合在自定义同步组件的实现中，并调用其模板方法，而这些模板方法会调用使用者重写的方法。**

这其实是**模板方法模式**的一个很经典的应用。



**如何使用**

​	首先，我们需要去继承`AbstractQueuedSynchronizer`这个类，然后我们根据我们的需求去重写相应的方法，比如要实现一个独占锁，那就去重写`tryAcquire`，`tryRelease`方法，要实现共享锁，就去重写`tryAcquireShared`，`tryReleaseShared`；最后，在我们的组件中调用AQS中的模板方法就可以了，而这些模板方法是会调用到我们之前重写的那些方法的。也就是说，我们只需要很小的工作量就可以实现自己的同步组件，重写的那些方法，仅仅是一些简单的对于共享资源state的获取和释放操作，至于像是获取资源失败，线程需要阻塞之类的操作，自然是AQS帮我们完成了。



**设计思想**

　　对于使用者来讲，我们无需关心获取资源失败，线程排队，线程阻塞/唤醒等一系列复杂的实现，这些都在AQS中为我们处理好了。我们只需要负责好自己的那个环节就好，也就是获取/释放共享资源state。很经典的模板方法设计模式的应用，AQS为我们定义好顶级逻辑的骨架，并提取出公用的线程入队列/出队列，阻塞/唤醒等一系列复杂逻辑的实现，将部分简单的可由使用者决定的操作逻辑延迟到子类中去实现即可。



实现自定义的同步器：JDK官方文档中的小例子

```java
package juc;

import java.util.concurrent.locks.AbstractQueuedSynchronizer;

/**
 * Created by chengxiao on 2017/3/28.
 */
public class Mutex implements java.io.Serializable {
    //静态内部类，继承AQS
    private static class Sync extends AbstractQueuedSynchronizer {
        //是否处于占用状态
        protected boolean isHeldExclusively() {
            return getState() == 1;
        }
        //当状态为0的时候获取锁，CAS操作成功，则state状态为1，
        public boolean tryAcquire(int acquires) {
            if (compareAndSetState(0, 1)) {
                setExclusiveOwnerThread(Thread.currentThread());
                return true;
            }
            return false;
        }
        //释放锁，将同步状态置为0
        protected boolean tryRelease(int releases) {
            if (getState() == 0) throw new IllegalMonitorStateException();
            setExclusiveOwnerThread(null);
            setState(0);
            return true;
        }
    }
        //同步对象完成一系列复杂的操作，我们仅需指向它即可
        private final Sync sync = new Sync();
        //加锁操作，代理到acquire（模板方法）上就行，acquire会调用我们重写的tryAcquire方法
        public void lock() {
            sync.acquire(1);
        }
        public boolean tryLock() {
            return sync.tryAcquire(1);
        }
        //释放锁，代理到release（模板方法）上就行，release会调用我们重写的tryRelease方法。
        public void unlock() {
            sync.release(1);
        }
        public boolean isLocked() {
            return sync.isHeldExclusively();
        }
}
```



```java
package juc;

import java.util.concurrent.CyclicBarrier;

/**
 * Created by chengxiao on 2017/7/16.
 */
public class TestMutex {
    private static CyclicBarrier barrier = new CyclicBarrier(31);
    private static int a = 0;
    private static  Mutex mutex = new Mutex();

    public static void main(String []args) throws Exception {
        //说明:我们启用30个线程，每个线程对i自加10000次，同步正常的话，最终结果应为300000；
        //未加锁前
        for(int i=0;i<30;i++){
            Thread t = new Thread(new Runnable() {
                @Override
                public void run() {
                    for(int i=0;i<10000;i++){
                        increment1();//没有同步措施的a++；
                    }
                    try {
                        barrier.await();//等30个线程累加完毕
                    } catch (Exception e) {
                        e.printStackTrace();
                    }
                }
            });
            t.start();
        }
        barrier.await();
        System.out.println("加锁前，a="+a);
        //加锁后
        barrier.reset();//重置CyclicBarrier
        a=0;
        for(int i=0;i<30;i++){
            new Thread(new Runnable() {
                @Override
                public void run() {
                    for(int i=0;i<10000;i++){
                        increment2();//a++采用Mutex进行同步处理
                    }
                    try {
                        barrier.await();//等30个线程累加完毕
                    } catch (Exception e) {
                        e.printStackTrace();
                    }
                }
            }).start();
        }
        barrier.await();
        System.out.println("加锁后，a="+a);
    }
    /**
     * 没有同步措施的a++
     * @return
     */
    public static void increment1(){
        a++;
    }
    /**
     * 使用自定义的Mutex进行同步处理的a++
     */
    public static void increment2(){
        mutex.lock();
        a++;
        mutex.unlock();
    }
}
```

结果

```java
加锁前，a=279204
加锁后，a=300000
```



## AQS详解

我们先来简单描述下AQS的基本实现，前面我们提到过，AQS维护一个共享资源state，通过内置的FIFO来完成获取资源线程的排队工作。（这个内置的同步队列称为"CLH"队列）。该队列由一个一个的Node结点组成，每个Node结点维护一个prev引用和next引用，分别指向自己的前驱和后继结点。AQS维护两个指针，分别指向队列头部head和尾部tail。

![](img/aqs4.png)

其实就是个**双端双向链表**。

　　当线程获取资源失败（比如`tryAcquire`时试图设置`state`状态失败），会被构造成一个结点加入CLH队列中，同时当前线程会被阻塞在队列中（通过`LockSupport.park`实现，其实是等待态）。当持有同步状态的线程释放同步状态时，会唤醒后继结点，然后此结点线程继续加入到对同步状态的争夺中。



### 独占式

#### 获取同步状态--acquire()

来看看acquire方法，lock方法一般会直接代理到acquire上

```java
public final void acquire(int arg) {
        if (!tryAcquire(arg) &&
            acquireQueued(addWaiter(Node.EXCLUSIVE), arg))
            selfInterrupt();
    }
```

我们来简单理一下代码逻辑：

​	**a.首先，调用使用者重写的tryAcquire方法，若返回true，意味着获取同步状态成功，后面的逻辑不再执行；若返回false，也就是获取同步状态失败，进入b步骤；**

​	**b.此时，获取同步状态失败，构造独占式同步结点，通过addWatiter将此结点添加到同步队列的尾部（此时可能会有多个线程结点试图加入同步队列尾部，需要以线程安全的方式添加）；**

​	**c.该结点以在队列中尝试获取同步状态，若获取不到，则阻塞结点线程，直到被前驱结点唤醒或者被中断。**



#### addWaiter

为获取同步状态失败的线程，构造成一个Node结点，添加到同步队列尾部

```java
private Node addWaiter(Node mode) {
        Node node = new Node(Thread.currentThread(), mode);//构造结点
        //指向尾结点tail
        Node pred = tail;
        //如果尾结点不为空，CAS快速尝试在尾部添加，若CAS设置成功，返回；否则，eng。
        if (pred != null) {
            node.prev = pred;
            if (compareAndSetTail(pred, node)) {
                pred.next = node;
                return node;
            }
        }
        enq(node);
        return node;
    }
```

先cas快速设置，若失败，进入enq方法　　

将结点添加到同步队列尾部这个操作，同时可能会有多个线程尝试添加到尾部，是非线程安全的操作。

以上代码可以看出，使用了`compareAndSetTail`这个cas操作保证安全添加尾结点。

　

#### enq

```java
private Node enq(final Node node) {
        for (;;) {
            Node t = tail;
            if (t == null) { //如果队列为空，创建结点，同时被head和tail引用
                if (compareAndSetHead(new Node()))
                    tail = head;
            } else {
                node.prev = t;
                if (compareAndSetTail(t, node)) {//cas设置尾结点，不成功就一直重试
                    t.next = node;
                    return t;
                }
            }
        }
    }
```

enq内部是个死循环，通过CAS设置尾结点，不成功就一直重试。很经典的CAS自旋的用法，我们在之前关于原子类的源码分析中也提到过。这是一种**乐观的并发策略**。



#### acquireQueued

```java
final boolean acquireQueued(final Node node, int arg) {
        boolean failed = true;
        try {
            boolean interrupted = false;
            for (;;) {//死循环
                final Node p = node.predecessor();//找到当前结点的前驱结点
                if (p == head && tryAcquire(arg)) {//如果前驱结点是头结点（即：当前节点是老二节点），才tryAcquire，其他结点是没有机会tryAcquire的。
                    setHead(node);//获取同步状态成功，将当前结点设置为头结点。
                    p.next = null; // 方便GC
                    failed = false;
                    return interrupted;
                }
                // 如果没有获取到同步状态，通过shouldParkAfterFailedAcquire判断是否应该阻塞，parkAndCheckInterrupt用来阻塞线程
                if (shouldParkAfterFailedAcquire(p, node) &&
                    parkAndCheckInterrupt())
                    interrupted = true;
            }
        } finally {
            if (failed)
                cancelAcquire(node);
        }
    }
```

`acquireQueued`内部也是一个死循环，只有前驱结点是头结点的结点，也就是老二结点，才有机会去`tryAcquire`；若`tryAcquire`成功，表示获取同步状态成功，将此结点设置为头结点；若是非老二结点，或者`tryAcquire`失败，则进入`shouldParkAfterFailedAcquire`去判断判断当前线程是否应该阻塞，若可以，调用`parkAndCheckInterrupt`阻塞当前线程，直到被中断或者被前驱结点唤醒。若还不能休息，继续循环。



#### shouldParkAfterFailedAcquire

- 用来判断当前结点线程是否能休息

```java
private static boolean shouldParkAfterFailedAcquire(Node pred, Node node) {
        //获取前驱结点的wait值 
        int ws = pred.waitStatus;
        if (ws == Node.SIGNAL)//若前驱结点的状态是SIGNAL，意味着当前结点可以被安全地park
            return true;
        if (ws > 0) {
        // ws>0，只有CANCEL状态ws才大于0。若前驱结点处于CANCEL状态，也就是此结点线程已经无效，从后往前遍历，找到一个非CANCEL状态的结点，将自己设置为它的后继结点
            do {
                node.prev = pred = pred.prev;
            } while (pred.waitStatus > 0);
            pred.next = node;
        } else {  
            // 若前驱结点为其他状态，将其设置为SIGNAL状态
            compareAndSetWaitStatus(pred, ws, Node.SIGNAL);
        }
        return false;
    }
```



若`shouldParkAfterFailedAcquire`返回`true`，也就是当前结点的前驱结点为`SIGNAL`状态，则意味着当前结点可以放心休息，进入`parking`状态了。`parkAncCheckInterrupt`阻塞线程并处理中断。



```java
private final boolean parkAndCheckInterrupt() {
        LockSupport.park(this);//使用LockSupport使线程进入阻塞状态
        return Thread.interrupted();// 线程是否被中断过
    }
```

至此，关于acquire的方法源码已经分析完毕，我们来简单总结下

> a.首先tryAcquire获取同步状态，成功则直接返回；否则，进入下一环节；
>
> b.线程获取同步状态失败，就构造一个结点，加入同步队列中，这个过程要保证线程安全；
>
> c.加入队列中的结点线程进入自旋状态，若是老二结点（即前驱结点为头结点），才有机会尝试去获取同步状态；否则，当其前驱结点的状态为SIGNAL，线程便可安心休息，进入阻塞状态，直到被中断或者被前驱结点唤醒。



#### 释放同步状态--release()

当前线程执行完自己的逻辑之后，需要释放同步状态，来看看release方法的逻辑

```java
public final boolean release(int arg) {
        if (tryRelease(arg)) {//调用使用者重写的tryRelease方法，若成功，唤醒其后继结点，失败则返回false
            Node h = head;
            if (h != null && h.waitStatus != 0)
                unparkSuccessor(h);//唤醒后继结点
            return true;
        }
        return false;
    }
```



#### `unparkSuccessor`:唤醒后继结点　

```java
private void unparkSuccessor(Node node) {
        //获取wait状态
        int ws = node.waitStatus;
        if (ws < 0)
            compareAndSetWaitStatus(node, ws, 0);// 将等待状态waitStatus设置为初始值0
        Node s = node.next;//后继结点
        if (s == null || s.waitStatus > 0) {//若后继结点为空，或状态为CANCEL（已失效），则从后尾部往前遍历找到一个处于正常阻塞状态的结点　　　　　进行唤醒
            s = null;
            for (Node t = tail; t != null && t != node; t = t.prev)
                if (t.waitStatus <= 0)
                    s = t;
        }
        if (s != null)
            LockSupport.unpark(s.thread);//使用LockSupprot唤醒结点对应的线程
    }
```



`release`的同步状态相对简单，需要找到头结点的后继结点进行唤醒，若后继结点为空或处于CANCEL状态，从后向前遍历找寻一个正常的结点，唤醒其对应线程。



### 共享式

共享式：共享式地获取同步状态。对于独占式同步组件来讲，同一时刻只有一个线程能获取到同步状态，其他线程都得去排队等待，其待重写的尝试获取同步状态的方法`tryAcquire`返回值为`boolean`，这很容易理解；对于共享式同步组件来讲，同一时刻可以有多个线程同时获取到同步状态，这也是“共享”的意义所在。其待重写的尝试获取同步状态的方法`tryAcquireShared`返回值为int。

```java
 protected int tryAcquireShared(int arg) {
        throw new UnsupportedOperationException();
    }
```

**1.当返回值大于0时，表示获取同步状态成功，同时还有剩余同步状态可供其他线程获取；**

**2.当返回值等于0时，表示获取同步状态成功，但没有可用同步状态了；**

**3.当返回值小于0时，表示获取同步状态失败。**



#### 获取同步状态--acquireShared

```java
public final void acquireShared(int arg) {
        if (tryAcquireShared(arg) < 0)//返回值小于0，获取同步状态失败，排队去；获取同步状态成功，直接返回去干自己的事儿。
            doAcquireShared(arg);
    }
```



```java
private void doAcquireShared(int arg) {
        final Node node = addWaiter(Node.SHARED);//构造一个共享结点，添加到同步队列尾部。若队列初始为空，先添加一个无意义的傀儡结点，再将新节点添加到队列尾部。
        boolean failed = true;//是否获取成功
        try {
            boolean interrupted = false;//线程parking过程中是否被中断过
            for (;;) {//死循环
                final Node p = node.predecessor();//找到前驱结点
                if (p == head) {//头结点持有同步状态，只有前驱是头结点，才有机会尝试获取同步状态
                    int r = tryAcquireShared(arg);//尝试获取同步装填
                    if (r >= 0) {//r>=0,获取成功
                        setHeadAndPropagate(node, r);//获取成功就将当前结点设置为头结点，若还有可用资源，传播下去，也就是继续唤醒后继结点
                        p.next = null; // 方便GC
                        if (interrupted)
                            selfInterrupt();
                        failed = false;
                        return;
                    }
                }
                if (shouldParkAfterFailedAcquire(p, node) &&//是否能安心进入parking状态
                    parkAndCheckInterrupt())//阻塞线程
                    interrupted = true;
            }
        } finally {
            if (failed)
                cancelAcquire(node);
        }
    }
```



大体逻辑与独占式的acquireQueued差距不大，只不过由于是共享式，会有多个线程同时获取到线程，也可能同时释放线程，空出很多同步状态，所以当排队中的老二获取到同步状态，如果还有可用资源，会继续传播下去。



#### setHeadAndPropagate

```java
private void setHeadAndPropagate(Node node, int propagate) {
        Node h = head; // Record old head for check below
        setHead(node);
        if (propagate > 0 || h == null || h.waitStatus < 0) {
            Node s = node.next;
            if (s == null || s.isShared())
                doReleaseShared();
        }
    }
```



#### 释放同步状态--releaseShared

```java
public final boolean releaseShared(int arg) {
        if (tryReleaseShared(arg)) {
            doReleaseShared();//释放同步状态
            return true;
        }
        return false;
    }
```



```java
private void doReleaseShared() {
        for (;;) {//死循环，共享模式，持有同步状态的线程可能有多个，采用循环CAS保证线程安全
            Node h = head;
            if (h != null && h != tail) {
                int ws = h.waitStatus;
                if (ws == Node.SIGNAL) {
                    if (!compareAndSetWaitStatus(h, Node.SIGNAL, 0))
                        continue;          
                    unparkSuccessor(h);//唤醒后继结点
                }
                else if (ws == 0 &&
                         !compareAndSetWaitStatus(h, 0, Node.PROPAGATE))
                    continue;                
            }
            if (h == head)              
                break;
        }
    }
```

代码逻辑比较容易理解，需要注意的是，共享模式，释放同步状态也是多线程的，此处采用了CAS自旋来保证。





### 总结

​	AQS是JUC中很多同步组件的构建基础，简单来讲，它内部实现主要是状态变量state和一个FIFO队列来完成，同步队列的头结点是当前获取到同步状态的结点，获取同步状态state失败的线程，会被构造成一个结点（或共享式或独占式）加入到同步队列尾部（采用自旋CAS来保证此操作的线程安全），随后线程会阻塞；释放时唤醒头结点的后继结点，使其加入对同步状态的争夺中。

　AQS为我们定义好了顶层的处理实现逻辑，我们在使用AQS构建符合我们需求的同步组件时，只需重写tryAcquire，tryAcquireShared，tryRelease，tryReleaseShared几个方法，来决定同步状态的释放和获取即可，至于背后复杂的线程排队，线程阻塞/唤醒，如何保证线程安全，都由AQS为我们完成了，这也是非常典型的模板方法的应用。AQS定义好顶级逻辑的骨架，并提取出公用的线程入队列/出队列，阻塞/唤醒等一系列复杂逻辑的实现，将部分简单的可由使用者决定的操作逻辑延迟到子类中去实现。　



## AQS之Condition队列

[http://www.iocoder.cn/JUC/sike/Condition/](http://www.iocoder.cn/JUC/sike/Condition/)


# Java内存区域

![](img/jmm11.png)

方法区（Method Area）：

```tex
方法区属于线程共享的内存区域，又称Non-Heap（非堆），
主要用于存储已被虚拟机加载的类信息、常量、静态变量、即时编译器编译后的代码等数据，
根据Java 虚拟机规范的规定，当方法区无法满足内存分配需求时，将抛出OutOfMemoryError 异常。
值得注意的是在方法区中存在一个叫运行时常量池(Runtime Constant Pool）的区域，它主要用于存放编译器生成的各种字面量和符号引用，这些内容将在类加载后存放到运行时常量池中，以便后续使用。
```



JVM堆（Java Heap）：

```tex
Java 堆也是属于线程共享的内存区域，它在虚拟机启动时创建，是Java 虚拟机所管理的内存中最大的一块，主要用于存放对象实例，几乎所有的对象实例都在这里分配内存，
注意Java 堆是垃圾收集器管理的主要区域，因此很多时候也被称做GC 堆，如果在堆中没有内存完成实例分配，并且堆也无法再扩展时，将会抛出OutOfMemoryError 异常。
```



程序计数器(Program Counter Register)：

```tex
属于线程私有的数据区域，是一小块内存空间，主要代表当前线程所执行的字节码行号指示器。字节码解释器工作时，通过改变这个计数器的值来选取下一条需要执行的字节码指令，分支、循环、跳转、异常处理、线程恢复等基础功能都需要依赖这个计数器来完成。
```



虚拟机栈(Java Virtual Machine Stacks)：

```tex
属于线程私有的数据区域，与线程同时创建，总数与线程关联，代表Java方法执行的内存模型。
每个方法执行时都会创建一个栈桢来存储方法的的变量表、操作数栈、动态链接方法、返回值、返回地址等信息。
每个方法从调用直结束就对于一个栈桢在虚拟机栈中的入栈和出栈过程，如下（图有误，应该为栈桢）：
```

![](img/jmm12.png)



本地方法栈(Native Method Stacks)：

```tex
本地方法栈属于线程私有的数据区域，这部分主要与虚拟机用到的 Native 方法相关
```





# java内存模型(JMM)

```properties
- 并发 并行
- 同步 异步
- 阻塞 非阻塞
- 原子性、可见性、有序性
- 可重入，可重入/不可重入是什么意思
```



## 1.概念

- JVM 中试图定义一种 JMM 来屏蔽各种硬件和操作系统的内存访问差异，以实现让 Java 程序在各种平台下都能达到一致的内存访问效果。

- JMM定义了**Java 虚拟机(JVM)在计算机内存(RAM)中的工作方式**。

- JMM 的主要目标是定义程序中各个变量的访问规则，即在虚拟机中将变量存储到内存和从内存中取出变量这样的底层细节。(此处的变量与 Java 编程中的变量有所区别，它包括了实例字段、静态字段和构成数组对象的元素，但不包括局部变量与方法参数，因为后者是线程私有的，不会被共享，自然就不会存在竞争问题。)

- JMM 是围绕着在并发过程中如何处理**原子性、可见性和有序性**这 3 个特征来建立的。





![](img/jmm1)



JMM定义了**线程和主内存之间的抽象关系**：线程之间的共享变量存储在主内存（main memory）中，每个线程都有一个私有的本地内存（local memory），本地内存中存储了该线程以读/写共享变量的副本。



## 2.JVM对JMM的实现

- 在JVM内部，Java内存模型把内存分成了两部分：线程栈区和堆区

![](img/jmm2)

- JVM中运行的每个线程都拥有自己的线程栈，线程栈包含了当前线程执行的方法调用相关信息，我们也把它称作调用栈。随着代码的不断执行，调用栈会不断变化。
- 线程栈还包含了当前方法的所有本地变量信息。一个线程只能读取自己的线程栈，
- 所有原始类型(boolean,byte,short,char,int,long,float,double)的本地变量都直接保存在线程栈当中，对于它们的值各个线程之间都是独立的。
- 堆区包含了Java应用创建的所有对象信息，不管对象是哪个线程创建的，其中的对象包括原始类型的封装类（如Byte、Integer、Long等等）。

![](img/jmm3)

- 一个本地变量如果是原始类型，那么它会被完全存储到栈区。
- 一个本地变量也有可能是一个对象的引用，这种情况下，这个本地引用会被存储到栈中，但是对象本身仍然存储在堆区。



- 对于一个对象的成员方法，这些方法中包含本地变量，仍需要存储在栈区，即使它们所属的对象在堆区。
- 对于一个对象的成员变量，不管它是原始类型还是包装类型，都会被存储到堆区。



- 堆中的对象可以被多线程共享。如果一个线程获得一个对象的引用，它便可访问这个对象的成员变量。如果两个线程同时调用了同一个对象的同一个方法，那么这两个线程便可同时访问这个对象的成员变量，但是对于本地变量，每个线程都会拷贝一份到自己的线程栈中。



![](img/jmm4)

![](img/jmm9.png)

### 硬件内存架构

![](img/jmm5)

### Java内存模型和硬件架构之间的桥接

- Java内存模型和硬件内存架构并不一致。硬件内存架构中并没有区分栈和堆，从硬件上看，不管是栈还是堆，大部分数据都会存到主存中，当然一部分栈和堆的数据也有可能会存到CPU寄存器中，如下图所示，Java内存模型和计算机硬件内存架构是一个交叉关系：

![](img/jmm6)

- 当对象和变量存储到计算机的各个内存区域时，必然会面临一些问题，其中最主要的两个问题是：

> 1. 共享对象对各个线程的可见性
> 2. 共享对象的竞争现象



#### 共享对象的可见性

- 当多个线程同时操作同一个共享对象时，如果没有合理的使用volatile和synchronization关键字，一个线程对共享对象的更新有可能导致其它线程不可见。

![](img/jmm7)

- 要解决共享对象可见性这个问题，我们可以使用`volatile`关键字。volatile 关键字可以保证变量会直接从主存读取，而对变量的更新也会直接写到主存。（总线锁，缓存锁）



#### 竞争现象



- 如果多个线程共享一个对象，如果它们同时修改这个共享对象，这就产生了竞争现象。

如下图所示，线程A和线程B共享一个对象obj。假设线程A从主存读取Obj.count变量到自己的CPU缓存，同时，线程B也读取了Obj.count变量到它的CPU缓存，并且这两个线程都对Obj.count做了加1操作。此时，Obj.count加1操作被执行了两次，不过都在不同的CPU缓存中。

如果这两个加1操作是串行执行的，那么Obj.count变量便会在原始值上加2，最终主存中的Obj.count的值会是3。然而下图中两个加1操作是并行的，不管是线程A还是线程B先flush计算结果到主存，最终主存中的Obj.count只会增加1次变成2，尽管一共有两次加1操作。

![](img/jmm8)

- 要解决上面的问题我们可以使用`synchronized`代码块。`synchronized`代码块可以保证同一个时刻只能有一个线程进入代码竞争区，`synchronized`代码块也能保证代码块中所有变量都将会从主存中读，当线程退出代码块时，对所有变量的更新将会flush到主存，不管这些变量是不是`volatile`类型的。
  







juc:

[https://www.cnblogs.com/chenpi/p/5614290.html](https://www.cnblogs.com/chenpi/p/5614290.html)

[https://segmentfault.com/a/1190000015558984](https://segmentfault.com/a/1190000015558984)

[http://javahao123.com/?p=765](http://javahao123.com/?p=765)



# transient

- 保证变量不被序列化

# volatile

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

# synchronized

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

示例中当100个线程去执行add()方法对count进行操作时，在无法保证线程有序性的前提下，最终结果肯定小于100







# CAS

- `CompareAndSwap`，即比较并交换。CAS是一个原子操作，它比较一个内存位置的值并且只有相等时修改这个内存位置的值为新的值，保证了新的值总是基于最新的信息计算的，如果有其他线程在这期间修改了这个值则CAS失败。

## 什么是cas

- 在 Java 并发领域，解决并发安全问题可以使用 synchronized 关键字，但它是一种独占形式的锁，属于悲观锁机制，性能会大打折扣。volatile 貌似也是一个不错的选择，但 volatile 只能保持变量的可见性，并不保证变量的原子性操作。而cas是一种原子操作，同时 CAS 是一种乐观机制。

## 怎么使用cas

## cas实现原理

- CAS 的思想很简单：**三个参数，一个当前内存值 V、旧的预期值 A、即将更新的值 B，当且仅当预期值 A 和内存值 V 相同时，将内存值修改为 B 并返回 true，否则什么都不做，并返回 false**。

**实例分析：（以AtomicInteger为例）**

```java
public class AtomicInteger extends Number implements java.io.Serializable {
  private static final long serialVersionUID = 6214790243416807050L;

  // setup to use Unsafe.compareAndSwapInt for updates
  private static final Unsafe unsafe = Unsafe.getUnsafe();
  private static final long valueOffset;

  static {
    try {
      valueOffset = unsafe.objectFieldOffset
        (AtomicInteger.class.getDeclaredField("value"));
    } catch (Exception ex) { throw new Error(ex); }
  }

  private volatile int value;
  
  // 省略部分代码
}
```

这里用到了 `sun.misc.Unsafe` 类，它可以提供硬件级别的原子操作，它可以获取某个属性在内存中的位置，也可以修改对象的字段值，只不过该类对一般开发而言，很少会用到，其底层是用 C/C++ 实现的，所以它的方式都是被 native 关键字修饰过的。

可以看得出 AtomicInteger 类存储的值是在 **value 字段**中，并且获取了 Unsafe 实例，在静态代码块中，还获取了 **value 字段在内存中的偏移量 valueOffset**。



接下来我们看个例子：

```java
public class AddIntTest {
  public AtomicInteger i;
  public void add() {
    i.getAndIncrement();
  }
}
```

如上，getAndIncrement() 方法底层利用 CAS 技术保证了并发安全。

```java
public final int getAndIncrement() {
  return unsafe.getAndAddInt(this, valueOffset, 1);
}
```

getAndAddInt 方法：

```java
public final int getAndAddInt(Object var1, long var2, int var4) {
  int var5;
  do {
    var5 = this.getIntVolatile(var1, var2);
  } while(!this.compareAndSwapInt(var1, var2, var5, var5 + var4));// 自旋
  return var5;
}
```

**重点讲解：**

var5 通过 `this.getIntVolatile(var1, var2)`方法获取，是个 native 方法，其目的是获取 var1中 var2 偏移量的值，其中 var1 就是 AtomicInteger， var2 是 valueOffset 值。

那么 CAS **核心重点**来了，`compareAndSwapInt` 就是实现 CAS 的核心方法，其原理是如果 var1 中的 value 值和 var5 相等，就证明没有其他线程改变过这个变量，那么就把 value 值更新为 `var5 + var4`，其中 var4 是更新的增量值；反之如果没有更新，那么 CAS 就一直采用自旋的方式继续进行操作（其实就是个 while 循环），这一步也是一个原子操作。

**示例分析**：

1. 设定 AtomicInteger 的 value 原始值为 A，从 Java 内存模型得知，线程 1 和线程 2 各自持有一份 value 的副本，值都是 A。
2. 线程 1 通过`getIntVolatile(var1, var2)`拿到 value 值 A，这时线程 1 被挂起。
3. 线程 2 也通过`getIntVolatile(var1, var2)`方法获取到 value 值 A，并执行`compareAndSwapInt`方法比较内存值也为 A，成功修改内存值为 B。
4. 这时线程 1 恢复执行`compareAndSwapInt`方法比较，发现自己手里的值 A 和内存的值 B 不一致，说明该值已经被其它线程提前修改过了。
5. 线程 1 重新执行`getIntVolatile(var1, var2)`再次获取 value 值，因为变量 value 被 volatile 修饰，所以其它线程对它的修改，线程 A 总是能够看到，线程A继续执行`compareAndSwapInt`进行比较替换，直到成功。

## cas使用场景

- java.util.concurrent 包很多功能都是建立在 CAS 之上，如 ReenterLock 内部的 AQS，各种Atomic类，其底层都用 CAS来实现原子操作。

## cas存在的问题

### `ABA`问题：

例如：

> 线程 1 从内存位置 V 取出 A，这时候线程 2 也从内存位置 V 取出 A，此时线程 1 处于挂起状态，线程 2 将位置 V 的值改成 B，最后再改成 A，这时候线程 1 再执行，发现位置 V 的值没有变化，尽管线程 1 也更改成功了，但是不代表这个过程就是没有问题的。

再例如：

> 现有一个用单向链表实现的栈，栈顶元素为 A，A.next 为 B，期望用 CAS 将栈顶替换成 B。
>
> 有线程 1 获取了元素 A，此时线程 1 被挂起，线程 2 也获取了元素 A，并将 A、B 出栈，再 push D、C、A，这时线程 1 恢复执行 CAS，因为此时栈顶元素依然为 A，线程 1 执行成功，栈顶元素变成了 B，但 B.next 为 null，这就会导致 C、D 被丢掉了。



**ABA解决办法**

- 通常，我们的乐观锁实现中都会带一个 `version` 字段来记录更改的版本，避免并发操作带来的问题。
- 在 Java 中，`AtomicStampedReference` 也实现了这个处理方式。

```java
public class AtomicStampedReference<V> {

    private static class Pair<T> {
        final T reference;
        final int stamp;
        private Pair(T reference, int stamp) {
            this.reference = reference;
            this.stamp = stamp;
        }
        static <T> Pair<T> of(T reference, int stamp) {
            return new Pair<T>(reference, stamp);
        }
    }

    private volatile Pair<V> pair;
    
    //省略代码
    
}
```

如上，每个 Pair 维护一个值，其中 reference 维护对象的引用，stamp 维护修改的版本号。

compareAndSet 方法：

```java
/**
     * Atomically sets the value of both the reference and stamp
     * to the given update values if the
     * current reference is {@code ==} to the expected reference
     * and the current stamp is equal to the expected stamp.
     *
     * @param expectedReference the expected value of the reference
     * @param newReference the new value for the reference
     * @param expectedStamp the expected value of the stamp
     * @param newStamp the new value for the stamp
     * @return {@code true} if successful
     */
public boolean compareAndSet(V   expectedReference,
                             V   newReference,
                             int expectedStamp,
                             int newStamp) {
    Pair<V> current = pair;
    return
        expectedReference == current.reference &&
        expectedStamp == current.stamp &&
        ((newReference == current.reference &&
          newStamp == current.stamp) ||
         casPair(current, Pair.of(newReference, newStamp)));
}
```

从 compareAndSet 方法得知，**如果要更改内存中的值，不但要值相同，还要版本号相同**。

举例分析：

```java
public class AtomicStampedReferenceTest {

  // 初始值为1，版本号为0
  private static AtomicStampedReference<Integer> a = new AtomicStampedReference<>(1, 0);

  // 计数器
  private static CountDownLatch countDownLatch = new CountDownLatch(1);

  public static void main(String[] args) {

    new Thread(() -> {
      System.out.println("线程名字：" + Thread.currentThread() + ", 当前 value = " + a.getReference());
      // 获取当前版本号
      int stamp = a.getStamp();

      // 计数器阻塞，直到计数器为0，才执行
      try {
        countDownLatch.await();
      } catch (InterruptedException e) {
        e.printStackTrace();
      }

      System.out.println("线程名字：" + Thread.currentThread() + ",CAS操作结果: " + a.compareAndSet(1, 2, stamp, stamp + 1));
    }, "线程1").start();

    // 线程2
    new Thread(() -> {
      // 将 value 值改成 2
      a.compareAndSet(1, 2, a.getStamp(), a.getStamp() + 1);
      System.out.println("线程名字" + Thread.currentThread() + "value = " + a.getReference());
      // 将 value 值又改成 1
      a.compareAndSet(2, 1, a.getStamp(), a.getStamp() + 1);
      System.out.println("线程名字" + Thread.currentThread() + "value = " + a.getReference());
      // 线程计数器
      countDownLatch.countDown();
    }, "线程2").start();

  }
}
```

这里我用 CountDownLatch 计数器实现线程先后执行顺序，线程2先执行完后，线程1才开始执行。

打印结果：

```java
线程名字：Thread[线程1,5,main], 当前 value = 1
线程名字Thread[线程2,5,main]value = 2
线程名字Thread[线程2,5,main]value = 1
线程名字：Thread[线程1,5,main],CAS操作结果: false
```





### 自旋问题：

从源码可以知道所说的自选无非就是操作结果失败后继续**循环操作**，这种操作也称之为**自旋锁**，是一种**乐观锁**机制，**一般来说都会给一个限定的自旋次数，防止进入死循环**。

自旋锁的优点是不需要休眠当前线程，因为自旋锁使用者**一般保持锁时间非常短**，因此选择自旋而不是休眠当前线程是提高并发性能的**关键点**，这是因为减少了很多不必要的线程上下文切换开销。

但是，如果 CAS 一直操作不成功，会造成**长时间原地自旋**，会给 CPU 带来非常大的执行开销。



## sun.misc.Unsafe类





# Atomic类

juc下的Atmic类都是线程安全的，他们的内部实现线程安全的方式是cas。



基本类型，使用原子的方式更新基本类型

- AtomicInteger：整形原子类
- AtomicLong：长整型原子类
- AtomicBoolean ：布尔型原子类

数组类型，使用原子的方式更新数组里的某个元素

- AtomicIntegerArray：整形数组原子类
- AtomicLongArray：长整形数组原子类
- AtomicReferenceArray ：引用类型数组原子类

引用类型

- AtomicReference：引用类型原子类
- AtomicStampedRerence：原子性更新引用类型里的字段原子类
- AtomicMarkableReference ：原子性更新带有标记位的引用类型

对象的属性修改类型

- AtomicIntegerFieldUpdater:原子性更新整形字段的更新器
- AtomicLongFieldUpdater：原子性更新长整形字段的更新器
- AtomicReferenceFieldUpdater ：原子性更新带有版本号的引用类型。该类将整数值与引用关联起来，可用于解决原子的更新数据和数据的版本号，可以解决使用 CAS 进行原子更新时可能出现的 ABA 问题。



## `AtomicIntegerArray`讲解

```java
public class AtomicIntegerArray implements java.io.Serializable {
    private static final long serialVersionUID = 2862133569453604235L;

    private static final Unsafe unsafe = Unsafe.getUnsafe();
    
    // arrayBaseOffset 获取数组首个元素地址偏移
    private static final int base = unsafe.arrayBaseOffset(int[].class);
    
    // shift就是数组元素的偏移量
    private static final int shift;
    
    private final int[] array;

    static {
        // scale数组元素的增量偏移量	int:4; long:8
        int scale = unsafe.arrayIndexScale(int[].class);
        // 对于int型数组，scale是4，用二进制&操作判断是否是2的倍数
        if ((scale & (scale - 1)) != 0)
            throw new Error("data type scale not a power of two");
        // 这里是处理int型的偏移量，shift是2       
        // 注：numberOfLeadingZeros(int): 给定一个int类型数据，返回这个数据的二进制串中从最左边算起连续的“0”的总数
        shift = 31 - Integer.numberOfLeadingZeros(scale);
    }
    
    //其余代码
}
```



# AQS

AQS(AbstractQueuedSynchronizer)是构建锁或者其他同步组件的基础框架，位于 `java.util.concurrent.locks` 下。

JUC(`java.util.concurrent`)里所有的锁机制都是基于AQS框架上构建的。

![](E:/notes/java/javaBasic/img/juc1.png)

首先通过上面我画的结构图（只是一个大致的框架，很多类并未列出），可以大致的了解到，JUC当中，锁、条件变量和一些并发工具类都围绕AQS进行构建。同时，线程池、阻塞队列等又依赖于锁和条件变量实现并发。所以说，<font color="#STCAIYUN">AQS是JUC并发包中的核心基础组件。</font>





# juc下的locks





# ThreadLocal

- 使用
- 原理/实现方式
- 存在的问题




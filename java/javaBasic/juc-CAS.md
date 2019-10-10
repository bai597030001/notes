[TOC]

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

  在 Java 中，`AtomicStampedReference` 也实现了这个处理方式。

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

private boolean casPair(Pair<V> cmp, Pair<V> val) {
       return UNSAFE.compareAndSwapObject(this, pairOffset, cmp, val);
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

- Unsafe类存在于`sun.misc`包中，其内部方法操作可以像C的指针一样直接操作内存，单从名称看来就可以知道该类是非安全的，毕竟Unsafe拥有着类似于C的指针操作。关于Unsafe类的主要功能点如下：

### 内存管理相关的api

```java
//分配内存指定大小的内存
public native long allocateMemory(long bytes);

//根据给定的内存地址address设置重新分配指定大小的内存
public native long reallocateMemory(long address, long bytes);

//用于释放allocateMemory和reallocateMemory申请的内存
public native void freeMemory(long address);

//将指定对象的给定offset偏移量内存块中的所有字节设置为固定值
public native void setMemory(Object o, long offset, long bytes, byte value);

//设置给定内存地址的值
public native void putAddress(long address, long x);

//获取指定内存地址的值
public native long getAddress(long address);

//设置给定内存地址的long值
public native void putLong(long address, long x);

//获取指定内存地址的long值
public native long getLong(long address);

//设置或获取指定内存的byte值
public native byte  getByte(long address);
public native void  putByte(long address, byte x);

//其他基本数据类型(long,char,float,double,short等)的操作与putByte及getByte相同

//操作系统的内存页大小
public native int pageSize();
```

### 提供实例对象新途径。

```java
//传入一个对象的class并创建该实例对象，但不会调用构造方法
public native Object allocateInstance(Class cls) throws InstantiationException;
```

### 类、实例对象、变量的操作

```java
//获取字段f在实例对象中的偏移量
public native long objectFieldOffset(Field f);

//静态属性的偏移量，用于在对应的Class对象中读写静态属性
public native long staticFieldOffset(Field f);

//返回值就是f.getDeclaringClass()
public native Object staticFieldBase(Field f);


//获得给定对象偏移量上的int值，所谓的偏移量可以简单理解为指针指向该变量的内存地址，
//通过偏移量便可得到该对象的变量，进行各种操作
public native int getInt(Object o, long offset);

//设置给定对象上偏移量的int值
public native void putInt(Object o, long offset, int x);

//获得给定对象偏移量上的引用类型的值
public native Object getObject(Object o, long offset);

//设置给定对象偏移量上的引用类型的值
public native void putObject(Object o, long offset, Object x);

//其他基本数据类型(long,char,byte,float,double)的操作与getInthe及putInt相同

//设置给定对象的int值，使用volatile语义，即设置后立马更新到内存对其他线程可见
public native void  putIntVolatile(Object o, long offset, int x);

//获得给定对象的指定偏移量offset的int值，使用volatile语义，总能获取到最新的int值。
public native int getIntVolatile(Object o, long offset);

//其他基本数据类型(long,char,byte,float,double)的操作与putIntVolatile及getIntVolatile相同，引用类型putObjectVolatile也一样。

//与putIntVolatile一样，但要求被操作字段必须有volatile修饰
public native void putOrderedInt(Object o,long offset,int x);
```

### 数组操作

```java
//获取数组第一个元素的偏移地址
public native int arrayBaseOffset(Class arrayClass);

//数组中一个元素占据的内存空间,arrayBaseOffset与arrayIndexScale配合使用，可定位数组中每个元素在内存中的位置
public native int arrayIndexScale(Class arrayClass);
```

### CAS 操作相关

- CAS是一些CPU直接支持的指令，也就是我们前面分析的无锁操作，**在Java中无锁操作CAS基于以下3个方法实现**

```java
//第一个参数o为给定对象，offset为对象内存的偏移量，通过这个偏移量迅速定位字段并设置或获取该字段的值，
//expected表示期望值，x表示要设置的值，下面3个方法都通过CAS原子指令执行操作。
public final native boolean compareAndSwapObject(Object o, long offset,Object expected, Object x);                                                                                                  

public final native boolean compareAndSwapInt(Object o, long offset,int expected,int x);

public final native boolean compareAndSwapLong(Object o, long offset,long expected,long x);
```

### Unsafe类中JDK 1.8新增的几个方法

```java
 //1.8新增，给定对象o，根据获取内存偏移量指向的字段，将其增加delta，
 //这是一个CAS操作过程，直到设置成功方能退出循环，返回旧值
 public final int getAndAddInt(Object o, long offset, int delta) {
     int v;
     do {
         //获取内存中最新值
         v = getIntVolatile(o, offset);
       //通过CAS操作
     } while (!compareAndSwapInt(o, offset, v, v + delta));
     return v;
 }

//1.8新增，方法作用同上，只不过这里操作的long类型数据
 public final long getAndAddLong(Object o, long offset, long delta) {
     long v;
     do {
         v = getLongVolatile(o, offset);
     } while (!compareAndSwapLong(o, offset, v, v + delta));
     return v;
 }

 //1.8新增，给定对象o，根据获取内存偏移量对于字段，将其 设置为新值newValue，
 //这是一个CAS操作过程，直到设置成功方能退出循环，返回旧值
 public final int getAndSetInt(Object o, long offset, int newValue) {
     int v;
     do {
         v = getIntVolatile(o, offset);
     } while (!compareAndSwapInt(o, offset, v, newValue));
     return v;
 }

// 1.8新增，同上，操作的是long类型
 public final long getAndSetLong(Object o, long offset, long newValue) {
     long v;
     do {
         v = getLongVolatile(o, offset);
     } while (!compareAndSwapLong(o, offset, v, newValue));
     return v;
 }

 //1.8新增，同上，操作的是引用类型数据
 public final Object getAndSetObject(Object o, long offset, Object newValue) {
     Object v;
     do {
         v = getObjectVolatile(o, offset);
     } while (!compareAndSwapObject(o, offset, v, newValue));
     return v;
 }
```

### 挂起与恢复

- 将一个线程进行挂起是通过park方法实现的，调用 **park**后，线程将一直阻塞直到超时或者中断等条件出现。**unpark**可以终止一个挂起的线程，使其恢复正常。Java对线程的挂起操作被封装在 **LockSupport**类中，LockSupport类中有各种版本pack方法，其底层实现最终还是使用**Unsafe.park()方法和Unsafe.unpark()**方法

```java
//线程调用该方法，线程将一直阻塞直到超时，或者是中断条件出现。  
public native void park(boolean isAbsolute, long time);  

//终止挂起的线程，恢复正常.java.util.concurrent包中挂起操作都是在LockSupport类实现的，其底层正是使用这两个方法，  
public native void unpark(Object thread); 
```

### 内存屏障

- 这里主要包括了loadFence、storeFence、fullFence等方法，这些方法是在Java 8新引入的，用于定义内存屏障，**避免代码重排序**，与Java内存模型相关

```java
//在该方法之前的所有读操作，一定在load屏障之前执行完成
public native void loadFence();

//在该方法之前的所有写操作，一定在store屏障之前执行完成
public native void storeFence();

//在该方法之前的所有读写操作，一定在full屏障之前执行完成，这个内存屏障相当于上面两个的合体功能
public native void fullFence();
```

### 其他操作

```java
//获取本机内存的页数，这个值永远都是2的幂次方  
public native int pageSize();  

//告诉虚拟机定义了一个没有安全检查的类，默认情况下这个类加载器和保护域来着调用者类  
public native Class defineClass(String name, byte[] b, int off, int len, ClassLoader loader, ProtectionDomain protectionDomain);  

//加载一个匿名类
public native Class defineAnonymousClass(Class hostClass, byte[] data, Object[] cpPatches);
//判断是否需要加载一个类
public native boolean shouldBeInitialized(Class<?> c);
//确保类一定被加载 
public native  void ensureClassInitialized(Class<?> c)
```



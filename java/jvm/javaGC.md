# 新生代、老年代、永久代、元空间

- Java 中的堆是 JVM 所管理的最大的一块内存空间，主要用于存放各种类的实例对象，如下图所示：

![](img/jvm_gc1.png)

- 在 Java 中，堆被划分成两个不同的区域：新生代 ( Young )、老年代 ( Old)。新生代 ( Young ) 又被划分为三个区域：Eden、S0、S1。 **这样划分的目的是为了使 JVM 能够更好的管理堆内存中的对象，包括内存的分配以及回收。**

## 新生代

- 年轻代用来存放新近创建的对象，尺寸随堆大小的增大和减小而相应的变化，默认值是保持为堆大小的1/15，可以通过 `-Xmn` 参数设置年轻代为固定大小，也可以通过 `-XX:NewRatio` 来设置年轻代与年老代的大小比例，即： Eden = 8/10 的新生代空间大小，from = to = 1/10 的新生代空间大小。年青代的特点是对象更新速度快，在短时间内产生大量的“死亡对象”。

- 由于年轻代的特点是产生大量的死亡对象,并且要是产生连续可用的空间, 所以使用**复制清除算法**和**并行收集器**进行垃圾回收.对年轻代的垃圾回收称作初级回收 (`minor gc`)。

- 初级回收将年轻代分为三个区域, 一个新生代 , 2个大小相同的复活代, 应用程序只能使用一个新生代和一个复活代, 当发生初级垃圾回收的时候,gc挂起程序, 然后将新生代和复活代中的存活对象复制到另外一个非活动的复活代中,然后一次性清除新生代和复活代，将原来的非复活代标记成为活动复活代。将在指定次数回收后仍然存在的对象移动到老年代中，初级回收后，得到一个空的可用的新生代。

- 新生代几乎是所有 Java 对象出生的地方，即 Java 对象申请的内存以及存放都是在这个地方。Java 中的大部分对象通常不需长久存活，具有朝生夕灭的性质。 当一个对象被判定为 “死亡” 的时候，GC 就有责任来回收掉这部分对象的内存空间。新生代是 GC 收集垃圾的频繁区域。 当对象在 Eden 出生后，在经过一次 Minor GC 后，如果对象还存活，并且能够被另外一块 Survivor 区域所容纳，则使用复制算法将这些仍然还存活的对象复制到另外一块 Survivor 区域中，然后清理所使用过的 Eden 以及 Survivor 区域，并且将这些对象的年龄设置为1，以后对象在 Survivor 区每熬过一次 Minor GC，就将对象的年龄 + 1，当对象的年龄达到某个值时 ( 默认是 15 岁，可以通过参数 -XX:MaxTenuringThreshold 来设定 )，这些对象就会成为老年代。 但这也不是一定的，对于一些较大的对象 ( 即需要分配一块较大的连续内存空间 ) 则是直接进入到老年代。

## 老年代

- Full GC 是发生在老年代的垃圾收集动作，所采用的是**标记-清除算法**。

- 现实的生活中，老年代的人通常会比新生代的人 “早死”。堆内存中的老年代(Old)不同于这个，老年代里面的对象几乎个个都是在 Survivor 区域中熬过来的，它们是不会那么容易就 “死掉” 了的。因此，Full GC 发生的次数不会有 Minor GC 那么频繁，并且做一次 Full GC 要比进行一次 Minor GC 的时间更长。 另外，标记-清除算法收集垃圾的时候会产生许多的内存碎片 ( 即不连续的内存空间 )，此后需要为较大的对象分配内存空间时，若无法找到足够的连续的内存空间，就会提前触发一次 GC 的收集动作。

## 永久代

- 永久代是Hotspot虚拟机特有的概念，是方法区的一种实现，别的JVM都没有这个东西。在Java 8中，永久代被彻底移除，取而代之的是另一块与堆不相连的本地内存——元空间。 
  永久代或者“Perm Gen”包含了JVM需要的应用元数据，这些元数据描述了在应用里使用的类和方法。注意，永久代不是Java堆内存的一部分。永久代存放JVM运行时使用的类。永久代同样包含了Java SE库的类和方法。永久代的对象在full GC时进行垃圾收集。

# GC日志

设置 JVM 参数为 -XX:+PrintGCDetails，使得控制台能够显示 GC 相关的日志信息，执行上面代码，下面是其中一次执行的结果。

![](img/jvm_gc2.png)

![](img/jvm_gc3.png)

Full GC 信息与 Minor GC 的信息是相似的，这里就不一个一个的画出来了。从 Full GC 信息可知，新生代可用的内存大小约为 18M，则新生代实际分配得到的内存空间约为 20M(为什么是 20M? 请继续看下面…)。老年代分得的内存大小约为 42M，堆的可用内存的大小约为 60M。可以计算出： 18432K ( 新生代可用空间 ) + 42112K ( 老年代空间 ) = 60544K ( 堆的可用空间 )新生代约占堆大小的 1/3，老年代约占堆大小的 2/3。也可以看出，GC 对新生代的回收比较乐观，而对老年代以及方法区的回收并不明显或者说不及新生代。并且在这里 Full GC 耗时是 Minor GC 的 22.89 倍。

# JVM参数选项

jvm 可配置的参数选项可以参考 Oracle 官方网站给出的相关信息：

http://www.oracle.com/technetwork/java/javase/tech/vmoptions-jsp-140102.html

下面只列举其中的几个常用和容易掌握的配置选项

![](img/jvm_gc4.png)

# 实例分析

```java
/** 
  -Xms60m 
  -Xmx60m 
  -Xmn20m 
  -XX:NewRatio=2 ( 若 Xms = Xmx, 并且设定了 Xmn, 那么该项配置就不需要配置了 ) 
  -XX:SurvivorRatio=8 
  -XX:PermSize=30m 
  -XX:MaxPermSize=30m 
  -XX:+PrintGCDetails
  */
public class TestVm{
    public void doTest(){
         Integer M = new Integer(1024 * 1024 * 1);  //单位, 兆(M) 
            byte[] bytes = new byte[1 * M]; //申请 1M 大小的内存空间 
            bytes = null;  //断开引用链
            System.gc();   //通知 GC 收集垃圾                       
            System.out.println();       //                       
            bytes = new byte[1 * M];  //重新申请 1M 大小的内存空间   
            bytes = new byte[1 * M];  //再次申请 1M 大小的内存空间   
            System.gc();
            System.out.println();                                 
        }
    public static void main(String[] args) {
         new TestVm().doTest();
     }
}
/**
  
  -Xms60m -Xmx60m -Xmn20m -XX:NewRatio=2 -XX:SurvivorRatio=8 -XX:PermSize=30m -XX:MaxPermSize=30m -XX:+PrintGCDetails
  
  java -Xms60m -Xmx60m -Xmn20m -XX:NewRatio=2 -XX:SurvivorRatio=8 -XX:+PrintGCDetails GCTest
  
**/
```

按上面代码中注释的信息设定 jvm 相关的参数项，并执行程序，下面是一次执行完成控制台打印的结果：

```powershell
[ GC [ PSYoungGen:  1351K -> 288K (18432K) ]  1351K -> 288K (59392K), 0.0012389 secs ]  [ Times: user=0.00 sys=0.00, real=0.00 secs ] 
[ Full GC (System)  [ PSYoungGen:  288K -> 0K (18432K) ]  [ PSOldGen:  0K -> 160K (40960K) ]  288K -> 160K (59392K)  [ PSPermGen:  2942K -> 2942K (30720K) ],  0.0057649 secs ] [ Times:  user=0.00  sys=0.00,  real=0.01 secs ] 

[ GC [ PSYoungGen:  2703K -> 1056K (18432K) ]  2863K -> 1216K(59392K),  0.0008206 secs ]  [ Times: user=0.00 sys=0.00, real=0.00 secs ] 
[ Full GC (System)  [ PSYoungGen:  1056K -> 0K (18432K) ]  [ PSOldGen:  160K -> 1184K (40960K) ]  1216K -> 1184K (59392K)  [ PSPermGen:  2951K -> 2951K (30720K) ], 0.0052445 secs ]  [ Times: user=0.02 sys=0.00, real=0.01 secs ] 

Heap 
PSYoungGen      
      total 18432K, used 327K [0x00000000fec00000, 0x0000000100000000, 0x0000000100000000)  
      eden  space 16384K, 2% used [0x00000000fec00000,0x00000000fec51f58,0x00000000ffc00000)   
      from space 2048K, 0% used [0x00000000ffe00000,0x00000000ffe00000,0x0000000100000000)  
      to   space 2048K, 0% used [0x00000000ffc00000,0x00000000ffc00000,0x00000000ffe00000) 

PSOldGen        
    total 40960K, used 1184K [0x00000000fc400000, 0x00000000fec00000, 0x00000000fec00000)  

 PSPermGen       
     total 30720K, used 2959K [0x00000000fa600000, 0x00000000fc400000, 0x00000000fc400000)  
     object space 30720K, 9% used [0x00000000fa600000,0x00000000fa8e3ce0,0x00000000fc400000)
从打印结果可以看出，堆中新生代的内存空间为 18432K ( 约 18M )，eden 的内存空间为 16384K ( 约 16M)，from / to survivor 的内存空间为 2048K ( 约 2M)。
```

这里所配置的 Xmn 为 20M，也就是指定了新生代的内存空间为 20M，可是从打印的堆信息来看，新生代怎么就只有 18M 呢? 另外的 2M 哪里去了? 别急，是这样的。新生代 = eden + from + to = 16 + 2 + 2 = 20M，可见新生代的内存空间确实是按 Xmn 参数分配得到的。而且这里指定了 SurvivorRatio = 8，因此，eden = 8/10 的新生代空间 = 8/10 * 20 = 16M。from = to = 1/10 的新生代空间 = 1/10 * 20 = 2M。堆信息中新生代的 total 18432K 是这样来的： eden + 1 个 survivor = 16384K + 2048K = 18432K，即约为 18M。因为 jvm 每次只是用新生代中的 eden 和 一个 survivor，因此新生代实际的可用内存空间大小为所指定的 90%。因此可以知道，这里新生代的内存空间指的是新生代可用的总的内存空间，而不是指整个新生代的空间大小。另外，可以看出老年代的内存空间为 40960K ( 约 40M )，堆大小 = 新生代 + 老年代。因此在这里，老年代 = 堆大小 - 新生代 = 60 - 20 = 40M。
最后，这里还指定了 PermSize = 30m，PermGen 即永久代 ( 方法区 )，它还有一个名字，叫非堆，主要用来存储由 jvm 加载的类文件信息、常量、静态变量等。

回到 doTest() 方法中，可以看到代码在第 14、18、19 这三行中分别申请了一块 1M 大小的内存空间，并在 16 和 20 这两行中分别显式的调用了 System.gc()。从控制台打印的信息来看，每次调 System.gc()，是先进行 Minor GC，然后再进行 Full GC。
第 16 行触发的 Minor GC 收集分析：
从信息 PSYoungGen : 1351K -> 288K，可以知道，在第 14 行为 bytes 分配的内存空间已经被回收完成。引起 GC 回收这 1M 内存空间的因素是第 15 行的 bytes = null; bytes 为 null 表明之前申请的那 1M 大小的内存空间现在已经没有任何引用变量在使用它了，并且在内存中它处于一种不可到达状态 ( 即没有任何引用链与 GC Roots 相连 )。那么，当 Minor GC 发生的时候，GC 就会来回收掉这部分的内存空间。
第 16行触发的 Full GC 收集分析：
在 Minor GC 的时候，信息显示 PSYoungGen : 1351K -> 288K，再看看 Full GC 中显示的 PSYoungGen : 288K -> 0K，可以看出，Full GC 后，新生代的内存使用变成0K 了，那么这 288K 到底哪去了 ? 难道都被 GC 当成垃圾回收掉了 ? 当然不是了。我还特意在 main 方法中 new 了一个 Test 类的实例，这里的 Test 类的实例属于小对象，它应该被分配到新生代内存当中，现在还在调用这个实例的 doTest 方法呢，GC 不可能在这个时候来回收它的。

接着往下看 Full GC 的信息，会发现一个很有趣的现象，PSOldGen: 0K -> 160K，可以看到，Full GC 后，老年代的内存使用从 0K 变成了 160K，想必你已经猜到大概是怎么回事了。当 Full GC 进行的时候，默认的方式是尽量清空新生代 ( YoungGen )，因此在调 System.gc() 时，新生代 ( YoungGen ) 中存活的对象会提前进入老年代。

第 20行触发的 Minor GC 收集分析：
从信息 PSYoungGen : 2703K -> 1056K，可以知道，在第 18行创建的，大小为 1M 的数组被 GC 回收了。在第 19 行创建的，大小也为 1M 的数组由于 bytes 引用变量还在引用它，因此，它暂时未被 GC 回收。

第 20 行触发的 Full GC 收集分析：
在 Minor GC 的时候，信息显示 PSYoungGen : 2703K -> 1056K，Full GC 中显示的 PSYoungGen : 1056K -> 0K，以及 PSOldGen: 160K -> 1184K，可以知道，新生代 ( YoungGen ) 中存活的对象又提前进入老年代了。

# jvm的gc算法

## 新生代回收：（复制回收算法）

在堆中，新生代主要存放的是哪些很快就会被GC回收掉的或者不是特别大的对象（是否设置了-XX：PretenureSizeThreshold 参数）。复制算法的新生代分为3个区：较大的Eden和两个较小的Survivor（默认的Eden：Survivor = 8:1）。发生在新生代的GC为Minor GC 。在Minor GC时会将新生代中还存活着的对象复制进一个Survivor中，然后对Eden和另一个Survivor进行清理。所以，平常可用的新生代大小为Eden的大小+一个Survivor的大小。


## 老年代回收：（标记-清除算法/标记-整理算法）

老年代则是存放那些在程序中经历了好几次回收仍然还活着或者特别大的对象（是否设置了-XX：PretenureSizeThreshold 参数）。老年代采用的是标记-清除或者标记-整理算法，这两个算法主要看虚拟机采用的哪个收集器，两种算法的区别是：标记-清除可能会产生大量连续的内存碎片。在老年代中的GC则为Major GC。Major GC和Full GC会造成stop-the-world。

标记：（一致）遍历GC Roots，将存活的对象标记

整理：移动所有存活对象，按照内存地址次序依次排列，将末端内存地址以后的内存全部回收

新生代进入老年代：

1.分配担保机制：当Minor GC时，新生代存活的对象大于Survivor的大小时，这时一个Survivor装不下它们，那么它们就会进入老年代。

2.如果设置了-XX：PretenureSizeThreshold3M 那么大于3M的对象就会直接就进入老年代。

3.在新生代的每一次Minor GC 都会给在新生代中的对象+1岁，默认到15岁时就会从新生代进入老年代，可以通过-XX：MaxTenuringThreshold来设置这个临界点。

相比较而言，在老年代中的对象比新生代中的对象不易回收许多。


## 永久代回收：（即方法区回收）

JVM的方法区，也被称为永久代。在这里都是放着一些被虚拟机加载的类信息，静态变量，常量等数据。这个区中的东西比老年代和新生代更不容易回收。

# 7种垃圾收集器以及使用

[链接](https://crowhawk.github.io/2017/08/15/jvm_3/)

[链接](https://juejin.im/post/5b651200f265da0fa00a38d7)

[链接](https://www.ibm.com/developerworks/cn/java/j-lo-JVMGarbageCollection/index.html)


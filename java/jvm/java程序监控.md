# java程序监控

- java 程序运行起来以后，如何查看其CPU，堆栈，线程数，GC等信息



## java visualVM

- jvisialVM是jconsule的加强版本
- jvisual的使用需要两个步骤（监控远程主机的java程序）
  - 1.远程主机开启jstatd
  - 2.被监控java程序启动时需要加上JMX的连接参数信息



### jstatd

- `jstatd`是一个RMI（Remove Method Invocation）的server应用，用于监控jvm的创建和结束，并且提供接口让监控工具（如VisualVM）可以远程连接到本机的jvms 。注意是jvms，就是说运行jstatd命令后可以用监控工具监控本用户（运行jstatd命令的用户）所有已经启动的java程序。

- 启动方式

  - 1.在服务器上新建文件，命名为jstatd.all.policy内容为：

    ```properties
    grant codebase "file:${java.home}/../lib/tools.jar" {  
       permission java.security.AllPermission;  
    };
    ```

  - 2.在远程主机上启动 jstatd 并且不要关闭。

    ```bash
    jstatd -J-Djava.security.policy=jstatd.all.policy
    ```

    想指定端口可以用下面命令：

    ```shell
    jstatd -J-Djava.security.policy=jstatd.policy -p 1099
    ```

  - 3.VisualVM 中连接

    在客户端用visualvm连接部署jstatd的服务器ip：`jvisualVM [--console new]`

    显示如下：

    ![](img/jvisualVM1.png)



### JMX连接

- jstatd的远程监控，CPU的监控我这里看不到，这时候就要用到 JMX的连接了。

JMX 配置

远程机器的**被监测程序启动时**需要加上JVM参数

```shell
-Dcom.sun.management.jmxremote=true \
-Dcom.sun.management.jmxremote.port=9090 \
-Dcom.sun.management.jmxremote.ssl=false \
-Dcom.sun.management.jmxremote.authenticate=false
```



备注：另外需要检查 `hostname –i`，看解析出来是否为本地的IP，如是127.0.0.1或者IP为多个IP中之一，则其他的IP无效，会连接不上。

也可以使用如下的参数手动指定 hostname 或者 ip 地址

```shell
-Djava.rmi.server.hostname=192.168.0.1
```



启动命令示例：

```shell
$ java -cp . \
-Djava.rmi.server.hostname=172.16.1.127 \
-Dcom.sun.management.jmxremote=true \
-Dcom.sun.management.jmxremote.port=9090 \
-Dcom.sun.management.jmxremote.ssl=false \
-Dcom.sun.management.jmxremote.authenticate=false \
-Xms90m -Xmx90m -Xmn30m -XX:NewRatio=2 -XX:SurvivorRatio=8 -XX:+PrintGCDetails \
-Dcom.sun.management.jmxremote=true -Dcom.sun.management.jmxremote.port=9090 GCTimeTest

```



然后就可以使用菜单中的 “`Add JMX Connection`”，连接远程地址。

```
以 tomcat 为例， 我们需要修改 catalina.sh 文件。
在下面这行上面添加文字。
```

1. \# ----- Execute The Requested Command ----------------------------------------- 

```
需要添加的文字：
```

JAVA_OPTS="-Djava.rmi.server.hostname=10.12.49.64 
-Dcom.sun.management.jmxremote 
-Dcom.sun.management.jmxremote.port=8081 
-Dcom.sun.management.jmxremote.ssl=false 
-Dcom.sun.management.jmxremote.authenticate=false $JAVA_OPTS" 
export JAVA_OPTS 

注：第一行的ip为tomcat所在服务器的ip；第三行的端口为jmx使用的端口，确保此端口是未被占用的；（不要少了第一行，我一开始配置的时候就是没有第一行，jmx一直连接不上）

```
启动tomcat，netstat -aux 看看8080,8081端口是否被监听，如果被监听，说明配置成功了，然后在visualvm的远程选择ip，右键，添加“jmx连接”，成功连接就成功了
```



## jconsole

- jconsole配合jvisualvm一起使用（jconsole可以查看cpu占用情况）
- jconsole可以查看java内存的具体占用（新生代，老年代，永久代）



**使用方式**

- 被监测程序启动时需要指定jmx

```shell
$ java -cp . \
-Djava.rmi.server.hostname=172.16.1.127 \
-Dcom.sun.management.jmxremote=true \
-Dcom.sun.management.jmxremote.port=9090 \
-Dcom.sun.management.jmxremote.ssl=false \
-Dcom.sun.management.jmxremote.authenticate=false \
-Xms90m -Xmx90m -Xmn30m -XX:NewRatio=2 -XX:SurvivorRatio=8 -XX:+PrintGCDetails \
-Dcom.sun.management.jmxremote=true -Dcom.sun.management.jmxremote.port=9090 GCTimeTest
```

- 命令行启动或连接

```shell
$ jconsole
$ jconsole 172.16.1.127:9090
```



## jstack

 jstack 命令可以**获取当前进程的所有线程信息**。每个线程堆中信息中，都可以查看到 线程ID、线程的状态（wait、sleep、running 等状态）、是否持有锁信息等。

jstack用于生成java虚拟机当前时刻的线程快照。线程快照是当前java虚拟机内每一条线程正在执行的方法堆栈的集合，生成线程快照的主要目的是定位线程出现长时间停顿的原因，如线程间死锁、死循环、请求外部资源导致的长时间等待等。 线程出现停顿的时候通过jstack来查看各个线程的调用堆栈，就可以知道没有响应的线程到底在后台做什么事情，或者等待什么资源。 如果java程序崩溃生成core文件，jstack工具可以用来获得core文件的java stack和native stack的信息，从而可以轻松地**知道java程序是如何崩溃和在程序何处发生问题**。另外，jstack工具还可以附属到正在运行的java程序中，看到当时运行的java程序的java stack和native stack的信息, 如果现在运行的java程序呈现hung的状态，jstack是非常有用的。



- jstack 查看线程具体在做什么，可看出哪些线程在长时间占用CPU，尽快定位问题和解决问题

```shell
1.top查找出哪个进程消耗的cpu高。执行top命令，默认是进程视图，其中PID是进程号
co_ad2    18   0 1817m 776m 9712 S  3.3  4.9  12:03.24 java                                                                                           
co_ad     21   0 3028m 2.5g 9432 S  1.0 16.3   6629:44 ja


这里我们分析21125这个java进程
2.top中shift+h 或“H”查找出哪个线程消耗的cpu高 
先输入top，然后再按shift+h 或“H”，此时打开的是线程视图，pid为线程号
co_ad2    15   0 1807m 630m 9492 S  1.3  4.0   0:05.12 java                                                                                           
co_ad2_s  15   0 1360m 560m 9176 S  0.3  3.6   0:46.72 java                                                                                           

这里我们分析21233这个线程，并且注意的是，这个线程是属于21125这个进程的。 

3.使用jstack命令输出这一时刻的线程栈，保存到文件，命名为jstack.log。注意：输出线程栈和保存top命令快照尽量同时进行。
  由于jstack.log文件记录的线程ID是16进制，需要将top命令展示的线程号转换为16进制。

4. jstack查找这个线程的信息 
jstack [进程]|grep -A 10 [线程的16进制] 
即： jstack 21125|grep -A 10 52f1  

-A 10表示查找到所在行的后10行。21233用计算器转换为16进制52f1，注意字母是小写。 
结果： 
 
"http-8081-11" daemon prio=10 tid=0x00002aab049a1800 nid=0x52bb in Object.wait() [0x0000000042c75000]  
   java.lang.Thread.State: WAITING (on object monitor)  
     at java.lang.Object.wait(Native Method)  
     at java.lang.Object.wait(Object.java:485)  
     at org.apache.tomcat.util.net.JIoEndpoint$Worker.await(JIoEndpoint.java:416)  

在结果中查找52f1，可看到当前线程在做什么。
```



## jstat

## jmap 

### 获取内存在某一时刻的快照

​        命令：jmap -dump:format=b,file=heap.bin <pid>

​        file：保存路径及文件名

​        pid：进程编号（windows通过任务管理器查看，linux通过ps aux查看）

​        dump文件可以通过MemoryAnalyzer分析查看，网址：http://www.eclipse.org/mat/，可以查看dump时对象数量，内存占用，线程情况等。

### 观察运行中的jvm物理内存的占用情况

观察运行中的jvm物理内存的占用情况，我们也可以用jmap命令。

​        参数如下：

-heap：打印jvm heap的情况

-histo：打印jvm heap的直方图。其输出信息包括类名，对象数量，对象占用大小。

-histo：live ：同上，但是只答应存活对象的情况

-permstat：打印permanent generation heap情况

​        命令使用：

​        jmap -heap 2083

​        可以观察到New Generation（Eden Space，From Space，To Space）,tenured generation,Perm Generation的内存使用情况

![](img/jmap1.jpg)

上图为tomcat应用出错前JVM的配置信息，可以明确的看到当时的信息：

​        MaxHeapSize堆内存大小为：3500M

​        MaxNewSize新生代内存大小：512M

​        PermSize永久代内存大小:192M

​        NewRatio设置年轻代（包括Eden和两个Survivor区）与年老代的比值（除去持久代）。设置为2，则年轻代与年老代所占比值为1：2，年轻代占整个堆栈的1/3。

​        SurvivorRatio设置年轻代中Eden区与Survivor区的大小比值。设置为8，则两个Survivor区与一个Eden区的比值为2:8，一个Survivor区占整个年轻代的1/10。

​        在New Generation中，有一个叫Eden的空间，主要是用来存放新生的对象，还有两个Survivor Spaces（from,to）, 它们用来存放每次垃圾回收后存活下来的对象。在Old Generation中，主要存放应用程序中生命周期长的内存对象，还有个Permanent Generation，主要用来放JVM自己的反射对象，比如类对象和方法对象等。

​        从上面的图可以看出来JVM的新生代设置太小，可以看出应用的新生代区完全占满了，无法再往新生代区增加新的对象此时的这些对象都处于活跃状态，所以不会被GC处理，但是tomcat应用还在继续产生新的对象，这样就会导致OOM的发生，这就是导致tomcat假死的原因。

## jhat

jhat(JVM Heap Analysis Tool)命令是与jmap搭配使用，用来分析jmap生成的dump，jhat内置了一个微型的HTTP/HTML服务器，生成dump的分析结果后， 可以在浏览器中查看。在此要注意，一般不会直接在服务器上进行分析，因为jhat是一个耗时并且耗费硬件资源的过程，一般把服务器生成的dump文件复制 到本地或其他机器上进行分析。

## jinfo

## jcmd

`jcmd`命令（[帮助文档](https://docs.oracle.com/en/java/javase/11/tools/jcmd.html#GUID-59153599-875E-447D-8D98-0078A5778F05)）可以向运行中的Java虚拟机(JVM)发送诊断命令。

它的命令格式如下：

```
jcmd <pid | main class> <command ... | PerfCounter.print | -f  file>
jcmd -l
jcmd -h
```
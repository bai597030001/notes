https://www.cnblogs.com/xiaoxi/p/6089984.html

https://blog.csdn.net/u010002184/article/details/74892663

同步容器类：使用了synchronized
1.Vector
2.HashTable

并发容器：
3.ConcurrentHashMap:分段
4.CopyOnWriteArrayList：写时复制
5.CopyOnWriteArraySet：写时复制

Queue:
6.ConcurrentLinkedQueue：是使用非阻塞的方式实现的基于链接节点的无界的线程安全队列，性能非常好。
（java.util.concurrent.BlockingQueue 接口代表了线程安全的队列。）
7.ArrayBlockingQueue：基于数组的有界阻塞队列
8.LinkedBlockingQueue：基于链表的有界阻塞队列。
9.PriorityBlockingQueue：支持优先级的无界阻塞队列，即该阻塞队列中的元素可自动排序。默认情况下，元素采取自然升序排列
10.DelayQueue：一种延时获取元素的无界阻塞队列。
11.SynchronousQueue：不存储元素的阻塞队列。每个put操作必须等待一个take操作，否则不能继续添加元素。内部其实没有任何一个元素，容量是0

Deque:
(Deque接口定义了双向队列。双向队列允许在队列头和尾部进行入队出队操作。)
12.ArrayDeque:基于数组的双向非阻塞队列。
13.LinkedBlockingDeque:基于链表的双向阻塞队列。

Sorted容器：
14.ConcurrentSkipListMap：是TreeMap的线程安全版本
15.ConcurrentSkipListSet：是TreeSet的线程安全版本

---

# java之Set接口详解

---
## Set接口
> Set不允许包含相同的元素，如果试图把两个相同元素加入同一个集合中，add方法返回false。  
> Set判断两个对象相同不是使用==运算符，而是根据equals方法。也就是说，只要两个对象用equals方法比较返回true，Set就不会接受这两个对象。  
> HashSet与TreeSet都是基于Set接口的实现类。其中TreeSet是Set的子接口SortedSet的实现类。

			 | SortedSet接口——TreeSet实现类 
	
	Set接口--|——HashSet实现类
	
			 |——LinkedHashSet实现类

### HashSet

- HashSet有以下特点:

>1.不能保证元素的排列顺序，顺序有可能发生变化  
2.不是同步的  
3.集合元素可以是null,但只能放入一个null  
4.当向HashSet结合中存入一个元素时，HashSet会调用该对象的hashCode()方法来得到该对象的hashCode值，然后根据 hashCode值来决定该对象在HashSet中存储位置。

**HashSet判断两个元素相等的标准是两个对象通过equals方法比较相等，并且两个对象的hashCode()方法返回值相等**

- 注意：  
如果要把一个对象放入HashSet中，重写该对象对应类的equals方法，也应该重写其hashCode()方法。  
其规则是如果两个对象通过equals方法比较返回true时，其hashCode也应该相同。  
另外，对象中用作equals比较标准的属性，都应该用来计算 hashCode的值。

### TreeSet

	TreeSet类型是J2SE中唯一可实现自动排序的类型

- TreeSet是SortedSet接口的唯一实现类，TreeSet可以确保集合元素处于排序状态。

- TreeSet支持两种排序方式，自然排序 和定制排序，其中自然排序为默认的排序方式。  

- 排序规则

> 自然排序：

	自然排序使用要排序元素的CompareTo（Object obj）方法来比较元素之间大小关系，然后将元素按照升序排列。
	
	Java提供了一个Comparable接口，该接口里定义了一个compareTo(Object obj)方法，该方法返回一个整数值，实现了该接口的对象就可以比较大小。
	
	obj1.compareTo(obj2)方法如果返回0，则说明被比较的两个对象相等，如果返回一个正数，则表明obj1大于obj2，如果是 负数，则表明obj1小于obj2。
	
	如果我们将两个对象的equals方法总是返回true，则这两个对象的compareTo方法返回应该返回0

> 定制排序：

	自然排序是根据集合元素的大小，以升序排列，如果要定制排序，应该使用Comparator接口，实现 int compare(To1,To2)方法

- 比较规则  
	
	**TreeSet判断两个对象不相等的方式是两个对象通过equals方法返回false，或者通过CompareTo方法比较没有返回0**

### LinkedHashSet

- LinkedHashSet集合同样是根据元素的hashCode值来决定元素的存储位置。但是它同时使用链表维护元素的次序。这样使得元素看起 来像是以插入顺序保存的。当遍历该集合时候，LinkedHashSet将会以元素的添加顺序访问集合的元素。

- **LinkedHashSet在迭代访问Set中的全部元素时，性能比HashSet好，但是插入时性能稍微逊色于HashSet**。


### java的HashCode方法

	1、如果两个对象相同，那么它们的hashCode值一定要相同；
	
	2、如果两个对象的hashCode相同，它们并不一定相同    

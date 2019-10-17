[https://segmentfault.com/a/1190000015558984](https://segmentfault.com/a/1190000015558984)



# juc-collections 集合框架

这里的juc-collections集合框架，是指`java.util.concurrent`包下的一些同步集合类，按类型划分可以分为：**符号表**、**队列**、**Set集合**、**列表**四大类，每个类都有自己适合的使用场景，整个juc-collections集合框架的结构如下图：
![](E:/notes/java/javaBasic/img/juc-collections1.png)



阻塞队列：

| 队列特性 | 有界队列           | 近似无界队列                             | 无界队列            | 特殊队列                          |
| :------- | :----------------- | :--------------------------------------- | :------------------ | :-------------------------------- |
| 有锁算法 | ArrayBlockingQueue | LinkedBlockingQueue、LinkedBlockingDeque | /                   | PriorityBlockingQueue、DelayQueue |
| 无锁算法 | /                  | /                                        | LinkedTransferQueue | SynchronousQueue                  |



## 队列

队列分类有 阻塞队列，延迟队列，优先队列，双端队列，有界队列，无界队列，同步队列



顶层接口

```java
/* @see java.util.Collection
 * @see LinkedList
 * @see PriorityQueue
 * @see java.util.concurrent.LinkedBlockingQueue
 * @see java.util.concurrent.BlockingQueue
 * @see java.util.concurrent.ArrayBlockingQueue
 * @see java.util.concurrent.LinkedBlockingQueue
 * @see java.util.concurrent.PriorityBlockingQueue
 * @since 1.5
 * @author Doug Lea
 * @param <E> the type of elements held in this collection
 */
public interface Queue<E> extends Collection<E> {

    //将指定的元素插入到此队列中
    boolean add(E e);

    //将指定的元素插入到此队列中
    boolean offer(E e);

    //检索并删除此队列的头。队列为空则抛出异常
    E remove();

    //检索并删除此队列的头，如果此队列为空，则返回{@code null}。
    E poll();

    //检索但不删除此队列的头，如果此队列为空，抛出异常。
    E element();
    
    //检索但不删除此队列的头，如果此队列为空，则返回{@code null}。
    E peek();
}
```



### 阻塞队列



ArrayBlockingQueue



LinkedBlockingQueue



LinkedBlockingDeque



PriorityBlockingQueue



DelayQueue



### 同步队列



ConcurrentLinkedQueue



ConcurrentLinkedDeque



SynchronousQueue



LinkedTransferQueue



## map



ConcurrentHashMap





ConcurrentSkipListMap





## list



CopyOnWriteArrayList







## set



CopyOnWriteArraySet





ConcurrentSkipListSet
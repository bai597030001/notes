# Java的四种引用类型

- java内存管理分为内存分配和内存回收，都不需要程序员负责，垃圾回收的机制主要是看对象是否有引用指向该对象。
- java对象的引用包括：  **强引用，软引用，弱引用，虚引用**
-   Java中提供这四种引用类型主要有两个目的：
  - 第一是可以让程序员通过代码的方式决定某些对象的生命周期；
  - 第二是有利于JVM进行垃圾回收。

## 强引用

- 是指创建一个对象并把这个对象赋给一个引用变量。

- 比如：

  ```java
  Object object =new Object();
  String str ="hello";
  ```

  - 强引用**有引用变量指向时永远不会被垃圾回收**，JVM宁愿抛出OutOfMemory错误也不会回收这种对象。

  ```java
  public class Main {  
      public static void main(String[] args) {  
          new Main().fun1();  
      }  
         
      public void fun1() {  
          Object object = new Object();  
          Object[] objArr = new Object[1000];  
   }  
  ```

  - 当运行至Object[] objArr = new Object[1000];这句时，如果内存不足，JVM会抛出OOM错误也不会回收object指向的对象。
  - 不过要注意的是，**当fun1运行完之后，object和objArr都已经不存在了，所以它们指向的对象都会被JVM回收**。

- 如果想中断强引用和某个对象之间的关联，可以**显示地将引用赋值为null**，这样一来的话，JVM在合适的时间就会回收该对象。

- 比如Vector类的clear方法中就是通过将引用赋值为null来实现清理工作的：

  ```java
  public synchronized E remove(int index) {  
      modCount++;  
      if (index >= elementCount)  
          throw new ArrayIndexOutOfBoundsException(index);  
      Object oldValue = elementData[index];  
    
      int numMoved = elementCount - index - 1;  
      if (numMoved > 0)  
          System.arraycopy(elementData, index+1, elementData, index,  
                   numMoved);  
      elementData[--elementCount] = null; // Let gc do its work  
    
      return (E)oldValue;  
  }  
  ```

## 软引用（SoftReference）

- 如果一个对象具有软引用，**内存空间足够，垃圾回收器就不会回收它**；
- 如果**内存空间不足**了，就**会回收这些对象的内**存。只要垃圾回收器没有回收它，该对象就可以被程序使用。
- 软引用可用来实现内存敏感的高速缓存,比**如网页缓存、图片缓存**等。
- 使用软引用能**防止内存泄露，增强程序的健壮性**。   

### SoftReference类

- SoftReference的特点是它的一个实例**保存对一个Java对象的软引用**， 该软引用的存在**不妨碍垃圾收集线程对该Java对象的回收**。

- 也就是说，一旦SoftReference保存了对一个Java对象的软引用后，在垃圾线程对 这个Java对象回收前，**SoftReference类所提供的get()方法返回Java对象的强引用**。
- 一旦垃圾线程回收该Java对象之 后，**get()方法将返回null**。



- 示例：

  ```java
  MyObject aReference = new  MyObject();  
  SoftReference aSoftRef = new SoftReference(aReference);  
  ```

  - 此时，对于这个MyObject对象，有两个引用路径，一个是来自SoftReference对象的软引用，一个来自变量aReference的强引用，所以这个MyObject对象是**强可及对象**。
  - 随即，我们可以结束aReference对这个MyObject实例的强引用:

  ```java
  aReference = null;
  ```

  - 此后，这个MyObject对象成为了软引用对象。如果垃圾收集线程进行内存垃圾收集，并不会因为有一个SoftReference对该对象的引用而始终保留该对象。

- Java虚拟机的垃圾收集线程对软可及对象和其他一般Java对象进行了区别对待:

  软可及对象的清理是由垃圾收集线程根据其特定算法按照内存需求决定的。

- 也就是说，垃圾收集线程会在虚拟机抛出OutOfMemoryError之前回收软可及对象，而且虚拟机会尽可能优先回收**长时间闲置不用的软可及对象**，对那些刚刚构建的或刚刚使用过的“新”软可反对象会被虚拟机尽可能保留。

- 在回收这些对象之前，我们可以通过:

  ```java
  MyObject anotherRef=(MyObject)aSoftRef.get();  
  ```

  重新获得对该实例的强引用。而回收之后，调用get()方法就只能得到null了。

### ReferenceQueue

- 使用ReferenceQueue清除失去了软引用对象的SoftReference：
- 作为一个Java对象，SoftReference对象除了具有保存软引用的特殊性之外，也具有Java对象的一般性。所以，当软可及对象被回收之后，虽然这个SoftReference对象的get()方法返回null,但这个SoftReference对象已经不再具有存在的价值，需要一个适当的清除机制，避免大量SoftReference对象带来的内存泄漏。
- 在java.lang.ref包里还提供了ReferenceQueue。如果在创建SoftReference对象的时候，使用了一个ReferenceQueue对象作为参数提供给SoftReference的构造方法，如:

```java
ReferenceQueue queue = new  ReferenceQueue();  
SoftReference  ref = new  SoftReference(aMyObject, queue);  
```

- 那么当这个SoftReference所软引用的aMyOhject被垃圾收集器回收的同时，ref所强引用的SoftReference对象被列入ReferenceQueue。
- 也就是说，ReferenceQueue中保存的对象是Reference对象，而且是已经失去了它所软引用的对象的Reference对象。另外从ReferenceQueue这个名字也可以看出，它是一个队列，当我们调用它的**poll()方法**的时候，如果这个队列中不是空队列，那么将返回队列前面的那个Reference对象。
- 在任何时候，我们都可以调用ReferenceQueue的poll()方法来检查是否有它所关心的非强可及对象被回收。如果队列为空，将返回一个null,否则该方法返回队列中前面的一个Reference对象。利用这个方法，我们可以检查哪个SoftReference所软引用的对象已经被回收。于是我们可以把这些失去所软引用的对象的SoftReference对象清除掉。常用的方式为:

```java
SoftReference ref = null;  
while ((ref = (EmployeeRef) q.poll()) != null) {  
    // 清除ref  
}  
```

**使用场景：**网页缓存、图片缓存

## 弱引用（WeakReference）

- 弱引用也是用来描述非必需对象的，当JVM进行垃圾回收时，无**论内存是否充足，都会回收**被弱引用关联的对象。在java中，用java.lang.ref.WeakReference类来表示。下面是使用示例：

```java
public class test {  
    public static void main(String[] args) {  
        WeakReference<People>reference=new WeakReference<People>(new People("zhouqian",20));  
        System.out.println(reference.get());  
        System.gc();//通知GVM回收资源  
        System.out.println(reference.get());  
    }  
}  
class People{  
    public String name;  
    public int age;  
    public People(String name,int age) {  
        this.name=name;  
        this.age=age;  
    }  
    @Override  
    public String toString() {  
        return "[name:"+name+",age:"+age+"]";  
    }  
}  
```

- 输出结果：

```java
[name:zhouqian,age:20]
null
```

- 第二个输出结果是null，这说明只要JVM进行垃圾回收，被弱引用关联的对象必定会被回收掉。不过要注意的是，这里所说的被弱引用关联的对象是指**只有弱引用与之关联**，如果存在强引用同时与之关联，则进行垃圾回收时也不会回收该对象（软引用也是如此）。
- 比如：将代码做一点小更改：

```java
package yinyong;  
  
import java.lang.ref.WeakReference;  
  
public class test {  
    public static void main(String[] args) {  
        People people=new People("zhouqian",20);  
        WeakReference<People>reference=new WeakReference<People>(people);
        //关联强引用
        System.out.println(reference.get());  
        System.gc();  
        System.out.println(reference.get());  
    }  
}  
class People{  
    public String name;  
    public int age;  
    public People(String name,int age) {  
        this.name=name;  
        this.age=age;  
    }  
    @Override  
    public String toString() {  
        return "[name:"+name+",age:"+age+"]";  
    }  
}//结果发生了很大的变化  
[name:zhouqian,age:20]  
[name:zhouqian,age:20]  
```

- 弱引用可以和一个引用队列（ReferenceQueue）联合使用，如果弱引用所引用的对象被JVM回收，这个弱引用就会被加入到与之关联的引用队列中。

**使用场景：**一旦我不需要某个引用，JVM会自动帮我处理它，这样我就不需要做其它操作。 

## 虚引用（PhantomReference）

- 虚引用和前面的软引用、弱引用不同，它并不影响对象的生命周期。在java中用java.lang.ref.PhantomReference类表示。如果一个对象与虚引用关联，则跟没有引用与之关联一样，在任何时候都可能被垃圾回收器回收。
- 要注意的是，**虚引用必须和引用队列关联使用**，当垃圾回收器准备回收一个对象时，如果发现它还有虚引用，就会把这个虚引用加入到与之 关联的引用队列中。程序可以通过判断引用队列中是否已经加入了虚引用，来了解被引用的对象是否将要被垃圾回收。如果程序发现某个虚引用已经被加入到引用队列，那么就可以在所引用的对象的内存被回收之前采取必要的行动。

```java
import java.lang.ref.PhantomReference;  
import java.lang.ref.ReferenceQueue;  
public class Main {  
    public static void main(String[] args) {  
        ReferenceQueue<String> queue = new ReferenceQueue<String>();  
        PhantomReference<String> pr = new PhantomReference<String>(new String("hello"), queue);  
        System.out.println(pr.get());  
    }  
}  
```

使用场景：它**被回收之前**，会被放入`ReferenceQueue`中。（注意：其它引用是被JVM回收后才被传入`ReferenceQueue`中的。）由于这个机制，所以虚引用大多被用于引用销毁前的处理工作。



# 总结使用



## 软引用

比如在一个博客管理系统里，为了提升访问性能，在用户在点击博文时，如果这篇博文没有缓存到内存中，则需要做缓存动作，这样其它用户在点击同样这篇文章时，就能直接从内存里装载，而不用走数据库，这样能降低响应时间。

我们可以通过数据库级别的缓存在做到这点，这里也可以通过软引用来实现，具体的实现步骤如下：

1、可以通过定义Content类来封装博文的内容，其中可以包括文章ID、文章内容、作者、发表时间和引用图片等相关信息。

2、可以定义一个类型为HashMap<String, SoftReference<Content>>的对象类保存缓存内容，其中键是String类型，表示文章ID，值是指向Content的软引用。

3、当用户点击某个ID的文章时，根据ID到第二步定义的HashMap里去找，如果找到，而且所对应的SoftReference<Content>值内容不是null，则直接从这里拿数据并做展示动作，这样不用走数据库，可以提升性能。

4、如果用户点击的某个文章的ID在HashMap里找不到，或者虽然找到，但对应的值内容是空，那么就从数据库去找，找到后显示这个文章，同时再把它插入到HashMap里，这里请注意，显示后需要撤销掉这个Content类型对象上的强引用，保证它上面只有一个软引用。



## 弱引用

WeakHashMap



## 虚引用
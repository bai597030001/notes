# java集合类

- Java集合大致可以分为Set、List、Queue和Map四种体系

```tex
其中Set代表无序、不可重复的集合；

List代表有序、可重复的集合；

Map则代表具有映射关系的集合；

Queue体系集合，代表一种队列集合实现。
```

- 集合类之间的关系

![](img/java-collection1.webp)



![](img/java-collection2.webp)

- `Collections`集合工具类提供了对集合排序、遍历等多种算法实现。
- `Arrays`工具类



## 1.List



### ArrayList

- `ArrayList` 是一个**动态数组**，它是**线程不安全**的，允许元素为null。
- 其底层数据结构依然是**数组**，它实现了`List<E>, RandomAccess, Cloneable, java.io.Serializable`接口，其中`RandomAccess`代表了其拥有**随机快速访问**的能力，`ArrayList`可以以O(1)的时间复杂度去根据下标访问元素。
- 数组，所以占据一块连续的内存空间，所以可以根据下标快速存取
- 当元素个数超过容量，便会进行扩容。扩容是其性能消耗比较大的地方（初始化时尽量指定大小）



```java
public class ArrayList<E> extends AbstractList<E>
        implements List<E>, RandomAccess, Cloneable, java.io.Serializable
{
    private static final long serialVersionUID = 8683452581122892189L;

	// 默认的数组存储容量
    private static final int DEFAULT_CAPACITY = 10;

	// 当指定数组的容量为0的时候使用这个变量赋值
    private static final Object[] EMPTY_ELEMENTDATA = {};

	// 默认的实例化的时候使用此变量赋值
	// 这样可以知道当第一个元素添加的时候进行扩容至多少
    private static final Object[] DEFAULTCAPACITY_EMPTY_ELEMENTDATA = {};

	// 真正存放数据的对象数组，并不被序列化 //非private以简化嵌套类访问
    transient Object[] elementData; // non-private to simplify nested class access

    private int size;

    public ArrayList(int initialCapacity) {
        if (initialCapacity > 0) {
            this.elementData = new Object[initialCapacity];
        } else if (initialCapacity == 0) {
            this.elementData = EMPTY_ELEMENTDATA;
        } else {
            throw new IllegalArgumentException("Illegal Capacity: "+
                                               initialCapacity);
        }
    }

    public ArrayList() {
        this.elementData = DEFAULTCAPACITY_EMPTY_ELEMENTDATA;
    }

    public ArrayList(Collection<? extends E> c) {
        elementData = c.toArray();
        if ((size = elementData.length) != 0) {
            // c.toArray might (incorrectly) not return Object[] (see 6260652)
            if (elementData.getClass() != Object[].class)
                elementData = Arrays.copyOf(elementData, size, Object[].class);
        } else {
            // replace with empty array.
            this.elementData = EMPTY_ELEMENTDATA;
        }
    }
}
```



- 这里我们主要分析一下`add`()与扩容机制



```java
//1
public boolean add(E e) {
    ensureCapacityInternal(size + 1);  // Increments modCount!!
    elementData[size++] = e;
    return true;
}

//2
private void ensureCapacityInternal(int minCapacity) {
    ensureExplicitCapacity(calculateCapacity(elementData, minCapacity));
}

//3
private static int calculateCapacity(Object[] elementData, int minCapacity) {
    //通过这个DEFAULTCAPACITY_EMPTY_ELEMENTDATA判断是否是使用默认构造函数初始化
    if (elementData == DEFAULTCAPACITY_EMPTY_ELEMENTDATA) {
        return Math.max(DEFAULT_CAPACITY, minCapacity);
    }
    return minCapacity;
}

//4
private void ensureExplicitCapacity(int minCapacity) {
    modCount++;

    // overflow-conscious code
    if (minCapacity - elementData.length > 0)
        grow(minCapacity);
}

//5
private void grow(int minCapacity) {
    // overflow-conscious code
    int oldCapacity = elementData.length;
    int newCapacity = oldCapacity + (oldCapacity >> 1);//扩容的长度是增加了原来数组数组的一半大小
    if (newCapacity - minCapacity < 0)
        newCapacity = minCapacity;//如果还不够 ，那么就用 能容纳的最小的数量。
    if (newCapacity - MAX_ARRAY_SIZE > 0)
        //如果新容量比预定义的最大容量（Integer.MAX_VALUE - 8）还大，那么调用hugeCapacity,将新容量设置为 Integer.MAX_VALUE 
        newCapacity = hugeCapacity(minCapacity);
    // minCapacity is usually close to size, so this is a win:
    elementData = Arrays.copyOf(elementData, newCapacity);
}

//6
private static int hugeCapacity(int minCapacity) {
    if (minCapacity < 0) // overflow
        throw new OutOfMemoryError();
    return (minCapacity > MAX_ARRAY_SIZE) ?
        Integer.MAX_VALUE :
    MAX_ARRAY_SIZE;
}
```



```java
public void add(int index, E element) {
    rangeCheckForAdd(index);//判断索引是否越界，若越界就会抛异常
	
    //扩容检查
    ensureCapacityInternal(size + 1);  // Increments modCount!!
    
    // 将指定下标空出，具体就是将index及其后的元素都往后挪动一位
    System.arraycopy(elementData, index, elementData, index + 1,
                     size - index);
    
    // 赋值
    elementData[index] = element;
    size++;// 长度加1
}
```





### LinkedList

- `LinkedList` 是**线程不安全**的，**允许元素为null**的**双向链表**。
- 其底层数据结构是链表，它实现List<E>, Deque<E>, Cloneable, java.io.Serializable接口，它实现了Deque<E>,所以它也可以作为一个双端队列。和ArrayList比，没有实现RandomAccess所以其以下标，随机访问元素速度较慢。

> 底层在根据下标查询Node的时候，会根据index判断目标Node在前半段还是后半段，然后决定是**顺序还是逆序查询**，**以提升时间效率**。



```java
public class LinkedList<E>
    extends AbstractSequentialList<E>
    implements List<E>, Deque<E>, Cloneable, java.io.Serializable
{
    transient int size = 0;

    /**
     * Pointer to first node.
     * Invariant: (first == null && last == null) ||
     *            (first.prev == null && first.item != null)
     */
    transient Node<E> first;

    /**
     * Pointer to last node.
     * Invariant: (first == null && last == null) ||
     *            (last.next == null && last.item != null)
     */
    transient Node<E> last;

    /**
     * Constructs an empty list.
     */
    public LinkedList() {
    }

    /**
     * Constructs a list containing the elements of the specified
     * collection, in the order they are returned by the collection's
     * iterator.
     *
     * @param  c the collection whose elements are to be placed into this list
     * @throws NullPointerException if the specified collection is null
     */
    public LinkedList(Collection<? extends E> c) {
        this();
        addAll(c);
    }
    
    private static class Node<E> {
        E item;//元素值
        Node<E> next;//后置节点
        Node<E> prev;//前置节点

        Node(Node<E> prev, E element, Node<E> next) {
            this.item = element;
            this.next = next;
            this.prev = prev;
        }
    }


	/**
     * Returns the (non-null) Node at the specified element index.
     *
     * 通过下标获取某个node 的时候，会根据index处于前半段还是后半段 进行一个折半，以提升查询效率
     */
    Node<E> node(int index) {
        // assert isElementIndex(index);

        if (index < (size >> 1)) {
            Node<E> x = first;
            for (int i = 0; i < index; i++)
                x = x.next;
            return x;
        } else {
            Node<E> x = last;
            for (int i = size - 1; i > index; i--)
                x = x.prev;
            return x;
        }
    }
}
```





### Vector

- Vector是线程安全的(synchronized实现)，允许元素为NULL的动态数组
- 底层是由**数组**实现，是一个**动态数组**，其容量能自动增长或者减少。Vector 继承 AbstractList 抽象类，实现 List、RandomAccess、Clone、java.io.Serializable 接口。其实现原理与 ArrayList 类似。
- Vector 实现 RandomAccess 接口，提供快速随机访问功能。
- Vector 实现 Clone 接口，重写 Object clone() 方法可以克隆对象。
- Vector 实现 java.io.Serializable 接口，可以进行序列化和反序列化，方便数据在网络进行传输。



与 ArrayList 一样，Vector 支持容量自动增长，但是增长方式与 ArrayList 略有不同。Vector 通过维护 **capacity** 和 **capacityIncrement** 这两个属性来优化其内存管理，而 capacityIncrement 是容量增长的系数，当 Vector 容量不足时，如果 capacityIncrement 为 0 的话，那么 Vector 容量扩容为原先的 2 倍（newCapacity = 2 * oldCapacity）；如果 capacityIncrement 大于 0，那么扩容后的容量为 oldCapacity + capacityIncrement。扩容的原理其实是对数组进行复制，对需要移动的元素进行移动，相当消耗性能的。



```java
public class Vector<E>
    extends AbstractList<E>
    implements List<E>, RandomAccess, Cloneable, java.io.Serializable
{
    protected Object[] elementData;

    protected int elementCount;

    protected int capacityIncrement;

    private static final long serialVersionUID = -2767605614048989439L;


    public Vector(int initialCapacity, int capacityIncrement) {
        super();
        if (initialCapacity < 0)
            throw new IllegalArgumentException("Illegal Capacity: "+
                                               initialCapacity);
        this.elementData = new Object[initialCapacity];
        this.capacityIncrement = capacityIncrement;
    }

    public Vector(int initialCapacity) {
        this(initialCapacity, 0);
    }

    public Vector() {
        this(10);
    }

    public Vector(Collection<? extends E> c) {
        elementData = c.toArray();
        elementCount = elementData.length;
        // c.toArray might (incorrectly) not return Object[] (see 6260652)
        if (elementData.getClass() != Object[].class)
            elementData = Arrays.copyOf(elementData, elementCount, Object[].class);
    }
}
```





## 2.Set

> Set不允许包含相同的元素，如果试图把两个相同元素加入同一个集合中，add方法返回false。  
>
> Set判断两个对象相同不是使用==运算符，而是根据equals方法。也就是说，只要两个对象用equals方法比较返回true，Set就不会接受这两个对象。  
>
> HashSet与TreeSet都是基于Set接口的实现类。其中TreeSet是Set的子接口SortedSet的实现类。



- set接口定义集合的基本操作

```java
public interface Set<E> extends Collection<E> {
    
    int size();
    
    boolean isEmpty();
    
    boolean contains(Object o);
    
    Iterator<E> iterator();
    
    Object[] toArray();
    
    <T> T[] toArray(T[] a);
    
    boolean add(E e);
    
    boolean remove(Object o);
    
    boolean containsAll(Collection<?> c);
    
    boolean addAll(Collection<? extends E> c);
    
    boolean retainAll(Collection<?> c);
    
    boolean removeAll(Collection<?> c);
    
    void clear();
    
    boolean equals(Object o);
    
    int hashCode();
    
    @Override
    default Spliterator<E> spliterator() {
        return Spliterators.spliterator(this, Spliterator.DISTINCT);
    }
}
```



- 





### HashSet

- HashSet有以下特点:

> 1.不能保证元素的排列顺序，顺序有可能发生变化  
> 2.不是同步的  
> 3.集合元素可以是null,但只能放入一个null  
> 4.当向HashSet结合中存入一个元素时，HashSet会调用该对象的hashCode()方法来得到该对象的hashCode值，然后根据 hashCode值来决定该对象在HashSet中存储位置。

**HashSet判断两个元素相等的标准是两个对象通过equals方法比较相等，并且两个对象的hashCode()方法返回值相等**

- 注意：  
  如果要把一个对象放入HashSet中，重写该对象对应类的equals方法，也应该重写其hashCode()方法。  
  其规则是如果两个对象通过equals方法比较返回true时，其hashCode也应该相同。  
  另外，对象中用作equals比较标准的属性，都应该用来计算 hashCode的值。



```java
public class HashSet<E>
    extends AbstractSet<E>
    implements Set<E>, Cloneable, java.io.Serializable
{
    static final long serialVersionUID = -5024744406713321676L;

    private transient HashMap<E,Object> map;

    // Dummy value to associate with an Object in the backing Map
    private static final Object PRESENT = new Object();

    /**
     * Constructs a new, empty set; the backing <tt>HashMap</tt> instance has
     * default initial capacity (16) and load factor (0.75).
     */
    public HashSet() {
        map = new HashMap<>();
    }

    /**
     * Constructs a new set containing the elements in the specified
     * collection.  The <tt>HashMap</tt> is created with default load factor
     * (0.75) and an initial capacity sufficient to contain the elements in
     * the specified collection.
     *
     * @param c the collection whose elements are to be placed into this set
     * @throws NullPointerException if the specified collection is null
     */
    public HashSet(Collection<? extends E> c) {
        map = new HashMap<>(Math.max((int) (c.size()/.75f) + 1, 16));
        addAll(c);
    }

    /**
     * Constructs a new, empty set; the backing <tt>HashMap</tt> instance has
     * the specified initial capacity and the specified load factor.
     *
     * @param      initialCapacity   the initial capacity of the hash map
     * @param      loadFactor        the load factor of the hash map
     * @throws     IllegalArgumentException if the initial capacity is less
     *             than zero, or if the load factor is nonpositive
     */
    public HashSet(int initialCapacity, float loadFactor) {
        map = new HashMap<>(initialCapacity, loadFactor);
    }

    /**
     * Constructs a new, empty set; the backing <tt>HashMap</tt> instance has
     * the specified initial capacity and default load factor (0.75).
     *
     * @param      initialCapacity   the initial capacity of the hash table
     * @throws     IllegalArgumentException if the initial capacity is less
     *             than zero
     */
    public HashSet(int initialCapacity) {
        map = new HashMap<>(initialCapacity);
    }

    /**
     * Constructs a new, empty linked hash set.  (This package private
     * constructor is only used by LinkedHashSet.) The backing
     * HashMap instance is a LinkedHashMap with the specified initial
     * capacity and the specified load factor.
     *
     * @param      initialCapacity   the initial capacity of the hash map
     * @param      loadFactor        the load factor of the hash map
     * @param      dummy             ignored (distinguishes this
     *             constructor from other int, float constructor.)
     * @throws     IllegalArgumentException if the initial capacity is less
     *             than zero, or if the load factor is nonpositive
     */
    HashSet(int initialCapacity, float loadFactor, boolean dummy) {
        map = new LinkedHashMap<>(initialCapacity, loadFactor);
    }
    
    // ......其他方法
}
```



### NavigableSet

`NavigableSet` 继承了 `SortedSet`，提供了关于搜索的更多方法

```java
public interface NavigableSet<E> extends SortedSet<E> {
 
    E lower(E e);
    
    E floor(E e);
    
    E ceiling(E e);
    
    E higher(E e);
    
    E pollFirst();
    
    E pollLast();
    
    Iterator<E> iterator();
    
    NavigableSet<E> descendingSet();
    
    Iterator<E> descendingIterator();
    
    NavigableSet<E> subSet(E fromElement, boolean fromInclusive,
                           E toElement,   boolean toInclusive);
    
    NavigableSet<E> headSet(E toElement, boolean inclusive);
    
    NavigableSet<E> tailSet(E fromElement, boolean inclusive);
    
    SortedSet<E> subSet(E fromElement, E toElement);
    
    SortedSet<E> headSet(E toElement);
    
    SortedSet<E> tailSet(E fromElement);
}    
```





### TreeSet

```
TreeSet类型是J2SE中唯一可实现自动排序的类型
```

- TreeSet是SortedSet接口的唯一实现类，TreeSet可以确保集合元素处于排序状态。
- TreeSet支持两种排序方式，自然排序 和定制排序，其中自然排序为默认的排序方式。  
- 排序规则

> 自然排序：

```
自然排序使用要排序元素的CompareTo（Object obj）方法来比较元素之间大小关系，然后将元素按照升序排列。

Java提供了一个Comparable接口，该接口里定义了一个compareTo(Object obj)方法，该方法返回一个整数值，实现了该接口的对象就可以比较大小。

obj1.compareTo(obj2)方法如果返回0，则说明被比较的两个对象相等，如果返回一个正数，则表明obj1大于obj2，如果是 负数，则表明obj1小于obj2。

如果我们将两个对象的equals方法总是返回true，则这两个对象的compareTo方法返回应该返回0
```

> 定制排序：

```
自然排序是根据集合元素的大小，以升序排列，如果要定制排序，应该使用Comparator接口，实现 int compare(To1,To2)方法
```

- 比较规则  

  **TreeSet判断两个对象不相等的方式是两个对象通过equals方法返回false，或者通过CompareTo方法比较没有返回0**



```java
public class TreeSet<E> extends AbstractSet<E>
    implements NavigableSet<E>, Cloneable, java.io.Serializable
{
    /**
     * The backing map.
     */
    private transient NavigableMap<E,Object> m;

    // Dummy value to associate with an Object in the backing Map
    private static final Object PRESENT = new Object();

    /**
     * Constructs a set backed by the specified navigable map.
     */
    TreeSet(NavigableMap<E,Object> m) {
        this.m = m;
    }

    public TreeSet() {
        this(new TreeMap<E,Object>());
    }

    public TreeSet(Comparator<? super E> comparator) {
        this(new TreeMap<>(comparator));
    }

    public TreeSet(Collection<? extends E> c) {
        this();
        addAll(c);
    }

    public TreeSet(SortedSet<E> s) {
        this(s.comparator());
        addAll(s);
    }
}
```





### LinkedHashSet

- `LinkedHashSet`集合同样是根据元素的`hashCode`值来决定元素的存储位置。但是它同时使用链表维护元素的次序。这样使得元素看起 来像是以插入顺序保存的。当遍历该集合时候，`LinkedHashSet`将会以元素的添加顺序访问集合的元素。
- **LinkedHashSet在迭代访问Set中的全部元素时，性能比HashSet好，但是插入时性能稍微逊色于HashSet**。
- `LinkedHashSet`继承`HashSet`，其构造函数全部调用`HashSet`的统一个构造函数，使其初始化一个`LinkedHashMap`作为其父类的成员变量



```java
public class LinkedHashSet<E>
    extends HashSet<E>
    implements Set<E>, Cloneable, java.io.Serializable {

    private static final long serialVersionUID = -2851667679971038690L;

    public LinkedHashSet(int initialCapacity, float loadFactor) {
        super(initialCapacity, loadFactor, true);
    }

    public LinkedHashSet(int initialCapacity) {
        super(initialCapacity, .75f, true);
    }

    public LinkedHashSet() {
        super(16, .75f, true);
    }

    public LinkedHashSet(Collection<? extends E> c) {
        super(Math.max(2*c.size(), 11), .75f, true);
        addAll(c);
    }

    @Override
    public Spliterator<E> spliterator() {
        return Spliterators.spliterator(this, Spliterator.DISTINCT | Spliterator.ORDERED);
    }
}
```





## 3.Map

我们知道，一般的Map都是无序的，也就是只能通过键的hash值进行定位。JDK为了实现有序的Map，提供了一个**SortedMap**接口，SortedMap提供了一些根据键范围进行查找的功能，比如返回整个Map中 key最小/大的键、返回某个范围内的子Map视图等等。

为了进一步对有序Map进行增强，JDK又引入了**NavigableMap**接口，该接口进一步扩展了SortedMap的功能，提供了根据指定Key返回最接近项、按升序/降序返回所有键的视图等功能。

同时，也提供了一个基于NavigableMap的实现类——**TreeMap**，TreeMap底层基于红黑树设计，是一种有序的Map。



![](img/jdk-map1.png)



### HashMap

[https://mp.weixin.qq.com/s/w282P0z7IDUcatOFtoJgiA](https://mp.weixin.qq.com/s/w282P0z7IDUcatOFtoJgiA)



#### 红黑树

红黑树具有以下5种性质：

（1）节点是红色或黑色。

（2）根节点是黑色。

（3）每个叶节点（NIL节点，空节点）是黑色的。

（4）每个红色节点的两个子节点都是黑色。(从每个叶子到根的所有路径上不能有两个连续的红色节点)

（5）从任一节点到其每个叶子的所有路径都包含相同数目的黑色节点。

红黑树的时间复杂度为O(log n)，与树的高度成正比。

红黑树每次的插入、删除操作都需要做平衡，平衡时有可能会改变根节点的位置，颜色转换，左旋，右旋等。



#### 数据结构

- 数组+链表+红黑树

> 数组的查询效率为O(1)，链表的查询效率是O(k)，红黑树的查询效率是O(log k)，k为桶中的元素个数，所以当元素数量非常多的时候，转化为红黑树能极大地提高效率。

```java
// 节点定义
static class Node<K,V> implements Map.Entry<K,V> {
    final int hash; //key值的高低16位异或(OXR)以后的数值
    final K key;
    V value;
    Node<K,V> next;

    Node(int hash, K key, V value, Node<K,V> next) {
        this.hash = hash;
        this.key = key;
        this.value = value;
        this.next = next;
    }

    public final K getKey()        { return key; }
    public final V getValue()      { return value; }
    public final String toString() { return key + "=" + value; }

    public final int hashCode() {
        return Objects.hashCode(key) ^ Objects.hashCode(value);
    }

    public final V setValue(V newValue) {
        V oldValue = value;
        value = newValue;
        return oldValue;
    }

    public final boolean equals(Object o) {
        if (o == this)
            return true;
        if (o instanceof Map.Entry) {
            Map.Entry<?,?> e = (Map.Entry<?,?>)o;
            if (Objects.equals(key, e.getKey()) &&
                Objects.equals(value, e.getValue()))
                return true;
        }
        return false;
    }
}
```



HashMap使用链表法避免哈希冲突（相同hash值），当链表长度大于TREEIFY_THRESHOLD（默认为8）时，将链表转换为红黑树，当然小于UNTREEIFY_THRESHOLD（默认为6）时，又会转回链表以达到性能均衡。 我们看一张HashMap的数据结构（数组+链表+红黑树 ）就更能理解table了：

![](E:/notes/java/javaBasic/img/hashMap1.webp)







#### 属性

```java
public class HashMap<K,V> extends AbstractMap<K,V> implements Map<K,V>, Cloneable, Serializable {
    // 序列号
    private static final long serialVersionUID = 362498820763181265L;    
    // 默认的初始容量是16
    static final int DEFAULT_INITIAL_CAPACITY = 1 << 4;   
    // 最大容量
    static final int MAXIMUM_CAPACITY = 1 << 30; 
    // 默认的填充因子
    static final float DEFAULT_LOAD_FACTOR = 0.75f;
    // 当桶(bucket)上的结点数大于这个值时会转成红黑树
    static final int TREEIFY_THRESHOLD = 8; 
    // 当桶(bucket)上的结点数小于这个值时树转链表
    static final int UNTREEIFY_THRESHOLD = 6;
    // 桶中结构转化为红黑树对应的table的最小大小
    static final int MIN_TREEIFY_CAPACITY = 64;
    // 存储元素的数组，总是2的幂次倍
    transient Node<k,v>[] table; 
    // 存放具体元素的集
    transient Set<map.entry<k,v>> entrySet;
    // 存放元素的个数，注意这个不等于数组的长度。
    transient int size;
    // 每次扩容和更改map结构的计数器
    transient int modCount;   
    // 临界值 当实际大小(容量*填充因子)超过临界值时，会进行扩容
    int threshold;
    // 填充因子
    final float loadFactor;
}
```

#### 构造函数

```java
public HashMap() {
    // 初始化填充因子
    this.loadFactor = DEFAULT_LOAD_FACTOR; 
}

public HashMap(int initialCapacity) {
    // 调用HashMap(int, float)型构造函数
    this(initialCapacity, DEFAULT_LOAD_FACTOR);
}

public HashMap(Map<? extends K, ? extends V> m) {
    // 初始化填充因子
    this.loadFactor = DEFAULT_LOAD_FACTOR;
    // 将m中的所有元素添加至HashMap中
    putMapEntries(m, false);
}

public HashMap(int initialCapacity, float loadFactor) {
    // 初始容量不能小于0，否则报错
    if (initialCapacity < 0)
        throw new IllegalArgumentException("Illegal initial capacity: " +
                                            initialCapacity);
    // 初始容量不能大于最大值，否则为最大值
    if (initialCapacity > MAXIMUM_CAPACITY)
        initialCapacity = MAXIMUM_CAPACITY;
    // 填充因子不能小于或等于0，不能为非数字
    if (loadFactor <= 0  Float.isNaN(loadFactor))
        throw new IllegalArgumentException("Illegal load factor: " +
                                            loadFactor);
    // 初始化填充因子                                        
    this.loadFactor = loadFactor;
    // 初始化threshold大小
    this.threshold = tableSizeFor(initialCapacity);    
}
```



```java
// 主要功能是返回一个比给定整数大且最接近的2的幂次方整数，如给定10，返回2的4次方16.
static final int tableSizeFor(int cap) {
   int n = cap - 1;
   n |= n >>> 1;
   n |= n >>> 2;
   n |= n >>> 4;
   n |= n >>> 8;
   n |= n >>> 16;
   return (n < 0) ? 1 : (n >= MAXIMUM_CAPACITY) ? MAXIMUM_CAPACITY : n + 1;
 }
```



![](E:/notes/java/javaBasic/img/hashMap5.webp)



```java
/**
  * 将m的所有元素存入本HashMap实例中
  */
 final void putMapEntries(Map<? extends K, ? extends V> m, boolean evict) {
   // 得到 m 中元素的个数
   int s = m.size();
   // 当 m 中有元素时，则需将map中元素放入本HashMap实例。
   if (s > 0) {
     // 判断table是否已经初始化，如果未初始化，则先初始化一些变量。（table初始化是在put时）
     if (table == null) { // pre-size
       // 根据待插入的map 的 size 计算要创建的 HashMap 的容量。
       float ft = ((float) s / loadFactor) + 1.0F;
       int t = ((ft < (float) MAXIMUM_CAPACITY) ? (int) ft : MAXIMUM_CAPACITY);
       // 把要创建的 HashMap 的容量存在 threshold 中
       if (t > threshold)
         threshold = tableSizeFor(t);
     }
     // 如果table初始化过，因为别的函数也会调用它，所以有可能HashMap已经被初始化过了。
     // 判断待插入的 map 的 size,若 size 大于 threshold，则先进行 resize()，进行扩容
     else if (s > threshold)
       resize();
     // 然后就开始遍历 带插入的 map ，将每一个 <Key ,Value> 插入到本HashMap实例。
     for (Map.Entry<? extends K, ? extends V> e : m.entrySet()) {
       K key = e.getKey();
       V value = e.getValue();
       // put(K,V)也是调用 putVal 函数进行元素的插入
       putVal(hash(key), key, value, false, evict);
     }
   }
 }
```



#### 为什么数组必须是2的倍数

如下所示的**`hash`计算**：

​	`key` 的 `hash`值的计算是通过hashCode()的高16位异或低16位实现的：

​	(`h = k.hashCode()) ^ (h >>> 16)`



比如，原来的 hashCode : 

1111 1111 1111 1111 0100 1100 0000 1010

移位后的hashCode: 

0000 0000 0000 0000 1111 1111 1111 1111

进行异或(OXR)运算 结果：

1111 1111 1111 1111 1011 0011 1111 0101



这样可以将hashcode高位和低位的值进行混合做异或运算，而且混合后，**低位的信息中加入了高位的信息，这样高位的信息被变相的保留了下来。掺杂的元素多了，那么生成的hash值的随机性会增大。**



至于为什么`table`大小必须是2的次幂(指长度扩为原来2倍)呢？

在扩容的时候，如果数组`table`长度是2的次幂，那么扩容后元素的位置要么是在原位置，要么是在原位置再移动2次幂的位置。

看下图可以明白这句话的意思，n为`table`的长度，图（a）表示扩容前的`key1`和`key2`两种`key`确定索引位置的示例，图（b）表示扩容后`key1`和`key2`两种`key`确定索引位置的示例，其中`hash1`是key1对应的哈希与高位运算结果。

![](E:/notes/java/javaBasic/img/hashMap2.webp)

元素在重新计算hash之后，因为n变为2倍，那么n-1的mask范围在高位多1bit(红色)，因此新的index就会发生这样的变化：

![](E:/notes/java/javaBasic/img/hashMap3.webp)

因此，我们在扩充HashMap的时候，只需要看看原来的hash值新增的那个bit是1还是0就好了，**是0的话索引没变，是1的话索引变成“原索引+oldCap”**，可以看看下图为16扩充为32的resize示意图 ：

![](E:/notes/java/javaBasic/img/hashMap4.webp)





#### put

```java
public V put(K key, V value) {
    return putVal(hash(key), key, value, false, true);
}
```



```java
static final int hash(Object key) {
    int h;
    return (key == null) ? 0 : (h = key.hashCode()) ^ (h >>> 16);//key值高低16位异或
}
```



```java
final V putVal(int hash, K key, V value, boolean onlyIfAbsent,
                   boolean evict) {
    Node<K,V>[] tab; Node<K,V> p; int n, i;
    // 步骤①：tab为空则创建 
    // table未初始化或者长度为0，进行扩容
    if ((tab = table) == null  (n = tab.length) == 0)
        n = (tab = resize()).length;
    // 步骤②：计算index，并对null做处理  
    // (n - 1) & hash 确定元素存放在哪个桶中，桶为空，新生成结点放入桶中(此时，这个结点是放在数组中)
    if ((p = tab[i = (n - 1) & hash]) == null)
        tab[i] = newNode(hash, key, value, null);
    // 桶中已经存在元素
    else {
        Node<K,V> e; K k;
        // 步骤③：节点key存在，直接覆盖value 
        // 比较桶中第一个元素(数组中的结点)的hash值相等，key相等
        if (p.hash == hash &&
            ((k = p.key) == key  (key != null && key.equals(k))))
                // 将第一个元素赋值给e，用e来记录
                e = p;
        // 步骤④：判断该链为红黑树 
        // hash值不相等，即key不相等；为红黑树结点
        else if (p instanceof TreeNode)
            // 放入树中
            e = ((TreeNode<K,V>)p).putTreeVal(this, tab, hash, key, value);
        // 步骤⑤：该链为链表 
        // 为链表结点
        else {
            // 在链表最末插入结点
            for (int binCount = 0; ; ++binCount) {
                // 到达链表的尾部
                if ((e = p.next) == null) {
                    // 在尾部插入新结点
                    p.next = newNode(hash, key, value, null);
                    // 结点数量达到阈值，转化为红黑树
                    if (binCount >= TREEIFY_THRESHOLD - 1) // -1 for 1st
                        treeifyBin(tab, hash);
                    // 跳出循环
                    break;
                }
                // 判断链表中结点的key值与插入的元素的key值是否相等
                if (e.hash == hash &&
                    ((k = e.key) == key  (key != null && key.equals(k))))
                    // 相等，跳出循环
                    break;
                // 用于遍历桶中的链表，与前面的e = p.next组合，可以遍历链表
                p = e;
            }
        }
        // 表示在桶中找到key值、hash值与插入元素相等的结点
        if (e != null) { 
            // 记录e的value
            V oldValue = e.value;
            // onlyIfAbsent为false或者旧值为null
            if (!onlyIfAbsent  oldValue == null)
                //用新值替换旧值
                e.value = value;
            // 访问后回调
            afterNodeAccess(e);
            // 返回旧值
            return oldValue;
        }
    }
    // 结构性修改
    ++modCount;
    // 步骤⑥：超过最大容量 就扩容 
    // 实际大小大于阈值则扩容
    if (++size > threshold)
        resize();
    // 插入后回调
    afterNodeInsertion(evict);
    return null;
}
```



#### resize

- 什么时候扩容

  在put元素的过程中，如果当前容器中元素的个数达到阈值（当前数组长度乘以加载因子的值，`table.length * loadFactor`）

- 扩容

  其实就是重新计算容量；而这个扩容是计算出所需容器的大小之后重新定义一个新的容器，将原来容器中的元素放入其中。

```java
final Node<K,V>[] resize() {
       // 保存当前table
       Node<K,V>[] oldTab = table;
       // 保存当前table的容量
       int oldCap = (oldTab == null) ? 0 : oldTab.length;
       // 保存当前阈值
       int oldThr = threshold;
       // 初始化新的table容量和阈值 
       int newCap, newThr = 0;
       /*
       1. resize（）函数在size　> threshold时被调用。oldCap大于 0 代表原来的 table 表非空，
          oldCap 为原表的大小，oldThr（threshold） 为 oldCap × load_factor
       */
       if (oldCap > 0) {
           // 若旧table容量已超过最大容量，更新阈值为Integer.MAX_VALUE（最大整形值），这样以后就不会自动扩容了。
           if (oldCap >= MAXIMUM_CAPACITY) {
               threshold = Integer.MAX_VALUE;
               return oldTab;
           }
            // 容量翻倍，使用左移，效率更高
           else if ((newCap = oldCap << 1) < MAXIMUM_CAPACITY &&
                    oldCap >= DEFAULT_INITIAL_CAPACITY)
               // 阈值翻倍
               newThr = oldThr << 1; // double threshold
       }
       /*
       2. resize（）函数在table为空被调用。oldCap 小于等于 0 且 oldThr 大于0，代表用户创建了一个 HashMap，但是使用的构造函数为      
          HashMap(int initialCapacity, float loadFactor) 或 HashMap(int initialCapacity)
          或 HashMap(Map<? extends K, ? extends V> m)，导致 oldTab 为 null，oldCap 为0， oldThr 为用户指定的 HashMap的初始容量。
   　　*/
       else if (oldThr > 0) // initial capacity was placed in threshold
           //当table没初始化时，threshold持有初始容量。还记得threshold = tableSizeFor(t)么;
           newCap = oldThr;
       /*
       3. resize（）函数在table为空被调用。oldCap 小于等于 0 且 oldThr 等于0，用户调用 HashMap()构造函数创建的　HashMap，所有值均采用默认值，oldTab（Table）表为空，oldCap为0，oldThr等于0，
       */
       else {               // zero initial threshold signifies using defaults
           newCap = DEFAULT_INITIAL_CAPACITY;
           newThr = (int)(DEFAULT_LOAD_FACTOR * DEFAULT_INITIAL_CAPACITY);
       }
       // 新阈值为0
       if (newThr == 0) {
           float ft = (float)newCap * loadFactor;
           newThr = (newCap < MAXIMUM_CAPACITY && ft < (float)MAXIMUM_CAPACITY ?
                     (int)ft : Integer.MAX_VALUE);
       }
       threshold = newThr;
       @SuppressWarnings({"rawtypes","unchecked"})
       // 初始化table
       Node<K,V>[] newTab = (Node<K,V>[])new Node[newCap];
       table = newTab;
       if (oldTab != null) {
           // 把 oldTab 中的节点　reHash 到　newTab 中去
           for (int j = 0; j < oldCap; ++j) {
               Node<K,V> e;
               if ((e = oldTab[j]) != null) {
                   oldTab[j] = null;
                   // 若节点是单个节点，直接在 newTab　中进行重定位
                   if (e.next == null)
                       newTab[e.hash & (newCap - 1)] = e;
                   // 若节点是　TreeNode 节点，要进行 红黑树的 rehash　操作
                   else if (e instanceof TreeNode)
                       ((TreeNode<K,V>)e).split(this, newTab, j, oldCap);
                   // 若是链表，进行链表的 rehash　操作
                   else { // preserve order
                       Node<K,V> loHead = null, loTail = null;
                       Node<K,V> hiHead = null, hiTail = null;
                       Node<K,V> next;
                       // 将同一桶中的元素根据(e.hash & oldCap)是否为0进行分割（代码后有图解，可以回过头再来看），分成两个不同的链表，完成rehash
                       do {
                           next = e.next;
                           // 根据算法　e.hash & oldCap 判断节点位置rehash　后是否发生改变
                           //最高位==0，这是索引不变的链表。
                           if ((e.hash & oldCap) == 0) { 
                               if (loTail == null)
                                   loHead = e;
                               else
                                   loTail.next = e;
                               loTail = e;
                           }
                           //最高位==1 （这是索引发生改变的链表）
                           else {  
                               if (hiTail == null)
                                   hiHead = e;
                               else
                                   hiTail.next = e;
                               hiTail = e;
                           }
                       } while ((e = next) != null);
                       if (loTail != null) {  // 原bucket位置的尾指针不为空(即还有node)  
                           loTail.next = null; // 链表最后得有个null
                           newTab[j] = loHead; // 链表头指针放在新桶的相同下标(j)处
                       }
                       if (hiTail != null) {
                           hiTail.next = null;
                           // rehash　后节点新的位置一定为原来基础上加上　oldCap，具体解释看下图
                           newTab[j + oldCap] = hiHead;
                       }
                   }
               }
           }
       }
       return newTab;
   }
}
```



#### get



#### HashMap线程不安全的点

初始化

put

resize



#### HashMap如何同步

```java
Map m = Collections.synchronizeMap(hashMap);
```



### LinkedHashMap

源码示例

```java
public class HashMap<K,V> extends AbstractMap<K,V>
    implements Map<K,V>, Cloneable, Serializable {
    
    // Callbacks to allow LinkedHashMap post-actions
    // 专门预留给LinkedHashMap的
    void afterNodeAccess(Node<K,V> p) { }
    
    //回调函数，新节点插入之后回调 ， 根据evict 和   判断是否需要删除最老插入的节点。
    void afterNodeInsertion(boolean evict) { }
    void afterNodeRemoval(Node<K,V> p) { }
}
```



```java
public class LinkedHashMap<K,V>
    extends HashMap<K,V>
    implements Map<K,V> {
	
	static class Entry<K,V> extends HashMap.Node<K,V> {
        Entry<K,V> before, after;
        Entry(int hash, K key, V value, Node<K,V> next) {
            super(hash, key, value, next);
        }
    }

    private static final long serialVersionUID = 3801124242820219131L;

    /**
     * The head (eldest) of the doubly linked list.
     */
    transient LinkedHashMap.Entry<K,V> head;

    /**
     * The tail (youngest) of the doubly linked list.
     */
    transient LinkedHashMap.Entry<K,V> tail;

    /**
     * The iteration ordering method for this linked hash map: <tt>true</tt>
     * for access-order, <tt>false</tt> for insertion-order.
     *
     *  定义了排序模式，对于访问顺序，为 true；对于插入顺序，则为 false。
     * 
     * @serial
     */
    final boolean accessOrder;
    
    
    //在构建新节点时，构建的是`LinkedHashMap.Entry` 不再是`Node`.
    Node<K,V> newNode(int hash, K key, V value, Node<K,V> e) {
        LinkedHashMap.Entry<K,V> p =
            new LinkedHashMap.Entry<K,V>(hash, key, value, e);
        linkNodeLast(p);
        return p;
    }
    
    
    // link at the end of list //将新增的节点，连接在链表的尾部
    private void linkNodeLast(LinkedHashMap.Entry<K,V> p) {
        LinkedHashMap.Entry<K,V> last = tail;
        tail = p;
        if (last == null)
            head = p;
        else {
            p.before = last;
            last.after = p;
        }
    }
    
    // 回调函数。HashMap中removeNode()中会调用 给LinkedHashMap预留的afterNodeRemoval方法
    // 该方法的作用是：当map中删除node节点后，LinkedHashMap需要将 before after 指针置空
    void afterNodeRemoval(Node<K,V> e) { // unlink
        LinkedHashMap.Entry<K,V> p =
            (LinkedHashMap.Entry<K,V>)e, b = p.before, a = p.after;
        p.before = p.after = null;
        if (b == null)
            head = a;
        else
            b.after = a;
        if (a == null)
            tail = b;
        else
            a.before = b;
    }

	//回调函数，新节点插入之后回调 ， 根据 evict 判断是否需要删除最老插入的节点。
    void afterNodeInsertion(boolean evict) { // possibly remove eldest
        LinkedHashMap.Entry<K,V> first;
        if (evict && (first = head) != null && removeEldestEntry(first)) {
            K key = first.key;
            removeNode(hash(key), key, null, false, true);
        }
    }

	// 回调函数，根据LinkedHashMap的排序模式accessOrder，判断是否需要将该节点移动到双向链表的尾部
    void afterNodeAccess(Node<K,V> e) { // move node to last
        LinkedHashMap.Entry<K,V> last;
        if (accessOrder && (last = tail) != e) {
            LinkedHashMap.Entry<K,V> p =
                (LinkedHashMap.Entry<K,V>)e, b = p.before, a = p.after;
            p.after = null;
            if (b == null)
                head = a;
            else
                b.after = a;
            if (a != null)
                a.before = b;
            else
                last = b;
            if (last == null)
                head = p;
            else {
                p.before = last;
                last.after = p;
            }
            tail = p;
            ++modCount;
        }
    }
}
```

使用示例

```java
public class LinkedHashMapTest {

    public static void main(String[] args) {

        System.out.println("accessOrder=false的情况:");
        
        Map<String, String> map = new LinkedHashMap<>();
        map.put("1", "a");
        map.put("2", "b");
        map.put("3", "c");
        map.put("4", "d");

        Iterator<Map.Entry<String, String>> iterator = map.entrySet().iterator();
        while (iterator.hasNext()) {
            System.out.println(iterator.next());
        }

        System.out.println("以下是accessOrder=true的情况:");

        map = new LinkedHashMap<>(10, 0.75f, true);
        map.put("1", "a");
        map.put("2", "b");
        map.put("3", "c");
        map.put("4", "d");
        map.get("2");	//2移动到了内部的链表末尾
        map.get("4");	//4调整至末尾
        map.put("3", "e");	//3调整至末尾
        map.put(null, null);//插入两个新的节点 null
        map.put("5", null);	//5
        iterator = map.entrySet().iterator();
        while (iterator.hasNext()) {
            System.out.println(iterator.next());
        }

    }
}
```



- LinkedHashMap对于HashMap的修改



`LinkedHashMap`并没有重写任何`put`()方法。但是其重写了构建新节点的`newNode`()方法.
`newNode`()会在`HashMap`的putVal()方法里被调用，`putVal`()方法会在批量插入数据`putMapEntries(Map<? extends K, ? extends V> m, boolean evict)`或者插入单个数据`public V put(K key, V value)`时被调用。



### TreeMap

基本概念：

- TreeMap集合是基于红黑树（Red-Black tree）的 `NavigableMap`实现。该集合最重要的特点就是**可排序**，该映射根据其键的自然顺序进行排序，或者根据创建映射时提供的 `Comparator` 进行排序，具体取决于使用的构造方法。
- 要想使用TreeMap存储并排序我们自定义的类（如`User`类），那么必须自己定义比较机制：
  - 一种方式是User类去实现`java.lang.Comparable`接口，并实现其`compareTo`()方法。
  - 另一种方式是写一个类（如`MyCompatator`）去实现`java.util.Comparator`接口，并实现compare()方法，然后将`MyCompatator`类实例对象作为TreeMap的构造方法参数进行传参（当然也可以使用匿名内部类）



```java
public class TreeMap<K,V>
    extends AbstractMap<K,V>
    implements NavigableMap<K,V>, Cloneable, java.io.Serializable {

    // 比较器对象
    private final Comparator<? super K> comparator;

	// 根节点
    private transient Entry<K,V> root;

    // 集合大小
    private transient int size = 0;

    // 树结构被修改的次数
    private transient int modCount = 0;
    
    
    // 静态内部类用来表示节点类型
    static final class Entry<K,V> implements Map.Entry<K,V> {
        K key;     // 键
        V value;   // 值
        Entry<K,V> left;    // 指向左子树的引用（指针）
        Entry<K,V> right;   // 指向右子树的引用（指针）
        Entry<K,V> parent;  // 指向父节点的引用（指针）
        boolean color = BLACK; // 
    }
    
    public TreeMap() {
        comparator = null;
    }
    
    public TreeMap(Comparator<? super K> comparator) {
        this.comparator = comparator;
    }

	public TreeMap(Map<? extends K, ? extends V> m) {
        comparator = null;
        putAll(m);
    }
    
    public TreeMap(SortedMap<K, ? extends V> m) {
        comparator = m.comparator();
        try {
            buildFromSorted(m.size(), m.entrySet().iterator(), null, null);
        } catch (java.io.IOException cannotHappen) {
        } catch (ClassNotFoundException cannotHappen) {
        }
    }
    
    public V put(K key, V value) {
        Entry<K,V> t = root;  // 获取根节点

        // 如果根节点为空，则该元素置为根节点 
        if (t == null) {
            compare(key, key); // type (and possibly null) check

            root = new Entry<>(key, value, null);
            size = 1;    // 集合大小为1
            modCount++;  // 结构修改次数自增
            return null;
        }

        int cmp;
        Entry<K,V> parent;
        Comparator<? super K> cpr = comparator;  // 比较器对象

        // 如果比较器对象不为空，也就是自定义了比较器
        if (cpr != null) {   
            do { // 循环比较并确定元素应插入的位置(也就是找到该元素的父节点)
                parent = t;  // t就是root

                // 调用比较器对象的compare()方法，该方法返回一个整数
                cmp = cpr.compare(key, t.key); 
                if (cmp < 0)      // 待插入元素的key"小于"当前位置元素的key，则查询左子树
                    t = t.left;
                else if (cmp > 0) // 待插入元素的key"大于"当前位置元素的key，则查询右子树
                    t = t.right;
                else              // "相等"则替换其value。
                    return t.setValue(value);
            } while (t != null);
        }

        // 如果比较器对象为空，使用默认的比较机制
        else {
            if (key == null)
                throw new NullPointerException();
            @SuppressWarnings("unchecked")
                Comparable<? super K> k = (Comparable<? super K>) key; // 取出比较器对象
            do {  // 同样是循环比较并确定元素应插入的位置(也就是找到该元素的父节点)
                parent = t;
                cmp = k.compareTo(t.key); // 同样调用比较方法并返回一个整数
                if (cmp < 0)       // 待插入元素的key"小于"当前位置元素的key，则查询左子树
                    t = t.left;
                else if (cmp > 0)  // 待插入元素的key"大于"当前位置元素的key，则查询右子树
                    t = t.right;
                else               // "相等"则替换其value。
                    return t.setValue(value);
            } while (t != null);
        }

        Entry<K,V> e = new Entry<>(key, value, parent);  // 根据key找到父节点后新建一个节点
        if (cmp < 0)  // 根据比较的结果来确定放在左子树还是右子树
            parent.left = e;
        else
            parent.right = e;
        fixAfterInsertion(e);
        size++;      // 集合大小+1
        modCount++;  // 集合结构被修改次数+1
        return null;
    }


	// 以getEntry()方法为基础的获取元素的方法，其中包括contains()，containsKey()，get()，remove()，replace等。
	
	final Entry<K,V> getEntry(Object key) {
        // 如果有自定义比较器对象，就按照自定义规则遍历二叉树
        if (comparator != null)
            return getEntryUsingComparator(key);
        if (key == null)
            throw new NullPointerException();
        @SuppressWarnings("unchecked")
            Comparable<? super K> k = (Comparable<? super K>) key;
        Entry<K,V> p = root;
        while (p != null) {    // 按照默认比较规则遍历二叉树
            int cmp = k.compareTo(p.key);
            if (cmp < 0)
                p = p.left;
            else if (cmp > 0)
                p = p.right;
            else
                return p;
        }
        return null;
    }

	// 以getFirstEntry()，getLastEntry()为基础的获取头和尾元素的方法，其中包括：firstKey()，lastKey()；firstEntry()，lastEntry()；pollFirstEntry()，pollLastEntry()；
	
	final Entry<K,V> getFirstEntry() { // 获取第一个元素也就是最小的元素，一直遍历左子树
        Entry<K,V> p = root;
        if (p != null)
            while (p.left != null)
                p = p.left;
        return p;
    }

    final Entry<K,V> getLastEntry() { // 获取最后个元素也就是最大的元素，一直遍历右子树
        Entry<K,V> p = root;
        if (p != null)
            while (p.right != null)
                p = p.right;
        return p;
    }

}
```



### NavigableMap

- `NavigableMap` 继承了 `SortedMap`，提供了关于搜索的更多方法



```java
public interface NavigableMap<K,V> extends SortedMap<K,V> {
 
    //返回第一个key小于参数的Entry
    Map.Entry<K,V> lowerEntry(K key);

    //返回第一个key小于参数的key
    K lowerKey(K key);

    //返回第一个key小于等于参数的Entry
    Map.Entry<K,V> floorEntry(K key);

    //返回第一个key小于等于参数的key
    K floorKey(K key);

    //返回第一个key大于等于参数的Entry
    Map.Entry<K,V> ceilingEntry(K key);

    //返回第一个key大于等于参数的key
    K ceilingKey(K key);

    //返回第一个key大于参数的Entry
    Map.Entry<K,V> higherEntry(K key);

    //返回第一个key大于参数的key
    K higherKey(K key);

    //返回key最小的Entry
    Map.Entry<K,V> firstEntry();

    //返回key最大的Entry
    Map.Entry<K,V> lastEntry();

    //删除并返回key最小的Entry
    Map.Entry<K,V> pollFirstEntry();

    //删除并返回key最大的Entry
    Map.Entry<K,V> pollLastEntry();

    //返回key降序排列的NavigableMap
    NavigableMap<K,V> descendingMap();

    //返回key升序排列的NavigableSet
    NavigableSet<K> navigableKeySet();

    //返回key降序排列的NavigableSet
    NavigableSet<K> descendingKeySet();

    //返回key升序排列的子映射，设置包含标志
    NavigableMap<K,V> subMap(K fromKey, boolean fromInclusive, K toKey, boolean toInclusive);

    //按key升序排列，返回子映射，开头到toKey，设置包含标志
    NavigableMap<K,V> headMap(K toKey, boolean inclusive);

    //按key升序排列，返回子映射，fromKey到末尾，设置包含标志
    NavigableMap<K,V> tailMap(K fromKey, boolean inclusive);

    //同时也继承了SortedMap的不带包含标志的子映射方法
    SortedMap<K,V> subMap(K fromKey, K toKey);
    SortedMap<K,V> headMap(K toKey);
    SortedMap<K,V> tailMap(K fromKey);
    
} 
```





### HashTable

线程安全的hashMap，通过`synchornized`实现

```java
public class Hashtable<K,V>
    extends Dictionary<K,V>
    implements Map<K,V>, Cloneable, java.io.Serializable {

    /**
     * The hash table data.
     */
    private transient Entry<?,?>[] table;

    /**
     * The total number of entries in the hash table.
     */
    private transient int count;

    /**
     * The table is rehashed when its size exceeds this threshold.  (The
     * value of this field is (int)(capacity * loadFactor).)
     *
     * @serial
     */
    private int threshold;

    /**
     * The load factor for the hashtable.
     *
     * @serial
     */
    private float loadFactor;

    /**
     * The number of times this Hashtable has been structurally modified
     * Structural modifications are those that change the number of entries in
     * the Hashtable or otherwise modify its internal structure (e.g.,
     * rehash).  This field is used to make iterators on Collection-views of
     * the Hashtable fail-fast.  (See ConcurrentModificationException).
     */
    private transient int modCount = 0;

    /** use serialVersionUID from JDK 1.0.2 for interoperability */
    private static final long serialVersionUID = 1421746759512286392L;

    public Hashtable(int initialCapacity, float loadFactor) {
        if (initialCapacity < 0)
            throw new IllegalArgumentException("Illegal Capacity: "+
                                               initialCapacity);
        if (loadFactor <= 0 || Float.isNaN(loadFactor))
            throw new IllegalArgumentException("Illegal Load: "+loadFactor);

        if (initialCapacity==0)
            initialCapacity = 1;
        this.loadFactor = loadFactor;
        table = new Entry<?,?>[initialCapacity];
        threshold = (int)Math.min(initialCapacity * loadFactor, MAX_ARRAY_SIZE + 1);
    }

    public Hashtable(int initialCapacity) {
        this(initialCapacity, 0.75f);
    }

    /**
     * Constructs a new, empty hashtable with a default initial capacity (11)
     * and load factor (0.75).
     */
    public Hashtable() {
        this(11, 0.75f);
    }

    public Hashtable(Map<? extends K, ? extends V> t) {
        this(Math.max(2*t.size(), 11), 0.75f);
        putAll(t);
    }
}
```







## 4.Queue



### PriorityQueue



### ArrayDeque



### LinkedList



## 5.Deque

双端队列，队首和队尾都可以进行“出队”和“入队”操作。

| 操作类型 | 抛出异常      | 返回特殊值    |
| :------- | :------------ | :------------ |
| 队首入队 | addFirst(e)   | offerFirst(e) |
| 队首出队 | removeFirst() | pollFirst()   |
| 队首读取 | getFirst()    | peekFirst()   |
| 队尾入队 | addLast(e)    | offerLast(e)  |
| 队尾出队 | removeLast()  | pollLast()    |
| 队尾读取 | getLast()     | peekLast()    |



除此之外，`Deque`还可以当作“栈”来使用，我们知道“栈”是一种具有**“LIFO”**特点的数据结构，`Deque`提供了`push`、`pop`、`peek`这三个栈方法，一般实现这三个方法时，可以利用已有方法，即有如下映射关系：

| 栈方法 | Deque方法     |
| :----- | :------------ |
| push   | addFirst(e)   |
| pop    | removeFirst() |
| peek   | peekFirst()   |



## 6.Stack

- 先进先出的数据结构



```java
public class Stack<E> extends Vector<E> {

    public Stack() {
    }

    public E push(E item) {
        addElement(item);

        return item;
    }

    public synchronized E pop() {
        E       obj;
        int     len = size();

        obj = peek();
        removeElementAt(len - 1);

        return obj;
    }

    public synchronized E peek() {
        int     len = size();

        if (len == 0)
            throw new EmptyStackException();
        return elementAt(len - 1);
    }

    public boolean empty() {
        return size() == 0;
    }

    public synchronized int search(Object o) {
        int i = lastIndexOf(o);

        if (i >= 0) {
            return size() - i;
        }
        return -1;
    }

    private static final long serialVersionUID = 1224463164541339165L;
}
```





# 集合操作工具类



## Arrays



## Collections



# java并发同步集合

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


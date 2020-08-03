# java中的==

基本类型：比较的是值

引用类型：比较的是虚拟内存地址

# java中的equals

JAVA当中所有的类都是继承于Object这个超类的，在Object类中定义了一个equals的方法

```java
public boolean equals(Object obj) {
    //this - s1
    //obj - s2
    return (this == obj);
}
```

<font color=#00dd00>可以看到，这个方法的初始默认行为是比较对象的内存地址值，一般来说，意义不大。所以，在一些类库当中这个方法被重写了，如String、Integer、Date。在这些类当中equals有其自身的实现（一般都是用来比较对象的成员变量值是否相同），而不再是比较类在堆内存中的存放地址了。</font>

## equals()的重写规则

- 自反性。对于任何非null的引用值x，x.equals(x)应返回true。

- 对称性。对于任何非null的引用值x与y，当且仅当：y.equals(x)返回true时，x.equals(y)才返回true。

- 传递性。对于任何非null的引用值x、y与z，如果y.equals(x)返回true，y.equals(z)返回true，那么x.equals(z)也应返回true。

- 一致性。对于任何非null的引用值x与y，假设对象上equals比较中的信息没有被修改，则多次调用x.equals(y)始终返回true或者始终返回false。

- 对于任何非空引用值x，x.equal(null)应返回false。



# java中的hashCode

在java中，我们可以使用hashCode()来获取对象的哈希码，其值就是对象的存储地址，这个方法在Object类中声明，因此所有的子类都含有该方法。

```java
public class Model {
	private String name;
	private double salary;
	private int sex;
	
	@Override
	public int hashCode() {
		return name.hashCode()+new Double(salary).hashCode() 
				+ new Integer(sex).hashCode();
	}
}
```



## java 7中对hashCode方法做了两个改进

使用更加安全的调用方式来返回散列码，也就是使用null安全的方法Objects.hashCode（注意不是Object而是java.util.Objects）方法，这个方法的优点是如果参数为null，就只返回0，否则返回对象参数调用的hashCode的结果。

### java.util.Objects

```java
public static int hashCode(Object o) {
    return o != null ? o.hashCode() : 0;
}
```

```java
import java.util.Objects;

public  class Model {
	private   String name;
	private double salary;
	private int sex;
	@Override
	public int hashCode() {
		return Objects.hashCode(name)+new Double(salary).hashCode() 
				+ new Integer(sex).hashCode();
	}
}
```



### java.util.Objects.hash(Object... objects)

java 7还提供了另外一个方法java.util.Objects.hash(Object... objects),当我们需要组合多个散列值时可以调用该方法

```JAVA
import java.util.Objects;

public  class Model {
	private String name;
	private double salary;
	private int sex;

	@Override
	public int hashCode() {
		return Objects.hash(name,salary,sex);
	}
}
```

## 数组类型的变量

可以调用Arrays.hashCode()来计算它的散列码，这个散列码是由数组元素的散列码组成的。
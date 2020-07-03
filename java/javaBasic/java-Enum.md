# 定义

- 枚举类型是一种特殊数据类型，能够为一个变量定义一组预定义的常量。变量必须等于为其预定义的值之一。

- 枚举是特殊的类，可以拥有成员变量和方法。

```java
public enum Direction {
	NORTH, SOUTH, EAST, WEST
}
```

在开发中枚举可以用来定义常量

```java
public enum ServiceCode {
    SYSTEM_NORMAL(200, "系统正常"),
    
    INVALID_PARAM(401,"无效参数"),

    SERVICE_ERROR(500, "服务异常");

    private int code;
    private String desc;

    ServiceCode(int code, String desc) {
        this.code = code;
        this.desc = desc;
    }

    public int getCode() {
        return code;
    }

    public void setCode(int code) {
        this.code = code;
    }

    public String getDesc() {
        return desc;
    }

    public void setDesc(String desc) {
        this.desc = desc;
    }
}
```

# 实现

> 1.枚举类型都隐式继承了`java.lang.Enum`类，因此不能继承其他类，但可以实现接口；  
>
> 2.枚举类型只能有私有的构造方法(`java`运行时创建，外部不能进行实例化)；  
>
> 3.不能泛型化；
>
> 4.当有字段和方法时，枚举常量的列表必须以分号结尾；  
>
> 5.编译器在创建枚举时会自动添加一些特殊方法。例如，它们具有一个静态values方法。
>
> 6.父类`Enum`中的方法



通过`javap`命令反编译`class`文件可以看出，并且它是被`final`修饰的，不能被继承。

```shell
$ javac ServiceCode.java

$ javap ServiceCode.class
```



```java
public final class com.example.demo.enums.ServiceCode extends java.lang.Enum<com.example.demo.enums.ServiceCode> {
    public static final com.example.demo.enums.ServiceCode SYSTEM_NORMAL;
    public static final com.example.demo.enums.ServiceCode INVALID_PARAM;
    public static final com.example.demo.enums.ServiceCode SERVICE_ERROR;
    public static com.example.demo.enums.ServiceCode[] values();
    public static com.example.demo.enums.ServiceCode valueOf(java.lang.String);
    public int getCode();
    public void setCode(int);
    public java.lang.String getDesc();
    public void setDesc(java.lang.String);
    static {};
}
```


# 常用方法

## compareTo()

用于与指定枚举对象比较顺序，同一个枚举实例只能与相同类型的枚举实例比较。如果该枚举对象位于指定枚举对象之后，则返回正整数；反之返回负整数；否则返回零；

## name()

返回此枚举实例的名称，即枚举值 ；

## values()

返回一个包含全部枚举值的数组，可以用来遍历所有枚举值

## ordinal()

返回枚举值在枚举类中的索引值(从0开始)，即枚举值在枚举声明中的顺序，这个顺序根据枚举值声明的顺序而定；

## toString()

返回枚举值的名称，与 name 方法类似

## valueOf()

返回带指定名称的指定枚举类型的枚举常量，名称必须与在此类型中声明枚举常量所用的标识符完全匹配(不允许使用额外的空白字符)。这个方法与toString相对应，因此重写 toString() 方法，一定要重写 valueOf() 方法(我们可以重写 toString() 方法，但不能自己重写 valueOf() 方法，当我们重写 toString() 方法时，valueOf() 方法会自动重写，不用我们理会。)；

## equals()

比较两个枚举类对象的引用。



# EnumSet和EnumMap

## EnumSet

- EnumSet这是一个用来操作Enum的集合，是一个抽象类，它有两个继承类：JumboEnumSet和RegularEnumSet。在使用的时候，需要制定枚举类型。 它的特点也是速度快。
​ 
- EnumSet的元素不允许为null；EnumSet非线程安全。

```java
EnumSet<Color> colorSet = EnumSet.allOf(Color.class);
	
	for (Color color : colorSet) {
	    System.out.println(color.getName());
	}
```

## EnumMap

- EnumMap是Map的实现类。它的key为枚举元素，value自定义。

- EnumMap内部使用数组来实现。

- EnumMap的key不允许为null，value可以为null，按照key在enum中的顺序进行保存，非线程安全。  
  《Effective JAVA》中作者建议用EnumMap代替叙述 索引，最好不要用序数来索引数组，而要使用EnumMap。

```java
EnumMap<Color, String> enumMap =  new EnumMap<Color, String>(Color.class);
	
	enumMap.put(Color.RED, "red");
	enumMap.put(Color.BULE, "bule");
	for (Map.Entry<Color, String> entry: enumMap.entrySet()){
	    System.out.println(entry.getKey().name() + ":" + entry.getValue());
	}
```

# 枚举实现单例模式

- 它不仅能避免多线程同步问题，而且还能防止反序列化重新创建新的对象。  
 如果用枚举去实现一个单例，这样的加载有点类似于饿汉模式，并没有起到lazy-loading的作用。

```java
public enum Singleton {
    /**
	* 定义一个枚举的元素，它就代表了Singleton的一个实例。
	*/

    uniqueInstance;
    /**
	* 单例可以有自己的操作
	*/
    public void otherMethods(){
        //功能处理
    }
}
```
# 1.定义

- 枚举类型是一种特殊数据类型，能够为一个变量定义一组预定义的常量。变量必须等于为其预定义的值之一。

- 枚举是特殊的类，可以拥有成员变量和方法。

# 2.使用

- demo1

```

	public enum Direction {
	    //实例列表末尾的分号是可选的
	    NORTH, SOUTH, EAST, WEST
	}
```

- demo2

在开发中枚举可以用来定义常量

```

	public enum ServiceCode {
	    SYSTEM_NORMAL(200, "系统正常"),
	    //异常 返回码范围 400 -419 业务无关异常
	    INVALID_PARAM(401,"无效参数"),
	    //服务端异常
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

# 3.详解和注意

```

	1.枚举类型都隐式继承了java.lang.Enum类，因此不能继承其他类，但可以实现接口；  

	​2.枚举类型只能有私有的构造方法(java运行时创建，外部不能进行实例化)；  

	​3.不能泛型化；  

	​4.当有字段和方法时，枚举常量的列表必须以分号结尾；  

	​5.编译器在创建枚举时会自动添加一些特殊方法。例如，它们具有一个静态values方法。

	6.父类Enum中的方法
```

- 通过javap命令反编译class文件可以看出，并且它是被final修饰的，不能被继承。

```

	$ javap Color.class
	​   Compiled from "Color.java"
	​   public final class com.zuoquan.lt.basic.enums.Color extends java.lang.Enum<com.zuoquan.lt.basic.enums.Color> {
	public static final com.zuoquan.lt.basic.enums.Color RED;
	public static final com.zuoquan.lt.basic.enums.Color GREEN;
	public static final com.zuoquan.lt.basic.enums.Color BULE;
	public static com.zuoquan.lt.basic.enums.Color[] values();
	public static com.zuoquan.lt.basic.enums.Color valueOf(java.lang.String);
	public int getIndex();
	public void setIndex(int);
	public java.lang.String getName();
	public void setName(java.lang.String);
	static {};
	​   }
```
# EnumSet和EnumMap

## EnumSet

- EnumSet这是一个用来操作Enum的集合，是一个抽象类，它有两个继承类：JumboEnumSet和RegularEnumSet。在使用的时候，需要制定枚举类型。 它的特点也是速度快。
​ 
- EnumSet的元素不允许为null；EnumSet非线程安全。

```

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

```

	 //EnumMap
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

```

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
# java常用类之BigDecimal

- 描述： float和double类型没有提供完全精确的结果，所以不应该被用于要求精确结果的场合。这时候BigDecimal就派上大用场了。

## 构造方法

```java

	1.public BigDecimal(double val)    将double表示形式转换为BigDecimal *不建议使用
	2.public BigDecimal(int val)　　将int表示形式转换成BigDecimal
	3.public BigDecimal(String val)　　将String表示形式转换成BigDecimal

### 为什么不建议采用第一种构造方法

```java

	public static void main(String[] args)
    {
        BigDecimal bigDecimal = new BigDecimal(2);
        BigDecimal bDouble = new BigDecimal(2.3);
        BigDecimal bString = new BigDecimal("2.3");
        System.out.println("bigDecimal=" + bigDecimal);
        System.out.println("bDouble=" + bDouble);
        System.out.println("bString=" + bString);
    }

	bigDecimal = 2;
	bDouble = 2.9999999999998223641......6875
	bString = 2.3

- 建议：通常建议优先使用String构造方法。

### double必须用作BigDecimal的源

- 当double必须用作BigDecimal的源时，请使用Double.toString(double)转成String，然后使用String构造方法，或使用BigDecimal的静态方法valueOf

```java

	public static void main(String[] args)
    {
        BigDecimal bDouble1 = BigDecimal.valueOf(2.3);
        BigDecimal bDouble2 = new BigDecimal(Double.toString(2.3));

        System.out.println("bDouble1=" + bDouble1);
        System.out.println("bDouble2=" + bDouble2);
        
    }

	bDouble = 2.3;
	bDouble = 2.3;

## BigDecimal加减乘除运算

```java

	public BigDecimal add(BigDecimal value);                        //加法

	public BigDecimal subtract(BigDecimal value);                   //减法 
	
	public BigDecimal multiply(BigDecimal value);                   //乘法
	
	public BigDecimal divide(BigDecimal value);                     //除法

### 注意

- BigDecimal除法可能出现不能整除的情况，比如 4.5/1.3，这时会报错  
  java.lang.ArithmeticException: Non-terminating decimal expansion; no exact representable decimal result.

- 其实divide方法有可以传三个参数 

	public BigDecimal divide(BigDecimal divisor, int scale, int roundingMode)
   
第一参数表示除数  
第二个参数表示小数点后保留位数    
第三个参数表示舍入模式，只有在作除法运算或四舍五入时才用到舍入模式，有下面这几种


- roundingMode:

```java

	ROUND_CEILING    //向正无穷方向舍入
	
	ROUND_DOWN    //向零方向舍入
	
	ROUND_FLOOR    //向负无穷方向舍入
	
	ROUND_HALF_DOWN    //向（距离）最近的一边舍入，除非两边（的距离）是相等,如果是这样，向下舍入, 例如1.55 保留一位小数结果为1.5
	
	ROUND_HALF_EVEN    //向（距离）最近的一边舍入，除非两边（的距离）是相等,如果是这样，如果保留位数是奇数，使用ROUND_HALF_UP，如果是偶数，使用ROUND_HALF_DOWN
	
	ROUND_HALF_UP    //向（距离）最近的一边舍入，除非两边（的距离）是相等,如果是这样，向上舍入, 1.55保留一位小数结果为1.6
	
	ROUND_UNNECESSARY    //计算结果是精确的，不需要舍入模式
	
	ROUND_UP    //向远离0的方向舍入

### setScale

- 需要对BigDecimal进行截断和四舍五入可用setScale方法

```java

	public static void main(String[] args)
    {
        BigDecimal a = new BigDecimal("4.5635");

        a = a.setScale(3, RoundingMode.HALF_UP);    //保留3位小数，且四舍五入
        System.out.println(a);
    }

### 注意点

- 减乘除其实最终都返回的是一个新的BigDecimal对象，因为BigInteger与BigDecimal都是不可变的（immutable）的，在进行每一步运算时，都会产生一个新的对象

```java

	public static void main(String[] args)
    {
        BigDecimal a = new BigDecimal("4.5");
        BigDecimal b = new BigDecimal("1.5");
        a.add(b);

        System.out.println(a);  //输出4.5. 加减乘除方法会返回一个新的BigDecimal对象，原来的a不变


    }

## 总结

- (1)商业计算使用BigDecimal。  
- (2)尽量使用参数类型为String的构造函数。  
- (3) BigDecimal都是不可变的（immutable）的，在进行每一步运算时，都会产生一个新的对象，所以在做加减乘除运算时千万要保存操作后的值。  
- (4)我们往往容易忽略JDK底层的一些实现细节，导致出现错误，需要多加注意。  
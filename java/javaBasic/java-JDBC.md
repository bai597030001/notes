# JDBC操作数据库

- 一般步骤

```java

    private static void originalJdbcTest() {
        //声明Connection对象
        Connection con;
        //驱动程序名
        String driver = "com.mysql.jdbc.Driver";
        //URL指向要访问的数据库名mydata
        String url = "jdbc:mysql://172.16.1.92:3306/GBIAP";
        //MySQL配置时的用户名
        String user = "root";
        //MySQL配置时的密码
        String password = "gsmrlab";
        //遍历查询结果集
        try {
            //加载驱动程序
            //Class.forName(driver);
            //TODO test
            DriverManager.registerDriver(new com.mysql.jdbc.Driver());
            //1.getConnection()方法，连接MySQL数据库！！
            con = DriverManager.getConnection(url, user, password);
            if (!con.isClosed()) {
                System.out.println("Succeeded connecting to the Database!");
            }
            //2.创建statement类对象，用来执行SQL语句！！
            Statement statement = con.createStatement();
            //要执行的SQL语句
            String sql = "SELECT * FROM user";
            PreparedStatement preparedStatement = con.prepareStatement(sql);
            //3.ResultSet类，用来存放获取的结果集！！
            ResultSet rs = statement.executeQuery(sql);
            rs = preparedStatement.executeQuery(sql);
            System.out.println("-----------------");
            System.out.println("执行结果如下所示:");
            System.out.println("-----------------");
            System.out.println("姓名" + "\t" + "年龄");
            System.out.println("-----------------");

            String name;
            int age;
            while (rs.next()) {
                //获取stuname这列数据
                name = rs.getString("name");
                //获取stuid这列数据
                age = rs.getInt("age");
                //输出结果
                System.out.println(name + "\t" + age);
            }
            rs.close();
            con.close();
        } catch (Exception e) {
            e.printStackTrace();
        } finally {
            System.out.println("数据库数据成功获取！！");
        }
    }
```

### Class.forName(driver)作用

- 返回的是一个类

- 作用是要求JVM查找并加载指定的类，也就是说JVM会执行该类的静态代码段。

```

	报道查看一个给定类或者接口的一个Class对象，如果没有给定classloader，那么会使用根类加载器。  

	如果initalize这个参数传了true，那么给定的类如果之前没有被初始化过，那么会被初始化。  

	我们在JDBC第一步的时候，传入的参数是com.mysql.jdbc.Driver。也就是说这个类会被初始化。  
```

- 我们看一下这个类里面的内容。

```java

	public class Driver extends NonRegisteringDriver implements java.sql.Driver {
	  
	 static {
	  try {
	   java.sql.DriverManager.registerDriver(new Driver());
	  } catch (SQLException E) {
	   throw new RuntimeException("Can't register driver!");
	  }
	 }
	  
	  
	 public Driver() throws SQLException {
	  // Required for Class.forName().newInstance()
	 }
	}
```

- 这个类也是超级简单的。一个构造函数和一个静态代码块。我们知道，类在初始化的时候，静态代码块的内容会被执行的。

- 也就是说Class.forName语句和直接写DriverManager.registerDriver(new Driver)两者功能是等同的。

- 总结

```

	Class.forName方法的作用，就是初始化给定的类。而我们给定的的MySQL的驱动程序类中，它在静态代码块中通过

	JDBC的DriverManager注册了一下驱动。我们也可以直接使用JDBC的驱动管理器注册的MySQL驱动。从而代替使用Class.forName。
```





# JDBC之DbUtils封装 

------

## jar包

```
import org.apache.commons.dbutils.QueryRunner;
import org.apache.commons.dbutils.handlers.ArrayListHandler;
import org.apache.commons.dbutils.handlers.MapListHandler;
```

## DbUtils三个核心类

### 1.DbUtils

- 连接数据库对象----jdbc辅助方法的集合类，**线程安全**  
- 构造方法：DbUtils()  
- 作用：控制连接，控制...，控制驱动加载的一个类。  

```
	* close ：关闭Connection 、Statement 或ResultSet ，忽略null； 

	* closeQuietly ：安静的关闭Connection 、Statement 或ResultSet ，它会尽力去关闭，忽略null和SQLException ； 

	* commitAndClose ：提交并关闭Connection ，忽略null； 

	* commitAndCloseQuietly ：提交并关闭Connection ，忽略null和SQLException ； 

	* loadDriver ：和Class.forName().newInstance() 功能一样，但它能捕获异常，并返回true 或false ； 

	* printStackTrace ：打印SQLException 的详细错误信息； 

	* printWarnings ：打印Connection 的警告信息； 

	* rollback ：回滚操作，忽略null； 

	* rollbackAndClose ：回滚操作并关闭，忽略null； 

	* rollbackAndCloseQuietly ：回滚操作并关闭，忽略null、SQLException 。
```

### 2.QueryRunner

- SQL语句的操作对象，可以设置查询结果集的封装策略，**线程安全**。

- 构造方法：  
  （1）QueryRunner()：创建一个与数据库无关的QueryRunner对象，后期再操作数据库的话，需要手动给一个Connection对象，它可以手动控制事务。  
  Connection.setAutoCommit(false);     设置手动管理事务  
  Connection.commit();     提交事务  

  （2）QueryRunner(DataSource ds)：创建一个与数据库关联的queryRunner对象，后期再操作数据库的时候，不需要Connection对象，自动管理事务。  
            DataSource：数据库连接池对象。  

- 构造函数与增删改查方法的组合：

```
	 QueryRunner()  
	       update(Connection conn, String sql, Object... params)  
	       query(Connection conn, String sql, ResultSetHandler<T> rsh, Object... params)  
	
	 QueryRunner(DataSource ds)  
	       update(String sql, Object... params)  
	       query(String sql, ResultSetHandler<T> rsh, Object... params)
```

```
* batch ：执行成批的INSERT、UPDATE、DELETE操作； 

* fillStatement ：用Object[] 或JavaBean的值填充PreparedStatement 中的占位符； 

* query ：执行查询操作（Statement 或PreparedStatement 均可），并用ResultSetHandler 来处理ResultSet ； 

* update ：执行INSERT或UPDATE操作（Statement 或PreparedStatement 均可）。 
```

### 3.结果集处理器

#### 3.1 顶层接口 ResultSetHandle

```
	public interface ResultSetHandler<T> {
	    T handle(ResultSet var1) throws SQLException;
	}

```

- 封装数据的策略对象------将封装结果集中的数据，转换到另一个对象
- 策略：封装数据到对象的方式（示例：将数据库保存在User、保存到数组、保存到集合）
- 方法介绍：handle（ResultSet rs）

#### 3.2 DbUtils给我们提供的ResultSetHandler实现类

##### ArrayHandler

- 将查询结果的**第一行**数据，保存到**Object数组**中

##### ArrayListHandler

- 将查询的结果，**每一行先封装到Object数组**中，然后将数据**存入List集合**

##### BeanHandler

- 将查询结果的**第一行数据**，封装到**javaBean对象**

##### BeanListHandler

- 将查询结果的**每一行封装到javaBean对象，然后再存入List集合**

##### ColumnListHandler

- 将查询结果的**指定列**的数据封装到**List集合**中

##### MapHandler

- 将查询结果的**第一行**数据封装到**map结合**（key==列名，value==列值）

##### MapListHandler

- 将查询结果的**每一行**封装到**map集合**（key==列名，value==列值），再**将map集合存入List集合**

##### BeanMapHandler

- 将查询结果的**每一行**数据，封装到**javaBean对象**，再存入**map集合**中（key==列名，value==列值）

##### KeyedHandler

- 将查询的结果的**每一行**数据，封装到**map1**（key==列名，value==列值 ），然后**将map1集合（有多个）存入map2集合（只有一个）**

##### ScalarHandler

- 封装类似count、avg、max、min、sum......函数的执行结果

##### 示例

```
@Test
public void test8(){
 
	//第一步：创建queryRunner对象，用来操作sql语句
	QueryRunner qr = new QueryRunner(JDBCUtils.getDataSource());
	 
	//第二步：创建sql语句
	String sql = "select * from user";
	 
	//第三步：执行sql语句,params:是sql语句的参数
	//注意，给sql语句设置参数的时候，按照user表中字段的顺序
	 
	try {
		List<User> list = qr.query(sql, new BeanListHandler<User>(User.class));
		System.out.println(list);
	} catch (SQLException e) {
		e.printStackTrace();
	}
 
}
```

## DbUtils使用事务

- 这里主要讲解空的构造函数和参数是链接池的构造函数

```
	参数是链接池的构造函数，说明我们把链接交给DBUtils来管理，事物也交给DBUtils来管理，采用的mysql默认对事物的管理方式，一条sql语言一提交

	QueryRunner(DataSource ds)

	update(String sql, Object... params)

	query(String sql, ResultSetHandler<T> rsh, Object... params)

	这三条sql语句应该是放在一起用的
```

```
	空的构造函数，链接是交给开发者来管理的，事物也是交给开发者来管理，需要手动的提交和回滚事物

	QueryRunner() -- 说明底层没有帮你管理连接，自己管理连接

	update(Connection conn, String sql, Object... params)

	query(Connection conn, String sql, ResultSetHandler<T> rsh, Object... params)

	这三条sql语句应该是放在一起使用的
```

- [https://yq.aliyun.com/articles/33705]()
- [https://blog.csdn.net/mchenys/article/details/83304866]()





# 数据源/连接池

------

## 起源

- 在JDBC中我们对于数据库的连接和释放，都是通过 DriverManager.getConnection(url, user, password); 来实现的。这种做法每次都会建立一个数据库连接进行sql操作。“获得连接”或“释放资源”是非常消耗系统资源的两个过程。这样如果操作频繁，势必会导致连接、查询效率低下。
- 为了解决此类性能问题，通常情况我们采用连接池技术，来共享连接Connection。这样我们就不需要每次都创建连接、释放连接了，这些操作都交给了连接池。

## java之连接池规范

- Java为数据库连接池提供了公共的接口：javax.sql.DataSource，各个厂商需要让自己的连接池实现这个接口。这样应用程序可以方便的切换不同厂商的连接池！常见的连接池：DBCP、C3P0、durid。

## 连接池/数据源定义

```
用池来管理Connection，这样可以重复使用Connection。有了池，所以我们就不用自己来创建Connection，

而是通过池来获取Connection对象。当使用完Connection后，调用Connection的close()方法也不会真的

关闭Connection，而是把Connection“归还”给池。池就可以再利用这个Connection对象了。
```

![](E:/notes/java/javaBasic/JDBC/img/jdbc1.png)

### C3P0

- 3种配置方式

#### 1.ComboPooledDataSource.setXXX

```java
	package C3P0; 
	import java.sql.Connection; 
	import java.sql.SQLException; 
	import java.beans.PropertyVetoException; 
	import com.mchange.v2.c3p0.ComboPooledDataSource; 
	
	public class DBPool{       
	   private static DBPool dbPool;       
	   private ComboPooledDataSource dataSource;     

	   static {       
	           dbPool = new DBPool();       
	   }       
	
	   public DBPool(){       
	           try {       
	                 dataSource = new ComboPooledDataSource();       
	                 dataSource.setUser("id");       
	                 dataSource.setPassword("pw");       
	                 dataSource.setJdbcUrl("jdbc:mysql://127.0.0.1:3306/test? 
	autoReconnect=true&useUnicode=true&characterEncoding=GB2312"); 
	                 dataSource.setDriverClass("com.mysql.jdbc.Driver"); 	
	           } catch (PropertyVetoException e) {       
	               throw new RuntimeException(e);       
	           }       
	   }       
	
	   public final static DBPool getInstance(){       
	           return dbPool;       
	   }       
	
	   public final Connection getConnection(){       
	           try {       
	               return dataSource.getConnection();       
	           }   catch (SQLException e)   {       
	               throw new RuntimeException("无法从数据源获取连接",e);       
	           }       
	   }     
		
	   public static void main(String[] args) throws SQLException { 
	        Connection con = null; 
	        try { 
	        con = DBPool.getInstance().getConnection(); 
	        } catch (Exception e){ 
	        } finally { 
	        if (con != null) 
	        con.close(); 
	        } 
	        } 
	
	}
```

#### 2.c3p0-config.xml

```xml
	<?xml version="1.0" encoding="UTF-8"?>
		<c3p0-config>
			<named-config name="userApp">
				<property name="driverClass">com.mysql.jdbc.Driver</property>
				<property name="jdbcUrl">jdbc:mysql://localhost:3306/test</property>
				<property name="user">root</property>
				<property name="password">123456</property>
				<property name="acquireIncrement">5</property>
				<property name="initialPoolSize">10</property>
				<property name="minPoolSize">10</property>
				<property name="maxPoolSize">20</property>
				<property name="maxStatements">0</property>
				</user-overrides>
			</named-config>
	</c3p0-config>
```

```java
	package cn.langzi.jdbc.c3p0;
	import java.sql.Connection;
	import java.sql.ResultSet;
	import java.sql.SQLException;
	import java.sql.Statement;
	import javax.sql.DataSource;
	import com.mchange.v2.c3p0.ComboPooledDataSource;
	
	public class DbConnection {
	    private static DataSource dataSource;
	    static{
	        dataSource = new ComboPooledDataSource("userApp");
	    }
	    public static Connection getConnectioon() throws SQLException{
	        return dataSource.getConnection();
	    }
	}
```

#### 3.加载配置文件

```java
	public final class ConnectionManager {
	
	 private static ConnectionManager instance;
	
	 public ComboPooledDataSource ds;
	
	 private static String c3p0Properties = "c3p0.properties";
	 
	 private ConnectionManager() throws Exception {
	  Properties p = new Properties();
	  p.load(this.getClass().getResourceAsStream(c3p0Properties));
	  ds = new ComboPooledDataSource();
	  ds.setUser(p.getProperty("user"));
	  ds.setPassword(p.getProperty("user"));
	  ds.setJdbcUrl(p.getProperty("user"));
	  ds.setDriverClass(p.getProperty("user"));
	  ds.setInitialPoolSize(Integer.parseInt(p.getProperty("initialPoolSize")));
	  ds.setMinPoolSize(Integer.parseInt(p.getProperty("minPoolSize")));
	  ds.setMaxPoolSize(Integer.parseInt(p.getProperty("maxPoolSize")));
	  ds.setMaxStatements(Integer.parseInt(p.getProperty("maxStatements")));
	  ds.setMaxIdleTime(Integer.parseInt(p.getProperty("maxIdleTime")));
	 }
	
	 
	
	 public static final ConnectionManager getInstance() {
	  if (instance == null) {
	   try {
	    instance = new ConnectionManager();
	   } catch (Exception e) {
	    e.printStackTrace();
	   }
	  }
	  return instance;
	 }
	
	 
	
	 public synchronized final Connection getConnection() {
	  try {
	   return ds.getConnection();
	  } catch (SQLException e) {
	   e.printStackTrace();
	  }
	  return null;
	 }
	
	 protected void finalize() throws Throwable {
	  DataSources.destroy(ds); // 关闭datasource
	  super.finalize();
	 }
	}
```

### DBCP

### Durid
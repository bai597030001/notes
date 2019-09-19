#JDBC之DbUtils封装 

--- 

## jar包

	import org.apache.commons.dbutils.QueryRunner;
	import org.apache.commons.dbutils.handlers.ArrayListHandler;
	import org.apache.commons.dbutils.handlers.MapListHandler;

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

	* batch ：执行成批的INSERT、UPDATE、DELETE操作； 

	* fillStatement ：用Object[] 或JavaBean的值填充PreparedStatement 中的占位符； 

	* query ：执行查询操作（Statement 或PreparedStatement 均可），并用ResultSetHandler 来处理ResultSet ； 

	* update ：执行INSERT或UPDATE操作（Statement 或PreparedStatement 均可）。 


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
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

### 
# Servlet详解

Servlet（Server Applet），全称Java Servlet。是用Java编写的服务器程序。其主要功能在于<font color=#00dd00>交互式地浏览和修改数据，生成动态Web内容。</font>狭义的Servlet是指Java语言实现的一个接口，广义的Servlet是指任何实现了这个Servlet接口的类，一般情况下，人们将Servlet理解为后者。 



 Servlet运行于支持Java的应用服务器中。从实现上讲，Servlet可以响应任何类型的请求，但绝大多数情况下Servlet只用来扩展基于HTTP协议的Web服务器。 



## 历史

![](img/servlet1.png)



## 工作模式

- 客户端发送请求至服务器
- 服务器启动并调用Servlet，Servlet根据客户端请求生成响应内容并将其传给服务器
- 服务器将响应返回客户端
- 其他



## 生命周期

当servlet被部署在应用服务器中（如Tomcat等）以后，由容器控制servlet的生命周期。除非特殊指定，否则在容器启动的时候，servlet是不会被加载的，servlet只会在第一次请求的时候被加载和实例化。servlet一旦被加载，一般不会从容器中删除，直至应用服务器关闭或重新启动。但当容器做存储器回收动作时，servlet有可能被删除。也正是因为这个原因，第一次访问servlet所用的时间要大大多于以后访问所用的时间。

servlet在服务器的运行生命周期为，在第一次请求（或其实体被内存垃圾回收后再被访问）时被加载并执行一次初始化方法，跟着执行正式运行方法，之后会被常驻并每次被请求时直接执行正式运行方法，直到服务器关闭或被清理时执行一次销毁方法后实体销毁。



## JSP

Java服务器页面JSP是HttpServlet的扩展。由于HttpServlet大多是用来响应HTTP请求，并返回Web页面（例如HTML、XML，所以不可避免地，在编写servlet时会涉及大量的HTML内容，这给servlet的书写效率和可读性带来很大障碍，JSP便是在这个基础上产生的。其功能是使用HTML的书写格式，在适当的地方加入Java代码片段，将程序员从复杂的HTML中解放出来，更专注于servlet本身的内容。

JSP在首次被访问的时候被应用服务器转换为servlet，在以后的运行中，容器直接调用这个servlet，而不再访问JSP页面。JSP的实质仍然是servlet。



# Servlet接口

Servlet: 这个就不用说了，Servlet的核心，具体Servlet中方法的处理规范可见以上描述的Servlet的生命周期

ServletConfig:  封装了对应的Servlet的相关配置信息，如servlet名字，servlet的初始参数以及Servlet所在的上下文对象，即ServletContext.  ServletConfig中的属性通常在Servlet初始化时进行初始化.

ServletRequest:  封装了所有来自client端的请求信息，如请求参数、cookie、attribute、请求类型、请求方式（安全还是非安全等）等，同时ServletRequest中的还需要明确指定部分属性，如 请求内容的编码（可以自己设定）等.  进一步的解释，可以参照下一章对HttpServletRequest的分析.

ServletResponse:  封装了server端资源到client端的所有相关信息，如 资源传输的buffer信息、响应的url地址信息、资源的编码信息等.

ServletInputStream/BufferedReader:  读取ServletRequest所封装的信息的I/O接口，ServletInputStream,采用字节流的方式读取；BufferedReader,采用字符流的方式读取.

ServletOutputSteam/PrintWriter:  将资源写入到client的I/O接口. ServletOutputSteam,采用字节流的方式进行写入;PrintWriter,采用字符流的方式进行写入.

GenericServlet:  抽象类，它定义了一个Servlet的基本实现，虽然它是Servlet的基本实现，但是它是与协议无关的（即不依赖于http协议，也不依赖于其它应用层协议）.  一般，基于协议的Servlet，如httpservlet，通常会继承该类.

RequestDispatcher: 我们在搭建web应用的过程中，可能会有这样的需求： 在当前servlet中处理完成后，需要导向（forwar）另外一个servlet或静态资源（html或text等），或者 是在当前servlet的处理过程中，需要将其它的资源包含（include）到当前的servlet资源里来。而RequestDisaptcher 接口中的forward和inluce方法就提供了实现以上两个需求的机制.



# ServletConfig和ServletContext

## ServletConfig

- 为servlet配置一些初始化参数(<init-param>标签).注意(还可以通过注解的方式进行配置)



##  ServletContext

WEB容器在启动时，它会为每个WEB应用程序都创建一个对应的ServletContext对象，它代表当前web应用。
1）ServletContext对象应用1：多个web组件之间使用它实现数据共享.context域对象。

2）通过servletContext对象获取到整个web应用的配置信息

3）通过servletContext对象实现servlet转发

	由于servlet中的java数据不易设置样式，所以serlvet可以将java数据转发到JSP页面中进行处理
	
	this.getServletContext().setAttribute("data","serlvet数据转发");
	RequestDispatcher rd = this.getServletContext().getRequestDispatcher("/viewdata.jsp");
	rd.forward(request, response);

 4）通过servletContext对象读取资源文件

	在web工程中，我们一般来说，是不能采用传统方式读取配置文件的，因为相对的是jvm的启动
	目录(tomcat的bin目录)，所以我们要使用web绝对目录来获取配置文件的地址
	
	读取资源文件的三种方式：
	
	第一种：使用ServletContext的getResourceAsStream方法：返回资源文件的读取字节流
		
		InputStream in = this.getServletContext().getResourceAsStream("/WEB-INF/classes/db.properties");
		Properties prop = new Properties();  
		prop.load(in);
		String url = prop.getProperty("url");
	
	第二种：使用ServletContext的getRealPath方法，获得文件的完整绝对路径path，再使用字节流读取path下的文件
		
		String path = this.getServletContext().getRealPath("/WEB-INF/classes/db.properties");
		String filename = path.substring(path.lastIndexOf("\\")+1); 
		//相比第一种方法的好处是：除了可以获取数据，还可以获取资源文件的名称
		FileInputStream in = new FileInputStream(path);
		Properties prop = new Properties();
		prop.load(in);
		String url = prop.getProperty("url");
	
	第三种：使用ServletContext的getResource方法，获得一个url对象，调用该类的openStream方法返回一个字节流，读取数据
		
		URL url = this.getServletContext().getResource("/WEB-INF/classes/db.properties");
		InputStream in = url.openStream();
		Properties prop = new Properties();
		prop.load(in);
		String url1 = prop.getProperty("url");

5）web工程中，不同位置的资源文件的读取方式

	一、当资源文件在包下面时
		InputStream in = this.getServletContext().getResourceAsStream("/WEB-INF/classes/cn/itcast/context/db.properties");
		System.out.println(in);
	
	二、资源文件在web-inf下
		in = this.getServletContext().getResourceAsStream("/WEB-INF/db.properties");
		System.out.println(in);
	
	三、资源文件在web工程中
		in = this.getServletContext().getResourceAsStream("/db.properties");
		System.out.println(in);

6）在非servlet程序中如何读取配置文件:用类装载器

	1）用类装载方式读取 
	 in = StudentDao.class.getClassLoader().getResourceAsStream("cn/itcast/context/db.properties");
	2）用类装载方式读取，把资源当作url对待
	 URL url = StudentDao.class.getClassLoader().getResource("db.properties");
	 这样可以获得资源文件名称：String path = url.getPath();
	3）注意：在线程休眠过程中，即使改动了资源文件，获取到的还是原始内容
	解决方案：
	  URL url = StudentDao.class.getClassLoader().getResource("db.properties");
	  String path = url.getPath();
	  
	  FileInputStream in = new FileInputStream(path);
	  Properties prop = new Properties();
	  prop.load(in);
	  System.out.println(prop.getProperty("url"));
	  
	  try {
	   Thread.sleep(1000*15);
	  } catch (InterruptedException e) {
		e.printStackTrace();
	  }
	  in = new FileInputStream(path);
	  prop = new Properties();
	  prop.load(in);
	  System.out.println(prop.getProperty("url"));
	 
	4）注意：用类装载器读取资源文件时，千万要注意，资源文件绝对不能太大，否则极易导致内存溢出


# Servlet Listener

Listener是Servlet的监听器，它可以监听客户端的请求、服务端的操作等。通过监听器，可以自动激发一些操作，比如监听在线的用户的数量。



![](img/Servlet1.jpg)



 Listener监听器就是一个实现特定接口的普通Java程序，这个程序专门用于监听一个java对象的方法调用或属性改变，当被监听对象发生上述事件后，监听器某个方法将立即被执行。 



 简单的说，被监听对象A中，关联着B对象。事件源A类对外提供一个方法，用于设置监听器对象B到A类的某一实例变量中。在需要监听事件源的方法中，方法体的某一处先构造创建一个Event对象，将this即B与相关的动作封装进Event对象中，然后调用监听器B对象的doXXXX(event)方法，将事件对象传入方法实参中。 



### ServletContext监听

ServletContextListener：用于对Servlet整个上下文进行监听（创建、销毁）。

```java
public void contextInitialized(ServletContextEvent sce);//上下文初始化
public void contextDestroyed(ServletContextEvent sce);//上下文销毁
public ServletContext getServletContext();//ServletContextEvent事件：取得一个ServletContext（application）对象
```

ServletContextAttributeListener：对Servlet上下文属性的监听（增删改属性）。

```java
public void attributeAdded(ServletContextAttributeEvent scab);//增加属性
public void attributeRemoved(ServletContextAttributeEvent scab);//属性删除
public void attributeRepalced(ServletContextAttributeEvent scab);//属性替换（第二次设置同一属性）
public String getName();//得到属性名称
public Object getValue();//取得属性的值
```

**例子**

```java
//MyServletContextListener类实现了ServletContextListener接口，因此可以对ServletContext对象的创建和销毁这两个动作进行监听。
public class MyServletContextListener implements ServletContextListener {
    @Override
    public void contextInitialized(ServletContextEvent sce) {
        System.out.println("ServletContext对象创建");
    }

    @Override
    public void contextDestroyed(ServletContextEvent sce) {
        System.out.println("ServletContext对象销毁");
    }
}
```



```xml
//web.xml文件中注册监听器
<?xml version="1.0" encoding="UTF-8"?>
<web-app version="3.0" 
    xmlns="http://java.sun.com/xml/ns/javaee" 
    xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance" 
    xsi:schemaLocation="http://java.sun.com/xml/ns/javaee 
    http://java.sun.com/xml/ns/javaee/web-app_3_0.xsd">
  <display-name></display-name>    
  <welcome-file-list>
    <welcome-file>index.jsp</welcome-file>
  </welcome-file-list>
  
  <!-- 注册针对ServletContext对象进行监听的监听器 -->
  <listener>
      <description>ServletContextListener监听器</description>
      <!--实现了ServletContextListener接口的监听器类 -->
      <listener-class>me.gacl.web.listener.MyServletContextListener</listener-class>
  </listener>
</web-app>
```



这样我们就完成了监听器的编写和注册，Web服务器在启动时，就会自动把在web.xml中配置的监听器注册到ServletContext对象上，这样开发好的MyServletContextListener监听器就可以对ServletContext对象进行监听了。

### Session监听

HttpSessionListener接口：对Session的整体状态的监听。

```java
public void sessionCreated(HttpSessionEvent se);//session创建
public void sessionDestroyed(HttpSessionEvent se);//session销毁
//HttpSessionEvent事件
public HttpSession getSession();//取得当前操作的session
```

HttpSessionAttributeListener接口：对session的属性监听。

```java
public void attributeAdded(HttpSessionBindingEvent se);//增加属性
public void attributeRemoved(HttpSessionBindingEvent se);//删除属性
public void attributeReplaced(HttpSessionBindingEvent se);//替换属性
//HttpSessionBindingEvent事件
public String getName();//取得属性的名称
public Object getValue();//取得属性的值
public HttpSession getSession();//取得当前的session
```

**例子**

```java
//MyHttpSessionListener类实现了HttpSessionListener接口，因此可以对HttpSession对象的创建和销毁这两个动作进行监听。
public class MyHttpSessionListener implements HttpSessionListener {
    @Override
    public void sessionCreated(HttpSessionEvent se) {
        System.out.println( se.getSession() + "创建了！！");
    }

    /* HttpSession的销毁时机需要在web.xml中进行配置，如下：
     * <session-config>
              <session-timeout>1</session-timeout>
          </session-config>
          这样配置就表示session在1分钟之后就被销毁
     */
    @Override
    public void sessionDestroyed(HttpSessionEvent se) {
        System.out.println("session销毁了！！");
    }
}
```



```xml
//web.xml文件中注册监听器
<!--注册针对HttpSession对象进行监听的监听器-->
<listener>
      <description>HttpSessionListener监听器</description>
      <listener-class>me.gacl.web.listener.MyHttpSessionListener</listener-class>
</listener>
<!-- 配置HttpSession对象的销毁时机 -->
<session-config>
      <!--配置HttpSession对象的1分钟之后销毁 -->
      <session-timeout>1</session-timeout>
</session-config>
```



当我们访问jsp页面时，HttpSession对象就会创建，此时就可以在HttpSessionListener观察到HttpSession对象的创建过程了。

### Request监听

ServletRequestListener：用于对Request请求进行监听（创建、销毁）。

```java
public void requestInitialized(ServletRequestEvent sre);//request初始化
public void requestDestroyed(ServletRequestEvent sre);//request销毁
//ServletRequestEvent事件
public ServletRequest getServletRequest();//取得一个ServletRequest对象
public ServletContext getServletContext();//取得一个ServletContext（application）对象
```

ServletRequestAttributeListener：对Request属性的监听（增删改属性）。

```java
public void attributeAdded(ServletRequestAttributeEvent srae);//增加属性
public void attributeRemoved(ServletRequestAttributeEvent srae);//属性删除
public void attributeReplaced(ServletRequestAttributeEvent srae);//属性替换（第二次设置同一属性）
//ServletRequestAttributeEvent事件：能取得设置属性的名称与内容
public String getName();//得到属性名称
public Object getValue();//取得属性的值
```

**例子**

```java
//MyServletRequestListener类实现了ServletRequestListener接口，因此可以对ServletRequest对象的创建和销毁这两个动作进行监听。 
public class MyServletRequestListener implements ServletRequestListener {
    @Override
    public void requestDestroyed(ServletRequestEvent sre) {
        System.out.println(sre.getServletRequest() + "销毁了！！");
        
    }

    @Override
    public void requestInitialized(ServletRequestEvent sre) {
        System.out.println(sre.getServletRequest() + "创建了！！");
    }
}
```



```xml
//web.xml文件中注册监听器  
<!--注册针对ServletRequest对象进行监听的监听器-->
<listener>
      <description>ServletRequestListener监听器</description>
      <listener-class>me.gacl.web.listener.MyServletRequestListener</listener-class>
</listener>
```



用户每一次访问都会创建request对象，当访问结束后，request对象就会销毁。



# Servlet规范

 https://waylau.gitbooks.io/servlet-3-1-specification/ 



# Idea创建servlet并部署

https://www.cnblogs.com/javabg/p/7976977.html
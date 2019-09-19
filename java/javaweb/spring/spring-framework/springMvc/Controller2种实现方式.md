# spring mvc 控制器的几种实现方式
---

博客链接如下

	https://blog.csdn.net/chengxuyuanxingcheng/article/details/79135470

	https://blog.csdn.net/qq_38225558/article/details/83271233

## 1. spring 3.0 之前的实现方式

- 在spring 3.0 之前，是没有注解的，如果想要实现一个控制器，只能通过 ***配置文件 + 继承*** 的方式实现。
- 在spring 3.0 之后，基本用 @Controller 注解代替了上述配置方法。

	### 1.1 基于Controller接口的控制器

	### 1.2 基于HttpRequestHandler接口

	### 1.3 基于AbstractController抽象类

	### 1.4 基于@Controller注解的控制器

	### 等等

	
---

注意事项

	- applicationContext.xml中一定要配置如下bean，否则会报 no mapping 映射类型的异常

	<!--配置Handle,映射"/hello请求"-->
	<bean name="/hello" class="com.spring.controller.HelloController"/>

	<!--处理映射器将bean 的name作为url进行查找，需要在配置Handle时指定name(即url)--> 
	<bean class="org.springframework.web.servlet.handler.BeanNameUrlHandlerMapping"/>     
	
	<!--SimpleControllerHandlerAdapter是一个处理器适配器，所有处理器适配器都要实现HandlerAdapter接口--> 
	<bean class="org.springframework.web.servlet.mvc.SimpleControllerHandlerAdapter"/>     

	<!--视图解析器--> 
	<bean class="org.springframework.web.servlet.view.InternalResourceViewResolver"/>

	
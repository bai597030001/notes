# SpringBoot - 配置文件加载位置与优先级
---
## 1.项目内部配置文件

- spring boot 启动后默认会扫描以下位置的application.properties或者application.yml文件作为Spring boot的配置文件

```

	–file:./config/  
	–file:./  
	–classpath:/config/  
	–classpath:/  

- 如下图所示：

![](img/springboot-conf.jpg)

- **以上是按照优先级从高到低的顺序，所有位置的文件都会被加载，高优先级配置内容会覆盖低优先级配置内容。**

- 改变默认配置位置

	`java -jar spring-boot-02-config-02-0.0.1-SNAPSHOT.jar --spring.config.location=D:/application.properties`

## 2.外部配置加载顺序

- SpringBoot也可以从以下位置加载配置

```

	1.命令行参数:  
		
		所有的配置都可以在命令行上进行指定；

		多个配置用空格分开； –配置项=值
	
		java -jar spring-boot-02-config-02-0.0.1-SNAPSHOT.jar --server.port=8087 --server.context-path=/abc
	
	2.来自java:comp/env的JNDI属性 
	3.Java系统属性（System.getProperties()） 
	4.操作系统环境变量 
	5.RandomValuePropertySource配置的random.*属性值
	6.jar包外部的application-{profile}.properties或application.yml(带spring.profile)配置文件 
	7.jar包内部的application-{profile}.properties或application.yml(带spring.profile)配置文件 
	8.jar包外部的application.properties或application.yml(不带spring.profile)配置文件 
	9.jar包内部的application.properties或application.yml(不带spring.profile)配置文件

- **优先级从高到低；高优先级的配置覆盖低优先级的配置，所有的配置会形成互补配置。**

---

- **由jar包外向jar包内进行寻找，优先加载带profile的，再加载不带profile的。**
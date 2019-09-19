# springboot命令行指定配置文件

* java的springboot项目，有时候为了配置方便，需要将配置文件单独拿出来，这样可以在不重新打包的情况下，修改配置文件即可重启程序。

* 有时候，当多个springboot项目jar包放在服务器同一目录下，那么多个模块对应的配置文件则需要单独重新命名。
 
---

## 所以有时候我们需要在命令行启动 springboot 程序的时候，指定配置文件

> java -Dspring.config.location=application.yaml,application.properties -jar network-0.0.1.jar

- 注意事项： 

	springboot项目默认启动(即： 不在命令行指定配置文件)和命令行指定配置文件这两种启动方式是不同的。

	当不在命令行指定配置文件，即用jar包默认的配置文件的时候，此时如果打包的源文件内有 没有加 @Configuration @Commpent @Service
	等注释的时候，是不会报错，且程序可正常启动的；

	但是当在命令行制定配置文件方式启动的时候，如果源文件内有 没有加 @Configuration @Commpent @Service 等注释，则会出现启动失败的现象。

	猜测: 应该是springboot启动时根据配置文件位置而进行了不同的配置，当命令行指定配置文件时，若此时源代码中有需要交给spring Ioc容器管理的bean没有加注解，spring扫描不到，则会直接报错。

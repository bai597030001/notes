# Tomcat详解

- tomcat 8.5.38



## 导入idea



## 编译



## 原理解析



![](img/tomcat2.png)



tomcat的组件主要包括:

- server：整个servlet容器，一个tomcat对应一个server，一个server包含多个service

  server在tomcat中的实现类是：StandardServer

- service： 一个service包含多个connector（ 因为 Tomcat 支持多种网络协议，包括 HTTP/1.1、HTTP/2、AJP 等等 ），和一个container（容器）

  多个connector共享一个container容器，

  service在tomcat中的实现类是：StandardService

- connector：链接器，负责处理客户端请求，解析不同协议及io方式。 连接器与容器之间通过 ServletRequest 和 ServletResponse 对象进行交流。 

- executor：线程池

- container：包含engine，host，context，wrapper等组件。

- engine：servlet引擎，container容器中顶层的容器对象，一个engine可以包含多个host主机

  engine在tomcat中的实现类是：StandardEngine

- host：engine容器的子容器，一个host对应一个网络域名，一个host包含多个context

  host在tomcat中的实现类是：StandardHost

- context：host容器的子容器，表示一个web应用

  context在tomcat中的实现类是：StandardContext

- wrapper：tomcat中最小的容器单元，表示web应用中的servlet

  wrapper在tomcat中的实现类是：StandardWrapper



 也可以从 server.xml 的配置结构可以看出 tomcat 整体的内部结构： 

```xml

<Server port="8005" shutdown="SHUTDOWN">

  <Service name="Catalina">

    <Connector connectionTimeout="20000" port="8080" protocol="HTTP/1.1" redirectPort="8443" URIEncoding="UTF-8"/>

    <Connector port="8009" protocol="AJP/1.3" redirectPort="8443"/>

    <Engine defaultHost="localhost" name="Catalina">

      <Host appBase="webapps" autoDeploy="true" name="localhost" unpackWARs="true">

        <Context docBase="handler-api" path="/handler" reloadable="true" source="org.eclipse.jst.jee.server:handler-api"/>
      </Host>
    </Engine>
  </Service>
</Server>
```





## 源码解析



![](img/tomcat3.png)



| 一级模块                    | 二级模块        | 代码量 | 用途                                                         |
| --------------------------- | --------------- | ------ | ------------------------------------------------------------ |
|                             | ant             |        | 使用ant集成jmx,方便运行时对tomcat进行管理                    |
|                             | connector       |        | 处理连接，如HttpServletRequest等                             |
|                             | core            |        | 核心实现                                                     |
|                             | deploy          |        | 部署模块                                                     |
|                             | filters         |        | 内置的一些Filter实现                                         |
|                             | loader          |        | High Availiable缩写，处理Tomcat集群                          |
|                             | manager         |        |                                                              |
|                             | mbeans          |        | tomcat内置的jmx                                              |
|                             | realm           |        | tomcat管理页面的控制权限                                     |
|                             | security        |        | 安全相关                                                     |
|                             | servlet4preview |        | servlet4.0前瞻                                               |
|                             | servlets        |        | servlet实现类                                                |
|                             | session         |        | HttpSession相关                                              |
|                             | ssi             |        | server side include,用于替换html片段，类似nginx处理ssi资源   |
|                             | startup         |        | 启动tomcat容器                                               |
|                             | storeconfig     |        | 配置信息                                                     |
|                             | values          |        | 阀门组件。Values是tomcat中责任链模式的实现，通过链接多个Value对请求进行处理 |
|                             | tribes          |        | 部落组件，用于tomcat之间通信                                 |
| catalina（核心部分）        | webresources    |        | 用于处理 jar,war等文件                                       |
|                             | ajp             |        | ajp协议                                                      |
|                             | http11          |        |                                                              |
| coyote（支持各种协议）      | http2           |        |                                                              |
|                             | dbcp            |        | 数据库连接池实现                                             |
|                             | jni             |        | native实现，需要使用tomcat提供的tomcat-native.tar.gz         |
|                             | websocket       |        | 对javax.websocket的实现                                      |
| tomcat（对javax的一些实现） | util            |        | 各种工具类，包括对jdk的封装                                  |
| el                          |                 |        | el表达式                                                     |
| jasper                      |                 |        | 支持jps                                                      |
| juli                        |                 |        | 日志                                                         |
| naming                      |                 |        | jndi                                                         |


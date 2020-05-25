# sleuth

目前开源的链路追踪组件有Google的Dapper，Twitter 的Zipkin，以及阿里的Eagleeye （鹰眼）等。

`Spring Cloud Sleuth` 的主要功能就是为 **分布式系统** 提供 **追踪解决方案**，并且兼容支持了 `Zipkin`，只需要在 `pom.xml` 文件中引入相应的 **依赖** 即可。

**服务追踪组件** `Zipkin`，`Spring Cloud Sleuth` 集成了 `Zipkin` 组件。它主要用于 **聚集** 来自各个 **异构系统** 的 **实时监控数据**，用来追踪 **微服务架构** 下的 **系统延时问题**。



## 相关术语

### 1.1. Span

`Span` 是一个基本的 **工作单元**，用于描述一次 `RPC` 调用，`Span` 通过一个 `64` 位的 `spanId` 作为 **唯一标识**。`Zipkin` 中的 `Span` 还有其他数据信息，比如 **摘要**、**时间戳事件**、**关键值注释** (`tags`) 以及 **进度** `ID` (通常是 `IP` 地址)。`Span` 在不断的启动和停止，同时记录了 **时间信息**，一个 `Span` 创建后，必须在未来的某个时刻停止它。

### 1.2. Trace

一系列 `Span` 组成的一个 **树状结构**。例如，如果你正在跑一个大型 **分布式系统**，可能需要创建一个 `Trace`。

### 1.3. Annotation

表示 **基本标注列表**，一个 `Annotation` 可以理解成 `Span` 生命周期中 **重要时刻** 的 **数据快照**，比如一个 `Annotation` 中一般包含 **发生时刻**（`timestamp`）、**事件类型**（`value`）、**端点**（`endpoint`）等信息。其中 `Annotation` 的 **事件类型** 包含以下四类：

- **cs - Client Sent**

**客户端** 发起一个请求，这个 `Annotion` 描述了这个 `Span` 的开始。

- **sr - Server Received**

**服务端** 获得请求并 **准备开始** 处理它，如果将 `sr` 减去 `cs` 的 **时间戳** 便可得到 **网络延迟**。

- **ss - Server Sent**

**服务端** 完成请求处理，如果将 `ss` 减去 `sr` 的 **时间戳**，便可得到 **服务端** 处理请求消耗的时间。

- **cr - Client Received**

**客户端** 成功接收到 **服务端** 的响应，如果将 `cr` 减去 `cs` 的 **时间戳**，便可得到 **整个请求** 所消耗的 **总时间**。



# zipkin

Zipkin是一个致力于收集分布式服务的时间数据的分布式跟踪系统。其主要涉及以下四个组件：

-  **collector:** 数据采集;
-  **storage:** 数据存储;
-  **search:** 数据查询;
-  **UI:** 数据展示.

Zipkin提供了可插拔数据存储方式：In-Memory、MySql、Cassandra以及Elasticsearch。



## 注意

Spring Boot 2.0之前，需要自己实现一个zipkin-server。

在Spring Boot 2.0之后官方不再建议自定义zipkin，建议使用官方提供的zipkin.jar包。

[官网链接](<https://dl.bintray.com/openzipkin/maven/io/zipkin/java/zipkin-server/>)

下载完后，java -jar 来启动，zipkin的默认端口为9411，通过浏览器访问

```shell
$ java -jar zipkin-server.jar --zipkin.storage.mysql.username=root --zipkin.storage.mysql.password=123456 --zipkin.storage.mysql.host=localhost --zipkin.storage.mysql.port=3306 
```



## 存在问题

1. sleuth与zipkin之间是基于http协议传输的，http协议传输有个不好的地方就是，它是短连接，即需要频繁通过三次握手建立链接，这在追踪很多服务时会造成不小的性能消耗。
2. 对于直接传输的方式，有个弊端就是一旦接收方意外断开连接，那么在传输链路中的一些数据将会丢失，如果这些数据是关键数据，那么后果将是非常严重的。同样一些场景下需要保存链路追踪的数据，以备后面观察对比，所以同样需要一个db来存储数据。



## 解决

1. 在sleuth-cli跟zipkin-server之间插入一个消息中间件rabbitmq/kafka
2. 将链路追踪的数据存储到DB上，目前zipkin暂时只支持mysql/elasticsearch



## 依赖

官方描述：

> 如果你想使用rabbitmq或kafka替换掉http,添加spring-rabbit或spring-kafka的依赖包，
>
> 默认目标名是zipkin(队列名),如果你使用kafka/mysql，
>
> 你需要设置属性：spring-zipkin-sender-type=kafka/mysql



也就是说，只需要引入下面这两个依赖包（`spring-cloud-sleuth-stream`已经被弃用）

```xml
<dependency> 
    <groupId>org.springframework.cloud</groupId>
    <artifactId>spring-cloud-starter-zipkin</artifactId>
</dependency>
<dependency> 
    <groupId>org.springframework.amqp</groupId>
    <artifactId>spring-rabbit</artifactId>
</dependency>
```



# 示例

1.从[官网](<https://dl.bintray.com/openzipkin/maven/io/zipkin/java/zipkin-server/>)下载`zipkin-server.jar`包，`java -jar`运行



2.zipkin客户端

```xml
<dependency>
	<groupId>org.springframework.cloud</groupId>
	<artifactId>spring-cloud-starter-zipkin</artifactId>
</dependency>
```



```yaml
spring:
  application:
    name: service-abis-server
  zipkin:
    enabled: true
    base-url: http://localhost:9411/
    locator.discovery.enabled: true
    service.name: service-abis-server
    sender.type: WEB # kafka/rabbit/web
  sleuth:
    web.client.enabled: true
    # 100%的概率将链路的数据上传给Zipkin Server，在默认的情况下，该值为0.1
    sampler.probability: 1.0
```



3.访问zipkin-server

```http
http://localhost:9411/
```




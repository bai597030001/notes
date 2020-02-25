# spring cloud bus

在微服务架构的系统中，我们通常会使用轻量级的消息代理来构建一个共用的消息主题让系统中所有微服务实例都能连接上来，由于该主题中产生的消息会被所有实例监听和消费，所以我们称它为消息总线。在总线上的各个实例都可以方便地广播一些需要让其他连接在该主题上的实例都知道的消息，例如配置信息的变更或者其他一些管理操作等。

由于消息总线在微服务架构系统的广泛使用，所以它同配置中心一样，几乎是微服务架构中的必备组件。`spring cloud`作为微服务架构综合性的解决方案，对此自然也有自己的实现，这就是`spring cloud bus`。通过`spring cloud bus`，可以非常容易的搭建起消息总线，同时实现了一些消息总线中的常用功能，比如配合`spring cloud config`实现微服务应用配置信息的动态更新等。



`Spring Cloud Bus`对自己的定位是 Spring Cloud 体系内的消息总线，使用 message broker 来连接分布式系统的所有节点。



![](E:/notes/java/javaweb/spring/springcloud/img/cloud-bus1.png)



## 消息代理

消息代理（message broker）是一种消息验证，传输，路由的架构模式。它在应用程序之间起到通信并最小化应用之间的依赖的作用，使得应用程序可以高效地解耦通信过程。消息代理是一个中间件产品，它的核心是一个消息的路由程序，用来实现接收和分发消息，并根据设定好的消息处理流来转发给正确的应用。它包括独立的通信和消息传递协议，能够实现组织内部和组织间的网络通信。设计代理的目的就是为了能够从应用程序中传入消息，并执行一些特别的操作，下面这些是企业应用中，我们经常使用消息代理的场景：

- 将消息路由到一个或多个目的地。
- 消息转化为其他的表现方式。
- 执行消息的聚集，消息的分解，并将结果发送到它们的目的地，然后重新组合响应返回给消息用户。
- 调用web服务来检索数据。
- 响应事件或错误。
- 使用发布-订阅模式来提供内容和基于主题的消息路由。

目前已经有非常多的开源产品可以供大家使用，比如：

- activemq
- kafka
- rabbitmq
- rocketmq
- ...

当前版本的`spring cloud bus`仅支持两款中间件产品：`rabbitmq`和`kafka`。



## 总线端点

消息总线目前支持将消息发送到正在侦听某个特定服务的所有节点或所有节点(由Eureka定义)。 `/bus/*` 执行器命名空间有一些http端点。现在，实现了两个。 第一个是 `/bus/env` ，发送 key/value 对来更新每个节点的Spring环境。 第二个是 `/bus/refresh` ，重新加载每个应用的配置，因为它们在之前已经连通了`/refresh`端点。

> SpringCloud Bus消息总线通过`RabbitMQ`和`Kafka`启动，因为这两者是最常用的实现（可以考虑使用`RocketMQ`实现）。但是，SpringCloud Stream是非常灵活的，并且绑定器使用的是`spring-cloud-bus`。



### 总线刷新端点 Bus Refresh Endpoint

`/actuator/bus-refresh` 端点清除 `RefreshScope` 缓存并重新绑定 `@ConfigurationProperties`。

为了暴露 `/actuator/bus-refresh` 端点，你需要添加以下内容到配置文件中：

```properties
management.endpoints.web.exposure.include=bus-refresh
```

### 总线环境端点 Bus Env Endpoint

`/actuator/bus-env` 端点跨实例使用指定的 `key/value` 对更新每个实例的环境。

为了暴露 `/actuator/bus-env` 端点，需要添加以下内容到配置文件:

```
management.endpoints.web.exposure.include=bus-env
```

`/actuator/bus-env` 端点接收以下形式的 `POST` 请求：

```http
{
	"name": "key1",
	"value": "value1"
}
```



## 寻址一个实例

每个应用的实例都有一个 service ID， 可以通过 `spring.cloud.bus.id` 设置其值，值为冒号分隔的多个形式。 默认值是是由 `spring.application.name` 、`server.port`(或者 `spring.application.index`如果设置了的话) 组合而成的。 默认id形式是： `app:index:id` :

- `app` 是 `spring.application.name` 指定的
- `index` 是 `spring.application.index`、`local.server.port`、`server.port`或者`0`指定的
- `id` 是`vcap.application.instance_id`，如果存在的话； 或者是一个随机值

HTTP 端点接收一个 "`destination`" path 参数，例如 `/bus-refresh/cunstomers:9000`，在这里 `destination` 是一个service ID。如果这个ID是被总线上的某个实例所有，则它将处理该消息，而所有其他实例将忽略该消息。

## 在一个服务上寻址所有的实例

前面的 `destination` 参数用于一个Spring `PatchMatcher`(path由`:`分隔) 去决定是否某个实例去处理消息。 使用实例， `/bus-env/cunstomers:**` 目标即所有的 customers 服务实例。

## 服务ID必须是唯一的

总线会尝试2次去消除对事件的处理————一次从源`ApplicationEvent` ，另一次是从队列中。 为此，总线根据当前服务ID检查发送服务ID。如果一个服务的多个实例拥有的是相同的服务ID，事件则不会处理。 当在本地机器运行时，每个服务都是在不同的端口，端口是服务ID的一部分。云计算提供了一个指标来区分。 为了确保服务ID在云计算之外是唯一的，需要为服务的每个实例都设置`spring.application.index`。

## 自定义消息代理

Spring Cloud Bus 使用 Spring Cloud Stream 去广播消息。所以，要让消息流动起来，你需要的仅仅是在你的classpath中包含你选择的绑定器实现。 对于RabbitMQ和Kafka则有一个方便的启动器`spring-cloud-starter-bus-[amqp|kafka]` 。 一般而言，Spring Cloud Stream 依靠 Spring Boot 的自动配置可以很方便的配置中间件。 举个例子，AMQP 代理寻址可以通过改变`spring.rabbitmq.*`配置属性来达到目的。 Spring Cloud Bus 在`spring.cloud.bus.*` 中有一些本地属性，例如`spring.cloud.bus.destination`是使用外部中间件的topic的name。通常情况下，默认的已经足够使用了。

可以查看 Spring Cloud Stream 文档可以获得更多的自定义消息代理设置。

## 跟踪总线事件Bus Events

总线事件（也是`RemoteApplicationEvent` 的子类）可以通过`spring.cloud.bus.trace.enabled=true`来设置。 如果设置了，SprinigBoot的`TraceRepository`(如果存在的话)会显示每个发送过的事件和每个服务实例的响应。以下是一个来自`/trace`端点的示例：

```json
{
  "timestamp": "2015-11-26T10:24:44.411+0000",
  "info": {
    "signal": "spring.cloud.bus.ack",
    "type": "RefreshRemoteApplicationEvent",
    "id": "c4d374b7-58ea-4928-a312-31984def293b",
    "origin": "stores:8081",
    "destination": "*:**"
  }
  },
  {
  "timestamp": "2015-11-26T10:24:41.864+0000",
  "info": {
    "signal": "spring.cloud.bus.sent",
    "type": "RefreshRemoteApplicationEvent",
    "id": "c4d374b7-58ea-4928-a312-31984def293b",
    "origin": "customers:9000",
    "destination": "*:**"
  }
  },
  {
  "timestamp": "2015-11-26T10:24:41.862+0000",
  "info": {
    "signal": "spring.cloud.bus.ack",
    "type": "RefreshRemoteApplicationEvent",
    "id": "c4d374b7-58ea-4928-a312-31984def293b",
    "origin": "customers:9000",
    "destination": "*:**"
  }
}
```

追踪情况显示了从`customers:9000`发送了一个`RefreshRemoteApplicationEvent`事件，并且广播到了所有的服务中，被`customers:9000`和`tores:8081`接收确认。

为了你自己解决确认信号，你需要添加一个`@EventListener`注解到你的应用的`AckRemoteApplicationEvent`和`SentApplicationEvent`类中以便跟踪。可选的方案是，你可以从`TraceRepository`获取数据。

> 任何Bus应用都可以跟踪acks。但是，有时候，在一个中心服务中是很有用处的。它可以做更多的复杂查询，或者将数据转发给指定的跟踪服务去处理。

## 广播你自己的事件

Bus总线可以携带任何`RemoteApplicationEvent`类型的事件。默认的传输形式是 JSON，反序列化器需要提前知道是哪种类型。 如果要注册一个新类型，你必须将它放进`org.springframework.cloud.bus.event`的子包下。

自定义事件名称的话，你需要在你的自定义类中添加`@JsonTypeName`注解，或者依靠默认的策略————使用class的简单名称(simple name)。

> 生产者、消费者需要访问类的定义

### 在自定义包中注册事件

如果你不能或者不想在`org.springframework.cloud.bus.event`子包下自定义事件，你必须通过`@RemoteApplicationEventScan`注解标记扫描`RemoteApplicationEvent`类型的事件类。`@RemoteApplicationEventScan` 也包含了子包。

示例，自定义事件通过继承`RemoteApplicationEvent`：

```java
package com.acme;

public class MyEvent extends RemoteApplicationEvent {
    ...
}
```

你可以按以下方式将事件注册到反序列化器中：

```java
package com.acme;

@Configuration
@RemoteApplicationEventScan
public class BusConfiguration {
    ...
}
```

在不指定值的情况下，注册使用`@RemoteApplicationEventScan`的类的包,这里注册了`com.acme`包及其子包；

你也可以通过在`@RemoteApplicationEventScan`注解上添加`value`、`basePackages`或者`basePackageClasses`属性来指定扫描包，例如：

```java
package com.acme;

@Configuration
//@RemoteApplicationEventScan({"com.acme", "foo.bar"})
//@RemoteApplicationEventScan(basePackages = {"com.acme", "foo.bar", "fizz.buzz"})
@RemoteApplicationEventScan(basePackageClasses = BusConfiguration.class)
public class BusConfiguration {
    ...
}
```

## 使用RabbitMQ实战Spring Cloud Bus消息总线

- 需要在配置文件中配置`spring-cloud-starter-bus-amqp`；
- 需要依赖RabbitMQ，可以参考[RabbitMQ系列](https://blog.csdn.net/zixiao217/article/category/6428331)

### 增加pom依赖

```xml
<dependencies>
		<dependency>
			<groupId>org.springframework.cloud</groupId>
			<artifactId>spring-cloud-starter-config</artifactId>
		</dependency>

		<dependency>
			<groupId>org.springframework.boot</groupId>
			<artifactId>spring-boot-starter-web</artifactId>
		</dependency>

		<dependency>
			<groupId>org.springframework.cloud</groupId>
			<artifactId>spring-cloud-starter-netflix-eureka-client</artifactId>
		</dependency>

		<dependency>
			<groupId>org.springframework.cloud</groupId>
			<artifactId>spring-cloud-starter-bus-amqp</artifactId>
		</dependency>

		<dependency>
			<groupId>org.springframework.boot</groupId>
			<artifactId>spring-boot-starter-actuator</artifactId>
		</dependency>
</dependencies>
```

### application.properties配置RabbitMQ信息、消息总线信息

```properties
# RabbitMQ相关配置信息
spring.rabbitmq.host=localhost
spring.rabbitmq.port=5672
spring.rabbitmq.username=guest
spring.rabbitmq.password=guest

# 总线配置
spring.cloud.bus.enabled=true
# 允许跟踪
spring.cloud.bus.trace.enabled=true
# 暴露端点
management.endpoints.web.exposure.include=bus-refresh
```

### 启动类编写

```java
@SpringBootApplication
@EnableEurekaClient
@EnableDiscoveryClient
@RestController
@RefreshScope  // /actuator/bus-refresh 端点清除 RefreshScope 缓存并重新绑定 ```@ConfigurationProperties```
public class ConfigClientApplication {

	/**
	 * http://localhost:8881/actuator/bus-refresh
	 */

	public static void main(String[] args) {
		SpringApplication.run(ConfigClientApplication.class, args);
	}

	@Value("${foo}")
	String foo;

	@RequestMapping(value = "/hi")
	public String hi(){
		return foo;
	}
}
```

### 测试消息总线实例

依次启动eureka-server、confg-cserver,启动两个config-client，端口为：8881、8882。 访问http://localhost:8881/hi 或者http://localhost:8882/hi 浏览器显示：

> foo version 3

> 这时我们去代码仓库将foo的值改为`foo version 4`，即改变配置文件foo的值。如果是传统的做法，需要重启服务，才能达到配置文件的更新。
>
> 此时，我们只需要发送post请求：http://localhost:8881/actuator/bus-refresh，你会发现config-client会重新读取配置文件。

这时我们再访问http://localhost:8881/hi 或者http://localhost:8882/hi 浏览器显示：

> foo version 4

另外，/actuator/bus-refresh接口可以指定服务，即使用前面提到的`destination`参数，比如 `/actuator/bus-refresh?destination=customers:**` 即刷新服务名为customers的所有服务。

当git文件更改的时候，通过pc端用post 向端口为8882的config-client发送请求/bus/refresh/；此时8882端口会发送一个消息，由消息总线向其他服务传递，从而使整个微服务集群都达到更新配置文件。



## Bus深入

### Bus 概念介绍

#### 事件

Bus 中定义了远程事件 `RemoteApplicationEvent`，该事件继承了 Spring 的事件 `ApplicationEvent`，且它目前有 4 个具体的实现：

- `EnvironmentChangeRemoteApplicationEvent`: 远程环境变更事件。主要用于接收一个 `Map` 类型的数据并更新到 Spring 上下文中 `Environment` 中的事件。文中的实例就是使用这个事件并配合 `EnvironmentBusEndpoint` 和 `EnvironmentChangeListener` 完成的。
- `AckRemoteApplicationEvent`: 远程确认事件。Bus 内部成功接收到远程事件后会发送回 `AckRemoteApplicationEvent` 确认事件进行确认
- `RefreshRemoteApplicationEvent`: 远程配置刷新事件。配合 `@RefreshScope` 以及所有的 `@ConfigurationProperties` 注解修饰的配置类的动态刷新
- `UnknownRemoteApplicationEvent`：远程未知事件。Bus 内部消息体进行转换远程事件的时候如果发生异常会统一包装成该事件

Bus 内部还存在一个非 `RemoteApplicationEvent` 事件 - `SentApplicationEvent` 消息发送事件。配合 Trace 进行远程消息发送的记录



这些事件会配合 `ApplicationListener` 进行操作，比如 `EnvironmentChangeRemoteApplicationEvent` 配了 `EnvironmentChangeListener` 进行配置的新增/修改：

```java
public class EnvironmentChangeListener
		implements ApplicationListener<EnvironmentChangeRemoteApplicationEvent> {

	private static Log log = LogFactory.getLog(EnvironmentChangeListener.class);

	@Autowired
	private EnvironmentManager env;

	@Override
	public void onApplicationEvent(EnvironmentChangeRemoteApplicationEvent event) {
		Map<String, String> values = event.getValues();
		log.info("Received remote environment change request. Keys/values to update "
				+ values);
		for (Map.Entry<String, String> entry : values.entrySet()) {
			env.setProperty(entry.getKey(), entry.getValue());
		}
	}
}
```

收到其它节点发送来的 `EnvironmentChangeRemoteApplicationEvent` 事件之后调用 `EnvironmentManager#setProperty` 进行配置的设置，该方法内部针对每一个配置项都会发送一个 `EnvironmentChangeEvent` 事件，然后被 `ConfigurationPropertiesRebinder` 所监听，进行 rebind 操作新增/更新配置。



#### Actuator Endpoint

Bus 内部暴露了 2 个 Endpoint，分别是 `EnvironmentBusEndpoint` 和 `RefreshBusEndpoint`，进行配置的新增/修改以及全局配置刷新。它们对应的 Endpoint id 即 url 是 `bus-env` 和 `bus-refresh`。



#### 配置

Bus 对于消息的发送必定涉及到 Topic，Group 之类的信息。这些内容都被封装到了 `BusProperties` 中，其默认的配置前缀为 `spring.cloud.bus`。

比如：

- `spring.cloud.bus.refresh.enabled` 用于开启/关闭全局刷新的 Listener。
- `spring.cloud.bus.env.enabled` 用于开启/关闭配置新增/修改的 Endpoint。
- `spring.cloud.bus.ack.enabled` 用于开启开启/关闭 `AckRemoteApplicationEvent` 事件的发送。
- `spring.cloud.bus.trace.enabled` 用于开启/关闭消息记录 Trace 的 Listener。

消息发送涉及到的 Topic 默认用的是 `springCloudBus`，可以配置进行修改，Group 可以设置成广播模式或使用 UUID 配合 offset 为 lastest 的模式。

每个 Bus 应用都有一个对应的 Bus id，官方取值方式较复杂：

```properties
${vcap.application.name:${spring.application.name:application}}:${vcap.application.instance_index:${spring.application.index:${local.server.port:${server.port:0}}}}:${vcap.application.instance_id:${random.value}}
```

建议手动配置 Bus id，因为 Bus 远程事件中的 destination 会根据 Bus id 进行匹配：

```properties
spring.cloud.bus.id=${spring.application.name}-${server.port}
```



### Bus 底层分析

Bus 的底层分析无非牵扯到几个方面：

- 消息是如何发送的
- 消息是如何接收的
- destination 是如何匹配的
- 远程事件收到后如何触发下一个 action

`BusAutoConfiguration` 自动化配置类被 `@EnableBinding(SpringCloudBusClient.class)` 所修饰。

`@EnableBinding` 的用法在 [干货｜Spring Cloud Stream 体系及原理介绍](https://mp.weixin.qq.com/s/e_pDTFmFcSqHH-uSIzNmMg) 中已经说明，且它的 value 为 `SpringCloudBusClient.class`，会在 `SpringCloudBusClient` 中基于代理创建出 input 和 output 的 `DirectChannel`：

```java
public interface SpringCloudBusClient {

	String INPUT = "springCloudBusInput";

	String OUTPUT = "springCloudBusOutput";

	@Output(SpringCloudBusClient.OUTPUT)
	MessageChannel springCloudBusOutput();

	@Input(SpringCloudBusClient.INPUT)
	SubscribableChannel springCloudBusInput();
}
```

springCloudBusInput 和 springCloudBusOutput 这两个 Binding 的属性可以通过配置文件进行修改(比如修改 topic)：

```yaml
spring.cloud.stream.bindings:
  springCloudBusInput:
    destination: my-bus-topic
  springCloudBusOutput:
    destination: my-bus-topic
```

消息的接收的发送：

```java
// BusAutoConfiguration

@EventListener(classes = RemoteApplicationEvent.class) // 1
public void acceptLocal(RemoteApplicationEvent event) {
	if (this.serviceMatcher.isFromSelf(event)
			&& !(event instanceof AckRemoteApplicationEvent)) { // 2
		this.cloudBusOutboundChannel.send(MessageBuilder.withPayload(event).build()); // 3
	}
}

@StreamListener(SpringCloudBusClient.INPUT) // 4
public void acceptRemote(RemoteApplicationEvent event) {
	if (event instanceof AckRemoteApplicationEvent) {
		if (this.bus.getTrace().isEnabled() && !this.serviceMatcher.isFromSelf(event)
				&& this.applicationEventPublisher != null) { // 5
			this.applicationEventPublisher.publishEvent(event);
		}
		// If it's an ACK we are finished processing at this point
		return;
	}
	if (this.serviceMatcher.isForSelf(event)
			&& this.applicationEventPublisher != null) { // 6
		if (!this.serviceMatcher.isFromSelf(event)) { // 7
			this.applicationEventPublisher.publishEvent(event);
		}
		if (this.bus.getAck().isEnabled()) { // 8
			AckRemoteApplicationEvent ack = new AckRemoteApplicationEvent(this,
					this.serviceMatcher.getServiceId(),
					this.bus.getAck().getDestinationService(),
					event.getDestinationService(), event.getId(), event.getClass());
			this.cloudBusOutboundChannel
					.send(MessageBuilder.withPayload(ack).build());
			this.applicationEventPublisher.publishEvent(ack);
		}
	}
	if (this.bus.getTrace().isEnabled() && this.applicationEventPublisher != null) { // 9
		// We are set to register sent events so publish it for local consumption,
		// irrespective of the origin
		this.applicationEventPublisher.publishEvent(new SentApplicationEvent(this,
				event.getOriginService(), event.getDestinationService(),
				event.getId(), event.getClass()));
	}
}
```

1. 利用 Spring 事件的监听机制监听本地所有的 `RemoteApplicationEvent` 远程事件(比如 `bus-env` 会在本地发送 `EnvironmentChangeRemoteApplicationEvent` 事件，`bus-refresh` 会在本地发送 `RefreshRemoteApplicationEvent` 事件。这些事件在这里都会被监听到)
2. 判断本地接收到的事件不是 `AckRemoteApplicationEvent` 远程确认事件(不然会死循环，一直接收消息，发送消息…)以及该事件是应用自身发送出去的(事件发送方是应用自身)，如果都满足执行步骤 3
3. 构造 Message 并将该远程事件作为 payload，然后使用 Spring Cloud Stream 构造的 Binding name 为 springCloudBusOutput 的 MessageChannel 将消息发送到 broker
4. `@StreamListener` 注解消费 Spring Cloud Stream 构造的 Binding name 为 springCloudBusInput 的 MessageChannel，接收的消息为远程消息
5. 如果该远程事件是 `AckRemoteApplicationEvent` 远程确认事件并且应用开启了消息追踪 trace 开关，同时该远程事件不是应用自身发送的(事件发送方不是应用自身，表示事件是其它应用发送过来的)，那么本地发送 `AckRemoteApplicationEvent` 远程确认事件表示应用确认收到了其它应用发送过来的远程事件。流程结束
6. 如果该远程事件是其它应用发送给应用自身的(事件的接收方是应用自身)，那么进行步骤 7 和 8，否则执行步骤 9
7. 该远程事件不是应用自身发送(事件发送方不是应用自身)的话，将该事件以本地的方式发送出去。应用自身一开始已经在本地被对应的消息接收方处理了，无需再次发送
8. 如果开启了 `AckRemoteApplicationEvent` 远程确认事件的开关，构造 `AckRemoteApplicationEvent` 事件并在远程和本地都发送该事件(本地发送是因为步骤 5 没有进行本地 `AckRemoteApplicationEvent` 事件的发送，也就是自身应用对自身应用确认; 远程发送是为了告诉其它应用，自身应用收到了消息)
9. 如果开启了消息记录 Trace 的开关，本地构造并发送 `SentApplicationEvent` 事件



`bus-env` 触发后所有节点的 `EnvironmentChangeListener` 监听到了配置的变化，控制台都会打印出以下信息：

```log
o.s.c.b.event.EnvironmentChangeListener  : Received remote environment change request. Keys/values to update {hangzhou=alibaba}
```

如果在本地监听远程确认事件 `AckRemoteApplicationEvent`，都会收到所有节点的信息，比如 node5 节点的控制台监听到的 `AckRemoteApplicationEvent` 事件如下：

```log
ServiceId [rocketmq-bus-node5:10005] listeners on {"type":"AckRemoteApplicationEvent","timestamp":1554124670484,"originService":"rocketmq-bus-node5:10005","destinationService":"**","id":"375f0426-c24e-4904-bce1-5e09371fc9bc","ackId":"750d033f-356a-4aad-8cf0-3481ace8698c","ackDestinationService":"**","event":"org.springframework.cloud.bus.event.EnvironmentChangeRemoteApplicationEvent"}
ServiceId [rocketmq-bus-node5:10005] listeners on {"type":"AckRemoteApplicationEvent","timestamp":1554124670184,"originService":"rocketmq-bus-node1:10001","destinationService":"**","id":"91f06cf1-4bd9-4dd8-9526-9299a35bb7cc","ackId":"750d033f-356a-4aad-8cf0-3481ace8698c","ackDestinationService":"**","event":"org.springframework.cloud.bus.event.EnvironmentChangeRemoteApplicationEvent"}
ServiceId [rocketmq-bus-node5:10005] listeners on {"type":"AckRemoteApplicationEvent","timestamp":1554124670402,"originService":"rocketmq-bus-node2:10002","destinationService":"**","id":"7df3963c-7c3e-4549-9a22-a23fa90a6b85","ackId":"750d033f-356a-4aad-8cf0-3481ace8698c","ackDestinationService":"**","event":"org.springframework.cloud.bus.event.EnvironmentChangeRemoteApplicationEvent"}
ServiceId [rocketmq-bus-node5:10005] listeners on {"type":"AckRemoteApplicationEvent","timestamp":1554124670406,"originService":"rocketmq-bus-node3:10003","destinationService":"**","id":"728b45ee-5e26-46c2-af1a-e8d1571e5d3a","ackId":"750d033f-356a-4aad-8cf0-3481ace8698c","ackDestinationService":"**","event":"org.springframework.cloud.bus.event.EnvironmentChangeRemoteApplicationEvent"}
ServiceId [rocketmq-bus-node5:10005] listeners on {"type":"AckRemoteApplicationEvent","timestamp":1554124670427,"originService":"rocketmq-bus-node4:10004","destinationService":"**","id":"1812fd6d-6f98-4e5b-a38a-4b11aee08aeb","ackId":"750d033f-356a-4aad-8cf0-3481ace8698c","ackDestinationService":"**","event":"org.springframework.cloud.bus.event.EnvironmentChangeRemoteApplicationEvent"}
```

回答一下这个章节开头提到的 4 个问题：

- 消息是如何发送的: 在 `BusAutoConfiguration#acceptLocal` 方法中通过 Spring Cloud Stream 发送事件到 `springCloudBus` topic 中
- 消息是如何接收的: 在 `BusAutoConfiguration#acceptRemote` 方法中通过 Spring Cloud Stream 接收 `springCloudBus` topic 的消息
- destination 是如何匹配的: 在 `BusAutoConfiguration#acceptRemote` 方法中接收远程事件方法里对 destination 进行匹配
- 远程事件收到后如何触发下一个 action: Bus 内部通过 Spring 的事件机制接收本地的 `RemoteApplicationEvent` 具体的实现事件再做下一步的动作(比如 `EnvironmentChangeListener` 接收了 `EnvironmentChangeRemoteApplicationEvent` 事件， `RefreshListener` 接收了 `RefreshRemoteApplicationEvent` 事件)

### 总结

Spring Cloud Bus 自身内容还是比较少的，不过需要提前了解 Spring Cloud Stream 体系以及 Spring 自身的事件机制，在此基础上再了解 Spring Cloud Bus 对本地事件和远程事件的处理逻辑。

目前 Bus 内置的远程事件较少，大多数为配置相关的事件，我们可以继承 `RemoteApplicationEvent` 并配合 `@RemoteApplicationEventScan` 注解构建自身的微服务消息体系。
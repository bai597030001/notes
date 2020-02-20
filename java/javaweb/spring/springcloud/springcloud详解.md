# 分布式、集群、微服务、SOA

分布式：不同模块部署在不同服务器上
作用：分布式解决网站高并发带来问题

集群：多台服务器部署相同应用构成一个集群
作用：通过负载均衡设备共同对外提供服务

SOA：业务系统分解为多个组件，让每个组件都独立提供离散，自治，可复用的服务能力，通过服务的组合和编排来实现上层的业务流程
作用：简化维护,降低整体风险,伸缩灵活

微服务：架构设计概念,各服务间隔离（分布式也是隔离）,自治（分布式依赖整体组合）其它特性(单一职责,边界,异步通信,独立部署)是分布式概念的跟严格执行SOA到微服务架构的演进过程
作用：各服务可独立应用，组合服务也可系统应用



分布式：一个业务分拆多个子业务，部署在不同的服务器上

集群：同一个业务，部署在多个服务器上



# CAP理论

- C：数据一致性(consistency)

- - **所有**节点拥有数据的最新版本

- A：可用性(availability)

- - 数据具备高可用性

- P：分区容错性(partition-tolerance)

- - **容忍网络出现分区**，分区之间网络不可达。



# 引言 spring cloud

前面也讲了，从分布式/微服务的角度而言：就是把我们一**大**的项目，**分解**成多个**小**的模块。这些小的模块组合起来，完成功能。

![](img/springcloud2.jpg)



拆分出多个模块以后，就会出现**各种各样**的问题，而SpringCloud提供了**一整套**的解决方案！



# eureka

spring cloud eureka作为spring cloud提供的服务发现注册中心，其在2.0之后闭源。Consul慢慢会成为主流。zk也可以作为服务发现注册中心。



## Eureka细节

```yaml
register-with-eureka: false     #false表示不向注册中心注册自己。
fetch-registry: false     #false表示自己端就是注册中心，我的职责就是维护服务实例，并不需要去检索服务
```

Eureka Client**分为服务提供者和服务消费者**。

- 但很可能，某服务**既是服务提供者又是服务消费者**。

如果在网上看到SpringCloud的**某个服务配置没有"注册"到Eureka-Server也不用过于惊讶**(但是它是可以获取Eureka服务清单的)

- 很可能只是作者把该服务认作为**单纯的服务消费者**，单纯的服务消费者无需对外提供服务，也就无须注册到Eureka中了



```yaml
eureka:
  client:
    register-with-eureka: false  # 当前微服务不注册到eureka中(消费端)
    service-url: 
      defaultZone: http://eureka7001.com:7001/eureka/,http://eureka7002.com:7002/eureka/,http://eureka7003.com:7003/eureka/
```



下面是Eureka的治理机制：

- 服务提供者

- - **服务注册：启动的时候会通过发送REST请求的方式将自己注册到Eureka Server上**，同时带上了自身服务的一些元数据信息。
  - **服务续约：**在注册完服务之后，**服务提供者会维护一个心跳**用来持续告诉Eureka Server: "我还活着 ” 、
  - **服务下线：当服务实例进行正常的关闭操作时，它会触发一个服务下线的REST请求**给Eureka Server, 告诉服务注册中心：“我要下线了 ”。

- 服务消费者

- - **获取服务：当我们启动服务消费者**的时候，它会发送一个REST请求给服务注册中心，来获取上面注册的服务清单
  - **服务调用：服务消费者在获取服务清单后，通过服务名**可以获得具体提供服务的实例名和该实例的元数据信息。在进行服务调用的时候，**优先访问同处一个Zone中的服务提供方**。



- Eureka Server(服务注册中心)：

- - **失效剔除：**默认每隔一段时间（默认为60秒） 将当前清单中超时（默认为90秒）**没有续约的服务剔除出去**。
  - **自我保护：**。EurekaServer 在运行期间，会统计心跳失败的比例在15分钟之内是否低于85%(通常由于网络不稳定导致)。 Eureka Server会将当前的**实例注册信息保护起来**， 让这些实例不会过期，尽可能**保护这些注册信息**。



# Ribbon



# springcloud概览

![](img/springcloud1.jpg)

## springboot与springcloud

- Spring boot 是 Spring 的一套快速配置脚手架，可以基于spring boot 快速开发单个**微服务**
- Spring Boot，看名字就知道是**Spring的引导**，就是**用于启动Spring**的，使得Spring的学习和使用变得快速无痛。不仅适合替换原有的工程结构，更适合**微服务开发**。
- Spring Cloud**基于Spring Boot**，为微服务体系开发中的**架构问题**，提供了一整套的解决方案——服务注册与发现，服务消费，服务保护与熔断，网关，分布式调用追踪，分布式配置管理等。
- Spring Cloud是一个基于Spring Boot实现的云应用开发工具；Spring boot专注于快速、方便集成的**单个个体**，Spring Cloud是关注**全局的服务治理框架**

![](img/springcloud2.png)



## SpringCloud的基础功能

- 服务治理： Spring Cloud Eureka
- 客户端负载均衡： Spring Cloud Ribbon
- 服务容错保护： Spring Cloud Hystrix
- 声明式服务调用： Spring Cloud Feign
- API网关服务：Spring Cloud Zuul
- 分布式配置中心： Spring Cloud Config

## SpringCloud的高级功能

- 消息总线： Spring Cloud Bus
- 消息驱动的微服务： Spring Cloud Stream
- 分布式服务跟踪： Spring Cloud Sleuth



# springcloud注解



## @SpringBootApplication

是springboot启动类，包括三个注解，他们的作用分别是：

@Configuration：表示将该类作用springboot配置文件类



@EnableAutoConfiguration：表示程序启动时，自动加载springboot默认的配置



@ComponentScan：表示程序启动是，自动扫描当前包及子包下所有类



## @EnableDiscoveryClient && @EnableEurekaClient



@EnableDiscoveryClient基于spring-cloud-commons，@EnableEurekaClient基于spring-cloud-netflix，如果选用的注册中心是eureka，那么就推荐@EnableEurekaClient，如果是其他的注册中心，那么推荐使用@EnableDiscoveryClient。



@ComponentScan && @Configuration && @EnableAutoConfiguration



## @ComponentScan 

如果不设置basePackage的话 默认会扫描包的所有类，所以最好还是写上basePackage （@componentScan({" ... "}）,减少加载时间。默认扫描\**/\*.class路径 比如这个注解在com.wuhulala 下面 ，那么会扫描这个包下的所有类还有子包的所有类,比如com.wuhulala.service包的应用

@Configuration 表示这个类中定义了Bean，会把这个类中bean加载到spring容器中



@EnableAutoConfiguration表示 会在你开启某些功能的时候自动配置，这个注解告诉Spring Boot根据添加的jar依赖猜测你想如何配置Spring。由于spring-boot-starter-web添加了Tomcat和Spring MVC，所以auto-configuration将假定你正在开发一个web应用，并对Spring进行相应地设置。



## @Mapper && @MapperScan

Mapper类上面添加注解@Mapper，这种方式要求每一个mapper类都需要添加此注解

使用@MapperScan可以指定要扫描的Mapper类的包的路径（@MapperScan("com.demo.\*.mapper") || @MapperScan("com.test.\*.mapper", "com.demo.\*.mapper")）



## @EnableTransactionManagement && @Transactional



Spring Boot 使用事务非常简单，首先使用注解 @EnableTransactionManagement （启注解事务管理，等同于xml配置方式的 ）开启事务支持后，然后在访问数据库的Service方法上添加注解 @Transactional 便可。



## @Async && @EnableAsync



@EnableAsync注解的意思是可以异步执行，就是开启多线程的意思。可以标注在方法、类上。



为了让@Async注解能够生效，需要在Spring Boot的主程序中配置@EnableAsync



@Async所修饰的函数不要定义为static类型，这样异步调用不会生效



## @Bean && @Configuration



@Bean标注在方法上(返回某个实例的方法)，等价于spring的xml配置文件中的，作用为：注册bean对象



@Configuration标注在类上，相当于把该类作为spring的xml配置文件中的，作用为：配置spring容器(应用上下文)



## @LoadBalanced



Spring Cloud的commons模块提供了一个@LoadBalanced注解，方便我们对RestTemplate添加一个LoadBalancerClient，以实现客户端负载均衡。通过源码可以发现这是一个标记注解,我们可以通过ribbon实现客户端的负载均衡功能。
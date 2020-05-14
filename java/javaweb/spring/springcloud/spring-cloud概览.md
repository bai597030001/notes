# 系统架构演变

单体应用架构--->垂直应用架构--->分布式架构--->SOA架构--->微服务架构，当然还有悄然兴起的Service Mesh(服务网格化)。



# 微服务架构解决方案

单体应用走向微服务以后，面临服务相关的各种问题。

## ServiceComb

Apache ServiceComb，前身是华为云的微服务引擎 CSE (Cloud Service Engine) 云服务，是全球
首个Apache微服务顶级项目。它提供了一站式的微服务开源解决方案，致力于帮助企业、用户和开发
者将企业应用轻松微服务化上云，并实现对微服务应用的高效运维管理。



## spring cloud

spring基于springboot提供的微服务一站式解决方案。

```properties
服务治理: Spring Cloud Eureka

客户端负载均衡: Spring Cloud Ribbon

服务容错保护: Spring Cloud Hystrix，Turbine

声明式服务调用: Spring Cloud Feign

API网关服务: Spring Cloud Zuul，Spring Cloud Gateway

分布式配置中心: Spring Cloud Config

分布式服务跟踪: Spring Cloud Sleuth，Zipkin
```



## spring cloud alibaba

阿里提供的微服务一站式解决方案。内部继承阿里众多组件。

```properties
Sentinel: 把流量作为切入点，从流量控制、熔断降级、系统负载保护等多个维度保护服务的稳定性。

Nacos: 一个更易于构建云原生应用的动态服务发现、配置管理和服务管理平台。

RocketMQ: 一款开源的分布式消息系统，基于高可用分布式集群技术，提供低延时的、高可靠的消息发布与订阅服务。

Dubbo: Apache Dubbo 是一款高性能 Java RPC 框架。

Seata: 阿里巴巴开源产品，一个易于使用的高性能微服务分布式事务解决方案。

Alibaba Cloud ACM: 一款在分布式架构环境中对应用配置进行集中管理和推送的应用配置中心产品。

Alibaba Cloud OSS: 阿里云对象存储服务（Object Storage Service，简称 OSS），是阿里云提供的海量、安全、低成本、高可靠的云存储服务。您可以在任何应用、任何时间、任何地点存储和访问任意类型的数据。

Alibaba Cloud SchedulerX: 阿里中间件团队开发的一款分布式任务调度产品，提供秒级、精准、高可靠、高可用的定时（基于 Cron 表达式）任务调度服务。

Alibaba Cloud SMS: 覆盖全球的短信服务，友好、高效、智能的互联化通讯能力，帮助企业迅速搭建客户触达通道。
```



## service mesh

服务网格，下一代微服务解决方案。



# springcloud概览

![](img/springcloud1.jpg)



![](img/springcloud3.png)



## springboot与springcloud

- Spring boot 是 Spring 的一套快速配置脚手架，可以基于spring boot 快速开发单个**微服务**
- Spring Boot，看名字就知道是**Spring的引导**，就是**用于启动Spring**的，使得Spring的学习和使用变得快速无痛。不仅适合替换原有的工程结构，更适合**微服务开发**。
- Spring Cloud**基于Spring Boot**，为微服务体系开发中的**架构问题**，提供了一整套的解决方案——服务注册与发现，服务消费，服务保护与熔断，网关，分布式调用追踪，分布式配置管理等。
- Spring Cloud是一个基于Spring Boot实现的云应用开发工具；Spring boot专注于快速、方便集成的**单个个体**，Spring Cloud是关注**全局的服务治理框架**



## SpringCloud组件

- 服务治理： Spring Cloud Eureka
- 客户端负载均衡： Spring Cloud Ribbon
- 服务容错保护： Spring Cloud Hystrix，Turbine
- 声明式服务调用： Spring Cloud Feign
- API网关服务：Spring Cloud Zuul，Spring Cloud Gateway
- 分布式配置中心： Spring Cloud Config
- 分布式服务跟踪： Spring Cloud Sleuth，Zipkin



- 消息总线： Spring Cloud Bus
- 消息驱动的微服务： Spring Cloud Stream





# 服务治理

服务的自动注册与发现、服务剔除



# 服务调用

RESTful调用

RPC调用（dubbo，gRPC，probuf）



# 服务网关

不同的微服务一般会有不同的网络地址，如果让客户端直接与各个微服务通信：

客户端需要调用不同的url地址，增加难度

在一定的场景下，存在跨域请求的问题

每个微服务都需要进行单独的身份认证

针对这些问题，API网关顺势而生。



# 服务容错

在微服务当中，一个请求经常会涉及到调用几个服务，如果其中某个服务不可用，没有做服务容错
的话，极有可能会造成一连串的服务不可用，这就是雪崩效应。



# 链路追踪

微服务按照不同的维度进行拆分，一次请求往往需要涉及到多个服务。这些模块有可能布在了几千台服务器，横跨多个不同的数据中心。因此，就需要对一次请求涉及的多个服务链路进行日志记录，性能监控即链路追踪
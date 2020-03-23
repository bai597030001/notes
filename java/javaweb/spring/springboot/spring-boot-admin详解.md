微服务的特点决定了功能模块的部署是分布式的，大部分功能模块都是运行在不同的机器上，彼此通过服务调用进行交互，前后台的业务流会经过很多个微服务的处理和传递。在这种框架下，微服务的监控显得尤为重要



# Spring Boot Actuator

Spring Boot 的 Actuator 提供了很多生产级的特性，比如监控和度量Spring Boot 应用程序。Actuator 的这些特性可以通过众多 REST 接口、远程 shell 和 JMX 获得。



**默认提供的endpoint**

| HTTP 方法 | 路径            | 描述                                                         |
| --------- | --------------- | ------------------------------------------------------------ |
| GET       | /autoconfig     | 提供了一份自动配置报告，记录哪些自动配置条件通过了，哪些没通过 |
| GET       | /configprops    | 描述配置属性(包含默认值)如何注入Bean                         |
| GET       | /beans          | 描述应用程序上下文里全部的Bean，以及它们的关系               |
| GET       | /dump           | 获取线程活动的快照                                           |
| GET       | /env            | 获取全部环境属性                                             |
| GET       | /env/{name}     | 根据名称获取特定的环境属性值                                 |
| GET       | /health         | 报告应用程序的健康指标，这些值由HealthIndicator的实现类提供  |
| GET       | /info           | 获取应用程序的定制信息，这些信息由info打头的属性提供         |
| GET       | /mappings       | 描述全部的URI路径，以及它们和控制器(包含Actuator端点)的映射关系 |
| GET       | /metrics        | 报告各种应用程序度量信息，比如内存用量和HTTP请求计数         |
| GET       | /metrics/{name} | 报告指定名称的应用程序度量值                                 |
| POST      | /shutdown       | 关闭应用程序，要求endpoints.shutdown.enabled设置为true       |
| GET       | /trace          | 提供基本的HTTP请求跟踪信息(时间戳、HTTP头等)                 |



**自定义endpoint**



## 快速上手

```xml
<dependencies>
  <dependency>
    <groupId>org.springframework.boot</groupId>
    <artifactId>spring-boot-starter-web</artifactId>
  </dependency>
  <dependency>
      <groupId>org.springframework.boot</groupId>
      <artifactId>spring-boot-starter-actuator</artifactId>
  </dependency>
</dependencies>
```



```properties
info.app.name=spring-boot-actuator
info.app.version= 1.0.0
info.app.test=test

management.endpoints.web.exposure.include=*
management.endpoint.health.show-details=always
#management.endpoints.web.base-path=/monitor

management.endpoint.shutdown.enabled=true
```

- `management.endpoints.web.base-path=/monitor` 代表启用单独的url地址来监控 Spring Boot 应用，为了安全一般都启用独立的端口来访问后端的监控信息
- `management.endpoint.shutdown.enabled=true` 启用接口关闭 Spring Boot



## 详解

在 Spring Boot 2.x 中为了安全期间，Actuator 只开放了两个端点 `/actuator/health` 和 `/actuator/info`。可以在配置文件中设置打开。

可以打开所有的监控点

```properties
management.endpoints.web.exposure.include=*
```

也可以选择打开部分

```properties
management.endpoints.web.exposure.exclude=beans,trace
```

Actuator 默认所有的监控点路径都在`/actuator/*`，当然如果有需要这个路径也支持定制。

```properties
management.endpoints.web.base-path=/manage
```

设置完重启后，再次访问地址就会变成`/manage/*`



# Spring Boot Admin

Spring Boot Admin是一个开源社区项目，用于管理和监控SpringBoot应用程序。 

应用程序作为Spring Boot Admin Client向为Spring Boot Admin Server注册（通过HTTP）或使用SpringCloud注册中心（例如Eureka，Consul）发现。 UI是的Vue.js应用程序，展示Spring Boot Admin Client的Actuator端点上的一些监控。



Spring Boot Admin是一个针对spring-boot的actuator接口进行UI美化封装的监控工具。它可以：在列表中浏览所有被监控spring-boot项目的基本信息，详细的Health信息、内存信息、JVM信息、垃圾回收信息、各种配置信息（比如数据源、缓存列表和命中率）等，还可以直接修改logger的level。



## 监控单体应用



### Admin Server端

```xml
<dependencies>
    <dependency>
        <groupId>org.springframework.boot</groupId>
        <artifactId>spring-boot-starter</artifactId>
    </dependency>

    <dependency>
        <groupId>org.springframework.boot</groupId>
        <artifactId>spring-boot-starter-web</artifactId>
    </dependency>

    <dependency>
        <groupId>org.springframework.boot</groupId>
        <artifactId>spring-boot-starter-test</artifactId>
        <scope>test</scope>
        <exclusions>
            <exclusion>
                <groupId>org.junit.vintage</groupId>
                <artifactId>junit-vintage-engine</artifactId>
            </exclusion>
        </exclusions>
    </dependency>

    <dependency>
        <groupId>de.codecentric</groupId>
        <artifactId>spring-boot-admin-starter-server</artifactId>
        <version>2.1.5</version>
    </dependency>
    <dependency>
        <groupId>de.codecentric</groupId>
        <artifactId>spring-boot-admin-server-ui</artifactId>
        <version>2.1.5</version>
    </dependency>

    <dependency>
        <groupId>org.springframework.boot</groupId>
        <artifactId>spring-boot-starter-actuator</artifactId>
    </dependency>

</dependencies>

<dependencyManagement>
    <dependencies>
        <dependency>
            <groupId>de.codecentric</groupId>
            <artifactId>spring-boot-admin-dependencies</artifactId>
            <version>2.1.5</version>
        </dependency>
    </dependencies>
</dependencyManagement>

```



配置文件

```yaml
server:
  port: 9091
spring:
  application:
    name: admin-server
```



启动

```java
@SpringBootApplication
@EnableAdminServer
public class SpringBootAdminServerApplication {
    public static void main(String[] args) {
        SpringApplication.run(SpringBootAdminServerApplication.class, args);
    }
}
```



启动服务端，浏览器访问`http://localhost:9091`



### Admin Client端



```xml
<dependency>
    <groupId>de.codecentric</groupId>
    <artifactId>spring-boot-admin-starter-client</artifactId>
    <version>2.1.5</version>
</dependency>

<dependencyManagement>
    <dependencies>
        <dependency>
            <groupId>de.codecentric</groupId>
            <artifactId>spring-boot-admin-dependencies</artifactId>
            <version>2.1.5</version>
        </dependency>
    </dependencies>
</dependencyManagement>
```



```yaml
server:
  port: 8082

spring:
  application:
    name: admin-client-pri
  boot:
    admin:
      client:
      	#配置需要接入的admin-server地址
        url: http://localhost:9091

logging:
  # 注意: spring boot admin想要 查看日志信息，需要添加以下配置才能开启；
  file: admin-client-pri.log #添加开启admin的日志监控

# management.endpoints.web.exposure.include='*'，代表开启全部监控
# management.endpoint.health.show-details=always，health endpoint开启显示全部细节。默认情况下/actuator/health是公开的，但不显示细节。
# management.endpoints.web.base-path=/monitor，启用指定的url地址访问根路径，默认路径为/actuator/*，开启则访问路径变为/monitor/*。
management:
  endpoints:
    web:
      exposure:
        include: '*'
  #      base-path: /monitor
  endpoint:
    health:
      show-details: always
    shutdown:
      enabled: true
```



启动

```java
@SpringBootApplication
public class AdminClientApplication {
  public static void main(String[] args) {
    SpringApplication.run(AdminClientApplication.class, args);
  }
}
```



配置完成之后，启动Client端服务，再次访问服务：`http://localhost:8000`可以看到客户端的相关信息。



### 缺点

每个被监控的服务都必须配置 Spring Boot Admin 的地址，还得引入依赖。

我们可以将 Spring Boot Admin 也注册到 Eureka 中，然后自动获取 Eureka 中注册的服务信息来统一查看。



## 结合注册中心使用

Spring Boot Admin将系统中，被监控的客户端通过**向监控服务端注册（HTTP）**，即可让服务端展示相关监控信息。

但是，如果系统内集成了SC（即注册中心），则**只需要向注册中心注册即可（eureka，consul，zookeeper等）**。



Spring Boot Admin结合Spring Cloud 注册中心使用，只需将admin-server和注册中心整合即可，admin-server 会自动从注册中心获取服务列表，然后挨个获取监控信息。



### 集成 Eureka

如果我们使用的是单个Spring Boot应用，就需要在每一个被监控的应用中配置Admin Server的地址信息；

如果应用都注册在注册中心（如eureka，zookeeper，consul）中就不需要再对每个应用进行配置，Spring Boot Admin会自动从注册中心抓取应用的相关信息，进行注册。



#### 新建eureka server

pom.xml

```xml
<properties>
    <java.version>1.8</java.version>
    <java.version>1.8</java.version>
    <project.build.sourceEncoding>UTF-8</project.build.sourceEncoding>
    <project.reporting.outputEncoding>UTF-8</project.reporting.outputEncoding>
    <spring-cloud.version>Greenwich.RELEASE</spring-cloud.version>
</properties>
<dependencies>

    <dependency>
        <groupId>org.springframework.boot</groupId>
        <artifactId>spring-boot-starter-web</artifactId>
    </dependency>

    <dependency>
        <groupId>de.codecentric</groupId>
        <artifactId>spring-boot-admin-starter-server</artifactId>
        <version>2.1.5</version>
    </dependency>
    <dependency>
        <groupId>de.codecentric</groupId>
        <artifactId>spring-boot-admin-dependencies</artifactId>
        <version>2.1.5</version>
        <type>pom</type>
        <scope>import</scope>
    </dependency>

    <dependency>
        <groupId>org.springframework.cloud</groupId>
        <artifactId>spring-cloud-starter-netflix-eureka-server</artifactId>
    </dependency>

</dependencies>

<dependencyManagement>
    <dependencies>

        <dependency>
            <groupId>de.codecentric</groupId>
            <artifactId>spring-boot-admin-dependencies</artifactId>
            <version>2.1.5</version>
        </dependency>

        <dependency>
            <groupId>org.springframework.cloud</groupId>
            <artifactId>spring-cloud-dependencies</artifactId>
            <version>${spring-cloud.version}</version>
            <type>pom</type>
            <scope>import</scope>
        </dependency>

    </dependencies>
</dependencyManagement>
```



application.yaml

```yaml
spring:
  application:
    name: eureka-server
server:
  port: 8761
eureka:
  instance:
    hostname: localhost
    # 在多机器独立部署的情况下是没有问题的，配置 prefer-ip-address 为 ture，
    # 代表发现服务时候优先按照 IP 去搜寻，对于多集群而言，可以保证尽快准确搜索到服务。
    # 而对于单机部署来说，IP 地址都是相同的，这会导致其余注册中心出现在
    # unavailable-replicas (不可用副本) 中。所以单机部署时候不建议开启这个
    # 参数（默认值为 false），多机部署时候可以开启。
    prefer-ip-address: false
    lease-renewal-interval-in-seconds: 5 # heartbeat interval
    lease-expiration-duration-in-seconds: 15 # offline timeout
  client:
    service-url:
      defaultZone: http://${eureka.instance.hostname}:${server.port}/eureka/
    register-with-eureka: true # 设置为 false,代表不向注册中心注册自己
    fetch-registry: false
management:
  endpoints:
    web:
      exposure:
        include: "*"
  endpoint:
    health:
      show-details: ALWAYS
```



启动

```java
@SpringBootApplication
@EnableEurekaServer
public class CloudEurekaServerApplication {

    public static void main(String[] args) {
        SpringApplication.run(CloudEurekaServerApplication.class, args);
    }

}
```





#### 修改admin-server



pom.xml

```xml
<dependency>
    <groupId>org.springframework.cloud</groupId>
    <artifactId>spring-cloud-starter-netflix-eureka-client</artifactId>
</dependency>
```



application.yaml，只需添加注册中心配置，替换原来得spring.boot配置的admin server配置即可

```yaml
server:
  port: 8082

spring:
  application:
    name: admin-client-pri
#  boot:
#    admin:
#      client:
#      	#配置需要接入的admin-server地址
#        url: http://localhost:9091

eureka:
  client:
    register-with-eureka: true
    fetch-registry: true
    service-url:
      defaultZone: http://localhost:8761/eureka/

logging:
  # 注意: spring boot admin想要 查看日志信息，需要添加以下配置才能开启；
  file: admin-client-pri.log #添加开启admin的日志监控

management:
  endpoints:
    web:
      exposure:
        include: '*'
  #      base-path: /monitor
  endpoint:
    health:
      show-details: always
    shutdown:
      enabled: true
```



启用服务注册

```java
@EnableDiscoveryClient
@EnableAdminServer
@SpringBootApplication
public class AdminServerApplication {

    public static void main(String[] args) {
        SpringApplication.run(AdminServerApplication.class, args);
    }

}
```



#### 修改admin-client



pom.xml

```xml
<dependency>
    <groupId>org.springframework.cloud</groupId>
    <artifactId>spring-cloud-starter-netflix-eureka-client</artifactId>
</dependency>
```



进行配置

```yaml
spring:
  application:
    name: admin-client
server:
  port: 9305
management:
  endpoints:
    web:
      exposure:
        include: '*'
  endpoint:
    health:
      show-details: always
logging:
  file: admin-client.log #添加开启admin的日志监控
eureka:
  client:
    register-with-eureka: true
    fetch-registry: true
    service-url:
      defaultZone: http://localhost:8761/eureka/
```



启动类

```java
@EnableDiscoveryClient
@SpringBootApplication
public class AdminClientApplication {

    public static void main(String[] args) {
        SpringApplication.run(AdminClientApplication.class, args);
    }

}
```


# zuul

`Zuul`支持`Ribbon`和`Hystrix`，也能够实现客户端的负载均衡。我们的`Feign`不也是实现客户端的负载均衡和`Hystrix`的吗？既然`Zuul`已经能够实现了，那我们的`Feign`还有必要吗

> - `zuul`是对外暴露的唯一接口，相当于路由的是controller的请求，而`Ribbon`和`Fegin`路由了`service`的请求
> - `zuul`做最外层请求的负载均衡 ，而`Ribbon`和`Fegin`做的是系统内部各个微服务的`service`的调用的负载均衡



## 使用

```xml
<properties>
    <java.version>1.8</java.version>
    <spring-cloud.version>Greenwich.SR2</spring-cloud.version>
</properties>

<dependencyManagement>
    <dependencies>
        <dependency>
            <groupId>org.springframework.cloud</groupId>
            <artifactId>spring-cloud-dependencies</artifactId>
            <version>${spring-cloud.version}</version>
            <type>pom</type>
            <scope>import</scope>
        </dependency>
    </dependencies>
</dependencyManagement>

<dependencies>

    <dependency>
        <groupId>org.springframework.cloud</groupId>
        <artifactId>spring-cloud-starter-netflix-zuul</artifactId>
    </dependency>

    <dependency>
        <groupId>org.springframework.cloud</groupId>
        <artifactId>spring-cloud-starter-netflix-eureka-client</artifactId>
    </dependency>

    <dependency>
        <groupId>org.springframework.boot</groupId>
        <artifactId>spring-boot-starter-actuator</artifactId>
    </dependency>
</dependencies>

<repositories>
    <repository>
        <id>spring-milestones</id>
        <name>Spring Milestones</name>
        <url>https://repo.spring.io/milestone</url>
        <snapshots>
            <enabled>false</enabled>
        </snapshots>
    </repository>
    <repository>
        <id>repository.springframework.maven.release</id>
        <name>Spring Framework Maven Release Repository</name>
        <url>http://maven.springframework.org/milestone/</url>
    </repository>
    <repository>
        <id>org.springframework</id>
        <url> http://maven.springframework.org/snapshot</url>
    </repository>
    <repository>
        <id>spring-milestone</id>
        <name>Spring Maven MILESTONE Repository</name>
        <url>http://repo.spring.io/libs-milestone</url>
    </repository>
    <repository>
        <id>spring-release</id>
        <name>Spring Maven RELEASE Repository</name>
        <url>http://repo.spring.io/libs-release</url>
    </repository>
</repositories>
```



```java
/**
 * @EnableZuulProxy --> { @EnableCircuitBreaker、@EnableDiscoveryClient } 包含了 eureka 客户端注解，同时也包含了 Hystrix 断路器模块注解。
 */
@SpringBootApplication
@EnableZuulProxy
public class CloudZuulApplication {

    public static void main(String[] args) {
        SpringApplication.run(CloudZuulApplication.class, args);
    }

}
```



```java
package com.example.cloudzuul;
/**
 * 用于定制FallBack返回的body、状态码、消息头Header
 */
@Component
public class CommonZuulFallbackProvider implements FallbackProvider {

    private Logger logger = LoggerFactory.getLogger(this.getClass());

    /**
     * getRoute方法的返回值就是要监听的挂掉的微服务名字，这里只能是serviceId(该名称一定要是注册进入 eureka 微服务中的那个 serviceId 名称)，不能是url，
     * 指定为“xxx”，那么在xxx服务不存在时，就会去执行后面代码的逻辑，设置Code，body什么的自定义内容返回给调用者。
     * @return
     */
    @Override
    public String getRoute() {
        //微服务配了路由的话，就用配置的名称
        //return "xxx";
        //如果要为所有路由提供默认回退，可以创建FallbackProvider类型的bean并使getRoute方法返回*或null
        //return "*";
        return null;
    }

    @Override
    public ClientHttpResponse fallbackResponse(String route, Throwable cause) {
        if (cause != null) {
            String reason =cause.getMessage();
            logger.info("Excption {}", reason);
        }
        return new ClientHttpResponse() {
            @Override
            public HttpStatus getStatusCode() {
                return HttpStatus.OK;
            }

            @Override
            public int getRawStatusCode() {
                return 200;
            }

            @Override
            public String getStatusText(){
                return "OK";
            }

            @Override
            public void close() {

            }

            @Override
            public InputStream getBody() {
                return new ByteArrayInputStream("The service is unavailable.".getBytes());
            }

            @Override
            public HttpHeaders getHeaders() {
                HttpHeaders headers = new HttpHeaders();
                headers.setContentType(MediaType.APPLICATION_JSON);
                return headers;
            }
        };
    }
}
```



```yaml
server:
  port: 7777

spring:
  application:
    name: cloud-zuul

eureka:
  instance:
    leaseRenewalIntervalInSeconds: 10 #表示eureka client发送心跳给server端的频率
    health-check-url-path: /actuator/health #健康检查页面的URL，相对路径，默认使用 HTTP 访问，如果需要使用 HTTPS则需要使用绝对路径配置

  client:
    registryFetchIntervalSeconds: 5 #表示eureka client间隔多久去拉取服务注册信息，默认为30秒
    service-url:
      defaultZone: ${EUREKA_SERVICE_URL:http://localhost:8761}/eureka/ #服务注册中心地址

zuul:
  #是否开启重试功能
  retryable: true
ribbon:
  #服务请求连接超时时间（毫秒）
  ConnectTimeout: 1000
  #服务请求处理超时时间（毫秒）
  ReadTimeout: 5000
  #对当前服务的重试次数
  MaxAutoRetries: 2
  #切换相同Server的次数
  MaxAutoRetriesNextServer: 2

hystrix:
  command: #用于控制HystrixCommand的行为
    default:
      execution:
        isolation:
          thread:
            #配置HystrixCommand执行的超时时间，执行超过该时间会进行服务降级处理
            timeoutInMilliseconds: 144000
  threadpool:
    default:
      #并发执行的最大线程数，默认10
      coreSize: 200
      #BlockingQueue的最大队列数，默认值-1
      maxQueueSize: 1000
      #即使maxQueueSize没有达到，达到queueSizeRejectionThreshold该值后，请求也会被拒绝，默认值5
      queueSizeRejectionThreshold: 800

management:
  endpoints:
    web:
      exposure:
        # 暴露xxx端点，如需暴露多个，用,分隔；如需暴露所有端点，用'*'
        include: '*'
  #      base-path: /monitor
  endpoint:
    health:
      # 是否展示健康检查详情
      show-details: always
    shutdown:
      enabled: true
```



访问：在访问的`url`路径中加上访问的`serviceId`（`eureka`中注册的），并且将`port`换成`zuul`模块的。



## 常用功能



### 配置路由规则

- 我们可以通过修改`application.yml`中的配置来配置路由规则，这里我们将匹配`/userService/**`的请求路由到`user-service`服务上去，匹配`/feignService/**`的请求路由到`feign-service`上去。

```yaml
zuul:
  routes: #给服务配置路由
    user-service:
      path: /userService/**
    feign-service:
      path: /feignService/**
```

访问http://localhost:8801/userService/user/1可以发现请求路由到了`user-service`上了；

访问http://localhost:8801/feignService/user/1可以发现请求路由到了`feign-service`上了。



### 默认路由规则

- `Zuul`和`Eureka`结合使用，可以实现路由的自动配置，自动配置的路由以服务名称为匹配路径，相当于如下配置：

```yaml
zuul:
  routes: #给服务配置路由
    user-service:
      path: /user-service/**
    feign-service:
      path: /feign-service/**
```



如果不想使用默认的路由规则，可以添加以下配置来忽略默认路由配置：

```yaml
zuul:
  ignored-services: user-service,feign-service #关闭默认路由配置
```



### 配置访问前缀

```yaml
zuul:
  prefix: /proxy #给网关路由添加前缀
```





### 负载均衡功能



### Header过滤及重定向添加Host

- `Zuul`在请求路由时，默认会过滤掉一些敏感的头信息，以下配置可以防止路由时的`Cookie`及Authorization的丢失：

```yaml
zuul:
  sensitive-headers: Cookie,Set-Cookie,Authorization #配置过滤敏感的请求头信息，设置为空就不会过滤

  sensitive-headers: Cookie,Set-Cookie,Authorization #配置过滤敏感的请求头信息，设置为空就不会过滤

```



### 查看路由信息

> 我们可以通过`SpringBoot Actuator`来查看`Zuul`中的路由信息。



```xml
<dependency>
    <groupId>org.springframework.boot</groupId>
    <artifactId>spring-boot-starter-actuator</artifactId>
</dependency>
```



```yaml
management:
  endpoints:
    web:
      exposure:
        include: 'routes'
```

- 通过访问http://localhost:8801/actuator/routes查看简单路由信息
- 通过访问[http://localhost:8801/actuator/routes/details](http://localhost:8801/actuator/routes)查看详细路由信息



## 过滤器

> 路由与过滤是`Zuul`的两大核心功能，路由功能负责将外部请求转发到具体的服务实例上去，是实现统一访问入口的基础，过滤功能负责对请求过程进行额外的处理，是请求校验过滤及服务聚合的基础。



### 过滤器类型

> `Zuul`中有以下几种典型的过滤器类型。



```properties
pre：在请求被路由到目标服务前执行，比如权限校验、打印日志等功能；

routing：在请求被路由到目标服务时执行，这是使用Apache HttpClient或Netflix Ribbon构建和发送原始HTTP请求的地方；

post：在请求被路由到目标服务后执行，比如给目标服务的响应添加头信息，收集统计数据等功能；

error：请求在其他阶段发生错误时执行。

static：特殊的 Filter 具体的可以看 StaticResponseFilter，它允许从 Zuul 本身生成响应，而不是将请求转发到源
```



### 过滤器的生命周期

![](E:/notes/java/javaweb/spring/springcloud/img/cloud-zuul1.png)



### 自定义过滤器

- 添加`PreLogFilter`类继承`ZuulFilter`

> 这是一个前置过滤器，用于在请求路由到目标服务前打印请求日志。



```java
@Component
public class PreLogFilter extends ZuulFilter {
    private Logger LOGGER = LoggerFactory.getLogger(this.getClass());

    /**
     * 过滤器类型，有pre、routing、post、error四种。
     */
    @Override
    public String filterType() {
        return FilterConstants.PRE_TYPE;
    }

    /**
     * 过滤器执行顺序，数值越小优先级越高。
     */
    @Override
    public int filterOrder() {
        return 1;
    }

    /**
     * 是否进行过滤，返回true会执行过滤。
     */
    @Override
    public boolean shouldFilter() {
        return true;
    }

    /**
     * 自定义的过滤器逻辑，当shouldFilter()返回true时会执行。
     */
    @Override
    public Object run() throws ZuulException {
        RequestContext requestContext = RequestContext.getCurrentContext();
        HttpServletRequest request = requestContext.getRequest();
        String host = request.getRemoteHost();
        String method = request.getMethod();
        String uri = request.getRequestURI();
        LOGGER.info("Remote host:{},method:{},uri:{}", host, method, uri);
        return null;
    }
}
```



### 核心过滤器

| 过滤器名称              | 过滤类型 | 优先级 | 过滤器的作用                                                 |
| :---------------------- | :------- | :----- | :----------------------------------------------------------- |
| ServletDetectionFilter  | pre      | -3     | 检测当前请求是通过DispatcherServlet处理运行的还是ZuulServlet运行处理的。 |
| Servlet30WrapperFilter  | pre      | -2     | 对原始的HttpServletRequest进行包装。                         |
| FormBodyWrapperFilter   | pre      | -1     | 将Content-Type为application/x-www-form-urlencoded或multipart/form-data的请求包装成FormBodyRequestWrapper对象。 |
| DebugFilter             | route    | 1      | 根据zuul.debug.request的配置来决定是否打印debug日志。        |
| PreDecorationFilter     | route    | 5      | 对当前请求进行预处理以便执行后续操作。                       |
| RibbonRoutingFilter     | route    | 10     | 通过Ribbon和Hystrix来向服务实例发起请求，并将请求结果进行返回。 |
| SimpleHostRoutingFilter | route    | 100    | 只对请求上下文中有routeHost参数的进行处理，直接使用HttpClient向routeHost对应的物理地址进行转发。 |
| SendForwardFilter       | route    | 500    | 只对请求上下文中有forward.to参数的进行处理，进行本地跳转。   |
| SendErrorFilter         | post     | 0      | 当其他过滤器内部发生异常时的会由它来进行处理，产生错误响应。 |
| SendResponseFilter      | post     | 1000   | 利用请求上下文的响应信息来组织请求成功的响应内容。           |

### 禁用过滤器

- 我们可以对过滤器进行禁用的配置，配置格式如下：

```yaml
zuul:
  filterClassName:
    filter:
      disable: true 
```

- 以下是禁用`PreLogFilter`的示例配置：

```yaml
zuul:
  PreLogFilter:
    pre:
      disable: true 
```



## 常用配置

```yaml
zuul:
  routes: #给服务配置路由
    user-service:
      path: /userService/**
    feign-service:
      path: /feignService/**
  ignored-services: user-service,feign-service #关闭默认路由配置
  prefix: /proxy #给网关路由添加前缀
  sensitive-headers: Cookie,Set-Cookie,Authorization #配置过滤敏感的请求头信息，设置为空就不会过滤
  add-host-header: true #设置为true重定向是会添加host请求头
  retryable: true # 关闭重试机制
  PreLogFilter:
    pre:
      disable: false #控制是否启用过滤器
```



## 整合hystrix和ribbon

- `zuul`本身就支持`hystrix`和`ribbon`



```yaml
zuul:
  #是否开启重试功能
  retryable: true
ribbon:
  #服务请求连接超时时间（毫秒）
  ConnectTimeout: 1000
  #服务请求处理超时时间（毫秒）
  ReadTimeout: 5000
  #对当前服务的重试次数
  MaxAutoRetries: 2
  #切换相同Server的次数
  MaxAutoRetriesNextServer: 2

hystrix:
  command: #用于控制HystrixCommand的行为
    default:
      execution:
        isolation:
          thread:
            #配置HystrixCommand执行的超时时间，执行超过该时间会进行服务降级处理
            timeoutInMilliseconds: 144000
  threadpool:
    default:
      #并发执行的最大线程数，默认10
      coreSize: 200
      #BlockingQueue的最大队列数，默认值-1
      maxQueueSize: 1000
      #即使maxQueueSize没有达到，达到queueSizeRejectionThreshold该值后，请求也会被拒绝，默认值5
      queueSizeRejectionThreshold: 800
```



## zuul回退机制

>  提供一个回退机制当路由后面的服务发生故障时

```java
/**
 * 自定义Zuul回退机制处理器(提供一个回退机制当路由后面的服务发生故障时)
 * 用于定制FallBack返回的body、状态码、消息头Header
 */
@Component
public class CommonZuulFallbackProvider implements FallbackProvider {

    private Logger logger = LoggerFactory.getLogger(this.getClass());

    /**
     * getRoute方法的返回值就是要监听的挂掉的微服务名字，这里只能是serviceId(该名称一定要是注册进入 eureka 微服务中的那个 serviceId 名称)，不能是url，
     * 指定为“xxx”，那么在xxx服务不存在时，就会去执行后面代码的逻辑，设置Code，body什么的自定义内容返回给调用者。
     * @return
     */
    @Override
    public String getRoute() {
        //微服务配了路由的话，就用配置的名称
        //return "xxx";
        //如果要为所有路由提供默认回退，可以创建FallbackProvider类型的bean并使getRoute方法返回*或null
        //return "*";
        return null;
    }

    @Override
    public ClientHttpResponse fallbackResponse(String route, Throwable cause) {
        if (cause != null) {
            String reason =cause.getMessage();
            logger.info("Excption {}", reason);
        }
        return new ClientHttpResponse() {
            @Override
            public HttpStatus getStatusCode() {
                return HttpStatus.OK;
            }

            @Override
            public int getRawStatusCode() {
                return 200;
            }

            @Override
            public String getStatusText(){
                return "OK";
            }

            @Override
            public void close() {

            }

            @Override
            public InputStream getBody() {
                return new ByteArrayInputStream("The service is unavailable.".getBytes());
            }

            @Override
            public HttpHeaders getHeaders() {
                HttpHeaders headers = new HttpHeaders();
                headers.setContentType(MediaType.APPLICATION_JSON);
                return headers;
            }
        };
    }
}
```



## zuul网关重试机制



### 依赖

重试包

```xml
<dependency>
    <groupId>org.springframework.retry</groupId>
    <artifactId>spring-retry</artifactId>
</dependency>
```



### 配置

```yaml
zuul:
  #是否开启重试功能（所有路由都将会进行重试）（此属性默认是false，所以不会重试）
  retryable: true
# 有时候我们不希望所有路由都有重试机制，我们可以配置指定路由进行重试:
# 这里的routename默认情况下就是你的服务名(我们可以通过管理端点/routes看到都有哪些路由
zuul: 
	routes:
		<routename>:
			retryable: true
ribbon:
  #服务请求连接超时时间（毫秒）
  ConnectTimeout: 1000
  #服务请求处理超时时间（毫秒）
  ReadTimeout: 3000
  #对第一次请求的服务的重试次数
  MaxAutoRetries: 0
  #要重试的下一个服务的最大数量(不包括第一个服务)
  MaxAutoRetriesNextServer: 1
  # 默认为false,则只允许GET请求被重试
  OkToRetryOnAllOperations: true

hystrix:
  command: #用于控制HystrixCommand的行为
    default:
      execution:
        isolation:
          thread:
            #配置HystrixCommand执行的超时时间，执行超过该时间会进行服务降级处理
            timeoutInMilliseconds: 12000
  threadpool:
    default:
      #并发执行的最大线程数，默认10
      coreSize: 200
      #BlockingQueue的最大队列数，默认值-1
      maxQueueSize: 1000
      #即使maxQueueSize没有达到，达到queueSizeRejectionThreshold该值后，请求也会被拒绝，默认值5
      queueSizeRejectionThreshold: 800
```



发起调用，让同一服务的同一方法sleep不同时间(3s，5s)，通过zuul调用，发现可以进行重试调用



### 补偿策略

重试的时候还有补偿策略，例如重试时间间隔（默认是没有间隔：`org.springframework.retry.backoff.NoBackOffPolicy`），我们可以实现自己的补偿策略，也可以用内部实现的一些补偿策略(需要定义一个bean)，如指数级的补偿策略(1秒，2秒，4秒类似这种指数级睡眠间隔增长，不超过10秒):

```java
@Configuration
public class MyConfiguration {
    @Bean
    LoadBalancedBackOffPolicyFactory backOffPolciyFactory() {
        return new LoadBalancedBackOffPolicyFactory() {
            @Override
            public BackOffPolicy createBackOffPolicy(String service) {
                return new ExponentialBackOffPolicy();
            }
        };
    }
}
```



也可以正对某些响应状态码进行重试(当调用`rcmd-service-data`返回`404,502`的时候，进行重试，其他状态码不重试):

```yaml
rcmd-service-data:
  ribbon:
    retryableStatusCodes: 404,502
```



### 总结

- 当`ribbon`使用的是`httpclient`时，重试机制是默认关闭的，如果要启动重试机制需要在项目中引用`spring-retry`包,以及手工打开`zuul.retryable=true`设置,其实除此之外`spring.cloud.loadbalancer.retry.enabled=true` 也是需要设置的只不过这个值默认为`true`，所以此处可以忽略设置。

- 当`ribbon`使用的是`okhttp`时，重试机制是自动打开的，重试的效果与我们设置的`ribbon`超时时间以及重试次数都有关系。

- 此外，重试默认都是只支持`get`请求，如果我把请求方式修改为`post`重试是不生效的，我们需要设置`OkToRetryOnAllOperations=true`, 这种情况不太建议，因为`post`请求大多都是写入请求，如果要支持重试，服务自身的幂等性一定要健壮。

  

# zuul限流

- 什么是`RateLimiter`、`Spring Cloud Zuul RateLimiter`



`RateLimiter`是Google开源的实现了令牌桶算法的限流工具（速率限制器）。http://ifeve.com/guava-ratelimiter/

`Spring Cloud Zuul RateLimiter`结合`Zuul`对`RateLimiter`进行了封装，通过实现`ZuulFilter`提供了服务限流功能



## 原理

基于 `zuul` 网关的过滤功能，新增 `RateLimitPreFilter`（order：-1） 以及 `RateLimitPostFilter`（order：990） 过滤器。

在内存或者缓存或者数据库中维护一个 Map，根据请求以及限流粒度生成 key，接收到新的请求时，value 值加 1。和限流策略中的 limit 或者 quota 对比，如果超出则报错。



## 限流粒度

| 限流粒度/类型                    | 说明                                                         |
| :------------------------------- | :----------------------------------------------------------- |
| Authenticated User               | 使用经过身份验证的用户名或“匿名”                             |
| Request Origin                   | 使用用户原始请求                                             |
| URL                              | 使用下游服务的请求路径                                       |
| ROLE                             | 使用经过身份验证的用户角色                                   |
| Request method                   | 使用HTTP请求方法                                             |
| Global configuration per service | 这个不验证请求Origin，Authenticated User或URI，要使用这个，请不要设置type |



```java
public enum RateLimitType {
    /**
     * Rate limit policy considering the user's origin.
     */
    ORIGIN {
        @Override
        public boolean apply(HttpServletRequest request, Route route, RateLimitUtils rateLimitUtils, String matcher) {
            if (matcher.contains("/")) {
                SubnetUtils subnetUtils = new SubnetUtils(matcher);
                return subnetUtils.getInfo().isInRange(rateLimitUtils.getRemoteAddress(request));
            }
            return matcher.equals(rateLimitUtils.getRemoteAddress(request));
        }

        @Override
        public String key(HttpServletRequest request, Route route, RateLimitUtils rateLimitUtils, String matcher) {
            return rateLimitUtils.getRemoteAddress(request);
        }
    },

    /**
     * Rate limit policy considering the authenticated user.
     */
    USER {
        @Override
        public boolean apply(HttpServletRequest request, Route route, RateLimitUtils rateLimitUtils, String matcher) {
            return matcher.equals(rateLimitUtils.getUser(request));
        }

        @Override
        public String key(HttpServletRequest request, Route route, RateLimitUtils rateLimitUtils, String matcher) {
            return rateLimitUtils.getUser(request);
        }
    },

    /**
     * Rate limit policy considering the request path to the downstream service.
     */
    URL {
        @Override
        public boolean apply(HttpServletRequest request, Route route, RateLimitUtils rateLimitUtils, String matcher) {
            return route == null || route.getPath().startsWith(matcher);
        }

        @Override
        public String key(HttpServletRequest request, Route route, RateLimitUtils rateLimitUtils, String matcher) {
            return Optional.ofNullable(route).map(Route::getPath).orElse(StringUtils.EMPTY);
        }
    },

    /**
     * Rate limit policy considering the authenticated user's role.
     */
    ROLE {
        @Override
        public boolean apply(HttpServletRequest request, Route route, RateLimitUtils rateLimitUtils, String matcher) {
            return rateLimitUtils.getUserRoles().contains(matcher.toUpperCase());
        }

        @Override
        public String key(HttpServletRequest request, Route route, RateLimitUtils rateLimitUtils, String matcher) {
            return matcher;
        }

        @Override
        public boolean isValid(String matcher) {
            return StringUtils.isNotEmpty(matcher);
        }
    },

    /**
     * @deprecated See {@link #HTTP_METHOD}
     */
    @Deprecated
    HTTPMETHOD {
        @Override
        public boolean apply(HttpServletRequest request, Route route, RateLimitUtils rateLimitUtils, String matcher) {
            return HTTP_METHOD.apply(request, route, rateLimitUtils, matcher);
        }

        @Override
        public String key(HttpServletRequest request, Route route, RateLimitUtils rateLimitUtils, String matcher) {
            return HTTP_METHOD.key(request, route, rateLimitUtils, matcher);
        }
    },

    /**
     * Rate limit policy considering the HTTP request method.
     */
    HTTP_METHOD {
        @Override
        public boolean apply(HttpServletRequest request, Route route, RateLimitUtils rateLimitUtils, String matcher) {
            return request.getMethod().equalsIgnoreCase(matcher);
        }

        @Override
        public String key(HttpServletRequest request, Route route, RateLimitUtils rateLimitUtils, String matcher) {
            return StringUtils.isEmpty(matcher) ? request.getMethod() : "http-method";
        }
    },

    /**
     * Rate limit policy considering an URL Pattern
     */
    URL_PATTERN {
        @Override
        public boolean apply(HttpServletRequest request, Route route, RateLimitUtils rateLimitUtils, String matcher) {
            return new AntPathMatcher().match(matcher.toLowerCase(), request.getRequestURI().toLowerCase());
        }

        @Override
        public String key(HttpServletRequest request, Route route, RateLimitUtils rateLimitUtils, String matcher) {
            return matcher;
        }

        @Override
        public boolean isValid(String matcher) {
            return StringUtils.isNotEmpty(matcher);
        }
    };

    public abstract boolean apply(HttpServletRequest request, Route route,
                                  RateLimitUtils rateLimitUtils, String matcher);

    public abstract String key(HttpServletRequest request, Route route,
                               RateLimitUtils rateLimitUtils, String matcher);

    /**
     * Helper method to validate specific cases per type.
     *
     * @param matcher The type matcher
     * @return The default behavior will always return true.
     */
    public boolean isValid(String matcher) {
        return true;
    }
}
```



默认的`key`生成策略实现为:`DefaultRateLimitKeyGenerator` 。如果不能满足需求，可以自定义 `RateLimitKeyGenerator` 实现。



## 存储方式

`RateLimiter`的限流数据是默认以`ConcurrentHashMap`方式存储在内存中的，当我们部署了`Zuul`集群的时候，就会影响我们的限流策略了。我们可以将限流数据存储在`Redis`中，这样就可以集中记录各个`Zuul`节点的限流数据，来保证限流的准确性。



保存在一个时间窗口内针对（url，user，ip 以及自定义粒度）的调用次数，主要为内存、缓存以及数据库等，具体支持存储方式如下：

```java
public static enum Repository {
    REDIS,
    CONSUL,
    JPA,
    BUCKET4J_JCACHE,
    BUCKET4J_HAZELCAST,
    BUCKET4J_IGNITE,
    BUCKET4J_INFINISPAN,
    IN_MEMORY;

    private Repository() {
    }
}
```

## 限流策略

```yaml
default-policy:
  limit: 20             # 单位时间内允许访问的次数，20 次
  quota: 20             # 单位时间内允许访问的总时间（统计每次请求的时间综合）, 20s
  refresh-interval: 60  # 单位时间设置，60s
  type:                 # 限流粒度：url + user 组合形式
    - url
    - user
```

以上配置意思是：在一个时间窗口 60s 内，最多允许 20 次访问，或者总请求时间小于 20s。相关代码参考：

```java
public Object run() {
    RequestContext ctx = RequestContext.getCurrentContext();
    HttpServletResponse response = ctx.getResponse();
    HttpServletRequest request = ctx.getRequest();
    Route route = this.route(request);
    this.policy(route, request).forEach((policy) -> {
        String key = this.rateLimitKeyGenerator.key(request, route, policy);
        Rate rate = this.rateLimiter.consume(policy, key, (Long)null);
        String httpHeaderKey = key.replaceAll("[^A-Za-z0-9-.]", "_").replaceAll("__", "_");
        Long limit = policy.getLimit();
        Long remaining = rate.getRemaining();
        if (limit != null) {
            response.setHeader("X-RateLimit-Limit-" + httpHeaderKey, String.valueOf(limit));
            response.setHeader("X-RateLimit-Remaining-" + httpHeaderKey, String.valueOf(Math.max(remaining, 0L)));
        }

        Long quota = policy.getQuota();
        Long remainingQuota = rate.getRemainingQuota();
        if (quota != null) {
            request.setAttribute("rateLimitRequestStartTime", System.currentTimeMillis());
            response.setHeader("X-RateLimit-Quota-" + httpHeaderKey, String.valueOf(quota));
            response.setHeader("X-RateLimit-Remaining-Quota-" + httpHeaderKey, String.valueOf(TimeUnit.MILLISECONDS.toSeconds(Math.max(remainingQuota, 0L))));
        }

        response.setHeader("X-RateLimit-Reset-" + httpHeaderKey, String.valueOf(rate.getReset()));
        if (limit != null && remaining < 0L || quota != null && remainingQuota < 0L) {  // limit 和 quota 任意一个不满足就返回报错
            ctx.setResponseStatusCode(HttpStatus.TOO_MANY_REQUESTS.value());
            ctx.put("rateLimitExceeded", "true");
            ctx.setSendZuulResponse(false);
            throw new RateLimitExceededException();
        }
    });
    return null;
}
```

## 配置文件

```yaml
eureka:
  client:
    service-url:
      defaultZone: http://localhost:8761/eureka/

server:
  port: 10001

spring:
  application:
    name: cloud-zuul
  redis:
    host: localhost
    port: 6379

zuul:
  routes:
    a:
      path: /a/**
      serviceId: service-client
    b:
      path: /b/**
      serviceId: service-feign
  ratelimit:
    key-prefix: wsk
    enabled: true
    repository: REDIS
    behind-proxy: true
    default-policy-list: #optional - will apply unless specific policy exists
      - limit: 1 #optional - request number limit per refresh interval window
        quota: 1 #optional - request time limit per refresh interval window (in seconds)
        refresh-interval: 3 #default value (in seconds)
#        type: #optional
#          - user
#          - origin
#          - url
    policy-list:
      a: #需要和服务同名
        - limit: 10 #optional - request number limit per refresh interval window
          quota: 100 #optional - request time limit per refresh interval window (in seconds)
          refresh-interval: 30 #default value (in seconds)
      b:
        - limit: 1 #optional - request number limit per refresh interval window
          quota: 1 #optional - request time limit per refresh interval window (in seconds)
          refresh-interval: 3 #default value (in seconds)
#          type: #optional
#            - user
#            - origin
#            - url
#          type: #optional value for each type
#            - user=anonymous
#            - origin=somemachine.com
#            - url=/api #url prefix
#            - role=user
```

限流策略的确定：根据路由寻找限流策略，如果没找到则使用默认策略。

```java
public abstract class AbstractRateLimitFilter extends ZuulFilter {

    protected List<Policy> policy(Route route, HttpServletRequest request) {
        // 根据路由查找 route-id，如果有 采用该路由的限流策略，如果没有 采用默认策略
        String routeId = (String)Optional.ofNullable(route).map(Route::getId).orElse((Object)null);
        return (List)this.properties.getPolicies(routeId).stream().filter((policy) -> {
            return this.applyPolicy(request, route, policy);
        }).collect(Collectors.toList());
    }
}

public class RateLimitProperties implements Validator {
    public List<RateLimitProperties.Policy> getPolicies(String key) {
        return StringUtils.isEmpty(key) ? this.defaultPolicyList : (List)this.policyList.getOrDefault(key, this.defaultPolicyList);
    }
}
```



## 参数说明

Property namespace: zuul.ratelimit

| Property name       | Values                                                       | Default Value                                     | 说明                |
| :------------------ | :----------------------------------------------------------- | :------------------------------------------------ | :------------------ |
| enabled             | true/false                                                   | false                                             | 是否启用限流        |
| behind-proxy        | true/false                                                   | false                                             |                     |
| key-prefix          | String                                                       | ${spring.application.name:rate-limit-application} | 限流key前缀         |
| repository          | CONSUL, REDIS, JPA, BUCKET4J_JCACHE, BUCKET4J_HAZELCAST, BUCKET4J_INFINISPAN, BUCKET4J_IGNITE | -                                                 | 必填，使用redis即可 |
| default-policy-list | List of Policy                                               | -                                                 | 默认策略            |
| policy-list         | Map of Lists of Policy                                       | -                                                 | 自定义策略          |
| postFilterOrder     | int                                                          | FilterConstants.SEND_RESPONSE_FILTER_ORDER - 10   | postFilter过滤顺序  |
| preFilterOrder      | int                                                          | FilterConstants.FORM_BODY_WRAPPER_FILTER_ORDER    | preFilter过滤顺序   |

Policy properties:

| Property name    | Values                    | Default Value | 说明                                                         |
| :--------------- | :------------------------ | :------------ | :----------------------------------------------------------- |
| limit            | number of calls           | -             | 单位时间内请求次数限制                                       |
| quota            | time of calls             | -             | 单位时间内累计请求时间限制（秒），非必要参数                 |
| refresh-interval | seconds                   | 60            | 单位时间（秒），默认60秒                                     |
| type             | [ORIGIN, USER, URL, ROLE] | []            | 限流方式：ORIGIN, USER, URL（每个Url 在z秒内访问次数不得超过x次且总计访问时间这不得超过y秒） |



## 对性能的影响

压力测试，如果不设置限流的情况下，TPS 为 2850；如果设置了限流，TPS 骤降到 600。

通过 JProfiler 结合代码分析，发现 AbstractCacheRateLimiter 有一个同步方法 consume，导致了大量线程都处于阻塞状态。


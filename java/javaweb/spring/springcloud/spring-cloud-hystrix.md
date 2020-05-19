# hystrix

## 配置

http://www.gxitsky.com/2019/03/31/springcloud-07-Hystrix-circuit-breaker-properties/

# Hystrix Dashboard

1.新建一个服务模块，`service-a`，提供`controller`功能（注意，需要在服务的`service`层提供`@HystrixCommand`注解）

```java
@HystrixCommand(fallbackMethod = "myFallback")
```

2.创建一个模块，`cloud-hystrix-dashboard`

## 引入3个依赖

```xml
<dependency>
    <groupId>org.springframework.cloud</groupId>
    <artifactId>spring-cloud-starter-hystrix</artifactId>
</dependency>
<dependency>
    <groupId>org.springframework.cloud</groupId>
    <artifactId>spring-cloud-starter-netflix-hystrix-dashboard</artifactId>
</dependency>
<dependency>
    <groupId>org.springframework.boot</groupId>
    <artifactId>spring-boot-starter-actuator</artifactId>
</dependency>
```

## @EnableHystrixDashboard

在启动类上加上 `@EnableHystrixDashboard` 注解，开启Hystrix Dashboard功能。

```java
import org.springframework.boot.SpringApplication;
import org.springframework.boot.autoconfigure.SpringBootApplication;
import org.springframework.cloud.netflix.hystrix.dashboard.EnableHystrixDashboard;

@SpringBootApplication
@EnableHystrixDashboard
public class ServiceHystrixDashboardApplication {

    public static void main(String[] args) {
        SpringApplication.run(ServiceHystrixDashboardApplication.class, args);
    }
}
```

## 修改配置文件

修改application.properties文件:

```properties
spring.application.name=service-hystrix-dashboard
server.port=5566
```

## 测试

启动`cloud-hystrix-dashboard`，在浏览器访问： http://localhost:2019/hystrix

## Hystrix Dashboard访问规则

Hystrix Dashboard共支持三种不同的监控方式

默认的集群监控：通过URL:http://turbine-hostname:port/turbine.stream开启，实现对默认集群的监控。

指定的集群监控：通过URL:http://turbine-hostname:port/turbine.stream?cluster=[clusterName]开启，实现对clusterName集群的监控。

单体应用的监控：通过URL:http://hystrix-app:port/hystrix.stream开启，实现对具体某个服务实例的监控。

> Delay：控制服务器上轮询监控信息的延迟时间，默认为2000毫秒，可以通过配置该属性来降低客户端的网络和CPU消耗。
>
> Title:该参数可以展示合适的标题。

## 将服务纳入监控

以`service-a`为例，在其上面进行改造。

- 首先我们启动 `eureka-service`注册中心

- 然后操作`service-a`实例：

  - 添加以上3个依赖 启动类加上

  - 在启动类添加注解 `@EnableCircuitBreaker` 开启断路器功能

  - 在启动类注入`Servlet bean`， springboot 版本如果是2.0，则需要添加 `ServletRegistrationBean` 因为`springboot`的默认路径不是 `"/hystrix.stream"`，只要在自己的项目里配置上下面的servlet就可以了：

    ```java
    @SpringBootApplication
    @EnableEurekaClient
    @EnableHystrix
    @EnableCircuitBreaker // 断路器开启
    public class ServiceHystrixApplication {
    
        @Autowired
        HystrixService hystrixService;
    
        @Bean
        @LoadBalanced  // 基于Ribbon+RestTemplate实现负载均衡
        RestTemplate restTemplate(){
            return new RestTemplate();
        }
    
        public static void main(String[] args) {
            SpringApplication.run(ServiceHystrixApplication.class, args);
        }
    
        /**
         * Hystrix-dashboard；springboot2.0以后需要注入Servlet
         *
         * 这里我指定的urlMapping 是/test/hystrix.stream 那么我在monitor stream 的时候填写的地址就是:
         * http://ip:port/actuator/hystrix.stream
         * @return
         */
        @Bean
        public ServletRegistrationBean getServlet(){
            HystrixMetricsStreamServlet streamServlet = new HystrixMetricsStreamServlet();
            ServletRegistrationBean registrationBean = new ServletRegistrationBean(streamServlet);
            registrationBean.setLoadOnStartup(1);
    		//registrationBean.addUrlMappings("/actuator/hystrix.stream");  // 这里指定的urlMapping 就是我们在仪表盘中monitor Stream 的地址后缀
            registrationBean.addUrlMappings("/actuator/hystrix.stream");
            registrationBean.setName("HystrixMetricsStreamServlet");
            return registrationBean;
        }
    
    }
    ```

- 其他的配置和代码保持不变。

- 在`application.properties`中增加配置： `hystrix.command.default.execution.isolation.thread.timeoutInMilliseconds=2000` 。

- 启动 `service-a`服务。

- 启动 `cloud-hystrix-dashboard` 服务。

- 在浏览器输入http://localhost:2019/hystrix 。

- 在`Hystrix-Dashboard`的主界面上输入: http://localhost:8766/acturator/hystrix.stream ， 然后点击 `Monitor Stream`按钮

可以看到：

![](img/cloud-hystrix-dashboard2.png)



填入url，Delay，Title，点击 Monitor Stream



注意，此时访问http://host-application:8084/actuator/hystrix.stream可以发现，后台一直处于 ping 状态

```txt
ping: 

ping: 

ping: 

ping: 

ping: 

......
```



需要访问一下要监控的服务提供的url服务，如：http://host-application:8084/a/dataPage 获取一下数据，然后可以看到http://host-application:8084/actuator/hystrix.stream中出现：

```txt
ping: 

ping: 

data: {"type":"HystrixCommand","name":"getPriDataPage","group":"ASignalController","currentTime":1589708047085,"isCircuitBreakerOpen":false,"errorPercentage":0,"errorCount":0,"requestCount":0,"rollingCountBadRequests":0,"rollingCountCollapsedRequests":0,"rollingCountEmit":0,"rollingCountExceptionsThrown":0,"rollingCountFailure":0,"rollingCountFallbackEmit":0,"rollingCountFallbackFailure":0,"rollingCountFallbackMissing":0,"rollingCountFallbackRejection":0,"rollingCountFallbackSuccess":0,"rollingCountResponsesFromCache":0,"rollingCountSemaphoreRejected":0,"rollingCountShortCircuited":0,"rollingCountSuccess":0,"rollingCountThreadPoolRejected":0,"rollingCountTimeout":0,"currentConcurrentExecutionCount":0,"rollingMaxConcurrentExecutionCount":0,"latencyExecute_mean":0,"latencyExecute":{"0":0,"25":0,"50":0,"75":0,"90":0,"95":0,"99":0,"99.5":0,"100":0},"latencyTotal_mean":0,"latencyTotal":{"0":0,"25":0,"50":0,"75":0,"90":0,"95":0,"99":0,"99.5":0,"100":0},"propertyValue_circuitBreakerRequestVolumeThreshold":20,"propertyValue_circuitBreakerSleepWindowInMilliseconds":5000,"propertyValue_circuitBreakerErrorThresholdPercentage":50,"propertyValue_circuitBreakerForceOpen":false,"propertyValue_circuitBreakerForceClosed":false,"propertyValue_circuitBreakerEnabled":true,"propertyValue_executionIsolationStrategy":"THREAD","propertyValue_executionIsolationThreadTimeoutInMilliseconds":4000,"propertyValue_executionTimeoutInMilliseconds":4000,"propertyValue_executionIsolationThreadInterruptOnTimeout":true,"propertyValue_executionIsolationThreadPoolKeyOverride":null,"propertyValue_executionIsolationSemaphoreMaxConcurrentRequests":10,"propertyValue_fallbackIsolationSemaphoreMaxConcurrentRequests":10,"propertyValue_metricsRollingStatisticalWindowInMilliseconds":10000,"propertyValue_requestCacheEnabled":true,"propertyValue_requestLogEnabled":true,"reportingHosts":1,"threadPool":"ASignalController"}

......
```



这时再去http://localhost:8766/acturator/hystrix.stream填入url，点击Monitor Stream，可以看到监控界面

![](img/cloud-hystrix-dashboard1.png)



**注意**：如果在windows上进行上述实验，则会一直报错，无法打开监控页面

```log
Proxy opening connection to: http://localhost:8762/actuator/hystrix.stream?delay=2000
2019-09-26 11:31:23.536 ERROR 135936 --- [nio-8762-exec-2] ashboardConfiguration$ProxyStreamServlet : Error proxying request: http://localhost:8762/actuator/hystrix.stream?delay=2000
java.net.SocketTimeoutException: Read timed out
at java.net.SocketInputStream.socketRead0(Native Method) ~[na:1.8.0_181]
at java.net.SocketInputStream.socketRead(SocketInputStream.java:116) ~[na:1.8.0_181]
```



但是浏览器直接请求：http://localhost:8762/actuator/hystrix.stream正常返回结果

```txt
data: {"type":"HystrixCommand","name":"getPriDataPage","group":"ASignalController","currentTime":1589708047085,"isCircuitBreakerOpen":false,"errorPercentage":0,"errorCount":0,"requestCount":0,"rollingCountBadRequests":0,"rollingCountCollapsedRequests":0,"rollingCountEmit":0,"rollingCountExceptionsThrown":0,"rollingCountFailure":0,"rollingCountFallbackEmit":0,"rollingCountFallbackFailure":0,"rollingCountFallbackMissing":0,"rollingCountFallbackRejection":0,"rollingCountFallbackSuccess":0,"rollingCountResponsesFromCache":0,"rollingCountSemaphoreRejected":0,"rollingCountShortCircuited":0,"rollingCountSuccess":0,"rollingCountThreadPoolRejected":0,"rollingCountTimeout":0,"currentConcurrentExecutionCount":0,"rollingMaxConcurrentExecutionCount":0,"latencyExecute_mean":0,"latencyExecute":{"0":0,"25":0,"50":0,"75":0,"90":0,"95":0,"99":0,"99.5":0,"100":0},"latencyTotal_mean":0,"latencyTotal":{"0":0,"25":0,"50":0,"75":0,"90":0,"95":0,"99":0,"99.5":0,"100":0},"propertyValue_circuitBreakerRequestVolumeThreshold":20,"propertyValue_circuitBreakerSleepWindowInMilliseconds":5000,"propertyValue_circuitBreakerErrorThresholdPercentage":50,"propertyValue_circuitBreakerForceOpen":false,"propertyValue_circuitBreakerForceClosed":false,"propertyValue_circuitBreakerEnabled":true,"propertyValue_executionIsolationStrategy":"THREAD","propertyValue_executionIsolationThreadTimeoutInMilliseconds":4000,"propertyValue_executionTimeoutInMilliseconds":4000,"propertyValue_executionIsolationThreadInterruptOnTimeout":true,"propertyValue_executionIsolationThreadPoolKeyOverride":null,"propertyValue_executionIsolationSemaphoreMaxConcurrentRequests":10,"propertyValue_fallbackIsolationSemaphoreMaxConcurrentRequests":10,"propertyValue_metricsRollingStatisticalWindowInMilliseconds":10000,"propertyValue_requestCacheEnabled":true,"propertyValue_requestLogEnabled":true,"reportingHosts":1,"threadPool":"ASignalController"}
```



<font color=#dd0000>Windows系统的原因，放在linux上面跑，放在linux上面跑，放在linux上面跑</font>



## 图标含义

Hystrix Dashboard Wiki上详细说明了图上每个指标的含义

![](img/cloud-hystrix-dashboard4.png)





![](img/cloud-hystrix-dashboard5.png)

![](img/cloud-hystrix-dashboard6.png)



# Turbine

通过Hystrix Dashboard我们可以在直观地看到各Hystrix Command的请求响应时间, 请求成功率等数据。但是只使用Hystrix Dashboard的话, 你只能看到单个应用内的服务信息, 这明显不够. 我们需要一个工具能让我们汇总系统内多个服务的数据并显示到Hystrix Dashboard上, 这个工具就是Turbine



<font color=#dd0000>`Turbine`可以将所有单独的`hystrix.stream`聚合成一个`turbine.stream`，以便在`Hystrix Dashboard`上查看，它使用`DiscoveryClient`接口找出生产`/hystrix.stream`的相关服务。</font>



## 添加依赖

新建模块 `cloud-turbine-dashboard`用于替换上边的`cloud-hystrix-dashboard`

```xml
<dependencies>
	<dependency>
		<groupId>org.springframework.cloud</groupId>
		<artifactId>spring-cloud-starter-turbine</artifactId>
	</dependency>
	<dependency>
		<groupId>org.springframework.cloud</groupId>
		<artifactId>spring-cloud-netflix-turbine</artifactId>
	</dependency>
	<dependency>
		<groupId>org.springframework.boot</groupId>
		<artifactId>spring-boot-starter-actuator</artifactId>
	</dependency>
	<dependency>
		<groupId>org.springframework.cloud</groupId>
		<artifactId>spring-cloud-starter-hystrix-dashboard</artifactId>
	</dependency>
</dependencies>
```



## 配置文件

```properties
server.port=5566
spring.application.name=cloud-turbine-dashboard
eureka.client.serviceUrl.defaultZone=http://localhost:8761/eureka/
turbine.appConfig=eureka-a,service-abis
turbine.clusterNameExpression=new String("default")
```



```java
import org.springframework.boot.SpringApplication;
import org.springframework.boot.autoconfigure.SpringBootApplication;
import org.springframework.cloud.client.discovery.EnableDiscoveryClient;
import org.springframework.cloud.netflix.hystrix.dashboard.EnableHystrixDashboard;
import org.springframework.cloud.netflix.turbine.EnableTurbine;

@EnableHystrixDashboard
@EnableTurbine
@EnableDiscoveryClient
@SpringBootApplication
public class CloudTurbineDashboardApplication {

    public static void main(String[] args) {
        SpringApplication.run(CloudTurbineDashboardApplication.class, args);
    }
}
```



## 访问

参见 [Hystrix Dashboard访问规则]

1.访问http://host-application:8084/actuator/hystrix.stream，http://host-application:8084/actuator/hystrix.stream，看是否在ping

2.如果在ping，访问一下服务提供的url，http://host-application:8084/a/dataPage，http://host-application:8084/a/dataPage，触发

3.http://host-application:5566/hystrix填入url：http://host-application:5566/turbine.stream，点击Monitor Stream，可以看到监控界面。这时随意触发http://host-application:8084/a/dataPage，http://host-application:8084/a/dataPage其中任意一个，都可以从界面看到界面变化



**注意**

<font color=#dd0000>Windows系统的原因，放在linux上面跑，放在linux上面跑，放在linux上面跑</font>
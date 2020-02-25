# Ribbon

Spring Cloud Ribbon是基于Netflix Ribbon实现的一套客户端负载均衡的工具。它是一个基于HTTP和TCP的客户端负载均衡器。它可以通过在客户端中配置ribbonServerList来设置服务端列表去轮询访问以达到均衡负载的作用。

当Ribbon与Eureka联合使用时，ribbonServerList会被DiscoveryEnabledNIWSServerList重写，扩展成从Eureka注册中心中获取服务实例列表。同时它也会用NIWSDiscoveryPing来取代IPing，它将职责委托给Eureka来确定服务端是否已经启动。

而当Ribbon与Consul（类似于Eureka的另一种服务注册中心）联合使用时，ribbonServerList会被ConsulServerList来扩展成从Consul获取服务实例列表。同时由ConsulPing来作为IPing接口的实现。

我们在使用Spring Cloud Ribbon的时候，不论是与Eureka还是Consul结合，都会在引入Spring Cloud Eureka或Spring Cloud Consul依赖的时候通过自动化配置来加载上述所说的配置内容，所以我们可以快速在Spring Cloud中实现服务间调用的负载均衡。



## 示例

```shell
# 1.启动一个eureka server 服务端
# 2.启动同一spirng boot 应用（多个实例），提供url访问，并注册到eureka
# 3.启动ribbon客户端，对spring boot应用进行负载均衡
```



```xml
<properties>
    <java.version>1.8</java.version>
    <spring-cloud.version>Greenwich.SR2</spring-cloud.version>
</properties>

<dependencies>
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
        <artifactId>spring-cloud-starter-netflix-ribbon</artifactId>
    </dependency>
</dependencies>

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
```



```yaml
server:
  port: 8888

spring:
  application:
    name: cloud-ribbon

eureka:
  instance:
    leaseRenewalIntervalInSeconds: 10 #表示eureka client发送心跳给server端的频率
    health-check-url-path: /actuator/health #健康检查页面的URL，相对路径，默认使用 HTTP 访问，如果需要使用 HTTPS则需要使用绝对路径配置

  client:
    registryFetchIntervalSeconds: 5 #表示eureka client间隔多久去拉取服务注册信息，默认为30秒
    service-url:
      defaultZone: ${EUREKA_SERVICE_URL:http://localhost:8761}/eureka/ #服务注册中心地址
```



启动类

```java
@SpringBootApplication
public class RibbonApplication {

    @LoadBalanced
    @Bean
    public RestTemplate restTemplate() {
        return new RestTemplate();
    }

    public static void main(String[] args) {
        SpringApplication.run(RibbonApplication.class, args);
    }

}
```



调用

```java
@RestController
@CrossOrigin(allowCredentials = "true")
public class PriSignalController {

    @Autowired
    private RestTemplate restTemplate;

    @Autowired
    private DiscoveryClient discoveryClient;

    @GetMapping("/service-instances/{applicationName}")
    public List<ServiceInstance> serviceInstancesByApplicationName(
            @PathVariable String applicationName) {
        return this.discoveryClient.getInstances(applicationName);
    }

    // 注意：url必须是eureka中注册的服务的名称,如果要调用真实的域名或者ip的url,会有错误
    @GetMapping("list")
    public String test(){
        List results = restTemplate.getForObject("http://EUREKA-PRI/pri/dataPage?pageNum=1&pageSize=10", List.class);
        return results.toString();
    }

}
```



## ribbon负载配置

```java
ribbon:
  #服务请求连接超时时间（毫秒）
  ConnectTimeout: 1000
  #服务请求处理超时时间（毫秒）
  ReadTimeout: 3000
  #对当前服务的重试次数
  MaxAutoRetries: 2
  #切换相同Server的次数
  MaxAutoRetriesNextServer: 0
```



## ribbon超时时间

```java
protected static int getRibbonTimeout(IClientConfig config, String commandKey) {
	int ribbonTimeout;
	// 这是比较异常的情况，不说
	if (config == null) {
		ribbonTimeout = RibbonClientConfiguration.DEFAULT_READ_TIMEOUT + RibbonClientConfiguration.DEFAULT_CONNECT_TIMEOUT;
	} else {
	   // 这里获取了四个参数，ReadTimeout，ConnectTimeout，MaxAutoRetries， MaxAutoRetriesNextServer
		int ribbonReadTimeout = getTimeout(config, commandKey, "ReadTimeout",
			IClientConfigKey.Keys.ReadTimeout, RibbonClientConfiguration.DEFAULT_READ_TIMEOUT);
		int ribbonConnectTimeout = getTimeout(config, commandKey, "ConnectTimeout",
			IClientConfigKey.Keys.ConnectTimeout, RibbonClientConfiguration.DEFAULT_CONNECT_TIMEOUT);
		int maxAutoRetries = getTimeout(config, commandKey, "MaxAutoRetries",
			IClientConfigKey.Keys.MaxAutoRetries, DefaultClientConfigImpl.DEFAULT_MAX_AUTO_RETRIES);
		int maxAutoRetriesNextServer = getTimeout(config, commandKey, "MaxAutoRetriesNextServer",
			IClientConfigKey.Keys.MaxAutoRetriesNextServer, DefaultClientConfigImpl.DEFAULT_MAX_AUTO_RETRIES_NEXT_SERVER);
		// 原来ribbonTimeout的计算方法在这里，以上文的设置为例
		// ribbonTimeout = (50000 + 50000) * (0 + 1) * (1 + 1) = 200000
		ribbonTimeout = (ribbonReadTimeout + ribbonConnectTimeout) * (maxAutoRetries + 1) * (maxAutoRetriesNextServer + 1);
	}
	return ribbonTimeout;
}
```



可以看到ribbonTimeout是一个总时间，所以从逻辑上来讲，作者希望<font color=##00dd00> hystrixTimeout要大于ribbonTimeout</font>，否则hystrix熔断了以后，ribbon的重试就都没有意义了。
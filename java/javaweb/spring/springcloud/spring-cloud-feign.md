# Feign

https://www.cnblogs.com/jmcui/p/11218824.html

了解了`Ribbon`和`Hystrix`以后，可以发现：他俩作为基础工具类框架**广泛地应用**在各个微服务的实现中。我们会发现对这两个框架的**使用几乎是同时出现**的。

为了**简化**我们的开发，`Spring Cloud Feign`出现了！它基于 `Netflix Feign` 实现，**整合**了 `Spring Cloud Ribbon` 与 `Spring Cloud Hystrix`, 除了整合这两者的强大功能之外，它还提 供了**声明式的服务调用**(不再通`RestTemplate`)。

> Feign是一种声明式、模板化的HTTP客户端。在Spring Cloud中使用Feign, 我们可以做到使用HTTP请求远程服务时能与调用本地方法一样的编码体验，开发者完全感知不到这是远程方法，更感知不到这是个HTTP请求。



## Feign 简介

- `Feign` 是一个声明式的伪 `http` 客户端。使用 `Feign` 时，仅仅需要创建一个接口并注解。
- `Feign` 支持可插拔的编码器、解码器。
- `Feign` 默认集成了 `Ribbon`，并和 `Eureka` 结合，实现了默认的负载均衡功能。
- 整合了 `Hystrix`，具有熔断能力。



## Feign和OpenFeign两者区别

```properties
Feign：
Feign内置了Ribbon，用来做客户端负载均衡，去调用服务注册中心的服务。
feign的使用方式是：使用Feign注解定义一个接口，调用这个接口，就可以调用服务注册中心的服务。

OpenFeign
OpenFeign是springcloud在feign的基础上支持了springmvc注解，如@RequestMapping等等。openFeign的@FeignClient注解可以解析springmvc的@RequestMapping注解下的接口，并通过动态代理的方式产生实现类，实现类中做负载均衡并调用其他服务
```



## 示例



### 准备

继续启用 `eureka-srv` 端口为 8761、`eureka-srv-cli, eureka-srv-cli1` 两个实例 端口为 8762、8763.

### 创建一个 Feign 服务

工程为 service-feign，引入依赖：

```xml
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
    <artifactId>spring-cloud-starter-openfeign</artifactId>
</dependency>
```

### 修改配置信息

配置文件中指定服务名伪： service-feign， 端口伪 8765， 注册到注册中心： http://localhost:8761/eureka/。 `application.properties`:

```properties
spring.application.name=service-feign
# 本实例端口
server.port=8765

# eureka注册中心的配置信息
registry.port=8761
eureka.instance.hostname=localhost
eureka.client.serviceUrl.defaultZone=http://${eureka.instance.hostname}:${registry.port}/eureka/
```

### 注解 `@EnableFeignClients`

```java
@EnableFeignClients  // 开启Feign的功能
@EnableDiscoveryClient
@SpringBootApplication
public class ServiceFeignApplication {

    public static void main(String[] args) {
        SpringApplication.run(ServiceFeignApplication.class, args);
    }

}
```

### 定义一个Feign接口

定义一个接口使用，使用 `@ FeignClient("服务名"")`，来指定调用哪个服务。 比如，我们调用 eureka-srv-cli1 应用的 /hi 接口，代码示例如下：

```java
@FeignClient("eureka-srv-cli1")  // 指定服务应用名
@Service
public interface IFeignServiceHi {

    @RequestMapping(value = "/hi" ,method = RequestMethod.GET)
    String sayHiFromClient(@RequestParam(value = "name") String name);
}
```

### 向外层暴露一个Controller，调用Feign接口定义的服务来消费

```java
@RestController
public class HiController {

    @Autowired
    IFeignServiceHi iFeignServiceHi;

    @GetMapping("/hi")
    public String sayHi(@RequestParam String name){
        return iFeignServiceHi.sayHiFromClient(name);
    }
}
```

### 测试

当然，测试之前，请先关闭Ribbon负载均衡实例，以免干扰； 再启动本例的Feign启动类，然后访问： `http://localhost:8765/hi?name=forezp`，也看到了和Ribbon负载均衡一样的效果：

> hi forezp,i am from port:8763
>
> hi forezp,i am from port:8762
>
> hi forezp,i am from port:8763
>
> hi forezp,i am from port:8762

### 总结

- 引入依赖: `spring-cloud-starter-openfeign`
- 启用Feign注解: 在启动类中使用 `@EnableFeignClients` 注解开启Feign的功能
- 编写Feign接口： 编写接口，并使用 `@FeignClient("eureka-srv-cli1")` 用于指定需要进行负载均衡的服务应用名



## Feign的Fallback机制

如同hystrix和ribbon中针对于方法的fallback机制，feign中针对一个类，使用fallback机制



### 方式一 fallback形式

- 这种方式无法捕获降级原因

```java
@FeignClient(name = "hello", fallback = HystrixClientFallback.class)
```

代码示例

```java
@FeignClient(name = "microservice-provider-user", fallback = HystrixClientFallback.class)
public interface UserFeignClient {
	@RequestMapping(method = RequestMethod.GET, value = "/user/{id}", consumes = "application/json")
    public User findById(@PathVariable("id") Long id);
}


static class HystrixClientFallback implements UserFeignClient {
	@Override
	public User findById(Long id) {
		User user = new User();
		user.setId(1L);
		return user;
    }
}
```

### 方式二 fallbackFactory形式

- 这种方式可以捕获降级的具体原因，即下述的`Throwable cause`

```java
@FeignClient(name = "microservice-provider-user", fallbackFactory = HystrixClientFallbackFactory.class)
```

代码示例

```java
@FeignClient(name = "microservice-provider-user", fallbackFactory = HystrixClientFallbackFactory.class)
public interface UserFeignClient {
	@RequestMapping(method = RequestMethod.GET, value = "/user/{id}", consumes = "application/json")
    public User findById(@PathVariable("id") Long id);
}    
    

@Component
static class HystrixClientFallbackFactory implements FallbackFactory<UserFeignClient> {
	
	private static Logger LOGGER = LoggerFactory.getLogger(HystrixClientFallbackFactory.class);
	
	@Override
	public UserFeignClient create(Throwable cause) {
		//打印日志
		HystrixClientFallbackFactory.LOGGER.info("fallback; reason was: " + cause.getMessage());
		
		return new UserFeignClient() {
			@Override
			public User findById(Long id) {
				User user = new User();
				user.setId(-1L);
				user.setUsername("我是HystrixClientFallbackFactory");
				
				return user;
			}
		};
	}	
}
```


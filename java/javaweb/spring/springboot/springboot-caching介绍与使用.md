# 1.springboot缓存介绍

[原文链接](https://www.cnblogs.com/yueshutong/p/9381540.html)

## 1.1spring缓存抽象

- Spring从3.1开始定义了org.springframework.cache.Cache和org.springframework.cache.CacheManager接口来统一不同的缓存技术；并支持使用JCache（JSR-107）注解简化我们开发；
- Cache接口下Spring提供了各种xxxCache的实现；如RedisCache，EhCacheCache ,ConcurrentMapCache等；
- 使用Spring缓存抽象时我们需要关注以下两点
  - 确定方法需要被缓存以及他们的缓存策略
  - 从缓存中读取之前缓存存储的数据

## 1.2概念&缓存注解

| 名称           | 解释                                                         |
| -------------- | ------------------------------------------------------------ |
| Cache          | 缓存接口，定义缓存操作。实现有：RedisCache、EhCacheCache、ConcurrentMapCache等 |
| CacheManager   | 缓存管理器，管理各种缓存（cache）组件                        |
| @Cacheable     | 主要针对方法配置，能够根据方法的请求参数对其进行缓存         |
| @CacheEvict    | 清空缓存                                                     |
| @CachePut      | 保证方法被调用，又希望结果被缓存。 与@Cacheable区别在于是否每次都调用方法，常用于更新 |
| @EnableCaching | 开启基于注解的缓存                                           |
| keyGenerator   | 缓存数据时key生成策略                                        |
| serialize      | 缓存数据时value序列化策略                                    |
| @CacheConfig   | 统一配置本类的缓存注解的属性                                 |



**@Cacheable/@CachePut/@CacheEvict 主要的参数：**

| 名称                           | 解释                                                         |
| ------------------------------ | ------------------------------------------------------------ |
| value                          | 缓存的名称，在 spring 配置文件中定义，必须指定至少一个 例如： @Cacheable(value=”mycache”) 或者 @Cacheable(value={”cache1”,”cache2”} |
| key                            | 缓存的 key，可以为空，如果指定要按照 SpEL 表达式编写， 如果不指定，则缺省按照方法的所有参数进行组合 例如： @Cacheable(value=”testcache”,key=”#id”) |
| condition                      | 缓存的条件，可以为空，使用 SpEL 编写，返回 true 或者 false， 只有为 true 才进行缓存/清除缓存 例如：@Cacheable(value=”testcache”,condition=”#userName.length()>2”) |
| unless                         | 否定缓存。当条件结果为true时，就不会缓存。 @Cacheable(value=”testcache”,unless=”#userName.length()>2”) |
| allEntries (@CacheEvict )      | 是否清空所有缓存内容，缺省为 false，如果指定为 true， 则方法调用后将立即清空所有缓存 例如： @CachEvict(value=”testcache”,allEntries=true) |
| beforeInvocation (@CacheEvict) | 是否在方法执行前就清空，缺省为 false，如果指定为 true， 则在方法还没有执行的时候就清空缓存，缺省情况下，如果方法 执行抛出异常，则不会清空缓存 例如： @CachEvict(value=”testcache”，beforeInvocation=true) |

## 1.3SpEL上下文数据

- Spring Cache提供了一些供我们使用的**SpEL上下文数据**，下表直接摘自Spring官方文档：

| 名称          | 位置       | 描述                                                         | 示例                   |
| ------------- | ---------- | ------------------------------------------------------------ | ---------------------- |
| methodName    | root对象   | 当前被调用的方法名                                           | `#root.methodname`     |
| method        | root对象   | 当前被调用的方法                                             | `#root.method.name`    |
| target        | root对象   | 当前被调用的目标对象实例                                     | `#root.target`         |
| targetClass   | root对象   | 当前被调用的目标对象的类                                     | `#root.targetClass`    |
| args          | root对象   | 当前被调用的方法的参数列表                                   | `#root.args[0]`        |
| caches        | root对象   | 当前方法调用使用的缓存列表                                   | `#root.caches[0].name` |
| Argument Name | 执行上下文 | 当前被调用的方法的参数，如findArtisan(Artisan artisan),可以通过#artsian.id获得参数 | `#artsian.id`          |
| result        | 执行上下文 | 方法执行后的返回值（仅当方法执行后的判断有效，如 unless cacheEvict的beforeInvocation=false） | `#result`              |

- **注意**

1.当我们要使用root对象的属性作为key时我们也可以将“#root”省略，因为Spring默认使用的就是root对象的属性。 如 

```
@Cacheable(key = "targetClass + methodName +#p0")
```

2.使用方法参数时我们可以直接使用“#参数名”或者“#p参数index”。 如：

```
@Cacheable(value="users", key="#id")
```

```
@Cacheable(value="users", key="#p0")
```

- **SpEL提供了多种运算符**

| **类型**   | **运算符**                                     |
| ---------- | ---------------------------------------------- |
| 关系       | <，>，<=，>=，==，!=，lt，gt，le，ge，eq，ne   |
| 算术       | +，- ，* ，/，%，^                             |
| 逻辑       | &&，\|\|，!，and，or，not，between，instanceof |
| 条件       | ?: (ternary)，?: (elvis)                       |
| 正则表达式 | matches                                        |
| 其他类型   | ?.，?[…]，![…]，^[…]，$[…]                     |

# 2.springboot缓存的使用

## 2.1导入依赖

```xml
<dependency>
	<groupId>org.springframework.boot</groupId>
	<artifactId>spring-boot-starter-cache</artifactId>
</dependency>
```

如果使用redis缓存，则只导入springboot整合redis的依赖即可：

```xml
<dependency>
	<groupId>org.springframework.boot</groupId>
	<artifactId>spring-boot-starter-data-redis</artifactId>
</dependency>
```

## 2.2启动类上开启缓存

```java
@SpringBootApplication
@EnableCaching  //开启缓存
public class DemoApplication{

    public static void main(String[] args) {
        SpringApplication.run(DemoApplication.class, args);
    }

}
```

## 2.3缓存@Cacheable

- `@Cacheable`注解会先查询是否已经有缓存，有会使用缓存，没有则会执行方法并缓存。

```java
	@Cacheable(key = "targetClass + methodName", value = "cache-test-namespace")
    @Override
    public List<FcodInfoEntity> getData() {
        String sql = String.format("select `Fcod`, `MajorCategory`, `MiddleCategory`, `CauseCategory`, `DeviceType` from %s;",
                env.getProperty("lj.c3-collect-analysis.fcod-table-name"));
        logger.info("excute sql : " + sql);
        return jdbcTemplate.query(sql, rowMapper);
    }
```

- 此处的`value`是必需的，它指定了你的缓存存放在哪块**命名空间**。

- 此处的`key`是使用的spEL表达式，参考上章。这里有一个小坑，如果你把`methodName`换成`method`运行会报错，观察它们的返回类型，原因在于`methodName`是`String`而`methoh`是`Method`。
- 此处的`FcodInfoEntity`实体类一定要**实现序列化**`public class User implements Serializable`，否则会报`java.io.NotSerializableException`异常。

**深入源码，查看它的其它属性：**

```java
String[] cacheNames() default {}; //和value注解差不多，二选一

String keyGenerator() default ""; //key的生成器。key/keyGenerator二选一使用。。若需要指定一个自定义的key生成器，我们需要去实现org.springframework.cache.interceptor.KeyGenerator接口，并使用该参数来指定。

String cacheManager() default ""; //指定缓存管理器，默认使用SimpleCacheManager

String cacheResolver() default ""; //指定缓存解析器。需通过org.springframework.cache.interceptor.CacheResolver接口来实现自己的缓存解析器，并用该参数指定。

String condition() default ""; //条件符合则缓存

String unless() default ""; //条件符合则不缓存

boolean sync() default false; //是否使用异步模式
```

## 2.4配置@CacheConfig

- 当我们需要缓存的地方越来越多，你可以使用`@CacheConfig(cacheNames = {"myCache"})`注解来**统一指定`value`的值**，这时可省略`value`，如果你在你的方法依旧写上了`value`，那么依然以方法的`value`值为准。

- 使用方法如下：

```java
@CacheConfig(cacheNames = {"myCache"})
public class BotRelationServiceImpl implements BotRelationService {
    @Override
    @Cacheable(key = "targetClass + methodName +#p0")//此处没写value
    public List<BotRelation> findAllLimit(int num) {
        return botRelationRepository.findAllLimit(num);
    }
    .....
}
```

**其它属性**

```java
String keyGenerator() default "";  //key的生成器。key/keyGenerator二选一使用

String cacheManager() default "";  //指定缓存管理器

String cacheResolver() default ""; //或者指定获取解析器
```

## 2.5更新@CachePut

- `@CachePut`注解的作用 主要针对方法配置，能够根据方法的请求参数对其结果进行缓存，和 `@Cacheable` 不同的是，它每次都会触发真实方法的调用 。简单来说就是用户更新缓存数据。但需要**注意**的是该注解的`value` 和 `key` 必须与要更新的缓存相同，也就是与`@Cacheable` 相同。

- **示例**：

```java
@CachePut(value = "emp", key = "targetClass + #p0")
public NewJob updata(NewJob job) {
    NewJob newJob = newJobDao.findAllById(job.getId());
    newJob.updata(job);
    return job;
}

@Cacheable(value = "emp", key = "targetClass +#p0")//清空缓存
public NewJob save(NewJob job) {
    newJobDao.save(job);
    return job;
}
```

**其它属性**

```java
String[] cacheNames() default {}; //与value二选一

String keyGenerator() default "";  //key的生成器。key/keyGenerator二选一使用

String cacheManager() default "";  //指定缓存管理器

String cacheResolver() default ""; //或者指定获取解析器

String condition() default ""; //条件符合则缓存
```

## 2.6清除@CacheEvict

- `@CachEvict` 的作用 主要针对方法配置，能够根据一定的条件对缓存进行清空 。

| 属性             | 解释                                                         | 示例                                                 |
| ---------------- | ------------------------------------------------------------ | ---------------------------------------------------- |
| allEntries       | 是否清空所有缓存内容，缺省为 false，如果指定为 true，则方法调用后将立即清空所有缓存 | @CachEvict(value=”testcache”,allEntries=true)        |
| beforeInvocation | 是否在方法执行前就清空，缺省为 false，如果指定为 true，则在方法还没有执行的时候就清空缓存，缺省情况下，如果方法执行抛出异常，则不会清空缓存 | @CachEvict(value=”testcache”，beforeInvocation=true) |

- 示例：

```java
@Cacheable(value = "emp",key = "#p0.id")
public NewJob save(NewJob job) {
    newJobDao.save(job);
    return job;
}

//清除一条缓存，key为要清空的数据
@CacheEvict(value="emp",key="#id")
public void delect(int id) {
    newJobDao.deleteAllById(id);
}

//方法调用后清空所有缓存
@CacheEvict(value="accountCache",allEntries=true)
public void delectAll() {
    newJobDao.deleteAll();
}

//方法调用前清空所有缓存
@CacheEvict(value="accountCache",beforeInvocation=true)
public void delectAll() {
    newJobDao.deleteAll();
}
```

**其他属性**

```
String[] cacheNames() default {}; //与value二选一

String keyGenerator() default "";  //key的生成器。key/keyGenerator二选一使用

String cacheManager() default "";  //指定缓存管理器

String cacheResolver() default ""; //或者指定获取解析器

String condition() default ""; //条件符合则清空
```

## 2.7组合@Caching

- 有时候我们可能组合多个Cache注解使用：比如用户新增成功后，我们要添加id-->user；username--->user；email--->user的缓存；此时就需要@Caching组合多个注解标签了。 

- **示例**：

```java
    @Caching(
            cacheable = {
                    @Cacheable(value = "user", key = "#user.id"),
                    @Cacheable(value = "user", key = "#user.name"),
                    @Cacheable(value = "user", key = "#user.email")
                    //...
            },
            put = {
                    @CachePut(value = "user", key = "#user.id"),
                    @CachePut(value = "user", key = "#user.name"),
                    @CachePut(value = "user", key = "#user.email")
                    //...
            },
            evict = {
                    @CacheEvict(value = "user", key = "#user.id"),
                    @CacheEvict(value = "user", key = "#user.name"),
                    @CacheEvict(value = "user", key = "#user.email")
                    //....
            })
    public User save(User user) {
        ....
    }
```

## 2.8自定义缓存注解

- 比如上述的那个@Caching组合，会让方法上的注解显得整个代码比较乱，此时可以使用自定义注解把这些注解组合到一个注解中
- 示例

```java
@Caching(  
        put = {  
                @CachePut(value = "user", key = "#user.id"),  
                @CachePut(value = "user", key = "#user.username"),  
                @CachePut(value = "user", key = "#user.email")  
        }  
)  
@Target({ElementType.METHOD, ElementType.TYPE})  
@Retention(RetentionPolicy.RUNTIME)  
@Inherited  
public @interface UserSaveCache {  
}
```

- 使用

```java
@UserSaveCache  
public User save(User user)
```

## 2.9示例

- 新增/修改数据时往缓存中写 

```java
@Caching(  
        put = {  
                @CachePut(value = "user", key = "#user.id"),  
                @CachePut(value = "user", key = "#user.username"),  
                @CachePut(value = "user", key = "#user.email")  
        }  
)  
public User save(User user)
```

```java
@Caching(  
        put = {  
                @CachePut(value = "user", key = "#user.id"),  
                @CachePut(value = "user", key = "#user.username"),  
                @CachePut(value = "user", key = "#user.email")  
        }  
)  
public User update(User user)
```

- 删除数据时从缓存中移除

```java
@Caching(  
        evict = {  
                @CacheEvict(value = "user", key = "#user.id"),  
                @CacheEvict(value = "user", key = "#user.username"),  
                @CacheEvict(value = "user", key = "#user.email")  
        }  
)  
public User delete(User user)
```

```java
@CacheEvict(value = "user", allEntries = true)  
 public void deleteAll()   
```

- 查找时从缓存中读

```java
@Caching(  
        cacheable = {  
                @Cacheable(value = "user", key = "#id")  
        }  
)  
public User findById(final Long id)    
```

```java
@Caching(  
         cacheable = {  
                 @Cacheable(value = "user", key = "#username")  
         }  
 )  
 public User findByUsername(final String username) 
```

```java
@Caching(  
          cacheable = {  
                  @Cacheable(value = "user", key = "#email")  
          }  
  )  
  public User findByEmail(final String email)
```

# 3.缓存策略

如果缓存满了，从缓存中移除数据的策略，常见的有FIFO， LRU 、LFU

- FIFO (First in First Out) 先进先出策略，即先放入缓存的数据先被移除
- LRU (Least Recently Used) 最久未使用策略， 即使用时间距离现在最久的那个数据被移除
- LFU (Least Frequently Used)  最少使用策略，即一定时间内使用次数（频率）最少的那个数据被移除
- TTL（Time To Live）存活期，即从缓存中创建时间点开始至到期的一个时间段（不管在这个时间段内有没被访问过都将过期）
- TTI （Time To Idle）空闲期，即一个数据多久没有被访问就从缓存中移除的时间。

# 4.缓存管理器（CacheManager）

1. 通过@EnableCaching注解自动化配置合适的缓存管理器（CacheManager），Spring Boot根据下面的顺序去侦测缓存提供者：

- Generic
- JCache (JSR-107)
- EhCache 2.x
- Hazelcast
- Infinispan
- Redis
- Guava
- Simple

2. 可以通过配置属性spring.cache.type来强制指定，即

```yaml
spring.cache.type = xxx
```


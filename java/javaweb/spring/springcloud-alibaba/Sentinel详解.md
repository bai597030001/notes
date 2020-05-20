# 定义

随着微服务的流行，服务和服务之间的稳定性变得越来越重要。Sentinel 以流量为切入点，从流量控制、熔断降级、系统负载保护等多个维度保护服务的稳定性。

Sentinel 具有以下特征:

- **丰富的应用场景**：Sentinel 承接了阿里巴巴近 10 年的双十一大促流量的核心场景，例如秒杀（即突发流量控制在系统容量可以承受的范围）、消息削峰填谷、集群流量控制、实时熔断下游不可用应用等。
- **完备的实时监控**：Sentinel 同时提供实时的监控功能。您可以在控制台中看到接入应用的单台机器秒级数据，甚至 500 台以下规模的集群的汇总运行情况。
- **广泛的开源生态**：Sentinel 提供开箱即用的与其它开源框架/库的整合模块，例如与 Spring Cloud、Dubbo、gRPC 的整合。您只需要引入相应的依赖并进行简单的配置即可快速地接入 Sentinel。
- **完善的 SPI 扩展点**：Sentinel 提供简单易用、完善的 SPI 扩展接口。您可以通过实现扩展接口来快速地定制逻辑。例如定制规则管理、适配动态数据源等。



# 组成

Sentinel 分为两个部分:

- 核心库（Java 客户端）不依赖任何框架/库，能够运行于所有 Java 运行时环境，同时对 Dubbo / Spring Cloud 等框架也有较好的支持。
- 控制台（Dashboard）基于 Spring Boot 开发，打包后可以直接运行，不需要额外的 Tomcat 等应用容器。



# 使用

## 1.引入依赖

```xml
<dependency>
    <groupId>com.alibaba.csp</groupId>
    <artifactId>sentinel-core</artifactId>
    <version>1.7.2</version>
</dependency>
```

## 2.定义资源

```java
/*
把需要控制流量的代码用 Sentinel API SphU.entry("HelloWorld") 和 entry.exit() 包围起来即可。
*/
public static void main(String[] args) {
    initFlowRules();
    while (true) {
        Entry entry = null;
        try {
            /* 埋点 */
	    entry = SphU.entry("HelloWorld");
            /*业务逻辑 - 开始*/
            System.out.println("hello world");
            /*业务逻辑 - 结束*/
	} catch (BlockException e1) {
            /*流控逻辑处理 - 开始*/
	    System.out.println("block!");
            /*流控逻辑处理 - 结束*/
	} finally {
	   if (entry != null) {
	       entry.exit();
	   }
	}
    }
}
```

这种方式对代码的侵入性较高，可使用基于注解的方式



### 注解支持

Sentinel 提供了 `@SentinelResource` 注解用于定义资源，并提供了 `AspectJ` 的扩展用于自动定义资源、处理 `BlockException` 等。

```xml
<dependency>
    <groupId>com.alibaba.csp</groupId>
    <artifactId>sentinel-annotation-aspectj</artifactId>
    <version>x.y.z</version>
</dependency>
```

```java
public class ConfigController {

    @RequestMapping("/get")
    @SentinelResource(value = "get")
    public String get() {
        return "abc";
    }
}
```



- `value`：资源名称，必需项

- `entryType`：entry 类型（`EntryType.IN`，`EntryType.OUT`），可选项（默认为 `EntryType.OUT`）

- `blockHandler` / `blockHandlerClass`

  > `blockHandler` 对应处理 `BlockException` 的函数名称，可选项。
  >
  > `blockHandler` 函数访问范围需要是 `public`，返回类型需要与原方法相匹配，参数类型需要和原方法相匹配并且最后加一个额外的参数，类型为 `BlockException`。
  >
  > `blockHandler` 函数默认需要和原方法在同一个类中。若希望使用其他类的函数，则可以指定 `blockHandlerClass` 为对应的类的 `Class` 对象，注意对应的函数必需为 static 函数，否则无法解析。

- `fallback` / `fallbackClass`

  > `fallback` 函数名称，可选项，用于在抛出异常的时候提供 `fallback` 处理逻辑。
  >
  > `fallback` 函数可以针对所有类型的异常（除了 `exceptionsToIgnore`里面排除掉的异常类型）进行处理。
  >
  > `fallback` 函数签名和位置要求：
  >
  > - 返回值类型必须与原函数返回值类型一致；
  > - 方法参数列表需要和原函数一致，或者可以额外多一个 `Throwable` 类型的参数用于接收对应的异常。
  > - `fallback` 函数默认需要和原方法在同一个类中。若希望使用其他类的函数，则可以指定 `fallbackClass` 为对应的类的 `Class` 对象，注意对应的函数必需为 static 函数，否则无法解析。

- `defaultFallback`（since 1.6.0）

  > 默认的 fallback 函数名称，可选项，通常用于通用的 fallback 逻辑（即可以用于很多服务或方法）。
  >
  > 默认 `fallback` 函数可以针对所有类型的异常（除了 `exceptionsToIgnore`里面排除掉的异常类型）进行处理。若同时配置了 `fallback` 和 `defaultFallback`，则只有 `fallback` 会生效。
  >
  > `defaultFallback` 函数签名要求：
  >
  > - 返回值类型必须与原函数返回值类型一致；
  > - 方法参数列表需要为空，或者可以额外多一个 `Throwable` 类型的参数用于接收对应的异常。
  > - `defaultFallback` 函数默认需要和原方法在同一个类中。若希望使用其他类的函数，则可以指定 `fallbackClass` 为对应的类的 `Class` 对象，注意对应的函数必需为 static 函数，否则无法解析。

- `exceptionsToIgnore`（since 1.6.0）：用于指定哪些异常被排除掉，不会计入异常统计中，也不会进入 `fallback` 逻辑中，而是会原样抛出。



**注意**：

​	1.6.0 之前的版本 `fallback` 函数只针对降级异常（`DegradeException`）进行处理，**不能针对业务异常进行处理**。

​	若 `blockHandler` 和 `fallback` 都进行了配置，则被限流降级而抛出 `BlockException` 时只会进入 `blockHandler` 处理逻辑。若未配置 `blockHandler`、`fallback` 和 `defaultFallback`，则被限流降级时会将 `BlockException` **直接抛出**（若方法本身未定义 `throws BlockException` 则会被 `JVM` 包装一层 `UndeclaredThrowableException`）。



```java
public class TestService {

    // 对应的 `handleException` 函数需要位于 `ExceptionUtil` 类中，并且必须为 static 函数.
    @SentinelResource(value = "test", blockHandler = "handleException", blockHandlerClass = {ExceptionUtil.class})
    public void test() {
        System.out.println("Test");
    }

    // 原函数
    @SentinelResource(value = "hello", blockHandler = "exceptionHandler", fallback = "helloFallback")
    public String hello(long s) {
        return String.format("Hello at %d", s);
    }
    
    // Fallback 函数，函数签名与原函数一致或加一个 Throwable 类型的参数.
    public String helloFallback(long s) {
        return String.format("Halooooo %d", s);
    }

    // Block 异常处理函数，参数最后多一个 BlockException，其余与原函数一致.
    public String exceptionHandler(long s, BlockException ex) {
        // Do some log here.
        ex.printStackTrace();
        return "Oops, error occurred at " + s;
    }
}
```





## 3.定义规则

```java
private static void initFlowRules(){
    List<FlowRule> rules = new ArrayList<>();
    FlowRule rule = new FlowRule();
    rule.setResource("HelloWorld");
    rule.setGrade(RuleConstant.FLOW_GRADE_QPS);
    // Set limit QPS to 20.
    rule.setCount(20);
    rules.add(rule);
    FlowRuleManager.loadRules(rules);
}
```



## 4.检查效果

在日志 `~/logs/csp/${appName}-metrics.log.xxx` 里看到下面的输出:

```log
|--timestamp-|------date time----|-resource-|p |block|s |e|rt
1529998904000|2018-06-26 15:41:44|HelloWorld|20|0    |20|0|0
1529998905000|2018-06-26 15:41:45|HelloWorld|20|5579 |20|0|728
1529998906000|2018-06-26 15:41:46|HelloWorld|20|15698|20|0|0
1529998907000|2018-06-26 15:41:47|HelloWorld|20|19262|20|0|0
1529998908000|2018-06-26 15:41:48|HelloWorld|20|19502|20|0|0
1529998909000|2018-06-26 15:41:49|HelloWorld|20|18386|20|0|0
```

`p` 代表通过的请求, `block` 代表被阻止的请求, `s` 代表成功执行完成的请求个数, `e` 代表用户自定义的异常, `rt` 代表平均响应时长。



# 规则的种类

Sentinel 的所有规则都可以在内存态中动态地查询及修改，修改之后立即生效。

同时 Sentinel 也提供相关 API，可以用来定制自己的规则策略。

Sentinel 支持以下几种规则：**流量控制规则**、**熔断降级规则**、**系统保护规则**、**来源访问控制规则** 和 **热点参数规则**。



## 流量控制规则 (FlowRule)

### 流量规则的定义

重要属性：

| Field           | 说明                                                         | 默认值                        |
| --------------- | ------------------------------------------------------------ | ----------------------------- |
| resource        | 资源名，资源名是限流规则的作用对象                           |                               |
| count           | 限流阈值                                                     |                               |
| grade           | 限流阈值类型，QPS 模式（1）或并发线程数模式（0）             | QPS 模式                      |
| limitApp        | 流控针对的调用来源                                           | `default`，代表不区分调用来源 |
| strategy        | 调用关系限流策略：直接、链路、关联                           | 根据资源本身（直接）          |
| controlBehavior | 流控效果（直接拒绝 / 排队等待 / 慢启动模式），不支持按调用关系限流 | 直接拒绝                      |
| clusterMode     | 是否集群限流                                                 | 否                            |

同一个资源可以同时有多个限流规则，检查规则时会依次检查。

### 通过代码定义流量控制规则

理解上面规则的定义之后，我们可以通过调用 `FlowRuleManager.loadRules()` 方法来用硬编码的方式定义流量控制规则，比如：

```java
private void initFlowQpsRule() {
    List<FlowRule> rules = new ArrayList<>();
    FlowRule rule = new FlowRule(resourceName);
    // set limit qps to 20
    rule.setCount(20);
    rule.setGrade(RuleConstant.FLOW_GRADE_QPS);
    rule.setLimitApp("default");
    rules.add(rule);
    FlowRuleManager.loadRules(rules);
}
```

更多详细内容可以参考 [流量控制](https://github.com/alibaba/Sentinel/wiki/流量控制)。

## 熔断降级规则 (DegradeRule)

熔断降级规则包含下面几个重要的属性：

| Field               | 说明                                                         | 默认值      |
| ------------------- | ------------------------------------------------------------ | ----------- |
| resource            | 资源名，即限流规则的作用对象                                 |             |
| count               | 阈值                                                         |             |
| grade               | 熔断策略，支持秒级 RT/秒级异常比例/分钟级异常数              | 秒级平均 RT |
| timeWindow          | 降级的时间，单位为 s                                         |             |
| rtSlowRequestAmount | RT 模式下 1 秒内连续多少个请求的平均 RT 超出阈值方可触发熔断（1.7.0 引入） | 5           |
| minRequestAmount    | 异常熔断的触发最小请求数，请求数小于该值时即使异常比率超出阈值也不会熔断（1.7.0 引入） | 5           |

同一个资源可以同时有多个降级规则。

理解上面规则的定义之后，我们可以通过调用 `DegradeRuleManager.loadRules()` 方法来用硬编码的方式定义流量控制规则。

```java
private void initDegradeRule() {
    List<DegradeRule> rules = new ArrayList<>();
    DegradeRule rule = new DegradeRule();
    rule.setResource(KEY);
    // set threshold RT, 10 ms
    rule.setCount(10);
    rule.setGrade(RuleConstant.DEGRADE_GRADE_RT);
    rule.setTimeWindow(10);
    rules.add(rule);
    DegradeRuleManager.loadRules(rules);
}
```

更多详情可以参考 [熔断降级](https://github.com/alibaba/Sentinel/wiki/熔断降级)。

## 系统保护规则 (SystemRule)

Sentinel 系统自适应限流从整体维度对应用入口流量进行控制，结合应用的 Load、CPU 使用率、总体平均 RT、入口 QPS 和并发线程数等几个维度的监控指标，通过自适应的流控策略，让系统的入口流量和系统的负载达到一个平衡，让系统尽可能跑在最大吞吐量的同时保证系统整体的稳定性。

系统规则包含下面几个重要的属性：

| Field             | 说明                                   | 默认值      |
| ----------------- | -------------------------------------- | ----------- |
| highestSystemLoad | `load1` 触发值，用于触发自适应控制阶段 | -1 (不生效) |
| avgRt             | 所有入口流量的平均响应时间             | -1 (不生效) |
| maxThread         | 入口流量的最大并发数                   | -1 (不生效) |
| qps               | 所有入口资源的 QPS                     | -1 (不生效) |
| highestCpuUsage   | 当前系统的 CPU 使用率（0.0-1.0）       | -1 (不生效) |

理解上面规则的定义之后，我们可以通过调用 `SystemRuleManager.loadRules()` 方法来用硬编码的方式定义流量控制规则。

```java
private void initSystemRule() {
    List<SystemRule> rules = new ArrayList<>();
    SystemRule rule = new SystemRule();
    rule.setHighestSystemLoad(10);
    rules.add(rule);
    SystemRuleManager.loadRules(rules);
}
```

注意系统规则只针对入口资源（EntryType=IN）生效。更多详情可以参考 [系统自适应保护文档](https://github.com/alibaba/Sentinel/wiki/系统自适应限流)。

## 访问控制规则 (AuthorityRule)

很多时候，我们需要根据调用方来限制资源是否通过，这时候可以使用 Sentinel 的访问控制（黑白名单）的功能。黑白名单根据资源的请求来源（`origin`）限制资源是否通过，若配置白名单则只有请求来源位于白名单内时才可通过；若配置黑名单则请求来源位于黑名单时不通过，其余的请求通过。

授权规则，即黑白名单规则（`AuthorityRule`）非常简单，主要有以下配置项：

- `resource`：资源名，即限流规则的作用对象
- `limitApp`：对应的黑名单/白名单，不同 origin 用 `,` 分隔，如 `appA,appB`
- `strategy`：限制模式，`AUTHORITY_WHITE` 为白名单模式，`AUTHORITY_BLACK` 为黑名单模式，默认为白名单模式

更多详情可以参考 [来源访问控制](https://github.com/alibaba/Sentinel/wiki/黑白名单控制)。

## 热点规则 (ParamFlowRule)

Sentinel 利用 LRU 策略统计最近最常访问的热点参数，结合令牌桶算法来进行参数级别的流控。热点参数限流支持集群模式。

```xml
<dependency>
    <groupId>com.alibaba.csp</groupId>
    <artifactId>sentinel-parameter-flow-control</artifactId>
    <version>x.y.z</version>
</dependency>
```

详情可以参考 [热点参数限流](https://github.com/alibaba/Sentinel/wiki/热点参数限流)



# 查询更改规则

引入了 transport 模块后，可以通过以下的 HTTP API 来获取所有已加载的规则：

```http
http://localhost:8719/getRules?type=<XXXX>
```

其中，`type=flow` 以 JSON 格式返回现有的限流规则，degrade 返回现有生效的降级规则列表，system 则返回系统保护规则。

获取所有热点规则：

```http
http://localhost:8719/getParamRules
```



# 持久化规则

规则配置，都是存在内存中的。即如果应用重启，这个规则就会失效。因此我们提供了开放的接口，您可以通过实现 `DataSource`接口的方式，来自定义规则的存储数据源。



通常建议：

- 整合动态配置系统，如 `ZooKeeper、Nacos、Apollo` 等，动态地实时刷新配置规则
- 结合 `RDBMS、NoSQL、VCS` 等来实现该规则
- 配合 `Sentinel Dashboard` 使用

[详情参考]([https://github.com/alibaba/Sentinel/wiki/%E5%8A%A8%E6%80%81%E8%A7%84%E5%88%99%E6%89%A9%E5%B1%95](https://github.com/alibaba/Sentinel/wiki/动态规则扩展))



# 规则生效的效果

## 判断限流降级异常

在 Sentinel 中所有流控降级相关的异常都是异常类 `BlockException` 的子类：

- 流控异常：`FlowException`
- 熔断降级异常：`DegradeException`
- 系统保护异常：`SystemBlockException`
- 热点参数限流异常：`ParamFlowException`

我们可以通过以下函数判断是否为 Sentinel 的流控降级异常：

```java
BlockException.isBlockException(Throwable t);
```

除了在业务代码逻辑上看到规则生效，我们也可以通过下面简单的方法，来校验规则生效的效果：

- **暴露的 HTTP 接口**：通过运行下面命令 `curl http://localhost:8719/cnode?id=<资源名称>`，观察返回的数据。如果规则生效，在返回的数据栏中的 `block` 以及 `block(m)` 中会有显示
- **日志**：Sentinel 提供秒级的资源运行日志以及限流日志，详情可以参考: [日志](https://github.com/alibaba/Sentinel/wiki/日志)

## block 事件

Sentinel 提供以下扩展接口，可以通过 `StatisticSlotCallbackRegistry` 向 `StatisticSlot` 注册回调函数：

- `ProcessorSlotEntryCallback`: callback when resource entry passed (`onPass`) or blocked (`onBlocked`)
- `ProcessorSlotExitCallback`: callback when resource entry successfully completed (`onExit`)

可以利用这些回调接口来实现报警等功能，实时的监控信息可以从 `ClusterNode` 中实时获取。



# 对比

|                | Sentinel                                                   | Hystrix                 | resilience4j                     |
| -------------- | ---------------------------------------------------------- | ----------------------- | -------------------------------- |
| 隔离策略       | 信号量隔离（并发线程数限流）                               | 线程池隔离/信号量隔离   | 信号量隔离                       |
| 熔断降级策略   | 基于响应时间、异常比率、异常数                             | 基于异常比率            | 基于异常比率、响应时间           |
| 实时统计实现   | 滑动窗口（LeapArray）                                      | 滑动窗口（基于 RxJava） | Ring Bit Buffer                  |
| 动态规则配置   | 支持多种数据源                                             | 支持多种数据源          | 有限支持                         |
| 扩展性         | 多个扩展点                                                 | 插件的形式              | 接口的形式                       |
| 基于注解的支持 | 支持                                                       | 支持                    | 支持                             |
| 限流           | 基于 QPS，支持基于调用关系的限流                           | 有限的支持              | Rate Limiter                     |
| 流量整形       | 支持预热模式、匀速器模式、预热排队模式                     | 不支持                  | 简单的 Rate Limiter 模式         |
| 系统自适应保护 | 支持                                                       | 不支持                  | 不支持                           |
| 控制台         | 提供开箱即用的控制台，可配置规则、查看秒级监控、机器发现等 | 简单的监控查看          | 不提供控制台，可对接其它监控系统 |


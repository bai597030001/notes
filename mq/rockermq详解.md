# 详解

https://juejin.cn/post/6844904008629354504

> RocketMQ是一个纯Java、分布式、队列模型的开源消息中间件，前身是MetaQ，是阿里参考Kafka特点研发的一个队列模型的消息中间件，后开源给apache基金会成为了apache的顶级开源项目，具有高性能、高可靠、高实时、分布式特点。

![](./img/rocketmq1.png)



**RocketMQ**啥都是**集群**部署的，这是他**吞吐量大**，**高可用**的原因之一，集群的模式也很花哨，可以支持多master 模式、多master多slave异步复制模式、多 master多slave同步双写模式。



## 角色

### Broker

- broker主要用于producer和consumer接收和发送消息
- broker会定时向nameserver提交自己的信息
- 是消息中间件的消息存储、转发服务器
- 每个Broker节点，在启动时，都会遍历NameServer列表，与每个NameServer建立长连接，注册自己的信息，之后定时上报

### Nameserver

- 理解成zookeeper的效果，只是他没用zk，而是自己写了个nameserver来替代zk
- 底层由netty实现，提供了路由管理、服务注册、服务发现的功能，是一个无状态节点
- nameserver是服务发现者，集群中各个角色（producer、broker、consumer等）都需要定时向nameserver上报自己的状态，以便互相发现彼此，超时不上报的话，nameserver会把它从列表中剔除
- nameserver可以部署多个，当多个nameserver存在的时候，其他角色同时向他们上报信息，以保证高可用，
- NameServer集群间互不通信，没有主备的概念
- nameserver内存式存储，nameserver中的broker、topic等信息默认不会持久化，所以他是无状态节点

### Producer

- 随机选择其中一个NameServer节点建立长连接，获得Topic路由信息（包括topic下的queue，这些queue分布在哪些broker上等等）
- 接下来向提供topic服务的master建立长连接（因为rocketmq只有master才能写消息），且定时向master发送心跳

### Consumer

- 通过NameServer集群获得Topic的路由信息，连接到对应的Broker上消费消息
- 由于Master和Slave都可以读取消息，因此Consumer会与Master和Slave都建立连接进行消费消息

## 核心流程

- Broker都注册到Nameserver上
- Producer发消息的时候会从Nameserver上获取发消息的topic信息
- Producer向提供服务的所有master建立长连接，且定时向master发送心跳
- Consumer通过NameServer集群获得Topic的路由信息
- Consumer会与所有的Master和所有的Slave都建立连接进行监听新消息



## 核心概念

### Message

消息载体。Message发送或者消费的时候必须指定Topic。Message有一个可选的Tag项用于过滤消息，还可以添加额外的键值对。

### Tag

Tag 是 Topic 的进一步细分，顾名思义，标签。每个发送的消息都能打tag，消费的时候可以根据tag进行过滤，选择性消费。

### topic

消息的逻辑分类，发消息之前必须要指定一个topic才能发，就是将这条消息发送到这个topic上。消费消息的时候指定这个topic进行消费。就是逻辑分类。

### queue

其实就是kafka中的分区（partition）的概念。

1个Topic会被分为N个Queue，数量是可配置的。message本身其实是存储到queue上的，消费者消费的也是queue上的消息。多说一嘴，比如1个topic4个queue，有5个Consumer都在消费这个topic，那么会有一个consumer浪费掉了，因为负载均衡策略，每个consumer消费1个queue，5>4，溢出1个，这个会不工作。

### Message Model

消息模型：集群（Clustering）和广播（Broadcasting）

### Message Order

消息顺序：顺序（Orderly）和并发（Concurrently）

### Producer Group

消息生产者组

### Consumer Group

消息消费者组



## 消费模式

### 集群模式（Clustering）

- 每条消息只需要被处理一次，broker只会把消息发送给消费集群中的一个消费者
- 在消息重投时，不能保证路由到同一台机器上
- 消费状态由broker维护

![](./img/rocketmq2.png)

### 广播模式（Broadcasting）

- 消费进度由consumer维护
- 保证每个消费者都消费一次消息
- 消费失败的消息不会重投

![](./img/rocketmq3.png)



# 安装部署



## 控制台

https://github.com/apache/rocketmq-externals

### 下载源码

### 修改配置（可选）

修改`rocketmq-consolesrcmainresourcesapplication.properties`文件的`server.port`默认8080。

### 编译打包

进入`rocketmq-console`，然后用maven进行编译打包

```shell
$ mvn clean package -DskipTests
```

### 启动控制台

```shell
java -jar rocketmq-console-ng-1.0.1.jar --rocketmq.config.namesrvAddr=127.0.0.1:9876
```

## 测试

> rocketmq给我们提供了测试工具和测试类，可以在安装完很方便的进行测试。

rocketmq给我们提供的默认测试工具在bin目录下，叫`tools.sh`。我们测试前需要配置这个脚本，为他指定namesrv地址才可以

```shell
vim tools.sh
# 在export JAVA_HOME上面添加如下这段代码
export NAMESRV_ADDR=localhost:9876
```

### 发送消息

```shell
$ ./tools.sh org.apache.rocketmq.example.quickstart.Producer
```

成功的话会看到哗哗哗的日志，因为这个类会发送1000条消息到TopicTest这个Topic下。

### 消费消息

```shell
$ ./tools.sh org.apache.rocketmq.example.quickstart.Consumer
```

成功的话会看到哗哗哗的日志，因为这个类会消费TopicTest下的全部消息。刚发送的1000条都会被消费掉。



# 持久化



# 消息推拉模式



# 保证消息不丢失

## producer

```java
// 发送消息到一个Broker
try {
    SendResult sendResult = mqProducer.send(msg);
} catch (RemotingException e) {
    e.printStackTrace();
} catch (MQBrokerException e) {
    e.printStackTrace();
} catch (InterruptedException e) {
    e.printStackTrace();
}
```

`send` 方法是一个同步操作，只要这个方法不抛出任何异常，就代表消息已经**发送成功**。

消息发送成功仅代表消息已经到了 Broker 端，Broker 在不同配置下，可能会返回不同响应状态:

- `SendStatus.SEND_OK`
- `SendStatus.FLUSH_DISK_TIMEOUT`
- `SendStatus.FLUSH_SLAVE_TIMEOUT`
- `SendStatus.SLAVE_NOT_AVAILABLE`

官方状态说明：

![](./img/rocketmq1.jpg)

另外 RocketMQ 还提供异步的发送的方式，适合于链路耗时较长，对响应时间较为敏感的业务场景。

```java
try {
    // 异步发送消息到，主线程不会被阻塞，立刻会返回
    mqProducer.send(msg, new SendCallback() {
        @Override
        public void onSuccess(SendResult sendResult) {
            // 消息发送成功，
        }

        @Override
        public void onException(Throwable e) {
            // 消息发送失败，可以持久化这条数据，后续进行补偿处理
        }
    });
} catch (RemotingException e) {
    e.printStackTrace();
} catch (InterruptedException e) {
    e.printStackTrace();
}
```

不管是同步还是异步的方式，都会碰到网络问题导致发送失败的情况。针对这种情况，我们可以设置合理的重试次数，当出现网络问题，可以自动重试。设置方式如下：

```java
// 同步发送消息重试次数，默认为 2
mqProducer.setRetryTimesWhenSendFailed(3);
// 异步发送消息重试次数，默认为 2
mqProducer.setRetryTimesWhenSendAsyncFailed(3);
```

## broker

### 持久化

默认情况下，消息只要到了 Broker 端，将会优先保存到内存中，然后立刻返回确认响应给生产者。随后 Broker 定期批量的将一组消息从内存异步刷入磁盘。

这种方式减少 I/O 次数，可以取得更好的性能，但是如果发生机器掉电，异常宕机等情况，消息还未及时刷入磁盘，就会出现丢失消息的情况。

若想保证 Broker 端不丢消息，保证消息的可靠性，我们需要将消息保存机制修改为同步刷盘方式，即消息**存储磁盘成功**，才会返回响应。

修改 Broker 端配置如下：

```conf
## 默认情况为 ASYNC_FLUSH 
flushDiskType = SYNC_FLUSH 
```

若 Broker 未在同步刷盘时间内（**默认为 5s**）完成刷盘，将会返回 `SendStatus.FLUSH_DISK_TIMEOUT` 状态给生产者。



### 集群部署

为了保证可用性，Broker 通常采用一主（**master**）多从（**slave**）部署方式。为了保证消息不丢失，消息还需要复制到 slave 节点。

默认方式下，消息写入 **master** 成功，就可以返回确认响应给生产者，接着消息将会异步复制到 **slave** 节点。

> 注：master 配置：flushDiskType = SYNC_FLUSH

此时若 master 突然**宕机且不可恢复**，那么还未复制到 **slave** 的消息将会丢失。

为了进一步提高消息的可靠性，我们可以采用同步的复制方式，**master** 节点将会同步等待 **slave** 节点复制完成，才会返回确认响应。



Broker master 节点 同步复制配置如下：

```conf
## 默认为 ASYNC_MASTER 
brokerRole=SYNC_MASTER
```

如果 **slave** 节点未在指定时间内同步返回响应，生产者将会收到 `SendStatus.FLUSH_SLAVE_TIMEOUT` 返回状态。



**总结**

结合producer和broker，若需要**严格保证消息不丢失**，broker 需要采用如下配置：

```conf
## master 节点配置
flushDiskType = SYNC_FLUSH
brokerRole=SYNC_MASTER

## slave 节点配置
brokerRole=slave
flushDiskType = SYNC_FLUSH
```

同时这个过程我们还需要生产者配合，判断返回状态是否是 `SendStatus.SEND_OK`。若是其他状态，就需要<font color=#00dd00>考虑补偿重试</font>。

虽然上述配置提高消息的高可靠性，但是会**降低性能**，生产实践中需要综合选择。



## consumer

Consumer消费完消息后要进行ACK确认，如果未确认则代表是消费失败，这时候Broker会进行重试策略（仅集群模式会重试）。ACK的意思就是：Consumer说：ok，我消费成功了。这条消息给我标记成已消费吧。



消费者从 broker 拉取消息，然后执行相应的业务逻辑。一旦执行成功，将会返回 `ConsumeConcurrentlyStatus.CONSUME_SUCCESS` 状态给 Broker。

如果 Broker 未收到消费确认响应或收到其他状态，消费者下次还会再次拉取到该条消息，进行重试。这样的方式有效避免了消费者消费过程发生异常，或者消息在网络传输中丢失的情况。

```java
// 注册回调实现类来处理从broker拉取回来的消息
consumer.registerMessageListener(new MessageListenerConcurrently() {
    @Override
    public ConsumeConcurrentlyStatus consumeMessage(List<MessageExt> msgs, ConsumeConcurrentlyContext context) {
        // 执行业务逻辑
        // 标记该消息已经被成功消费
        return ConsumeConcurrentlyStatus.CONSUME_SUCCESS;
    }
});
// 启动消费者实例
consumer.start();
```

以上消费消息过程的，我们需要**注意返回消息状态**。只有当业务逻辑真正执行成功，我们才能返回 `ConsumeConcurrentlyStatus.CONSUME_SUCCESS`。否则我们需要返回 `ConsumeConcurrentlyStatus.RECONSUME_LATER`，稍后再重试。



# 顺序消费



# 重复消费



# 分布式事务消息



# rocketmq-client

```xml
<dependency>
    <groupId>org.apache.rocketmq</groupId>
    <artifactId>rocketmq-client</artifactId>
    <version>4.2.0</version>
</dependency>
```



```java
import org.apache.rocketmq.client.producer.DefaultMQProducer;
import org.apache.rocketmq.client.producer.SendResult;
import org.apache.rocketmq.common.message.Message;
import org.apache.rocketmq.remoting.common.RemotingHelper;

public class Producer {

    public static void main(String[] args) throws Exception {
        //创建一个消息生产者，并设置一个消息生产者组
        DefaultMQProducer producer = new DefaultMQProducer("niwei_producer_group");

        //指定 NameServer 地址
        producer.setNamesrvAddr("localhost:9876");

        //初始化 Producer，整个应用生命周期内只需要初始化一次
        producer.start();

        for (int i = 0; i < 100; i++) {
            //创建一条消息对象，指定其主题、标签和消息内容
            Message msg = new Message(
                    "topic_example_java" /* 消息主题名 */,
                    "TagA" /* 消息标签 */,
                    ("Hello Java demo RocketMQ " + i).getBytes(RemotingHelper.DEFAULT_CHARSET) /* 消息内容 */
            );

            //发送消息并返回结果
            SendResult sendResult = producer.send(msg);

            System.out.printf("%s%n", sendResult);
        }

        // 一旦生产者实例不再被使用则将其关闭，包括清理资源，关闭网络连接等
        producer.shutdown();
    }
}
```



```java
import org.apache.rocketmq.client.consumer.DefaultMQPushConsumer;
import org.apache.rocketmq.client.consumer.listener.ConsumeConcurrentlyContext;
import org.apache.rocketmq.client.consumer.listener.ConsumeConcurrentlyStatus;
import org.apache.rocketmq.client.consumer.listener.MessageListenerConcurrently;
import org.apache.rocketmq.common.consumer.ConsumeFromWhere;
import org.apache.rocketmq.common.message.MessageExt;

import java.io.UnsupportedEncodingException;
import java.util.Date;
import java.util.List;

public class Consumer {

    public static void main(String[] args) throws Exception {
        //创建一个消息消费者，并设置一个消息消费者组
        DefaultMQPushConsumer consumer = new DefaultMQPushConsumer("niwei_consumer_group");
        //指定 NameServer 地址
        consumer.setNamesrvAddr("localhost:9876");
        //设置 Consumer 第一次启动时从队列头部开始消费还是队列尾部开始消费
        consumer.setConsumeFromWhere(ConsumeFromWhere.CONSUME_FROM_FIRST_OFFSET);
        //订阅指定 Topic 下的所有消息
        consumer.subscribe("topic_example_java", "*");

        //注册消息监听器
        consumer.registerMessageListener(new MessageListenerConcurrently() {
            public ConsumeConcurrentlyStatus consumeMessage(List<MessageExt> list, ConsumeConcurrentlyContext context) {
                //默认 list 里只有一条消息，可以通过设置参数来批量接收消息
                if (list != null) {
                    for (MessageExt ext : list) {
                        try {
                            System.out.println(new Date() + new String(ext.getBody(), "UTF-8"));
                        } catch (UnsupportedEncodingException e) {
                            e.printStackTrace();
                        }
                    }
                }
                return ConsumeConcurrentlyStatus.CONSUME_SUCCESS;
            }
        });

        // 消费者对象在使用之前必须要调用 start 初始化
        consumer.start();
        System.out.println("消息消费者已启动");
    }
}
```



# Spring集成

目前在 Spring 框架中集成 RocketMQ 有三种方式，一是将消息生产者和消费者定义成 bean 对象交由 Spring 容器管理，二是使用 RocketMQ 社区的外部项目 rocketmq-jms（https://github.com/apache/rocketmq-externals/tree/master/rocketmq-jms）然后通过 spring-jms 方式集成使用，三是如果你的应用是基于 spring-boot 的，可以使用 RocketMQ 的外部项目 rocketmq-spring-boot-starter（https://github.com/apache/rocketmq-externals/tree/master/rocketmq-spring-boot-starter）比较方便的收发消息。
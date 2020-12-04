# 安装配置



## 管理界面

默认用户名密码 guest



## 配置rabbitmq账号密码

执行以下命令：

```shell
$ rabbitmq-plugins enable rabbitmq_management
```



添加新用户：

```shell
$ rabbitmqctl add_user admin admin
```



这里添加的用户名和密码为 admin

添加用户标签（超级管理员）：

```shell
$ rabbitmqctl set_user_tags admin administrator
```



添加成功后可以执行以下命令查看当前的用户列表：

```shell
$ rabbitmqctl list_users
```



访问网页：

```
http://localhost:15672/ 使用默认账号：guest/guest
```



## 常见操作

```shell
$ service rabbitmq-server start #启动
$ rabbitmq-server -detached   #后台运行rabbitmq

$ service rabbitmq-server stop #停止

$ service rabbitmq-server restart #重启

$ service rabbitmq-server status #查看状态

$ service rabbitmq-server etc #查看有哪些命令可以使用

$ rabbitmq-plugins enable rabbitmq_management   #启动后台管理

$ rabbitmqctl status # 查看状态

$ rabbitmq-pluginsenable xxx # 开启某个插件
$ rabbitmq-pluginsdisablexxx # 关闭某个插件

$ rabbitmqctl list_bindings # 查看绑定
$ rabbitmqctl list_exchanges # 查看交换器
$ rabbitmqctl list_queues # 查看已声明的队列
```

## 模拟器

http://tryrabbitmq.com/

# AMQP 相关概念



![](./img/rabbit_mq.png)

## 信道

如果项目需要发布消息，那么必须要链接到 RabbitMQ，而项目与 RabbitMQ之间使用 TCP 连接，假如每次发布消息都要连接TCP，这不仅会造成连接资源严重浪费，还会造成服务器性能瓶颈，所以 RabbitMQ 为所有的线程只用一条 TCP 连接，怎么实现的呢？RabbitMQ 引入了信道的概念，所有需要发布消息的线程都包装成一条信道在 TCP 中传输，理论上 一条 TCP 连接支持无限多个信道，模型如下：

![](./img/rabbit_mq_channel.png)

## 队列

消息队列，用来保存消息直到发送给消费者。它是消息的容器，也是消息的终点。一个消息可投入一个或多个队列。消息一直在队列里面，等待消费者连接到这个队列将其取走。

## 绑定

绑定，用于消息队列和交换器之间的关联。一个绑定就是基于路由键将交换器和消息队列连接起来的路由规则，所以可以将交换器理解成一个由绑定构成的路由表

![](./img/rabbit_mq_binding.png)

## 交换器

我们向 RabbitMQ 发送消息，实际上是把消息发到交换器了，再由交换器根据相关路由规则发到特定队列上，在队列上监听的消费者就可以进行消费了，目前 RabbitMQ 共四种类型：direct、fanout、topic、headers 。

headers 匹配 AMQP 消息的 header 而不是路由键，此外 headers 交换器和 direct 交换器完全一致，但性能差很多，目前几乎用不到了，所以直接看另外三种类型：

### direct交换器

消息中的路由键（routing key）如果和 Binding 中的 binding key 一致， 交换器就将消息发到对应的队列中。路由键与队列名完全匹配，如果一个队列绑定到交换机要求路由键为“dog”，则只转发 routing key 标记为“dog”的消息，不会转发“dog.puppy”，也不会转发“dog.guard”等等。它是完全匹配、单播的模式。

### fanout交换器

每个发送到 fanout 交换器中的消息，他不会去匹配路由键，直接把消息投递到所有绑定到 fanout 交换器中的队列上，它就像一个广播站一样，它会向所有收听广播的用户发送消息。对应到系统上，它允许你针对一个消息作不同操作，比如用户上传了一张新的图片，系统要同时对这个事件进行不同的操作，比如删除旧的图片缓存、增加积分奖励等等。这样就大大降低了系统之间的耦合度了。

### topic交换器

topic 交换器有点类似于 direct 交换器，它通过模式匹配分配消息的路由键属性，将路由键和某个模式进行匹配，此时队列需要绑定到一个模式上。它将路由键和绑定键的字符串切分成单词，这些单词之间用点隔开。它同样也会识别两个通配符：符号“#”和符号"* " 。#匹配0个或多个单词，*匹配不多不少一个单词。



# 客户端

```xml
<dependency>
    <groupId>com.rabbitmq</groupId>
    <artifactId>amqp-client</artifactId>
    <version>4.11.1</version>
</dependency>
```

生产者

```java
public class Producer {

    public static void main(String[] argv) throws Exception {
        //创建连接工厂
        ConnectionFactory factory = new ConnectionFactory();
        factory.setUsername("guest");
        factory.setPassword("guest");
        //设置 RabbitMQ 地址
        factory.setHost("host-application");
        //建立到代理服务器到连接
        Connection conn = factory.newConnection();
        //获得信道
        Channel channel = conn.createChannel();
        //声明交换器
        String exchangeName = "hello-exchange";
        channel.exchangeDeclare(exchangeName, "fanout", true);

        String routingKey = "hola";
        //发布消息
        byte[] messageBodyBytes = "messageBodyBytes".getBytes();
        // 设置消息过期时间
        AMQP.BasicProperties properties = new AMQP.BasicProperties.Builder()
                .deliveryMode(2) // 设置持久化
                //.expiration("6000") // 消息过期时间
                .build();
        // channel.basicPublish(exchangeName, routingKey, MessageProperties.PERSISTENT_TEXT_PLAIN, messageBodyBytes);
        channel.basicPublish(exchangeName, routingKey, properties, messageBodyBytes);

        channel.close();
        conn.close();
    }
}
```

消费者

```java
public class Consumer {

    public static void main(String[] argv) throws Exception {
        ConnectionFactory factory = new ConnectionFactory();
        factory.setUsername("guest");
        factory.setPassword("guest");
        factory.setHost("host-application");
        //建立到代理服务器到连接
        Connection conn = factory.newConnection();
        //获得信道
        final Channel channel = conn.createChannel();
        //声明交换器
        String exchangeName = "hello-exchange";
        channel.exchangeDeclare(exchangeName, "fanout", true);
        //声明队列
        String queueName = "hello-queue";
        // 设置队列过期属性
//        Map<String, Object> args = new HashMap<String, Object>(4){{
//            put("vhost", "/");
//            put("username","guest");
//            put("password", "guest");
//            // 过期时间
//            put("x-message-ttl",6000);
//        }};
        channel.queueDeclare(queueName, true, false, false, null).getQueue();
        System.out.println("声明队列名称：" + queueName);
        String routingKey = "hola";
        //绑定队列，通过键 hola 将队列和交换器绑定起来
        channel.queueBind(queueName, exchangeName, routingKey);

        while (true) {
            //消费消息
            boolean autoAck = false;
            String consumerTag = "";
            channel.basicConsume(queueName, autoAck, consumerTag, new DefaultConsumer(channel) {
                @Override
                public void handleDelivery(String consumerTag, Envelope envelope,
                                           AMQP.BasicProperties properties, byte[] body) throws IOException {
                    String routingKey = envelope.getRoutingKey();
                    String contentType = properties.getContentType();
                    System.out.println("消费的路由键：" + routingKey);
                    System.out.println("消费的内容类型：" + contentType);
                    long deliveryTag = envelope.getDeliveryTag();
                    //确认消息
                    channel.basicAck(deliveryTag, false);
                    System.out.println("消费的消息体内容：");
                    String bodyStr = new String(body, "UTF-8");
                    System.out.println(bodyStr);
                }
            });
        }
    }
}
```

# HA

# 持久化

消息的可靠性是RabbitMQ的一大特色，那么RabbitMQ是如何保证消息可靠性的呢——消息持久化。

为了保证RabbitMQ在退出或者crash等异常情况下数据没有丢失，需要将queue，exchange和Message都持久化。

## queue的持久化

queue的持久化是通过durable=true来实现的。

```java
Connection connection = connectionFactory.newConnection();
Channel channel = connection.createChannel();
// 第二个参数设置为true,即durable=true.
channel.queueDeclare("queue.persistent.name", true, false, false, null);
```

源码

```java
/**
     * Declare a queue
     * @see com.rabbitmq.client.AMQP.Queue.Declare
     * @see com.rabbitmq.client.AMQP.Queue.DeclareOk
     * @param queue the name of the queue
     * @param durable true if we are declaring a durable queue (the queue will survive a server restart)
     * @param exclusive true if we are declaring an exclusive queue (restricted to this connection)
     * @param autoDelete true if we are declaring an autodelete queue (server will delete it when no longer in use)
     * @param arguments other properties (construction arguments) for the queue
     * @return a declaration-confirm method to indicate the queue was successfully declared
     * @throws java.io.IOException if an error is encountered
     
     durable:
     	是否持久化
     
     exclusive：
     	排他队列，如果一个队列被声明为排他队列，该队列仅对首次申明它的连接可见，并在连接断开时自动删除。
     	这里需要注意三点：
     		1. 排他队列是基于连接可见的，同一连接的不同信道是可以同时访问同一连接创建的排他队列；
     		2.“首次”，如果一个连接已经声明了一个排他队列，其他连接是不允许建立同名的排他队列的，这个与普通队列不同；
     		3.即使该队列是持久化的，一旦连接关闭或者客户端退出，该排他队列都会被自动删除的，这种队列适用于一个客户端发送读取消息的应用场景
     		
     autoDelete：
     	自动删除，如果该队列没有任何订阅的消费者的话，该队列会被自动删除。这种队列适用于临时队列。
     */
    Queue.DeclareOk queueDeclare(String queue, boolean durable, boolean exclusive, boolean autoDelete,
                                 Map<String, Object> arguments) throws IOException;


    Queue.DeclareOk queueDeclare() throws IOException;
    Queue.DeclareOk queueDeclare(String queue, boolean durable, boolean exclusive, boolean autoDelete,
                                     Map<String, Object> arguments) throws IOException;
    void queueDeclareNoWait(String queue, boolean durable, boolean exclusive, boolean autoDelete,
                                Map<String, Object> arguments) throws IOException;
	// 可以用来检测一个queue是否已经存在。如果该队列存在，则会返回true；如果不存在，就会返回异常，但是不会创建新的队列。
    Queue.DeclareOk queueDeclarePassive(String queue) throws IOException;
```

## 消息的持久化

如过将queue的持久化标识durable设置为true,则代表是一个持久的队列，那么在服务重启之后，也会存在，因为服务会把持久化的queue存放在硬盘上，当服务重启的时候，会重新加载之前被持久化的queue。

队列是可以被持久化，但是里面的消息是否为持久化那还要看消息的持久化设置。也就是说，重启之前那个queue里面还没有发出去的消息的话，重启之后那队列里面是不是还存在原来的消息，这个就要取决于发生着在发送消息时对消息的设置了。

如果要在重启后保持消息的持久化必须设置消息是持久化的标识。

```java
channel.basicPublish("exchange.persistent", "persistent", MessageProperties.PERSISTENT_TEXT_PLAIN, "persistent_test_message".getBytes());
```

关键是：`MessageProperties.PERSISTENT_TEXT_PLAIN`

源码

```java
void basicPublish(String exchange, String routingKey, BasicProperties props, byte[] body) throws IOException;
void basicPublish(String exchange, String routingKey, boolean mandatory, BasicProperties props, byte[] body)
        throws IOException;
void basicPublish(String exchange, String routingKey, boolean mandatory, boolean immediate, BasicProperties props, byte[] body)
        throws IOException;
```

```java
public BasicProperties(
            String contentType,//消息类型如：text/plain
            String contentEncoding,//编码
            Map<String,Object> headers,
            Integer deliveryMode,//1:nonpersistent 2:persistent // deliveryMode=1代表不持久化，deliveryMode=2代表持久化。
            Integer priority,//优先级
            String correlationId,
            String replyTo,//反馈队列
            String expiration,//expiration到期时间
            String messageId,
            Date timestamp,
            String type,
            String userId,
            String appId,
            String clusterId)
```



设置了队列和消息的持久化之后，当broker服务重启的之后，消息依旧存在。单只设置队列持久化，重启之后消息会丢失；单只设置消息的持久化，重启之后队列消失，既而消息也丢失。单单设置消息持久化而不设置队列的持久化显得毫无意义。

## exchange的持久化

如果不设置exchange的持久化对消息的可靠性来说没有什么影响，但是如果exchange不设置持久化，那么当broker服务重启之后，exchange将不复存在，那么既而发送方rabbitmq producer就无法正常发送消息。所以同样应该设置exchange的持久化。

```java
Exchange.DeclareOk exchangeDeclare(String exchange, String type, boolean durable) throws IOException;
Exchange.DeclareOk exchangeDeclare(String exchange, String type, boolean durable, boolean autoDelete,
                                   Map<String, Object> arguments) throws IOException;
Exchange.DeclareOk exchangeDeclare(String exchange, String type) throws IOException;
Exchange.DeclareOk exchangeDeclare(String exchange,
                                          String type,
                                          boolean durable,
                                          boolean autoDelete,
                                          boolean internal,
                                          Map<String, Object> arguments) throws IOException;
void exchangeDeclareNoWait(String exchange,
                           String type,
                           boolean durable,
                           boolean autoDelete,
                           boolean internal,
                           Map<String, Object> arguments) throws IOException;
Exchange.DeclareOk exchangeDeclarePassive(String name) throws IOException;
```

设置

```java
// 在声明的时候将durable字段设置为true
channel.exchangeDeclare(exchangeName, “direct/topic/header/fanout”, true);
```

## ACK

将queue，exchange, message等都设置了持久化之后就能保证100%保证数据不丢失了吗？不是。

首先，从consumer端来说，如果这时autoAck=true，那么当consumer接收到相关消息之后，还没来得及处理就crash掉了，那么这样也算数据丢失。

这种情况下，只需将autoAck设置为false(方法定义如下)，然后在正确处理完消息之后进行手动ack（channel.basicAck()）.

```java
String basicConsume(String queue, boolean autoAck, Consumer callback) throws IOException;
```

## 镜像队列

消息在正确存入RabbitMQ之后，还需要有一段时间（这个时间很短，但不可忽视）才能存入磁盘之中，RabbitMQ并不是为每条消息都做fsync的处理，

可能仅仅保存到cache中而不是物理磁盘上，在这段时间内RabbitMQ broker发生crash, 消息保存到cache但是还没来得及落盘，那么这些消息将会丢失。

怎么解决呢？

可以引入RabbitMQ的mirrored-queue即镜像队列，这个相当于配置了副本，当master在此特殊时间内crash掉，可以自动切换到slave，这样有效的保障了HA, 

除非整个集群都挂掉，这样也不能完全的100%保障RabbitMQ不丢消息，但比没有mirrored-queue的要好很多，很多现实生产环境下都是配置了mirrored-queue的。

## 事务机制或者Confirm机制

在producer引入事务机制或者Confirm机制来确保消息已经正确的发送至broker端

### 事务机制

```java
// 通过txSelect开启事务之后，我们便可以发布消息给broker代理服务器了，如果txCommit提交成功了，则消息一定到达了broker了;
// 如果在txCommit执行之前broker异常崩溃或者由于其他原因抛出异常，这个时候我们便可以捕获异常通过txRollback回滚事务了。
channel.txSelect();
channel.basicPublish(ConfirmConfig.exchangeName, ConfirmConfig.routingKey, MessageProperties.PERSISTENT_TEXT_PLAIN, ConfirmConfig.msg_10B.getBytes());
channel.txCommit();
```

### Confirm机制

生产者将信道设置成confirm模式，一旦信道进入confirm模式，所有在该信道上面发布的消息都会被指派一个唯一的ID(从1开始)，一旦消息被投递到所有匹配的

队列之后，broker就会发送一个确认给生产者（包含消息的唯一ID）,这就使得生产者知道消息已经正确到达目的队列了。

<font color=#00dd00>如果消息和队列是可持久化的，那么确认消息会将消息写入磁盘之后发出</font>，broker回传给生产者的确认消息中deliver-tag域包含了确认消息的序列号，此外broker也可以设置basic.ack的multiple域，表示到这个序列号之前的所有消息都已经得到了处理。



confirm模式最大的好处在于他是异步的，一旦发布一条消息，生产者应用程序就可以在等信道返回确认的同时继续发送下一条消息，当消息最终得到确认之后，

生产者应用便可以通过回调方法来处理该确认消息，如果RabbitMQ因为自身内部错误导致消息丢失，就会发送一条nack消息，生产者应用程序同样可以在回调方

法中处理该nack消息。



在channel 被设置成 confirm 模式之后，所有被 publish 的后续消息都将被 confirm（即 ack） 或者被nack一次。但是没有对消息被 confirm 的快慢做任何保证，并且同一条消息不会既被 confirm又被nack 。



**开启confirm模式**

生产者通过调用channel.confirmSelect()方法将channel设置为confirm模式

## 总结

RabbitMQ的可靠性涉及producer端的确认机制、broker端的镜像队列的配置以及consumer端的确认机制，要想确保消息的可靠性越高，那么性能也会随之而降。

# 消息推拉模式

rabbitmq中支持两种消息处理的模式，一种是订阅模式（也叫push模式），由broker主动将消息推送给订阅队列的消费者；

另一种是检索模式（也叫pull模式），需要消费者调用channel.basicGet方法，主动从队列中拉取消息。

## 订阅模式（push）

```java
boolean autoAck = false;

channel.basicConsume(queueName, autoAck, "myConsumerTag",
    new DefaultConsumer(channel) {
        
        public void (String consumerTag,
                                   Envelope envelope,
                                   AMQP.BasicProperties properties,
                                   byte[] body)
            throws IOException {
            String routingKey = envelope.getRoutingKey();
            String contentType = properties.getContentType();
            long deliveryTag = envelope.getDeliveryTag();
            // (process the message components here ...)
            // channel.basicAck(deliveryTag, multiple);
            channel.basicAck(deliveryTag, false); // 消费端确认消息处理成功
        }
    });
```

## 检索模式（pull）

```java
boolean autoAck = false;
GetResponse response = channel.basicGet(queueName, autoAck);
if (response == null) {
    // No message retrieved.
} else {
    AMQP.BasicProperties props = response.getProps();
    byte[] body = response.getBody();
    long deliveryTag = response.getEnvelope().getDeliveryTag();
    ...
    ...
    channel.basicAck(method.deliveryTag, false); // acknowledge receipt of the message
}
```

# 顺序消费

如果存在多个消费者，那么就让每个消费者对应一个queue，然后把要发送 的数据全都放到一个queue，这样就能保证所有的数据只到达一个消费者从而保证每个数据到达数据库都是顺序的。

拆分多个queue，每个queue一个consumer，就是多一些queue而已，确实是麻烦点；或者就一个queue但是对应一个consumer，然后这个consumer内部用内存队列做排队，然后分发给底层不同的worker来处理

![](./img/rabbit_mq1.png)

# 重复消费

RabbitMQ 不保证消息不重复，如果你的业务需要保证严格的不重复消息，需要你自己在业务端去重。

**如何保证消息队列消费的幂等性**

业务场景1：从生产者拿到个数据后要写库，先根据主键查一下，如果这个数据有了就别插了直接update

业务场景2：如果是写redis的都没问题，因为每次都是set，redis天然的幂等性

业务场景3：需要让生产者发送每条数据的时候加上一个全局唯一的id,消费的时候先根据id去比如redis查一下判断是否消费过，若没有则处理然后这个id写redis（将<id,message>以K-V形式写入redis）,若消费过就不处理

业务场景4：如果数据库有唯一建约束了，插入只会报错，不会导致数据库出现脏数据，本身幂等了
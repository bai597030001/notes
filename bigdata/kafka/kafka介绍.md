[Kafka 入门介绍-博客链接](<https://lotabout.me/2018/kafka-introduction/>)

[kafka官网](<https://kafka.apache.org/>)

[confluent](<https://www.confluent.io/>)



# 相关概念

`massage`： kafka中最基本的传递对象，有固定格式。 

`topic`：话题，代表 一类消息，如page view，click行为等。

`producer`： 产生信息的主体，可以是服务器日志信息等。

`consumer`： 消费producer产生话题消息的主体。

`Consumer Group`：每个 Consumer 属于一个特定的 Consumer Group（可为每个 Consumer 指定 group name，若不指定 group name 则属于默认的 group）。

`broker`： 消息处理结点，多个broker组成kafka集群。

`partition`： topic的物理分组，每个partition都是一个有序队列。

`segment`： 多个大小相等的段组成了一个partition。

`offset`： 一个连续的用于定位被追加到分区的每一个消息的序列号，最大值为64位的long大小，19位数字字符长度。

- topic、partition、segment、offset的关系：

![](img/kafka1.webp)

- 每个topic可以分为多个partition，一个partition相当于一个大目录，每个partition下面有多个**大小相等**的segment文件，这个segment是由message组成的，而每一个的segment不一定由大小相等的message组成。segment大小及生命周期在server.properties文件中配置。offset用于定位位于段里的唯一消息。

  ```properties
  # 以下配置控制日志段 segments 的处理。策略可以将其设置为在一段时间之后，或在给定大小累积之后删除段。
  # 只要满足*其中*一个条件，段就会被删除。删除总是发生在日志的最后。
  
  # 日志文件的最小删除年龄
  log.retention.hours=168
  
  # 基于日志大小的保留策略。除非剩下的片段被从日志中删除
  # 段位于 log.retention.bytes. 功能独立于 log.retention.hours.
  log.retention.bytes=1073741824
  
  # 日志段文件的最大大小。当达到这个大小时，将创建一个新的日志段。
  log.segment.bytes=1073741824
  
  # The interval at which log segments are checked to see if they can be deleted according to the retention policies
  # 时间间隔： 检查日志segments以查看是否可以根据保留策略删除它们 的 时间间隔
  log.retention.check.interval.ms=300000
  ```

  

- segment
  - segment由index和data文件组成，两个文件成对出现，分别存储索引和数据。
  - segment文件命名规则：对于所有的partition来说，segment名称从0开始，之后的每一个segment名称为上一个segment文件最后一条消息的offset值。

- offset

对于分区中的一个offset例如等于345552怎么去查找相应的message呢？

> 先找到该message所在的segment文件，通过二分查找的方式寻找小于等于345552的offset，假如叫S的segment符合要求，如果S等于345552则S上一个segment的最后一个message即为所求；如果S小于345552则依次遍历当前segment即可找到。

- topic 和 partition

![](img/kafka2.png)

- kafka消息传递语义

```properties
# 有这么几种可能的 delivery guarantee：
At most once 消息可能会丢，但绝不会重复传输
At least one 消息绝不会丢，但可能会重复传输
Exactly once 每条消息肯定会被传输一次且仅传输一次，很多时候这是用户所想要的。
```

- offset更新的方式

```properties
自动提交，设置enable.auto.commit=true，更新的频率根据参数【auto.commit.interval.ms】来定。这种方式也被称为【at most once】，fetch到消息后就可以更新offset，无论是否消费成功。

手动提交，设置enable.auto.commit=false，这种方式称为【at least once】。fetch到消息后，等消费完成再调用方法【consumer.commitSync()】，手动更新offset；如果消费失败，则offset也不会更新，此条消息会被重复消费一次。
```

- 消息分发

  > Kafka 中最基本的数据单元就是消息，而一条消息其实是由 Key + Value 组成的（Key 是可选项，可传空值，Value 也可以传空值）。在发送一条消息时，我们可以指定这个 Key，那么 Producer 会根据 Key 和 partition 机制来判断当前这条消息应该发送并存储到哪个 partition 中（这个就跟分片机制类似）。我们可以根据需要进行扩展 Producer 的 partition 机制（默认算法是 hash 取 %）。

  扩展自己的 partition：

  ```java
  
  package dongguabai.kafka.partition;
   
  import org.apache.kafka.clients.producer.Partitioner;
  import org.apache.kafka.common.Cluster;
  import org.apache.kafka.common.PartitionInfo;
   
  import java.util.List;
  import java.util.Map;
  import java.util.Random;
   
  /**
   * 消息发送后会调用自定义的策略
   *
   * @author Dongguabai
   * @date 2019/1/18 15:40
   */
  public class MyPartitioner implements Partitioner {
   
      @Override
      public int partition(String topic, Object key, byte[] keyBytes, Object value, byte[] valueBytes, Cluster cluster) {
          //获取当前 topic 有多少个分区（分区列表）
          List<PartitionInfo> partitions = cluster.partitionsForTopic(topic);
          int partitionNum = 0;
          if (key == null) { //之前介绍过 Key 是可以传空值的
              partitionNum = new Random().nextInt(partitions.size());   //随机
          } else {
              //取 %
              partitionNum = Math.abs((key.hashCode()) % partitions.size());
          }
          System.out.println("key：" + key + "，value：" + value + "，partitionNum：" + partitionNum);
          //发送到指定分区
          return partitionNum;
      }
   
      @Override
      public void close() {
   
      }
   
      @Override
      public void configure(Map<String, ?> configs) {
   
      }
  }
  ```

  

# 实例讲解

## 创建话题

- 创建一个 test2 test-demo（注意这里的 partitions 参数为 3）：

```shell
# --topic后面的test0是topic的名称
# --zookeeper应该和server.properties文件中的zookeeper.connect一样
# --config指定当前topic上有效的参数值
# --partitions指定topic的partition数量，如果不指定该数量，默认是server.properties文件中的num.partitions配置值
# --replication-factor指定每个partition的副本个数，默认1个

$ kafka-topics --create --partitions 3 --replication-factor 2  --zookeeper spark-master:2181 --topic test-demo

Created topic "test-demo".
```



## 删除话题

```shell
# 移除话题，若移除话题失败需要在Kafka服务端配置中添加设定 
# delete.topic.enble = true
$ kafka-topics --delete --topic [话题名称] --zookeeper [Zookeeper集群IP:端口]

eg:
$ kafka-topics --delete --topic test --zookeeper hadoop-cluster01:2181

#删除zookeeper中该topic相关的目录命令：
$ rm -r /kafka/config/topics/test0
$ rm -r /kafka/brokers/topics/test0
```



## 修改话题

```shell
$ kafka-topics --zookeeper zk_host:port --alter --topic my_topic_name --partitions 3
```

关于修改partition数量，有两点需要注意：

1. partition只能增加，不支持减少
2. 新增的partition只会对将来写入的数据起作用，以前存在的数据不会被移动到新的partition中

除了修改partition数量，我们还可以修改topic层面的配置，增加配置项：

```shell
$ kafka-topics --zookeeper zk_host:port/chroot --alter --topic my_topic_name --config x=y
```

删除配置项：

```shell
$ kafka-topics --zookeeper zk_host:port/chroot --alter --topic my_topic_name --delete-config x
```



## 查看话题

- 查看`test-demo`

```shell
[root@spark-master ~]# kafka-topics --describe  --zookeeper spark-master:2181 | grep test-demo
Topic:test-demo PartitionCount:3        ReplicationFactor:2     Configs:
      Topic: test-demo        Partition: 0    Leader: 1       Replicas: 1,5   Isr: 1,5
      Topic: test-demo        Partition: 1    Leader: 2       Replicas: 2,1   Isr: 2,1
      Topic: test-demo        Partition: 2    Leader: 3       Replicas: 3,2   Isr: 3,2
```



> PartitionCount:3 表示该话题有3个分区（0，1，2）；Leader1，2，3表示3个分区分别为broker1，2，3；Replicas表示该分区的备份broker id



```shell
#查看topic：
$ kafka-topics --list --zookeeper 172.16.0.126:2181
```



- 进入kafka数据目录查看

```shell
# broker 1
[root@spark-master ~]# ls /home/data/kafka/kafkadata/ | grep test-demo
test-demo-0
test-demo-1

# broker 2
[root@spark-slave0 ~]# ls /home/data/kafka/kafkadata/ | grep test-demo
test-demo-1
test-demo-2

# broker 5
[root@spark-slave1 ~]# ls /home/data/kafka/kafkadata/ | grep test-demo
test-demo-0

# broker 3
[root@spark-slave2 ~]# ls /home/data/kafka/kafkadata/ | grep test-demo
test-demo-2

```



## 生产数据

- 生产数据（producer）

```shell
[root@spark-slave0 ~]# kafka-console-producer --broker-list spark-slave1:9092 --topic test-demo
>hello:world
>question:what is your name?
>answer:jams
```



## 消费数据

- 消费数据（consumer）

```shell
#消费消息：
#使用 --from-beginning 参数输出该话题从创建开始后的消息
#使用 --consumer.config 参数指定消费端使用的配置文件
#使用 --offset [偏移量] --partion [分区编号] 参数自定义读取消息时的偏移量
$ kafka-console-consumer --bootstrap-server [listeners IP:端口] --topic [话题名称] [--consumer-property group.id=group_test]

eg:

$ kafka-console-consumer --bootstrap-server spark-slave1:9092 --topic test-demo [--consumer-property group.id=group_test] --from-beginning
question:what is your name?
answer:jams
hello:world
```



- 查看有哪些消费者

```shell
$ kafka-consumer-groups  --bootstrap-server 172.16.0.200:9092 --list
Note: This will not show information about old Zookeeper-based consumers.

console-consumer-56274
group_test
console-consumer-49717
console-consumer-27024
```



- 查看Group详情

```shell
$ kafka-consumer-groups  --bootstrap-server 172.16.0.200:9092 --group group_test  --describe
Note: This will not show information about old Zookeeper-based consumers.

Consumer group 'group_test' has no active members.

TOPIC                          PARTITION  CURRENT-OFFSET  LOG-END-OFFSET  LAG        CONSUMER-ID                                       HOST                           CLIENT-ID
test                           0          2               8               6          -                                                 -                              -
```



- 删除group

```shell
kafka-consumer-groups  --bootstrap-server 172.16.0.200:9092 --group group_test  --delete
```



## 偏移量操作

### 查看Offset

```shell
$ kafka-run-class kafka.tools.GetOffsetShell --broker-list 172.16.0.200:9092 --topic test --time -1

test:0:8
```



```shell
$ kafka-consumer-groups --bootstrap-server spark-slave1:9092 --describe --group group_test
Note: This will not show information about old Zookeeper-based consumers.

Consumer group 'group_test' has no active members.

TOPIC                          PARTITION  CURRENT-OFFSET  LOG-END-OFFSET  LAG        CONSUMER-ID                                       HOST                           CLIENT-ID
test-demo                      2          2               2               0          -                                                 -                              -
test-demo                      1          2               2               0          -                                                 -                              -
test-demo                      0          4               4               0          -                                                 -                              -

# current-offset 当前已经消费到偏移量为2,可以理解为已经消费2条。
# log-end-offset可以理解为总共2条记录。
# lag可以理解为未消费记录条数。
```



### 查看partition状态

```shell
$ bin/kafka-run-class kafka.tools.GetOffsetShell --broker-list spark-slave1:9092 --topic test-demo --time -1

test-demo:2:2
test-demo:1:2
test-demo:0:4
```



### 调整offset

前提是：consumer group状态必须是inactive的，即不能是处于正在工作中的状态。

```shell
# kafka-consumer-groups --bootstrap-server spark-slave1:9092 --reset-offsets --group group_test --topic test-demo --to-offset 20 --execute
Note: This will not show information about old Zookeeper-based consumers.


TOPIC                          PARTITION  NEW-OFFSET
test-demo                      2          20
test-demo                      1          20
test-demo                      0          20
```

讲解：

![](img/kafka6.png)

- 确定topic作用域——当前有3种作用域指定方式：--all-topics（为consumer group下所有topic的所有分区调整位移），--topic t1 --topic t2（为指定的若干个topic的所有分区调整位移），--topic t1:0,1,2（为指定的topic分区调整位移）

- 确定位移重设策略——当前支持8种设置规则：

  - --to-earliest：把位移调整到分区当前最小位移
  - --to-latest：把位移调整到分区当前最新位移
  - --to-current：把位移调整到分区当前位移
  - --to-offset <offset>： 把位移调整到指定位移处
  - --shift-by N： 把位移调整到当前位移 + N处，注意N可以是负数，表示向前移动
  - --to-datetime <datetime>：把位移调整到大于给定时间的最早位移处，datetime格式是yyyy-MM-ddTHH:mm:ss.xxx，比如2017-08-04T00:00:00.000
  - --by-duration <duration>：把位移调整到距离当前时间指定间隔的位移处，duration格式是PnDTnHnMnS，比如PT0H5M0S
  - --from-file <file>：从CSV文件中读取调整策略

- 确定执行方案——当前支持3种方案：

- - 什么参数都不加：只是打印出位移调整方案，不具体执行
  - --execute：执行真正的位移调整
  - --export：把位移调整方案按照CSV格式打印，方便用户成csv文件，供后续直接使用



## 查看数据文件

```shell
# --print-data-log 是表示查看消息内容的，不加此项是查看不到详细的消息内容。如果要查看多个log文件可以用逗号分隔。
$ kafka-run-class kafka.tools.DumpLogSegments --files /home/data/kafka/kafkadata/test-demo-1/00000000000000000000.log --print-data-log
```



## 查看索引文件

```shell
$ kafka-run-class kafka.tools.DumpLogSegments --files /home/data/kafka/kafkadata/test-demo-1/00000000000000000000.index
```



## 扩展集群



# 文件存储机制



## topic存储

- topic以partition的方式存储

- 如何设置partition值
  1. 一个partition只能被一个消费者消费（一个消费者可以同时消费多个partition）因此，如果设置的partition的数量小于consumer的数量，就会有消费者消费不到数据。所以，推荐partition的数量一定要大于同时运行的consumer的数量。
  2. 建议partition的数量大于集群broker的数量，这样leader partition就可以均匀的分布在各个broker中，最终使得集群负载均衡。



## partiton存储

- 每个partion(目录)相当于一个巨型文件被平均分配到多个大小相等segment(段)数据文件中。但每个段segment file消息数量不一定相等，这种特性方便old segment file快速被删除。
- 每个partiton只需要支持顺序读写就行了，segment文件大小和生命周期由**服务端配置参数**决定。

```properties
# 以下配置控制日志段 segments 的处理。策略可以将其设置为在一段时间之后，或在给定大小累积之后删除段。
# 只要满足*其中*一个条件，段就会被删除。删除总是发生在日志的最后。

# 日志文件的最小删除年龄
log.retention.hours=168

# 基于日志大小的保留策略。除非剩下的片段被从日志中删除
# 段位于 log.retention.bytes. 功能独立于 log.retention.hours.
log.retention.bytes=1073741824

# 日志段文件的最大大小。当达到这个大小时，将创建一个新的日志段。
log.segment.bytes=1073741824

# The interval at which log segments are checked to see if they can be deleted according to the retention policies
# 时间间隔： 检查日志segments以查看是否可以根据保留策略删除它们 的 时间间隔
log.retention.check.interval.ms=300000
```



## segment存储

producer发message到某个topic，message会被均匀的分布到多个partition上（随机或根据用户指定的回调函数进行分布），kafka broker收到message后，往对应partition的最后一个segment上添加该消息。当某个segment上的消息条数达到配置值或消息发布时间超过阈值时，segment上的消息会被flush到磁盘，只有flush到磁盘上的消息consumer才能消费，segment达到一定的大小后将不会再往该segment写数据，broker会创建新的segment。



每个partition在内存中对应一个index，记录每个segment中的第一条消息偏移。

- segment file组成：由2大部分组成，分别为index file和data file，此2个文件一一对应，成对出现，后缀".index"和“.log”分别表示为segment索引文件、数据文件.
- segment文件命名规则：partion全局的第一个segment从0开始，后续每个segment文件名为上一个全局partion的最大offset(偏移message数)。数值最大为64位long大小，19位数字字符长度，没有数字用0填充。



每个segment中存储很多条消息，消息id由其逻辑位置决定，即从消息id可直接定位到消息的存储位置，避免id到位置的额外映射。

下面文件列表是笔者在Kafka broker上做的一个实验，创建一个topicXXX包含1 partition，设置每个segment大小为500MB,并启动producer向Kafka broker写入大量数据,如下图2所示segment文件列表形象说明了上述2个规则：

![](img/kafka3.png)

以上述图2中一对segment file文件为例，说明segment中index<—->data file对应关系物理结构如下：

![](img/kafka4.png)

上述图3中索引文件存储大量元数据，数据文件存储大量消息，索引文件中元数据指向对应数据文件中message的物理偏移地址。其中以索引文件中 元数据3,497为例，依次在数据文件中表示第3个message(在全局partiton表示第368772个message)、以及该消息的物理偏移 地址为497。

从上述图3了解到segment data file由许多message组成，下面详细说明message物理结构如下：

![](img/kafka5.png)

**参数说明**：

| 关键字              | 解释说明                                                     |
| :------------------ | :----------------------------------------------------------- |
| 8 byte offset       | 在parition(分区)内的每条消息都有一个有序的id号，这个id号被称为偏移(offset),它可以唯一确定每条消息在parition(分区)内的位置。即**offset表示partiion的第多少message** |
| 4 byte message size | message大小                                                  |
| 4 byte CRC32        | 用crc32校验message                                           |
| 1 byte “magic"      | 表示本次发布Kafka服务程序协议版本号                          |
| 1 byte “attributes" | 表示为独立版本、或标识压缩类型、或编码类型。                 |
| 4 byte key length   | 表示key的长度,当key为-1时，K byte key字段不填                |
| K byte key          | 可选                                                         |
| value bytes payload | 表示实际消息数据。                                           |



## 通过offset查找message

例如读取offset=368776的message，需要通过下面2个步骤查找。

- 第一步查找segment file

  上述图2为例，其中00000000000000000000.index表示最开始的文件，起始偏移量(offset)为0.第二个文件 00000000000000368769.index的消息量起始偏移量为368770 = 368769 + 1.同样，第三个文件00000000000000737337.index的起始偏移量为737338=737337 + 1，其他后续文件依次类推，以起始偏移量命名并排序这些文件，只要根据offset **二分查找**文件列表，就可以快速定位到具体文件。

  当offset=368776时定位到00000000000000368769.index|log

- 第二步通过segment file查找message通过第一步定位到segment file，当offset=368776时，依次定位到00000000000000368769.index的元数据物理位置和 00000000000000368769.log的物理偏移地址，然后再通过00000000000000368769.log顺序查找直到 offset=368776为止。

segment index file采取稀疏索引存储方式，它减少索引文件大小，通过mmap可以直接内存操作，稀疏索引为数据文件的每个对应message设置一个元数据指针,它 比稠密索引节省了更多的存储空间，但查找起来需要消耗更多的时间。



# 复制

## 特性

1）一个partition的复制个数（replication factor）包括这个partition的leader本身。

2）所有对partition的读和写都通过leader。

3）Followers通过pull获取leader上log（message和offset）

4）如果一个follower挂掉、卡住或者同步太慢，leader会把这个follower从”in sync replicas“（ISR）列表中删除。

5）当所有的”in sync replicas“的follower把一个消息写入到自己的log中时，这个消息才被认为是”committed“的。

6）如果针对某个partition的所有复制节点都挂了，Kafka默认选择最先复活的那个节点作为leader（这个节点不一定在ISR里）。



## Leader选举

Kafka在Zookeeper中为每一个partition动态的维护了一个ISR，这个ISR里的所有replica都跟上了leader，只有ISR里的成员才能有被选为leader的可能（unclean.leader.election.enable=false）。

在这种模式下，对于f+1个副本，一个Kafka topic能在保证不丢失已经commit消息的前提下容忍f个副本的失败，在大多数使用场景下，这种模式是十分有利的。事实上，为了容忍f个副本的失败，“少数服从多数”的方式和ISR在commit前需要等待的副本的数量是一样的，但是ISR需要的总的副本的个数几乎是“少数服从多数”的方式的一半。



# 生产者-消费者

## Producers

![](img/kafka5.webp)



```java
public class ProducerRecord<K, V> {
    private final String topic;
    private final Integer partition;
    private final Headers headers;
    private final K key;
    private final V value;
    private final Long timestamp;
}
```

注：ProducerRecord允许用户在创建消息对象的时候就直接指定要发送的分区，这样producer后续发送该消息时可以直接发送到指定分区，而不用先通过Partitioner计算目标分区了。另外，我们还可以直接指定消息的时间戳——但一定要慎重使用这个功能，因为它有可能会令时间戳索引机制失效。



```java
public final class RecordMetadata {
    public static final int UNKNOWN_PARTITION = -1;
    private final long offset;
    private final long timestamp;
    private final int serializedKeySize;
    private final int serializedValueSize;
    private final TopicPartition topicPartition;
    private volatile Long checksum;
}
```



### 消息发送流程

用户首先构建待发送的消息对象ProducerRecord；

然后调用KafkaProducer#send方法进行发送。

KafkaProducer接收到消息后首先对其进行序列化，然后结合本地缓存的元数据信息一起发送给partitioner去确定目标分区，最后追加写入到内存中的消息缓冲池(accumulator)。此时KafkaProducer#send方法成功返回。同时，KafkaProducer中还有一个专门的Sender IO线程负责将缓冲池中的消息分批次发送给对应的broker，完成真正的消息发送逻辑。



 **特点：**

 总共创建两个线程：执行KafkaPrducer#send逻辑的线程——我们称之为“用户主线程”；

?									执行发送逻辑的IO线程——我们称之为“Sender线程”。 

 batch机制——“分批发送“机制。每个批次(batch)中包含了若干个PRODUCE请求，因此具有更高的吞吐量。 



### 消息发送方式

1、发送并忘记( fire-and-forget)：我们把消息发送给服务器，但井不关心它是否正常到达。大多数情况下，消息会正常到达，因为 Kafka是高可用的，而且生产者会自动尝试重发。不过，使用这种方式有时候也会丢失一些消息。

```java
ProducerRecord<String, String> record = new ProducerRecord<String, String>("CustomerCountry", "Precision Products", "France");

try {
  producer.send(record);
} catch (Exception e) {
  e.printStackTrace();
}
```

2、同步发送：我们使用send()方怯发送消息， 它会返回一个Future对象，调用get()方法进行等待， 就可以知道悄息是否发送成功。

```java
ProducerRecord<String, String> record = new ProducerRecord<String, String>("CustomerCountry", "Precision Products", "France");

try {
  producer.send(record).get();
} catch (Exception e) {
  e.printStackTrace();
}
```

3、异步发送：我们调用 send() 方怯，并指定一个回调函数， 服务器在返回响应时调用该函数。

```java
private class DemoProducerCallback implements Callback {
  @Override
  public void onCompletion(RecordMetadata recordMetadata, Exception e) {
    if (e != null) {
	  e.printStackTrace();
	}
  }
}

ProducerRecord<String, String> record = new ProducerRecord<String, String>("CustomerCountry", "Precision Products", "France");

producer.send(record, new DemoProducerCallback());
```



### 消息发送内部流程

 当用户调用`KafkaProducer.send(ProducerRecord, Callback)`时Kafka内部流程分析： 

#### 序列化+计算目标分区

 这是KafkaProducer#send逻辑的第一步，即为待发送消息进行序列化并计算目标分区，如下图所示： 

![](img/kafka6.webp)

如上图所示，一条所属`topic`是"`test`"，消息体是"`message`"的消息被序列化之后结合`KafkaProducer`缓存的元数据(比如该`topic`分区数信息等)共同传给后面的`Partitioner`实现类进行目标分区的计算。

#### 追加写入消息缓冲区(accumulator)

producer创建时会创建一个默认32MB(由buffer.memory参数指定)的accumulator缓冲区，专门保存待发送的消息。除了之前在“关键参数”段落中提到的linger.ms和batch.size等参数之外，该数据结构中还包含了一个特别重要的集合信息：消息批次信息(batches)。该集合本质上是一个HashMap，里面分别保存了每个topic分区下的batch队列，即前面说的批次是按照topic分区进行分组的。这样发往不同分区的消息保存在对应分区下的batch队列中。举个简单的例子，假设消息M1, M2被发送到test的0分区但属于不同的batch，M3分送到test的1分区，那么batches中包含的信息就是：{"test-0" -> [batch1, batch2], "test-1" -> [batch3]}。
 单个topic分区下的batch队列中保存的是若干个消息批次。每个batch中最重要的3个组件包括：
 compressor: 负责执行追加写入操作
 batch缓冲区：由batch.size参数控制，消息被真正追加写入到的地方
 thunks：保存消息回调逻辑的集合
 这一步的目的就是将待发送的消息写入消息缓冲池中，具体流程如下图所示：

![](img/kafka7.webp)

 这一步执行完毕之后理论上讲KafkaProducer.send方法就执行完毕了，用户主线程所做的事情就是等待Sender线程发送消息并执行返回结果了。 

#### Sender线程预处理及消息发送

此时，该Sender线程登场了。严格来说，Sender线程自KafkaProducer创建后就一直都在运行着 。它的工作流程基本上是这样的：

- 不断轮询缓冲区寻找**已做好发送准备的分区** ；
- 将轮询获得的各个batch按照目标分区所在的leader broker进行分组；
- 将分组后的batch通过底层创建的**Socket连接**发送给各个broker；
- 等待服务器端发送response回来。

为了说明上的方便，还是基于图的方式来解释Sender线程的工作原理：

![](img/kafka8.webp)

####  

#### Sender线程处理response

上图中Sender线程会发送PRODUCE请求给对应的broker，broker处理完毕之后发送对应的PRODUCE response。一旦Sender线程接收到response将依次(按照消息发送顺序)调用batch中的回调方法，如下图所示：

![](img/kafka9.webp)

做完这一步，producer发送消息就可以算作是100%完成了。通过这4步我们可以看到新版本producer发送事件完全是异步过程。**因此在调优producer前我们就需要搞清楚性能瓶颈到底是在用户主线程还是在Sender线程**。
 由于KafkaProducer是线程安全的，因此在使用上有两种基本的使用方法：



### 相关参数

```properties
# producer采用分批发送机制，该参数即控制一个batch的大小。默认是16KB
batch.size

# 指定生产者在发送批量消息前等待的时间，当设置此参数后，即便没有达到批量消息的指定大小，
# 到达时间后生产者也会发送批量消息到broker。默认情况下，生产者的发送消息线程只要空闲了就
# 会发送消息，即便只有一条消息。设置这个参数后，发送线程会等待一定的时间，这样可以批量发送
# 消息增加吞吐量，但同时也会增加延迟。
linger.ms

# acks控制多少个副本必须写入消息后生产者才能认为写入成功，这个参数对消息丢失可能性有很大影响。这个参数有三种取值：

# 	acks=0：生产者把消息发送到broker即认为成功，不等待broker的处理结果。这种方式的吞吐最高，但也是最容易丢失消息的。
#
# 	acks=1：生产者会在该分区的群首（leader）写入消息并返回成功后，认为消息发送成功。
#	如果群首写入消息失败，生产者会收到错误响应并进行重试。这种方式能够一定程度避免消息丢失，
#	但如果群首宕机时该消息没有复制到其他副本，那么该消息还是会丢失。另外，如果我们使用同步方式来发送，
#	延迟会比前一种方式大大增加（至少增加一个网络往返时间）；如果使用异步方式，应用感知不到延迟，
#	吞吐量则会受异步正在发送中的数量限制。
# 	
#	acks=all：生产者会等待所有副本成功写入该消息，这种方式是最安全的，能够保证消息不丢失，但是延迟也是最大的。
request.required.acks = 0, 1, n, -1/all

# 设置生产者缓冲发送的消息的内存大小，如果应用调用send方法的速度大于生产者发送的速度，
# 那么调用会阻塞或者抛出异常，具体行为取决于block.on.buffer.full（这个参数在0.9.0.0版本
# 被max.block.ms代替，允许抛出异常前等待一定时间）参数。
buffer.memory

# 指定使用消息压缩，参数可以取值为snappy、gzip或者lz4
# 	snappy压缩算法由Google研发，这种算法在性能和压缩比取得比较好的平衡；
# 	相比之下，gzip消耗更多的CPU资源，但是压缩效果也是最好的。通过使用压缩，我们可以节省网络带宽和Kafka存储成本。
compresstion.type

# 当生产者发送消息收到一个可恢复异常时，会进行重试，这个参数指定了重试的次数。
#	在实际情况中，这个参数需要结合retry.backoff.ms（重试等待间隔）来使用，
#	建议总的重试时间比集群重新选举群首的时间长，这样可以避免生产者过早结束重试导致失败。
retries

# 这个参数可以是任意字符串，它是broker用来识别消息是来自哪个客户端的。在broker进行打印日志、衡量指标或者配额限制时会用到。
client.id

# 这个参数指定生产者可以发送多少消息到broker并且等待响应.
# 设置此参数较高的值可以提高吞吐量，但同时也会增加内存消耗。另外，如果设置过高反而会降低吞吐量，
# 因为批量消息效率降低。设置为1，可以保证发送到broker的顺序和调用send方法顺序一致，
# 即便出现失败重试的情况也是如此。
max.in.flight.requests.per.connection

# 这些参数控制生产者等待broker的响应时间。
#	request.timeout.ms指定发送数据的等待响应时间，
#	metadata.fetch.timeout.ms指定获取元数据（例如获取分区的群首信息）的等待响应时间。
#	timeout.ms则指定broker在返回结果前等待其他副本（与acks参数相关）响应的时间，如果时间到了但其他副本没有响应结果，则返回消息写入失败。
timeout.ms
request.timeout.ms
metadata.fetch.timeout.ms

#  指定应用调用send方法或者获取元数据方法（例如partitionFor）时的阻塞时间，超过此时间则抛出timeout异常。
max.block.ms

# 限制生产者发送数据包的大小，数据包的大小与消息的大小、消息数相关。如果我们指定了最大数据包大小为1M，
# 那么最大的消息大小为1M，或者能够最多批量发送1000条消息大小为1K的消息。
# 另外，broker也有message.max.bytes参数来控制接收的数据包大小。
# 在实际中，建议这些参数值是匹配的，避免生产者发送了超过broker限定的数据大小。
max.request.size

# 这两个参数设置用来发送/接收数据的TCP连接的缓冲区，如果设置为-1则使用操作系统自身的默认值。
# 如果生产者与broker在不同的数据中心，建议提高这个值，因为不同数据中心往往延迟比较大。
receive.buffer.bytes
send.buffer.bytes
```



### 保证分区的顺序

 Kafka保证分区的顺序，也就是说，如果生产者以一定的顺序发送消息到Kafka的某个分区，那么Kafka在分区内部保持此顺序，而且消费者也按照同样的顺序消费。但是，应用调用send方法的顺序和实际发送消息的顺序不一定是一致的。举个例子，如果retries参数不为0，而max.in.flights.requests.per.session参数大于1，那么有可能第一个批量消息写入失败，但是第二个批量消息写入成功，然后第一个批量消息重试写入成功，那么这个顺序乱序的。因此，如果需要保证消息顺序，建议设置max.in.flights.requests.per.session为1，这样可以在第一个批量消息发送失败重试时，第二个批量消息需要等待。 



### 序列化

 [http://www.dengshenyu.com/%E5%88%86%E5%B8%83%E5%BC%8F%E7%B3%BB%E7%BB%9F/2017/11/12/kafka-producer.html](http://www.dengshenyu.com/分布式系统/2017/11/12/kafka-producer.html)



## Consumers

- Kafka提供了两套consumer api，分为`high-level api`和`sample-api`。

  - **Sample-api** 是一个底层的API，通过直接操作底层API获取数据的方式获取Kafka中的数据，需要自行给定分区、偏移量等属性。优点：可操作性强；缺点：代码相对而言比较复杂。(入口类：SimpleConsumer) 
  - **High Level Consumer API**：高度抽象的Kafka消费者API；将底层具体获取数据、更新offset、设置偏移量等操作屏蔽掉，直接将操作数据流的处理工作提供给编写程序的人员。优点是：操作简单；缺点：可操作性太差，无法按照自己的业务场景选择处理方式。(入口类：ConsumerConnector)

- **在kafka中，当前读到哪条消息的offset值是由consumer来维护的**，因此，consumer可以自己决定如何读取kafka中的数据。比如，consumer可以通过重设offset值来重新消费已消费过的数据。不管有没有被消费，kafka会保存数据一段时间，这个时间周期是**可配置**的，只有到了过期时间，kafka才会删除这些数据。

- High-level API封装了对集群中一系列broker的访问，可以透明的消费一个topic。它自己维持了已消费消息的状态，即每次消费的都是下一个消息。

  High-level API还支持以组的形式消费topic，如果consumers有同一个组名，那么kafka就相当于一个队列消息服务，而各个consumer均衡的消费相应partition中的数据。若consumers有不同的组名，那么此时kafka就相当与一个广播服务，会把topic中的所有消息广播到每个consumer。

  

  High level api和Low level api是针对consumer而言的，和producer无关。

  

  High level api是consumer读的partition的offsite是存在zookeeper上。High level api 会启动另外一个线程去每隔一段时间，offsite自动同步到zookeeper上。换句话说，如果使用了High level api， 每个message只能被读一次，一旦读了这条message之后，无论我consumer的处理是否ok。High level api的另外一个线程会自动的把offiste+1同步到zookeeper上。如果consumer读取数据出了问题，offsite也会在zookeeper上同步。因此，如果consumer处理失败了，会继续执行下一条。这往往是不对的行为。因此，Best Practice是一旦consumer处理失败，直接让整个conusmer group抛Exception终止，但是最后读的这一条数据是丢失了，因为在zookeeper里面的offsite已经+1了。等再次启动conusmer group的时候，已经从下一条开始读取处理了。

  

  Low level api是consumer读的partition的offsite在consumer自己的程序中维护。不会同步到zookeeper上。但是为了kafka manager能够方便的监控，一般也会手动的同步到zookeeper上。这样的好处是一旦读取某个message的consumer失败了，这条message的offsite我们自己维护，我们不会+1。下次再启动的时候，还会从这个offsite开始读。这样可以做到exactly once对于数据的准确性有保证。

- Consumer与Partition的关系（ **Consumer Rebalance** ）：
  - 如果consumer比partition多，是浪费，因为kafka的设计是在一个partition上是不允许并发的，所以consumer数不要大于partition数
  - 如果consumer比partition少，一个consumer会对应于多个partitions，这里主要合理分配consumer数和partition数，否则会导致partition里面的数据被取的不均匀
  - 如果consumer从多个partition读到数据，不保证数据间的顺序性，kafka只保证在一个partition上数据是有序的，但多个partition，根据你读的顺序会有不同
  - 增减consumer，broker，partition会导致rebalance，所以rebalance后consumer对应的partition会发生变化
  - High-level接口中获取不到数据的时候是会block的



# 消息投递语义

kafka支持3种消息投递语义
	

- At most once：最多一次，消息可能会丢失，但不会重复

  > \>1. 设置‘enable.auto.commit’ 为 true.
  >
  > \>2. 设置 ‘auto.commit.interval.ms’ 为一个较小的值.

- At least once：最少一次，消息不会丢失，可能会重复

  > 1. 设置‘enable.auto.commit’ 为 false 或者
  >
  > 设置‘enable.auto.commit’ 为 true 并设置‘auto.commit.interval.ms’ 为一个较大的值.

- Exactly once：只且一次，消息不丢失不重复，只且消费一次



# 配置文件

```properties
############################# Server Basics #############################

# The id of the broker. This must be set to a unique integer for each broker.
broker.id=1

############################# Socket Server Settings #############################

# The address the socket server listens on. It will get the value returned from 
# java.net.InetAddress.getCanonicalHostName() if not configured.
#   FORMAT:
#     listeners = listener_name://host_name:port
#   EXAMPLE:
#     listeners = PLAINTEXT://your.host.name:9092
listeners = PLAINTEXT://hadoop-cluster01:9092 

# Hostname and port the broker will advertise to producers and consumers. If not set, 
# it uses the value for "listeners" if configured.  Otherwise, it will use the value
# returned from java.net.InetAddress.getCanonicalHostName().
#advertised.listeners=PLAINTEXT://your.host.name:9092

# Maps listener names to security protocols, the default is for them to be the same. See the config documentation for more details
#listener.security.protocol.map=PLAINTEXT:PLAINTEXT,SSL:SSL,SASL_PLAINTEXT:SASL_PLAINTEXT,SASL_SSL:SASL_SSL

# The number of threads that the server uses for receiving requests from the network and sending responses to the network
num.network.threads=3

# The number of threads that the server uses for processing requests, which may include disk I/O
num.io.threads=8

# The send buffer (SO_SNDBUF) used by the socket server
socket.send.buffer.bytes=102400

# The receive buffer (SO_RCVBUF) used by the socket server
socket.receive.buffer.bytes=102400

# The maximum size of a request that the socket server will accept (protection against OOM)
socket.request.max.bytes=104857600


############################# Log Basics #############################

# A comma seperated list of directories under which to store log files
log.dirs=/home/data/kafka/kafkalog

# The default number of log partitions per topic. More partitions allow greater
# parallelism for consumption, but this will also result in more files across
# the brokers.
num.partitions=1

# The number of threads per data directory to be used for log recovery at startup and flushing at shutdown.
# This value is recommended to be increased for installations with data dirs located in RAID array.
num.recovery.threads.per.data.dir=1

############################# Internal Topic Settings  #############################
# The replication factor for the group metadata internal topics "__consumer_offsets" and "__transaction_state"
# For anything other than development testing, a value greater than 1 is recommended for to ensure availability such as 3.
offsets.topic.replication.factor=3
transaction.state.log.replication.factor=3
transaction.state.log.min.isr=3

############################# Log Flush Policy #############################

# 消息被立即写入文件系统，但是在默认情况下，我们只同步fsync()
# 操作系统缓存是惰性的。以下配置控制数据到磁盘的刷新。
# 这里有一些重要的权衡:
# 	1。持久性:如果不使用复制，未刷新的数据可能会丢失。
# 	2。延迟:非常大的刷新间隔可能会导致延迟峰值，因为会有大量数据需要刷新。
# 	3。吞吐量:刷新通常是最昂贵的操作，一个小的刷新间隔可能导致过度的查找。

# 下面的设置允许您配置刷新策略，以便在一段时间或之后刷新数据

#每N条消息(或两者都有)。这可以在全局范围内完成，并在每个主题的基础上覆盖。
# every N messages (or both). This can be done globally and overridden on a per-topic basis.

# The number of messages to accept before forcing a flush of data to disk
# 在将数据刷新到磁盘之前可以接受的消息条数
log.flush.interval.messages=10000

# The maximum amount of time a message can sit in a log before we force a flush
# 在强制刷新之前，消息可以驻留在日志中的最长时间
log.flush.interval.ms=1000

############################# Log Retention Policy #############################

# 以下配置控制日志段 segments 的处理。策略可以将其设置为在一段时间之后，或在给定大小累积之后删除段。
# 只要满足*其中*一个条件，段就会被删除。删除总是发生在日志的最后。

# 日志文件的最小删除年龄
log.retention.hours=168

# 基于日志大小的保留策略。除非剩下的片段被从日志中删除
# 段位于 log.retention.bytes. 功能独立于 log.retention.hours.
log.retention.bytes=1073741824

# 日志段文件的最大大小。当达到这个大小时，将创建一个新的日志段。
log.segment.bytes=1073741824

# The interval at which log segments are checked to see if they can be deleted according to the retention policies
# 时间间隔： 检查日志segments以查看是否可以根据保留策略删除它们 的 时间间隔
log.retention.check.interval.ms=300000

############################# Zookeeper #############################

# Zookeeper connection string (see zookeeper docs for details).
# This is a comma separated host:port pairs, each corresponding to a zk
# server. e.g. "127.0.0.1:3000,127.0.0.1:3001,127.0.0.1:3002".
# You can also append an optional chroot string to the urls to specify the
# root directory for all kafka znodes.
# zookeeper.connect=localhost:2181
zookeeper.connect=hadoop-cluster01:2181,hadoop-cluster02:2181,hadoop-cluster03:2181

# Timeout in ms for connecting to zookeeper
zookeeper.connection.timeout.ms=6000

##################### Confluent Metrics Reporter #######################
# Confluent Control Center and Confluent Auto Data Balancer integration
#
# Uncomment the following lines to publish monitoring data for
# Confluent Control Center and Confluent Auto Data Balancer
# If you are using a dedicated metrics cluster, also adjust the settings
# to point to your metrics kakfa cluster.
#metric.reporters=io.confluent.metrics.reporter.ConfluentMetricsReporter
#confluent.metrics.reporter.bootstrap.servers=localhost:9092
#
# Uncomment the following line if the metrics cluster has a single broker
#confluent.metrics.reporter.topic.replicas=1

##################### Confluent Proactive Support ###################### 
# If set to true, and confluent-support-metrics package is installed
# then the feature to collect and report support metrics
# ("Metrics") is enabled.  If set to false, the feature is disabled.
#
confluent.support.metrics.enable=true

############################# Group Coordinator Settings #############################

# The following configuration specifies the time, in milliseconds, that the GroupCoordinator will delay the initial consumer rebalance.
# The rebalance will be further delayed by the value of group.initial.rebalance.delay.ms as new members join the group, up to a maximum of max.poll.interval.ms.
# The default value for this is 3 seconds.
# We override this to 0 here as it makes for a better out-of-the-box experience for development and testing.
# However, in production environments the default value of 3 seconds is more suitable as this will help to avoid unnecessary, and potentially expensive, rebalances during application startup.
group.initial.rebalance.delay.ms=0


# The customer ID under which support metrics will be collected and
# reported.
#
# When the customer ID is set to "anonymous" (the default), then only a
# reduced set of metrics is being collected and reported.
#
# Confluent customers
# -------------------
# If you are a Confluent customer, then you should replace the default
# value with your actual Confluent customer ID.  Doing so will ensure
# that additional support metrics will be collected and reported.
#
confluent.support.customer.id=anonymous
```



# kafka速度快



## 顺序读写

Kafka 的消息是不断追加到文件中的，这个特性使它可以充分利用磁盘的顺序读写能力。



顺序读写降低了硬盘磁头的寻道时间，只需要很少的扇区旋转时间，所以速度远快于随机读写。Kafka 官方给出的测试数据(Raid-5, 7200rpm)

顺序I/O: 600MB/s

随机I/O: 100KB/s



## mmap（写入数据）

 即便是顺序写入硬盘，硬盘的访问速度还是不可能追上内存。所以Kafka的数据并不是实时的写入硬盘 ，它充分利用了现代操作系统分页存储来利用内存提高I/O效率。 

Memory Mapped Files(后面简称mmap)也被翻译成 内存映射文件 ，在64位操作系统中一般可以表示20G的数据文件，**它的工作原理是直接利用操作系统的Page来实现文件到物理内存的直接映射。**



完成映射之后你对物理内存的操作会被同步到硬盘上（操作系统在适当的时候）。



通过mmap，进程像读写硬盘一样读写内存（当然是虚拟机内存），也不必关心内存的大小有虚拟内存为我们兜底。



使用这种方式可以获取很大的I/O提升，省去了用户空间到内核空间复制的开销（调用文件的read会把数据先放到内核空间的内存中，然后再复制到用户空间的内存中。）



但也有一个很明显的缺陷——不可靠，写到mmap中的数据并没有被真正的写到硬盘，操作系统会在程序主动调用flush的时候才把数据真正的写到硬盘。



Kafka提供了一个参数——producer.type来控制是不是主动flush，如果Kafka写入到mmap之后就立即flush然后再返回Producer叫 同步 (sync)；写入mmap之后立即返回Producer不调用flush叫异步 (async)。



## 零拷贝（读取数据）

### 传统文件IO

 传统模式下，文件传输的操作流程，例如一个程序要把文件内容发送到网络。这个过程发生在用户空间，文件和网络socket属于硬件资源，两者之间有一个内核空间，在操作系统内部，整个过程为:



> 1、基于sendfile实现Zero Copy调用read函数，文件数据被copy到内核缓冲区
>
> 2、read函数返回，文件数据从内核缓冲区copy到用户缓冲区
>
> 3、write函数调用，将文件数据从用户缓冲区copy到内核与socket相关的缓冲区。
>
> 4、数据从socket缓冲区copy到相关协议引擎。



![](img/kafka2.webp)

以上细节是传统read/write方式进行网络文件传输的方式，我们可以看到，在这个过程当中，文件数据实际上是经过了四次copy操作：

> 硬盘—>内核buf—>用户buf—>socket相关缓冲区—>协议引擎



### sendfile

而kafka所用的“**零拷贝(zero-copy)**”系统调用机制，就是跳过“用户缓冲区”的拷贝，建立一个磁盘空间和内存空间的直接映射，数据不再复制到“用户态缓冲区”系统上下文切换减少2次，可以提升一倍性能

![](img/kafka3.webp)

 sendfile系统调用，以简化网络上和两个本地文件之间的数据传输。sendfile的引入不仅减少了数据复制，还减少了上下文切换。 

```c++
sendfile(socket, file, len);
```

运行流程如下：

> 1、sendfile系统调用，文件数据被copy至内核缓冲区
>
> 2、再从内核缓冲区copy至内核中socket相关的缓冲区
>
> 3、最后再socket相关的缓冲区copy到协议引擎



## 文件分段

 Kafka 的队列 topic 被分为了多个区 partition, 每个 partition 又分为了多个 segment，所以一个队列中的消息实际上是保存在 N 多个片段文件中。 

![](img/kafka4.webp)

 通过分段的方式，每次文件操作都是对一个小文件的操作，非常轻便，同时也增加了并行处理能力。 



## 批量发送 batch



## 数据压缩



## 页缓存
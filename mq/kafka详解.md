[kafka����](<https://kafka.apache.org/>)

[confluent](<https://www.confluent.io/>)



[Toc]



# ��ظ���

- `broker`�� ��Ϣ�����㣬���broker���kafka��Ⱥ��

- `topic`�����⣬���� һ����Ϣ����page view��click��Ϊ�ȡ�

- `partition`�� topic��������飬ÿ��partition����һ��������С�

- `segment`�� �����С��ȵĶ������һ��partition��

- `massage`�� kafka��������Ĵ��ݶ����й̶���ʽ�� 

- `offset`�� һ�����������ڶ�λ��׷�ӵ�������ÿһ����Ϣ�����кţ����ֵΪ64λ��long��С��19λ�����ַ����ȡ�

- `producer`�� ������Ϣ�����壬�����Ƿ�������־��Ϣ�ȡ�

- `consumer`�� ����producer����������Ϣ�����塣

- `Consumer Group`��ÿ�� Consumer ����һ���ض��� Consumer Group����Ϊÿ�� Consumer ָ�� group name������ָ�� group name ������Ĭ�ϵ� group����



## topic��partition��segment��offset�Ĺ�ϵ



![](img/kafka1.webp)

- ÿ��`topic`���Է�Ϊ���`partition`��һ��`partition`�൱��һ����Ŀ¼��ÿ��`partition`�����ж��**��С���**��`segment`�ļ������`segment`����`message`��ɵģ���ÿһ����`segment`��һ���ɴ�С��ȵ�`message`��ɡ�`segment`��С������������`server.properties`�ļ������á�`offset`���ڶ�λλ�ڶ����Ψһ��Ϣ��

  ```properties
  # �������ÿ�����־�� segments �Ĵ������Կ��Խ�������Ϊ��һ��ʱ��֮�󣬻��ڸ�����С�ۻ�֮��ɾ���Ρ�
  # ֻҪ����*����*һ���������ξͻᱻɾ����ɾ�����Ƿ�������־�����
  
  # ��־�ļ�����Сɾ������
  log.retention.hours=168
  
  # ������־��С�ı������ԡ�����ʣ�µ�Ƭ�α�����־��ɾ��
  # ��λ�� log.retention.bytes. ���ܶ����� log.retention.hours.
  log.retention.bytes=1073741824
  
  # ��־���ļ�������С�����ﵽ�����Сʱ��������һ���µ���־�Ρ�
  log.segment.bytes=1073741824
  
  # The interval at which log segments are checked to see if they can be deleted according to the retention policies
  # ʱ������ �����־segments�Բ鿴�Ƿ���Ը��ݱ�������ɾ������ �� ʱ����
  log.retention.check.interval.ms=300000
  ```

  

- `segment`
  
  - `segment`��`index`��`data`�ļ���ɣ������ļ��ɶԳ��֣��ֱ�洢���������ݡ�
  - `segment`�ļ��������򣺶������е�`partition`��˵��`segment`���ƴ�0��ʼ��֮���ÿһ��`segment`����Ϊ��һ��`segment`�ļ����һ����Ϣ��`offset`ֵ��
  
- `offset`

  ���ڷ����е�һ��`offset`�������345552��ôȥ������Ӧ��`message`�أ�

  > ���ҵ���message���ڵ�segment�ļ���ͨ�����ֲ��ҵķ�ʽѰ��С�ڵ���345552��offset�������S��segment����Ҫ�����S����345552��S��һ��segment�����һ��message��Ϊ�������SС��345552�����α�����ǰsegment�����ҵ���

- `topic` �� `partition`

![](img/kafka2.png)



## ΪʲôҪ��topic�����partitions�ĸ���

topic���߼��ĸ��partition������ĸ�����û���˵��͸���ġ�producerֻ��Ҫ������Ϣ�����ĸ�topic����consumerֻ�����Լ������ĸ�topic����������ÿ����Ϣ����������Ⱥ���ĸ�broker��

Ϊ�����ܿ��ǣ����topic�ڵ���Ϣֻ����һ��broker�������broker���Ϊƿ�����޷�����<font color=#dd0000>ˮƽ��չ</font>�����԰�topic�ڵ����ݷֲ���������Ⱥ����һ����Ȼ��Ȼ����Ʒ�ʽ��Partition��������ǽ��ˮƽ��չ�����һ��������



- kafka��Ϣ��������

```properties
# ����ô���ֿ��ܵ� delivery guarantee��
At most once ��Ϣ���ܻᶪ�����������ظ�����
At least one ��Ϣ�����ᶪ�������ܻ��ظ�����
Exactly once ÿ����Ϣ�϶��ᱻ����һ���ҽ�����һ�Σ��ܶ�ʱ�������û�����Ҫ�ġ�
```

- offset���µķ�ʽ

```properties
�Զ��ύ������enable.auto.commit=true
# ���µ�Ƶ�ʸ��ݲ�����auto.commit.interval.ms�����������ַ�ʽҲ����Ϊ��at most once����fetch ����Ϣ��Ϳ��Ը���offset�������Ƿ����ѳɹ���

�ֶ��ύ������enable.auto.commit=false
# ���ַ�ʽ��Ϊ��at least once����fetch����Ϣ�󣬵���������ٵ��÷�����consumer.commitSync()�����ֶ�����offset���������ʧ�ܣ���offsetҲ������£�������Ϣ�ᱻ�ظ�����һ�Ρ�
```



## kafka��Ϣ�ַ�

Ĭ�ϵķ��������ǣ�

- ����ڷ���Ϣ��ʱ��ָ���˷���������ϢͶ�ݵ�ָ���ķ���
- ���û��ָ��������������Ϣ��key��Ϊ�գ������key�Ĺ�ϣֵ��ѡ��һ������
- �����û��ָ������������Ϣ��keyҲ�ǿգ�������ѯ�ķ�ʽѡ��һ������

> Kafka ������������ݵ�Ԫ������Ϣ����һ����Ϣ��ʵ���� Key + Value ��ɵģ�Key �ǿ�ѡ��ɴ���ֵ��Value Ҳ���Դ���ֵ�����ڷ���һ����Ϣʱ�����ǿ���ָ����� Key����ô Producer ����� Key �� partition �������жϵ�ǰ������ϢӦ�÷��Ͳ��洢���ĸ� partition �У�����͸���Ƭ�������ƣ������ǿ��Ը�����Ҫ������չ Producer �� partition ���ƣ�Ĭ���㷨�� hash ȡ %����



��չ�Լ��� partition��

```java
import org.apache.kafka.clients.producer.Partitioner;
import org.apache.kafka.common.Cluster;
import org.apache.kafka.common.PartitionInfo;
 
import java.util.List;
import java.util.Map;
import java.util.Random;
 
/**
 * ��Ϣ���ͺ������Զ���Ĳ���
 *
 * @author Dongguabai
 * @date 2019/1/18 15:40
 */
public class MyPartitioner implements Partitioner {
 
    @Override
    public int partition(String topic, Object key, byte[] keyBytes, Object value, byte[] valueBytes, Cluster cluster) {
        //��ȡ��ǰ topic �ж��ٸ������������б�
        List<PartitionInfo> partitions = cluster.partitionsForTopic(topic);
        int partitionNum = 0;
        if (key == null) { //֮ǰ���ܹ� Key �ǿ��Դ���ֵ��
            partitionNum = new Random().nextInt(partitions.size());   //���
        } else {
            //ȡ %
            partitionNum = Math.abs((key.hashCode()) % partitions.size());
        }
        System.out.println("key��" + key + "��value��" + value + "��partitionNum��" + partitionNum);
        //���͵�ָ������
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



# ʵ������

## ��������

- ����һ�� test2 test-demo��ע������� partitions ����Ϊ 3����

```shell
# --topic�����test0��topic������
# --zookeeperӦ�ú�server.properties�ļ��е�zookeeper.connectһ��
# --configָ����ǰtopic����Ч�Ĳ���ֵ
# --partitionsָ��topic��partition�����������ָ����������Ĭ����server.properties�ļ��е�num.partitions����ֵ
# --replication-factorָ��ÿ��partition�ĸ���������Ĭ��1��

$ kafka-topics --create --partitions 3 --replication-factor 2  --zookeeper spark-master:2181 --topic test-demo

Created topic "test-demo".
```



## ɾ������

```shell
# �Ƴ����⣬���Ƴ�����ʧ����Ҫ��Kafka���������������趨 
# delete.topic.enble = true
$ kafka-topics --delete --topic [��������] --zookeeper [Zookeeper��ȺIP:�˿�]

# eg:
$ kafka-topics --delete --topic test-demo --zookeeper hadoop-cluster01:2181

#ɾ��zookeeper�и�topic��ص�Ŀ¼���
$ rm -r /kafka/config/topics/test-demo0
$ rm -r /kafka/brokers/topics/test-demo0
```



## �޸Ļ���

```shell
$ kafka-topics --zookeeper zk_host:port --alter --topic my_topic_name --partitions 3
```

�����޸�partition��������������Ҫע�⣺

1. partitionֻ�����ӣ���֧�ּ���
2. ������partitionֻ��Խ���д������������ã���ǰ���ڵ����ݲ��ᱻ�ƶ����µ�partition��

�����޸�partition���������ǻ������޸�topic��������ã����������

```shell
$ kafka-topics --zookeeper zk_host:port/chroot --alter --topic my_topic_name --config x=y
```

ɾ�������

```shell
$ kafka-topics --zookeeper zk_host:port/chroot --alter --topic my_topic_name --delete-config x
```



## �鿴����

- �鿴`test-demo`

```shell
[root@spark-master ~]# kafka-topics --describe  --zookeeper spark-master:2181 | grep test-demo
Topic:test-demo PartitionCount:3        ReplicationFactor:2     Configs:
      Topic: test-demo        Partition: 0    Leader: 1       Replicas: 1,3   Isr: 1,3
      Topic: test-demo        Partition: 1    Leader: 2       Replicas: 2,1   Isr: 2,1
      Topic: test-demo        Partition: 2    Leader: 3       Replicas: 3,2   Isr: 3,2
```

> PartitionCount:3 ��ʾ�û�����3��������0��1��2����
>
> Leader1��2��3��ʾ3�������ֱ�Ϊbroker1��2��3��
>
> Replicas��ʾ�÷����ı���broker id



```shell
#�鿴topic��
$ kafka-topics --list --zookeeper 172.16.0.126:2181
```



- ����kafka����Ŀ¼�鿴

```shell
# broker 1
[root@spark-master ~]# ls /home/data/kafka/kafkadata/ | grep test-demo
test-demo-0
test-demo-1

# broker 2
[root@spark-slave0 ~]# ls /home/data/kafka/kafkadata/ | grep test-demo
test-demo-1
test-demo-2

# broker 3
[root@spark-slave2 ~]# ls /home/data/kafka/kafkadata/ | grep test-demo
test-demo-0
test-demo-2

```



## ��������

- �������ݣ�producer��

```shell
[root@spark-slave0 ~]# kafka-console-producer --broker-list spark-slave1:9092 --topic test-demo
>hello:world
>question:what is your name?
>answer:jams
```



## ��������

- �������ݣ�consumer��

```shell
#������Ϣ��
#ʹ�� --from-beginning ��������û���Ӵ�����ʼ�����Ϣ
#ʹ�� --consumer.config ����ָ�����Ѷ�ʹ�õ������ļ�
#ʹ�� --offset [ƫ����] --partion [�������] �����Զ����ȡ��Ϣʱ��ƫ����
$ kafka-console-consumer --bootstrap-server [listeners IP:�˿�] --topic [��������] [--consumer-property group.id=group_test]

# eg:

$ kafka-console-consumer --bootstrap-server spark-slave1:9092 --topic test-demo [--consumer-property group.id=group_test] --from-beginning
question:what is your name?
answer:jams
hello:world
```



- �鿴����Щ������

```shell
$ kafka-consumer-groups  --bootstrap-server 172.16.0.200:9092 --list
Note: This will not show information about old Zookeeper-based consumers.

console-consumer-56274
group_test
console-consumer-49717
console-consumer-27024
```



- �鿴Group����

```shell
$ kafka-consumer-groups  --bootstrap-server 172.16.0.200:9092 --group group_test  --describe
Note: This will not show information about old Zookeeper-based consumers.

Consumer group 'group_test' has no active members.

TOPIC                          PARTITION  CURRENT-OFFSET  LOG-END-OFFSET  LAG        CONSUMER-ID                                       HOST                           CLIENT-ID
test                           0          2               8               6          -                                                 -                              -
```



- ɾ��group

```shell
$ kafka-consumer-groups  --bootstrap-server 172.16.0.200:9092 --group group_test  --delete
```



## ƫ��������

### �鿴Offset

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

# current-offset ��ǰ�Ѿ����ѵ�ƫ����Ϊ2,�������Ϊ�Ѿ�����2����
# log-end-offset�������Ϊ�ܹ�2����¼��
# lag�������Ϊδ���Ѽ�¼������
```



### �鿴partition״̬

```shell
$ bin/kafka-run-class kafka.tools.GetOffsetShell --broker-list spark-slave1:9092 --topic test-demo --time -1

test-demo:2:2
test-demo:1:2
test-demo:0:4
```



### ����offset

ǰ���ǣ�consumer group״̬������inactive�ģ��������Ǵ������ڹ����е�״̬��

```shell
# kafka-consumer-groups --bootstrap-server spark-slave1:9092 --reset-offsets --group group_test --topic test-demo --to-offset 20 --execute
Note: This will not show information about old Zookeeper-based consumers.


TOPIC                          PARTITION  NEW-OFFSET
test-demo                      2          20
test-demo                      1          20
test-demo                      0          20
```

���⣺

![](img/kafka6.png)

- ȷ��topic�����򡪡���ǰ��3��������ָ����ʽ��--all-topics��Ϊconsumer group������topic�����з�������λ�ƣ���--topic t1 --topic t2��Ϊָ�������ɸ�topic�����з�������λ�ƣ���--topic t1:0,1,2��Ϊָ����topic��������λ�ƣ�

- ȷ��λ��������ԡ�����ǰ֧��8�����ù���

  - --to-earliest����λ�Ƶ�����������ǰ��Сλ��
  - --to-latest����λ�Ƶ�����������ǰ����λ��
  - --to-current����λ�Ƶ�����������ǰλ��
  - --to-offset <offset>�� ��λ�Ƶ�����ָ��λ�ƴ�
  - --shift-by N�� ��λ�Ƶ�������ǰλ�� + N����ע��N�����Ǹ�������ʾ��ǰ�ƶ�
  - --to-datetime <datetime>����λ�Ƶ��������ڸ���ʱ�������λ�ƴ���datetime��ʽ��yyyy-MM-ddTHH:mm:ss.xxx������2017-08-04T00:00:00.000
  - --by-duration <duration>����λ�Ƶ��������뵱ǰʱ��ָ�������λ�ƴ���duration��ʽ��PnDTnHnMnS������PT0H5M0S
  - --from-file <file>����CSV�ļ��ж�ȡ��������

- ȷ��ִ�з���������ǰ֧��3�ַ�����

- - ʲô���������ӣ�ֻ�Ǵ�ӡ��λ�Ƶ���������������ִ��
  - --execute��ִ��������λ�Ƶ���
  - --export����λ�Ƶ�����������CSV��ʽ��ӡ�������û���csv�ļ���������ֱ��ʹ��



## �鿴�����ļ�

```shell
# --print-data-log �Ǳ�ʾ�鿴��Ϣ���ݵģ����Ӵ����ǲ鿴������ϸ����Ϣ���ݡ����Ҫ�鿴���log�ļ������ö��ŷָ���
$ kafka-run-class kafka.tools.DumpLogSegments --files /home/data/kafka/kafkadata/test-demo-1/00000000000000000000.log --print-data-log
```



## �鿴�����ļ�

```shell
$ kafka-run-class kafka.tools.DumpLogSegments --files /home/data/kafka/kafkadata/test-demo-1/00000000000000000000.index
```



## ��չ��Ⱥ



# �ļ��洢����



## topic�洢

- topic��partition�ķ�ʽ�洢

- �������partitionֵ

  topic�µ�һ������ֻ�ܱ�ͬһ��consumer group�µ�һ��consumer�߳������ѡ�������֮������������һ��consumer�߳̿������Ѷ�����������ݣ�����Kafka�ṩ��ConsoleConsumer��Ĭ�Ͼ�ֻ��һ���߳����������з��������ݣ���

  ��ˣ�������õ�partition������С��consumer���������ͻ������������Ѳ������ݡ����ԣ��Ƽ�partition������һ��Ҫ����ͬʱ���е�consumer��������

  <font color=#dd0000>�ܽ᣺������������ͬ�������߸��������ޡ�</font>

  ����partition���������ڼ�Ⱥbroker������������leader partition�Ϳ��Ծ��ȵķֲ��ڸ���broker�У�����ʹ�ü�Ⱥ���ؾ��⡣



## partiton�洢

- ÿ��partion(Ŀ¼)�൱��һ�������ļ���ƽ�����䵽�����С���segment(��)�����ļ��С���ÿ����segment file��Ϣ������һ����ȣ��������Է���old segment file���ٱ�ɾ����
- ÿ��partitonֻ��Ҫ֧��˳���д�����ˣ�segment�ļ���С������������**��������ò���**������

```properties
# �������ÿ�����־�� segments �Ĵ������Կ��Խ�������Ϊ��һ��ʱ��֮�󣬻��ڸ�����С�ۻ�֮��ɾ���Ρ�
# ֻҪ����*����*һ���������ξͻᱻɾ����ɾ�����Ƿ�������־�����

# ��־�ļ�����Сɾ������
log.retention.hours=168

# ������־��С�ı������ԡ�����ʣ�µ�Ƭ�α�����־��ɾ��
# ��λ�� log.retention.bytes. ���ܶ����� log.retention.hours.
log.retention.bytes=1073741824

# ��־���ļ�������С�����ﵽ�����Сʱ��������һ���µ���־�Ρ�
log.segment.bytes=1073741824

# The interval at which log segments are checked to see if they can be deleted according to the retention policies
# ʱ������ �����־segments�Բ鿴�Ƿ���Ը��ݱ�������ɾ������ �� ʱ����
log.retention.check.interval.ms=300000
```



## segment�洢

producer��message��ĳ��topic��message�ᱻ���ȵķֲ������partition�ϣ����������û�ָ���Ļص��������зֲ�����kafka broker�յ�message������Ӧpartition�����һ��segment����Ӹ���Ϣ����ĳ��segment�ϵ���Ϣ�����ﵽ����ֵ����Ϣ����ʱ�䳬����ֵʱ��segment�ϵ���Ϣ�ᱻflush�����̣�ֻ��flush�������ϵ���Ϣconsumer�������ѣ�segment�ﵽһ���Ĵ�С�󽫲���������segmentд���ݣ�broker�ᴴ���µ�segment��



ÿ��partition���ڴ��ж�Ӧһ��index����¼ÿ��segment�еĵ�һ����Ϣƫ�ơ�

- segment file��ɣ���2�󲿷���ɣ��ֱ�Ϊindex file��data file����2���ļ�һһ��Ӧ���ɶԳ��֣���׺".index"�͡�.log���ֱ��ʾΪsegment�����ļ��������ļ�.
- segment�ļ���������partionȫ�ֵĵ�һ��segment��0��ʼ������ÿ��segment�ļ���Ϊ��һ��ȫ��partion�����offset(ƫ��message��)����ֵ���Ϊ64λlong��С��19λ�����ַ����ȣ�û��������0��䡣



ÿ��segment�д洢�ܶ�����Ϣ����Ϣid�����߼�λ�þ�����������Ϣid��ֱ�Ӷ�λ����Ϣ�Ĵ洢λ�ã�����id��λ�õĶ���ӳ�䡣

�����ļ��б��Ǳ�����Kafka broker������һ��ʵ�飬����һ��topicXXX����1 partition������ÿ��segment��СΪ500MB,������producer��Kafka brokerд��������ݣ�����ͼ2��ʾsegment�ļ��б�����˵��������2������

![](img/kafka3.png)

������ͼ2��һ��segment file�ļ�Ϊ����˵��segment��index<��->data file��Ӧ��ϵ����ṹ���£�

![](img/kafka4.png)

����ͼ3�������ļ��洢����Ԫ���ݣ������ļ��洢������Ϣ�������ļ���Ԫ����ָ���Ӧ�����ļ���message������ƫ�Ƶ�ַ�������������ļ��� Ԫ����3,497Ϊ���������������ļ��б�ʾ��3��message(��ȫ��partiton��ʾ��368772��message)���Լ�����Ϣ������ƫ�� ��ַΪ497��

������ͼ3�˽⵽segment data file�����message��ɣ�������ϸ˵��message����ṹ���£�

![](img/kafka5.png)

**����˵��**��

| �ؼ���              | ����˵��                                                     |
| :------------------ | :----------------------------------------------------------- |
| 8 byte offset       | ��parition(����)�ڵ�ÿ����Ϣ����һ�������id�ţ����id�ű���Ϊƫ��(offset),������Ψһȷ��ÿ����Ϣ��parition(����)�ڵ�λ�á���**offset��ʾpartiion�ĵڶ���message** |
| 4 byte message size | message��С                                                  |
| 4 byte CRC32        | ��crc32У��message                                           |
| 1 byte ��magic"      | ��ʾ���η���Kafka�������Э��汾��                          |
| 1 byte ��attributes" | ��ʾΪ�����汾�����ʶѹ�����͡���������͡�                 |
| 4 byte key length   | ��ʾkey�ĳ���,��keyΪ-1ʱ��K byte key�ֶβ���                |
| K byte key          | ��ѡ                                                         |
| value bytes payload | ��ʾʵ����Ϣ���ݡ�                                           |



## ͨ��offset����message

�����ȡoffset=368776��message����Ҫͨ������2��������ҡ�

- ��һ������segment file

  ����ͼ2Ϊ��������00000000000000000000.index��ʾ�ʼ���ļ�����ʼƫ����(offset)Ϊ0.�ڶ����ļ� 00000000000000368769.index����Ϣ����ʼƫ����Ϊ368770 = 368769 + 1.ͬ�����������ļ�00000000000000737337.index����ʼƫ����Ϊ737338=737337 + 1�����������ļ��������ƣ�����ʼƫ����������������Щ�ļ���ֻҪ����offset **���ֲ���**�ļ��б��Ϳ��Կ��ٶ�λ�������ļ���

  ��offset=368776ʱ��λ��00000000000000368769.index|log

- �ڶ���ͨ��segment file����messageͨ����һ����λ��segment file����offset=368776ʱ�����ζ�λ��00000000000000368769.index��Ԫ��������λ�ú� 00000000000000368769.log������ƫ�Ƶ�ַ��Ȼ����ͨ��00000000000000368769.log˳�����ֱ�� offset=368776Ϊֹ��

segment index file��ȡϡ�������洢��ʽ�������������ļ���С��ͨ��mmap����ֱ���ڴ������ϡ������Ϊ�����ļ���ÿ����Ӧmessage����һ��Ԫ����ָ��,�� �ȳ���������ʡ�˸���Ĵ洢�ռ䣬������������Ҫ���ĸ����ʱ�䡣



# ����

## ����

1��һ��partition�ĸ��Ƹ�����replication factor���������partition��leader����

2�����ж�partition�Ķ���д��ͨ��leader��

3��Followersͨ��pull��ȡleader��log��message��offset��

4�����һ��follower�ҵ�����ס����ͬ��̫����leader������follower�ӡ�in sync replicas����ISR���б���ɾ����

5�������еġ�in sync replicas����follower��һ����Ϣд�뵽�Լ���log��ʱ�������Ϣ�ű���Ϊ�ǡ�committed���ġ�

6��������ĳ��partition�����и��ƽڵ㶼���ˣ�KafkaĬ��ѡ�����ȸ�����Ǹ��ڵ���Ϊleader������ڵ㲻һ����ISR���



## Leaderѡ��

Kafka��Zookeeper��Ϊÿһ��partition��̬��ά����һ��ISR�����ISR�������replica��������leader��ֻ��ISR��ĳ�Ա�����б�ѡΪleader�Ŀ��ܣ�unclean.leader.election.enable=false����

������ģʽ�£�����f+1��������һ��Kafka topic���ڱ�֤����ʧ�Ѿ�commit��Ϣ��ǰ��������f��������ʧ�ܣ��ڴ����ʹ�ó����£�����ģʽ��ʮ�������ġ���ʵ�ϣ�Ϊ������f��������ʧ�ܣ����������Ӷ������ķ�ʽ��ISR��commitǰ��Ҫ�ȴ��ĸ�����������һ���ģ�����ISR��Ҫ���ܵĸ����ĸ��������ǡ��������Ӷ������ķ�ʽ��һ�롣



# ������-������

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

ע��ProducerRecord�����û��ڴ�����Ϣ�����ʱ���ֱ��ָ��Ҫ���͵ķ���������producer�������͸���Ϣʱ����ֱ�ӷ��͵�ָ����������������ͨ��Partitioner����Ŀ������ˡ����⣬���ǻ�����ֱ��ָ����Ϣ��ʱ���������һ��Ҫ����ʹ��������ܣ���Ϊ���п��ܻ���ʱ�����������ʧЧ��



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



### ��Ϣ��������

�û����ȹ��������͵���Ϣ����`ProducerRecord`��

Ȼ�����`KafkaProducer#send`�������з��ͣ�

`KafkaProducer`���յ���Ϣ�����ȶ���������л���

Ȼ���ϱ��ػ����Ԫ������Ϣһ���͸�`partitioner`ȥȷ��Ŀ�������

���׷��д�뵽�ڴ��е���Ϣ�����(`accumulator`)����ʱ`KafkaProducer#send`�����ɹ����أ�

ͬʱ��`KafkaProducer`�л���һ��ר�ŵ�`Sender IO`�̸߳��𽫻�����е���Ϣ�����η��͸���Ӧ��`broker`�������������Ϣ�����߼���



 **�ص㣺**

�ܹ����������̣߳�ִ��KafkaPrducer#send�߼����̡߳������ǳ�֮Ϊ���û����̡߳���

?									  ִ�з����߼���IO�̡߳������ǳ�֮Ϊ��Sender�̡߳��� 

batch���ơ������������͡����ơ�ÿ������(batch)�а��������ɸ�PRODUCE������˾��и��ߵ��������� 



### ��Ϣ���ͷ�ʽ

1�����Ͳ�����( fire-and-forget)�����ǰ���Ϣ���͸����������������������Ƿ�����������������£���Ϣ�����������Ϊ Kafka�Ǹ߿��õģ����������߻��Զ������ط���������ʹ�����ַ�ʽ��ʱ��Ҳ�ᶪʧһЩ��Ϣ��

```java
ProducerRecord<String, String> record = new ProducerRecord<String, String>("CustomerCountry", "Precision Products", "France");

try {
  producer.send(record);
} catch (Exception e) {
  e.printStackTrace();
}
```

2��ͬ�����ͣ�����ʹ��send()����������Ϣ�� ���᷵��һ��Future���󣬵���get()�������еȴ��� �Ϳ���֪����Ϣ�Ƿ��ͳɹ���

```java
ProducerRecord<String, String> record = new ProducerRecord<String, String>("CustomerCountry", "Precision Products", "France");

try {
  producer.send(record).get();
} catch (Exception e) {
  e.printStackTrace();
}
```

3���첽���ͣ����ǵ��� send() ���ӣ���ָ��һ���ص������� �������ڷ�����Ӧʱ���øú�����

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



### ��Ϣ�����ڲ�����

 ���û�����`KafkaProducer.send(ProducerRecord, Callback)`ʱKafka�ڲ����̷����� 



#### ���л�+����Ŀ�����

 ����`KafkaProducer#send`�߼��ĵ�һ������Ϊ��������Ϣ�������л�������Ŀ�����������ͼ��ʾ�� 

![](img/kafka6.webp)

����ͼ��ʾ��һ������`topic`��"`test`"����Ϣ����"`message`"����Ϣ�����л�֮����`KafkaProducer`�����Ԫ����(�����`topic`��������Ϣ��)��ͬ���������`Partitioner`ʵ�������Ŀ������ļ��㡣



#### ׷��д����Ϣ������(accumulator)

`producer`����ʱ�ᴴ��һ��Ĭ��`32MB`(��`buffer.memory`����ָ��)��`accumulator`��������ר�ű�������͵���Ϣ������`linger.ms`��`batch.size`�Ȳ���֮�⣬�����ݽṹ�л�������һ���ر���Ҫ�ļ�����Ϣ����Ϣ������Ϣ(`batches`)���ü��ϱ�������һ��`HashMap`������ֱ𱣴���ÿ��`topic`�����µ�`batch`���У���ǰ��˵�������ǰ���`topic`�������з���ġ�����������ͬ��������Ϣ�����ڶ�Ӧ�����µ�`batch`�����С�

�ٸ��򵥵����ӣ�������ϢM1, M2�����͵�test��0���������ڲ�ͬ��`batch`��M3���͵�test��1��������ô`batches`�а�������Ϣ���ǣ�

```java
{
	"test-0" -> [batch1, batch2], 
	"test-1" -> [batch3]
}
```



����`topic`�����µ�`batch`�����б���������ɸ���Ϣ���Ρ�ÿ��`batch`������Ҫ��3�����������

?	`compressor`: ����ִ��׷��д�����

?	`batch`����������`batch.size`�������ƣ���Ϣ������׷��д�뵽�ĵط�

?	`thunks`��������Ϣ�ص��߼��ļ���

��һ����Ŀ�ľ��ǽ������͵���Ϣд����Ϣ������У�������������ͼ��ʾ��

![](img/kafka7.webp)

 ��һ��ִ�����֮�������Ͻ�`KafkaProducer.send`������ִ������ˣ��û����߳�������������ǵȴ�`Sender`�̷߳�����Ϣ��ִ�з��ؽ���ˡ� 



#### Sender�߳�Ԥ������Ϣ����

��ʱ����`Sender`�̵߳ǳ��ˡ��ϸ���˵��`Sender`�߳���`KafkaProducer`�������һֱ���������� �����Ĺ������̻������������ģ�

- ������ѯ������Ѱ�������÷���׼���ķ��� ��
- ����ѯ��õĸ���`batch`����Ŀ��������ڵ�`leader broker`���з��飻
- ��������`batch`ͨ���ײ㴴����**Socket����**���͸�����broker��
- �ȴ��������˷���`response`������

Ϊ��˵���ϵķ��㣬���ǻ���ͼ�ķ�ʽ������`Sender`�̵߳Ĺ���ԭ��

![](img/kafka8.webp)



#### Sender�̴߳���response

��ͼ��`Sender`�̻߳ᷢ��`PRODUCE`�������Ӧ��`broker`��`broker`�������֮���Ͷ�Ӧ��`PRODUCE response`��һ��`Sender`�߳̽��յ�`response`��<font color="#00dd00">����(������Ϣ����˳��)</font>����`batch`�еĻص�����������ͼ��ʾ��

![](img/kafka9.webp)

������һ����`producer`������Ϣ�Ϳ���������100%����ˡ�

ͨ����4�����ǿ��Կ����°汾`producer`�����¼���ȫ���첽���̡�**����ڵ���producerǰ���Ǿ���Ҫ���������ƿ�����������û����̻߳�����Sender�߳�**��

����`KafkaProducer`���̰߳�ȫ�ģ������ʹ���������ֻ�����ʹ�÷�����

|                     | ˵��                              | ����                                                         | ����                                                         |
| ------------------- | --------------------------------- | ------------------------------------------------------------ | ------------------------------------------------------------ |
| ��KafkaProducerʵ�� | �����̹߳���һ��KafkaProducerʵ�� | ʵ�ּ򵥣����ܺ�                                             | 1.�����̹߳���һ���ڴ滺��أ�������Ҫ�϶���ڴ�ռ�<br />2.һ�����������û��̶߳��޷����� |
| ��KafkaProducerʵ�� | ÿ���߳�ά��һ��KafkaProducerʵ�� | 1.ÿ���߳�ӵ��ר����KafkaProducerʵ�����������ռ��Լ�һ�����ò�����֧�ָ�ϸ���ȵĵ���<br />2.����KafkaProducerʵ��������Ӱ������ʵ���Ĺ��� | �ϴ�Ķ�����俪��                                           |



### ��ز���

```properties
# producer���÷������ͻ��ƣ��ò���������һ��batch�Ĵ�С��Ĭ����16KB
batch.size

# ָ���������ڷ���������Ϣǰ�ȴ���ʱ�䣬�����ô˲����󣬼���û�дﵽ������Ϣ��ָ����С��
# ����ʱ���������Ҳ�ᷢ��������Ϣ��broker��Ĭ������£������ߵķ�����Ϣ�߳�ֻҪ�����˾�
# �ᷢ����Ϣ������ֻ��һ����Ϣ��������������󣬷����̻߳�ȴ�һ����ʱ�䣬����������������
# ��Ϣ��������������ͬʱҲ�������ӳ١�
linger.ms

# acks���ƶ��ٸ���������д����Ϣ�������߲�����Ϊд��ɹ��������������Ϣ��ʧ�������кܴ�Ӱ�졣�������������ȡֵ��

# 	acks=0�������߰���Ϣ���͵�broker����Ϊ�ɹ������ȴ�broker�Ĵ����������ַ�ʽ��������ߣ���Ҳ�������׶�ʧ��Ϣ�ġ�
#
# 	acks=1�������߻��ڸ÷�����Ⱥ�ף�leader��д����Ϣ�����سɹ�����Ϊ��Ϣ���ͳɹ���
#	���Ⱥ��д����Ϣʧ�ܣ������߻��յ�������Ӧ���������ԡ����ַ�ʽ�ܹ�һ���̶ȱ�����Ϣ��ʧ��
#	�����Ⱥ��崻�ʱ����Ϣû�и��Ƶ�������������ô����Ϣ���ǻᶪʧ�����⣬�������ʹ��ͬ����ʽ�����ͣ�
#	�ӳٻ��ǰһ�ַ�ʽ������ӣ���������һ����������ʱ�䣩�����ʹ���첽��ʽ��Ӧ�ø�֪�����ӳ٣�
#	������������첽���ڷ����е��������ơ�
# 	
#	acks=-1/all�������߻�ȴ����и����ɹ�д�����Ϣ�����ַ�ʽ���ȫ�ģ��ܹ���֤��Ϣ����ʧ�������ӳ�Ҳ�����ġ�
request.required.acks = 0, 1, n, -1/all

# ���������߻��巢�͵���Ϣ���ڴ��С(����Batch�����ܵĴ�С->accumulator)�����Ӧ�õ���send�������ٶȴ��������߷��͵��ٶȣ�
# ��ô���û����������׳��쳣��������Ϊȡ����block.on.buffer.full�����������0.9.0.0�汾
# ��max.block.ms���棬�����׳��쳣ǰ�ȴ�һ��ʱ�䣩������
buffer.memory

# ָ��ʹ����Ϣѹ������������ȡֵΪsnappy��gzip����lz4
# 	snappyѹ���㷨��Google�з��������㷨�����ܺ�ѹ����ȡ�ñȽϺõ�ƽ�⣻
# 	���֮�£�gzip���ĸ����CPU��Դ������ѹ��Ч��Ҳ����õġ�ͨ��ʹ��ѹ�������ǿ��Խ�ʡ��������Kafka�洢�ɱ���
compresstion.type

# �������߷�����Ϣ�յ�һ���ɻָ��쳣ʱ����������ԣ��������ָ�������ԵĴ�����
#	��ʵ������У����������Ҫ���retry.backoff.ms�����Եȴ��������ʹ�ã�
#	�����ܵ�����ʱ��ȼ�Ⱥ����ѡ��Ⱥ�׵�ʱ�䳤���������Ա��������߹���������Ե���ʧ�ܡ�
retries

# ������������������ַ���������broker����ʶ����Ϣ�������ĸ��ͻ��˵ġ���broker���д�ӡ��־������ָ������������ʱ���õ���
client.id

# �������ָ�������߿��Է��Ͷ�����Ϣ��broker���ҵȴ���Ӧ.
# ���ô˲����ϸߵ�ֵ�����������������ͬʱҲ�������ڴ����ġ����⣬������ù��߷����ή����������
# ��Ϊ������ϢЧ�ʽ��͡�����Ϊ1�����Ա�֤���͵�broker��˳��͵���send����˳��һ�£�
# �������ʧ�����Ե����Ҳ����ˡ�
max.in.flight.requests.per.connection

# ��Щ�������������ߵȴ�broker����Ӧʱ�䡣
#	request.timeout.msָ���������ݵĵȴ���Ӧʱ�䣬
#	metadata.fetch.timeout.msָ����ȡԪ���ݣ������ȡ������Ⱥ����Ϣ���ĵȴ���Ӧʱ�䡣
#	timeout.ms��ָ��broker�ڷ��ؽ��ǰ�ȴ�������������acks������أ���Ӧ��ʱ�䣬���ʱ�䵽�˵���������û����Ӧ������򷵻���Ϣд��ʧ�ܡ�
timeout.ms
request.timeout.ms
metadata.fetch.timeout.ms

#  ָ��Ӧ�õ���send�������߻�ȡԪ���ݷ���������partitionFor��ʱ������ʱ�䣬������ʱ�����׳�timeout�쳣��
max.block.ms

# ���������߷������ݰ��Ĵ�С�����ݰ��Ĵ�С����Ϣ�Ĵ�С����Ϣ����ء��������ָ����������ݰ���СΪ1M��
# ��ô������Ϣ��СΪ1M�������ܹ������������1000����Ϣ��СΪ1K����Ϣ��
# ���⣬brokerҲ��message.max.bytes���������ƽ��յ����ݰ���С��
# ��ʵ���У�������Щ����ֵ��ƥ��ģ����������߷����˳���broker�޶������ݴ�С��
max.request.size

# ����������������������/�������ݵ�TCP���ӵĻ��������������Ϊ-1��ʹ�ò���ϵͳ�����Ĭ��ֵ��
# �����������broker�ڲ�ͬ���������ģ�����������ֵ����Ϊ��ͬ�������������ӳٱȽϴ�
receive.buffer.bytes
send.buffer.bytes
```



### ��֤������˳��

`Kafka`���Ա�֤ͬһ�����������Ϣ������ġ�Ҳ����˵����������߰���һ����˳������Ϣ�� `broker`�ͻᰴ�����˳�������д�������������Ҳ�ᰴ��ͬ����˳���ȡ���ǡ���ĳЩ����� �� ˳���Ƿǳ���Ҫ�ġ������`retries` ��Ϊ����������ͬʱ�� `max.in.flight.requests.per.connection` ��Ϊ�� 1���������ô�������һ��������Ϣд��ʧ�ܣ����ڶ�������д��ɹ��� `broker`������д���һ�����Ρ������ʱ��һ������Ҳд��ɹ����� ô�������ε�˳��ͷ������ˡ�

 һ����˵�����ĳЩ����Ҫ����Ϣ������ģ���ô��Ϣ�Ƿ�д��ɹ�Ҳ�� �ܹؼ��ģ����Բ������˳���Ƿǳ���Ҫ�ġ������`retries` ��Ϊ 0�����԰� `max.in.flight.requests.per.connection`��Ϊ 1�������������߳��Է��͵�һ����Ϣʱ���Ͳ�������������Ϣ���͸� broker����������**������Ӱ�������ߵ�������** ������ֻ���� ����Ϣ��˳�����ϸ�Ҫ�������²�����ô����



### ���л�

 [http://www.dengshenyu.com/%E5%88%86%E5%B8%83%E5%BC%8F%E7%B3%BB%E7%BB%9F/2017/11/12/kafka-producer.html](http://www.dengshenyu.com/�ֲ�ʽϵͳ/2017/11/12/kafka-producer.html)



## Consumers

- Kafka�ṩ������consumer api����Ϊ`high-level api`��`sample-api`��

  - **Sample-api** ��һ���ײ��API��ͨ��ֱ�Ӳ����ײ�API��ȡ���ݵķ�ʽ��ȡKafka�е����ݣ���Ҫ���и���������ƫ���������ԡ�

    �ŵ㣺�ɲ�����ǿ��

    ȱ�㣺������Զ��ԱȽϸ��ӡ�(����ࣺ`SimpleConsumer`) 

  - **High Level Consumer API**���߶ȳ����Kafka������API�����ײ�����ȡ���ݡ�����offset������ƫ�����Ȳ������ε���ֱ�ӽ������������Ĵ������ṩ����д�������Ա��

    �ŵ��ǣ������򵥣�

    ȱ�㣺�ɲ�����̫��޷������Լ���ҵ�񳡾�ѡ����ʽ��(����ࣺ`ConsumerConnector`)

- **��kafka�У���ǰ����������Ϣ��offsetֵ����consumer��ά����**����ˣ�consumer�����Լ�������ζ�ȡkafka�е����ݡ����磬consumer����ͨ������offsetֵ���������������ѹ������ݡ�������û�б����ѣ�kafka�ᱣ������һ��ʱ�䣬���ʱ��������**������**�ģ�ֻ�е��˹���ʱ�䣬kafka�Ż�ɾ����Щ���ݡ�

- High-level API��װ�˶Լ�Ⱥ��һϵ��broker�ķ��ʣ�����͸��������һ��topic�����Լ�ά������������Ϣ��״̬����ÿ�����ѵĶ�����һ����Ϣ��

  High-level API��֧���������ʽ����topic�����consumers��ͬһ����������ôkafka���൱��һ��������Ϣ���񣬶�����consumer�����������Ӧpartition�е����ݡ���consumers�в�ͬ����������ô��ʱkafka���൱��һ���㲥���񣬻��topic�е�������Ϣ�㲥��ÿ��consumer��

  

  **High level api��Low level api�����consumer���Եģ���producer�޹ء�**

  

  High level api��consumer����partition��offsite�Ǵ���zookeeper�ϡ�High level api ����������һ���߳�ȥÿ��һ��ʱ�䣬offsite�Զ�ͬ����zookeeper�ϡ����仰˵�����ʹ����High level api�� ÿ��messageֻ�ܱ���һ�Σ�һ����������message֮��������consumer�Ĵ����Ƿ�ok��High level api������һ���̻߳��Զ��İ�offiste+1ͬ����zookeeper�ϡ����consumer��ȡ���ݳ������⣬offsiteҲ����zookeeper��ͬ������ˣ����consumer����ʧ���ˣ������ִ����һ�����������ǲ��Ե���Ϊ����ˣ�Best Practice��һ��consumer����ʧ�ܣ�ֱ��������conusmer group��Exception��ֹ��������������һ�������Ƕ�ʧ�ˣ���Ϊ��zookeeper�����offsite�Ѿ�+1�ˡ����ٴ�����conusmer group��ʱ���Ѿ�����һ����ʼ��ȡ�����ˡ�

  

  Low level api��consumer����partition��offsite��consumer�Լ��ĳ�����ά��������ͬ����zookeeper�ϡ�����Ϊ��kafka manager�ܹ�����ļ�أ�һ��Ҳ���ֶ���ͬ����zookeeper�ϡ������ĺô���һ����ȡĳ��message��consumerʧ���ˣ�����message��offsite�����Լ�ά�������ǲ���+1���´���������ʱ�򣬻�������offsite��ʼ����������������exactly once�������ݵ�׼ȷ���б�֤��

  

  

### Consumer��Partition�Ĺ�ϵ�� **Consumer Rebalance** ��

- ���consumer��partition�࣬���˷ѣ���Ϊkafka���������һ��partition���ǲ��������ģ�����consumer����Ҫ����partition��
- ���consumer��partition�٣�һ��consumer���Ӧ�ڶ��partitions��������Ҫ�������consumer����partition��������ᵼ��partition��������ݱ�ȡ�Ĳ�����
- ���consumer�Ӷ��partition�������ݣ�����֤���ݼ��˳���ԣ�kafkaֻ��֤��һ��partition������������ģ������partition�����������˳����в�ͬ
- ����consumer��broker��partition�ᵼ��rebalance������rebalance��consumer��Ӧ��partition�ᷢ���仯
- High-level�ӿ��л�ȡ�������ݵ�ʱ���ǻ�block��



# ��ϢͶ������

Kafka֧�ֵ�������ϢͶ������:

- `at most once`:����һ�Σ���Ϣ���ܻᶪ���������ظ�
- `at least once`:����һ�Σ���Ϣ�϶����ᶪʧ���������ظ�
- `exactly once`:����ֻ��һ�Σ���Ϣ����ʧ���ظ�����ֻ����һ�Ρ�



## 0.11.0֮ǰ�İ汾

### Producer ��Ϣ�����߶�

�� producer �� leader ��������ʱ������ͨ�� `request.required.acks` �������������ݿɿ��Եļ���

- acks=1��Ĭ�ϣ������ҽ���leader�յ���Ϣ**����commitȷ���ź�**����Ϊ���ͳɹ������ leader 崻�����ᶪʧ���ݡ�
- acks=0��producer������Ϣ����ɷ��ͣ�**����ȴ�**���� broker ��ȷ�ϡ�������������ݴ���Ч����ߣ��������ݿɿ���ȷ����͵ġ�
- acks=-1��ALL�������Ͷ���Ҫ�ȴ� **ISR �б��������б�ȷ�Ͻ�������**�����һ�η�����ɣ��ɿ�����ߣ��ӳ�Ҳ�ϴ�

�����漰��producer�˵�acks���ú�broker�˵ĸ����������Լ�`min.insync.replicas`�����á�



### Broker ��Ϣ���ն�

acks=1����ʾ��leader��Ƭ����д��Ϣ�ɹ��ͷ�����Ӧ��producer����ʱ��Ϊ��Ϣ���ͳɹ���
���leaderд�ɹ������Ϲ��ˣ���û�н����д�ɹ�����Ϣͬ���������ķ�Ƭ��������ô�����Ƭ��ʱ��ISR�б�Ϊ�գ�
���unclean.leader.election.enable=true���ͻᷢ��log truncation����־��ȡ����ͬ���ᷢ����Ϣ��ʧ��
���unclean.leader.election.enable=false����ô�����Ƭ�ϵķ���Ͳ������ˣ�producer�������Ƭ����Ϣ�ͻ����쳣��

������������min.insync.replicas=2��unclean.leader.election.enable=false��producer�˵�acks=all���������ͳɹ�����Ϣ�;����ᶪʧ��



### Consumer ��Ϣ�����߶�

����Ҫ�������߹ر��Զ��ύ(`enable.auto.commit:false`)��ͬʱ��������ÿ�� poll ������ҵ���߼����������ֶ�ͬ���ύ��`commitSync`������������������ѹ����з��� `crash`���´�����ʱ��Ȼ���֮ǰ��λ�ÿ�ʼ���ѣ��Ӷ���֤ÿ���ύ�����ݶ��ܱ����ѡ�



### ��Ϣȥ��

���ǵ� producer,broker,consumer ֮�䶼�п��������Ϣ�ظ�����������Ҫ����ն���Ҫ֧����Ϣȥ�صĹ��ܣ���ý���ҵ����Ϣ������ݵ���������������Щ������������� hbase��elasticsearch ��Ȼ��֧���ݵȲ�����

������
�ڽ��նˣ�����ר�ŵ���������ȡ kafka ���ݴ��� hbase��hbase �� rowkey �������Ҫ����`Id`�� `timestamp`�������̴߳� kafka ��ȡ���ݺ����л���Ȼ���������� hbase��ֻ�в���ɹ������ kafka �г־û� offset�������ĺô��ǣ�������м�����һ���׶η�����������ָ��󶼻����һ�γ־û� offset ��λ�ÿ�ʼ�������ݣ�������������ݶ�ʧ�������;���ظ����ѵ����ݣ������ hbase �� rowkey ����ͬ�ģ�����ֻ�Ḳ�ǲ����ظ������մﵽ����һ�¡�



## 0.11.0֮��İ汾

### �ݵ��Է���

��0.11֮ǰ��Ҫ��ͨ������ϵͳ�����ݵ�������֤`Exactly Once`�����������м���ȱ�ݣ�

- Ҫ������ϵͳ֧���ݵȲ�����������`Kafka`�����ó���
- ʵ���ż���Խϸߣ���Ҫ�û���`Kafka`�Ĺ������Ʒǳ��˽�
- ����`Kafka Stream`���ԣ�`Kafka Producer`������ǡ����Ρ�ϵͳ������`Producer`�����ݵȴ������ԣ��ǾͿ�����`Kafka Stream`��һ���̶���֧��`Exactly once`���塣

0.11֮��İ汾��������idempotent producer���ƣ�ͨ��**Producer ID��PID����Sequence Number**ʵ��Producer���ݵ����塣

- `Producer ID`��ÿ���µ�`Producer`�ڳ�ʼ����ʱ��ᱻ����һ��Ψһ��PID
- `Sequence Number`������ÿ��`PID`����`Producer`�������ݵ�ÿ��`<Topic, Partition>`����Ӧһ����0��ʼ����������`Sequence Number`��

`Broker`��Ҳ�����ڴ���Ϊÿ��`<PID, Topic, Partition>`ά��һ����ţ�����ÿ��`Commit`һ����Ϣʱ�����Ӧ��ŵ��������ڽ��յ�ÿ����Ϣ���������ű�`Broker`ά������ţ������һ��`Commit`����Ϣ����ţ���һ����`Broker`��������������䶪����

- �����Ϣ��ű�Brokerά������Ŵ�һ���ϣ�˵���м���������δд�룬Ҳ�����򣬴�ʱBroker�ܾ�����Ϣ��Producer�׳�InvalidSequenceNumber
- �����Ϣ���С�ڵ���Brokerά������ţ�˵������Ϣ�ѱ����棬��Ϊ�ظ���Ϣ��Brokerֱ�Ӷ�������Ϣ��Producer�׳�DuplicateSequenceNumber

���ֻ��ƺܺõ�<font color=#dd0000>����������ظ����������������</font>��



**���������**��

����`producer`�˵��²��� `enable.idempotent=true`��



��������������������Ҫ��֤ԭ���Ե�д������������д�뵽�����������ϢҪôȫ���ɹ���Ҫôȫ���ع���

��ʱ�����Ҫʹ��������producer������ transcational.idΪһ��ָ���ַ�����

<font color=#dd0000>�����ݵ�producerֻ�ܱ�֤�����������ظ���Ϣ��������Ա�֤�����д����Ϣ�������ԡ�</font>



����producer��ʵ����exactly once����ôconsumer���أ�

consumer�����ڿ����޷�����������������Ϣ��������Ϣ���ܱ�ɾ�����������񲢲��ܽ��consumer��exactly once�����⣬���ǿ��ܻ�����Ҫ�Լ������ⷽ����߼��������Լ�����offset���ύ����Ҫ�Զ��ύ��Ҳ�ǿ���ʵ��exactly once�ġ�

**����һ��ѡ�����ʹ��kafka�Լ������������棬Ҳ����Kafka Streams��**

**����processing.guarantee=exactly_once���Ϳ�������ʵ��exactly once�ˡ�**



**ע��**

����˵�����ֻ����Ե���Producer��һ��session�ڵ����������Producer���ˣ�����������һ��Producer�����ҷ���������һ��PID�������Ͳ��ܴﵽ���ص�Ŀ���ˣ�����kafka��������Transactional Guarantees�������Ա�֤����



## �ݵ�

�ݵ��ԣ�

> һ��ָ producer Ͷ���˶�����Ϣ��consumer �������˶�����Ϣ�����ᷢ����Ϣ��ʧ������Ϣ�ظ��������



����`producer`�����`broker`������`enable.idempotence = true`,ÿ��`producer`�ڳ�ʼ����ʱ�򶼻ᱻ����һ��Ψһ��`Producer ID`��`producer`��ָ��`topic`��`partition`������Ϣʱ��Я��һ���Լ�ά����������`Sequence Number`��`broker`��ά��һ��`<pid,topic,partition>`��Ӧ��`seqNum`�� ÿ��`broker`���յ�`producer`��������Ϣ�����֮ǰ��`seqNum`���ȶԣ�����պô�һ������ܣ������ȣ�˵����Ϣ�ظ�;���������һ����˵���м���ڶ���Ϣ���ܾ����ܡ�

�����ƽ������������:

- `broker`������Ϣ�󣬷���`ACK`ǰ崻���`producer`��Ϊû�з��ͳɹ������ԣ������Ϣ�ظ�
- ǰһ����Ϣ����ʧ�ܣ���һ���ɹ���ǰһ����Ϣ���Ժ�ɹ��������Ϣ����

## �����Ա�֤

kafka�������Ա�֤��ͬʱ����`Topic` `Partitions`������Ϣ��Ҫô���ɹ���Ҫô��ʧ�ܡ�

�������ݵȲ�����ֻ�ܱ�֤����`producer`����ͬһ��`<topic,partition>`��`exactly once`,�����ܱ�֤����`topic` `partitions`д����ʱ��ԭ���ԡ������ܱ�֤�����д����ʱ��ԭ���ԡ�

����ĳ�������ǣ���ĳ��`topic`������Ϣ������ת�����д����һ��`topic`�С�

�����Ա�֤����ʹӦ�ó����������ݺ��������ݵ���һ��ԭ�ӵ�Ԫ��������ʹ�����������ѿ���`<Topic, Partition>`��Ӧ�ó���Ҳ�����������󣬴���һ�������ָ���Ҳ������ָ���

��Ϊ��Ϣ�����ǿ�`topic`��`partition`�ģ�����Ϊʵ����һЧ����������Ӧ�ó����ṩһ���ȶ��ģ������󲻱䣩Ψһ��`Transaction ID`��ʹ��`PID` �� `Transaction ID` һһ��Ӧ������

## conusmer��

���������Ա�ֻ֤�����producer�˵ģ���consumer����Ȼ�޷���֤��

���������kafka�е�topic�����������д����һ��topic����ô���Խ�������Ϣ�ͷ�����Ϣ��Ϊһ������ ���Ҫ��������Ϣ��Ľ�����浽�ⲿϵͳ����Ҫ�õ����׶��ύ�ˡ�



# �����ļ�

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

# ��Ϣ������д���ļ�ϵͳ��������Ĭ������£�����ֻͬ��fsync()
# ����ϵͳ�����Ƕ��Եġ��������ÿ������ݵ����̵�ˢ�¡�
# ������һЩ��Ҫ��Ȩ��:
# 	1���־���:�����ʹ�ø��ƣ�δˢ�µ����ݿ��ܻᶪʧ��
# 	2���ӳ�:�ǳ����ˢ�¼�����ܻᵼ���ӳٷ�ֵ����Ϊ���д���������Ҫˢ�¡�
# 	3��������:ˢ��ͨ�������Ĳ�����һ��С��ˢ�¼�����ܵ��¹��ȵĲ��ҡ�

# �������������������ˢ�²��ԣ��Ա���һ��ʱ���֮��ˢ������

#ÿN����Ϣ(�����߶���)���������ȫ�ַ�Χ����ɣ�����ÿ������Ļ����ϸ��ǡ�
# every N messages (or both). This can be done globally and overridden on a per-topic basis.

# The number of messages to accept before forcing a flush of data to disk
# �ڽ�����ˢ�µ�����֮ǰ���Խ��ܵ���Ϣ����
log.flush.interval.messages=10000

# The maximum amount of time a message can sit in a log before we force a flush
# ��ǿ��ˢ��֮ǰ����Ϣ����פ������־�е��ʱ��
log.flush.interval.ms=1000

############################# Log Retention Policy #############################

# �������ÿ�����־�� segments �Ĵ������Կ��Խ�������Ϊ��һ��ʱ��֮�󣬻��ڸ�����С�ۻ�֮��ɾ���Ρ�
# ֻҪ����*����*һ���������ξͻᱻɾ����ɾ�����Ƿ�������־�����

# ��־�ļ�����Сɾ������
log.retention.hours=168

# ������־��С�ı������ԡ�����ʣ�µ�Ƭ�α�����־��ɾ��
# ��λ�� log.retention.bytes. ���ܶ����� log.retention.hours.
log.retention.bytes=1073741824

# ��־���ļ�������С�����ﵽ�����Сʱ��������һ���µ���־�Ρ�
log.segment.bytes=1073741824

# The interval at which log segments are checked to see if they can be deleted according to the retention policies
# ʱ������ �����־segments�Բ鿴�Ƿ���Ը��ݱ�������ɾ������ �� ʱ����
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



# �ͻ���

`kafka-manager`

https://blog.51cto.com/liqingbiao/2417010

https://juejin.im/post/5dd2261df265da0bc3309393



# kafka�ٶȿ�



## ˳���д

���̴���������ǻ�е�ṹ��SSD�������۵ķ�Χ�ڣ����������Ϣ�����д�ķ�ʽ������̣�����Ҫ�����桢��ͷ�������ķ�ʽѰַ��Ѱַ��һ������е������Ҳ���ʱ��Ϊ����߶�дӲ�̵��ٶȣ�**Kafka����ʹ��˳��I/O**��

˳��I/O���ǲ���׷�����ݵ��ļ��еģ��������ʹ�����Գ�����ô��̵�˳���д������

![](img/kafka-io1.jpeg)

> ÿ��partition����һ���ļ���ÿ����Ϣ����append ���� partition �У�����˳��д���̣����Ч�ʷǳ��ߡ����ַ�����һ��ȱ�ݡ��� û�а취ɾ������ ������Kafka�ǲ���ɾ�����ݵģ���������е����ݶ�����������ÿ�������ߣ�Consumer����ÿ��Topic����һ��offset������ʾ��ȡ���˵ڼ������� ��



˳���д������Ӳ�̴�ͷ��Ѱ��ʱ�䣬ֻ��Ҫ���ٵ�������תʱ�䣬�����ٶ�Զ���������д��Kafka �ٷ������Ĳ�������(Raid-5, 7200rpm)

˳��I/O: 600MB/s

���I/O: 100KB/s



## mmap��д�����ݣ�

 ������˳��д��Ӳ�̣�Ӳ�̵ķ����ٶȻ��ǲ�����׷���ڴ档����Kafka�����ݲ�����ʵʱ��д��Ӳ�� ��������������ִ�����ϵͳ��ҳ�洢�������ڴ����I/OЧ�ʡ� 

Memory Mapped Files(������mmap)Ҳ������� �ڴ�ӳ���ļ� ����64λ����ϵͳ��һ����Ա�ʾ20G�������ļ���**���Ĺ���ԭ����ֱ�����ò���ϵͳ��Page��ʵ���ļ��������ڴ��ֱ��ӳ�䡣**



���ӳ��֮����������ڴ�Ĳ����ᱻͬ����Ӳ���ϣ�����ϵͳ���ʵ���ʱ�򣩡�



ͨ��mmap���������дӲ��һ����д�ڴ棨��Ȼ��������ڴ棩��Ҳ���ع����ڴ�Ĵ�С�������ڴ�Ϊ���Ƕ��ס�



ʹ�����ַ�ʽ���Ի�ȡ�ܴ��I/O������ʡȥ���û��ռ䵽�ں˿ռ临�ƵĿ����������ļ���read��������ȷŵ��ں˿ռ���ڴ��У�Ȼ���ٸ��Ƶ��û��ռ���ڴ��С���



��Ҳ��һ�������Ե�ȱ�ݡ������ɿ���д��mmap�е����ݲ�û�б�������д��Ӳ�̣�����ϵͳ���ڳ�����������flush��ʱ��Ű�����������д��Ӳ�̡�



Kafka�ṩ��һ����������producer.type�������ǲ�������flush�����Kafkaд�뵽mmap֮�������flushȻ���ٷ���Producer�� ͬ�� (sync)��д��mmap֮����������Producer������flush���첽 (async)��



## �㿽������ȡ���ݣ�

[https://www.ibm.com/developerworks/cn/linux/l-cn-zerocopy2/](https://www.ibm.com/developerworks/cn/linux/l-cn-zerocopy2/)

�㿽����Zero-copy������ָ�ڼ����ִ�в���ʱ��CPU ����Ҫ�Ƚ����ݴ�һ���ڴ������Ƶ���һ���ڴ����򣬴Ӷ����Լ����������л��Լ� CPU �Ŀ���ʱ�䡣

���������������ݱ��������豸���û�����ռ䴫�ݵĹ����У��������ݿ�������������ϵͳ���ã�ʵ�� CPU ������룬�������� CPU ���ⷽ��ĸ��ء�



ʵ���㿽���õ�������Ҫ������ DMA ���ݴ��似�����ڴ�����ӳ�似����

- �㿽�����ƿ��Լ����������ں˻��������û����̻�����֮�䷴���� I/O ����������
- �㿽�����ƿ��Լ����û����̵�ַ�ռ���ں˵�ַ�ռ�֮����Ϊ�������л��������� CPU ������



### ��ͳ�ļ�IO

 ��ͳģʽ�£��ļ�����Ĳ������̣�����һ������Ҫ<font color=#dd0000>���ļ����ݷ��͵�����</font>��������̷������û��ռ䣬�ļ�������socket����Ӳ����Դ������֮����һ���ں˿ռ䣬�ڲ���ϵͳ�ڲ�����������Ϊ:



![](img/kafka2.webp.)

����ͼ�п��Կ��������������Ĵ����ݿ�������ʹʹ����`DMA`����������Ӳ����ͨѶ��CPU��Ȼ��Ҫ�����������ݿ��������ͬʱ�����û�̬���ں�̬Ҳ�����˶���������л�������Ҳ������CPU������



����

![](img/kafka10.webp)



### mmap

```c
#include <sys/mman.h>

void *mmap(void *addr, size_t length, int prot, int flags, int fd, off_t offset);

int munmap(void *addr, size_t length);
```





���Ǽ��ٿ���������һ�ַ����ǵ���mmap()������read����

```c
buf = mmap(diskfd, len);
write(sockfd, buf, len);
```

Ӧ�ó������`mmap()`�������ϵ����ݻ�ͨ��`DMA`���������ں˻����������Ų���ϵͳ�������ں˻�������Ӧ�ó����������Ͳ���Ҫ���ں˻��������������û��ռ俽����Ӧ�ó����ٵ���`write()`,����ϵͳֱ�ӽ��ں˻����������ݿ�����`socket`�������У���һ�ж��������ں�̬�����`socket`�������ٰ����ݷ�������ȥ��

![](img/kafka-io1.webp)

ʹ��mmap���read�����Լ�����һ�ο������������������ܴ�ʱ������������Ч�ʡ�



**����**

��ʹ��`mmap`ʱ�����ܻ�����һЩ���ص����塣���磬������`map`��һ���ļ������ǵ�����ļ�����һ�����̽ض�(`truncate`)ʱ, `write`ϵͳ���û���Ϊ���ʷǷ���ַ����`SIGBUS`�ź���ֹ��`SIGBUS`�ź�Ĭ�ϻ�ɱ����Ľ��̲�����һ��`coredump`,����������ᱻ��ֹ��



**�������**

1��ΪSIGBUS�źŽ����źŴ������

> ������`SIGBUS`�ź�ʱ���źŴ������򵥵ط��أ�`write`ϵͳ�����ڱ��ж�֮ǰ�᷵���Ѿ�д����ֽ���������`errno`�ᱻ���ó�success,��������һ�����Ĵ���취����Ϊ�㲢û�н�������ʵ�ʺ��ġ�

2��ʹ���ļ������

> ͨ������ʹ�����ַ��������ļ���������ʹ�������������Ϊ�ļ����ں�����һ���������������������Ҫ�ض�����ļ�ʱ���ں˻������Ƿ���һ��ʵʱ��`RT_SIGNAL_LEASE`�źţ����������ں������ƻ���ӳ����ļ��ϵĶ�д���������ڳ�����ʷǷ��ڴ沢�ұ�`SIGBUS`ɱ��֮ǰ�����`write`ϵͳ���ûᱻ�жϡ�`write`�᷵���Ѿ�д����ֽ�����������`errno`Ϊsuccess��



����Ӧ����`mmap`�ļ�֮ǰ�����������ڲ������ļ��������

```c
if(fcntl(diskfd, F_SETSIG, RT_SIGNAL_LEASE) == -1) {
    perror("kernel lease set signal");
    return -1;
}
/* l_type can be F_RDLCK F_WRLCK  ����*/
/* l_type can be  F_UNLCK ����*/
if(fcntl(diskfd, F_SETLEASE, l_type)){
    perror("kernel lease set type");
    return -1;
}
```





### sendfile

```c
#include<sys/sendfile.h>

ssize_t sendfile(int out_fd, int in_fd, off_t *offset, size_t count);
```

ϵͳ����`sendfile()`�ڴ��������ļ���������`in_fd`�ʹ�������ļ���������`out_fd`֮�䴫���ļ����ݣ��ֽڣ���<font color=#dd0000>������`out_fd`����ָ��һ���׽��֣�ֻ���ļ��� socket �������ݴ��䡣������`in_fd`ָ����ļ������ǿ���`mmap`�ġ�</font>��Щ����������`sendfile`��ʹ�ã�ʹ`sendfile`ֻ�ܽ����ݴ��ļ����ݵ��׽����ϣ���֮���С�
 ʹ��`sendfile`�������������ݿ����Ĵ��������������������л������ݴ���ʼ��ֻ������`kernel space`��



![](img/kafka3.webp.)

> �����ǵ���`sendfile`ʱ��������������̽ض����ļ��ᷢ��ʲô�أ���������û�������κ��źŴ������`sendfile`���ý����������ڱ��ж�֮ǰ�Ѿ�������ֽ�����`errno`�ᱻ��Ϊsuccess����������ڵ���sendfile֮ǰ���ļ���������`sendfile`����Ϊ��Ȼ��֮ǰ��ͬ�����ǻ����յ�RT_SIGNAL_LEASE���źš�



### ���� DMA �ռ��������ܵ� sendfile



ĿǰΪֹ�������Ѿ����������ݿ����Ĵ����ˣ�������Ȼ����һ�ο���������<font color=#dd0000>ҳ���浽socket����Ŀ���</font>����ô�ܲ��ܰ��������Ҳʡ���أ�

������Ӳ���ϵİ����������ǿ��԰쵽�ġ�֮ǰ�����ǰ�ҳ��������ݿ�����socket�����У�ʵ���ϣ����ǽ�����Ҫ�ѻ���������������`socket`���������ٰ����ݳ��ȴ���ȥ������`DMA`������ֱ�ӽ�ҳ�����е����ݴ�����͵������оͿ����ˡ�

�ܽ�һ�£�`sendfile`ϵͳ��������`DMA`���潫�ļ����ݿ������ں˻�����ȥ��Ȼ�󽫴����ļ�λ�úͳ�����Ϣ�Ļ��������������socket������ȥ����һ�����Ὣ�ں��е����ݿ�����socket�������У�`DMA`����Ὣ�ں˻����������ݿ�����Э��������ȥ�����������һ�ο�����

������һ���ռ�������������ҪӲ���Լ���������֧�ֵġ�



![](img/kafka-io2.webp)



### splice

`sendfile`ֻ�����ڽ����ݴ��ļ��������׽����ϣ��޶�������ʹ�÷�Χ��Linux��`2.6.17`�汾����`splice`ϵͳ���ã������������ļ����������ƶ����ݣ�

```c
#define _GNU_SOURCE         /* See feature_test_macros(7) */
#include <fcntl.h>

ssize_t splice(int fd_in, loff_t *off_in, int fd_out, loff_t *off_out, size_t len, unsigned int flags);
```

`splice`�����������ļ�������֮���ƶ����ݣ�������Ҫ�������ں˿ռ���û��ռ����ؿ���������`fd_in`����`len`���ȵ����ݵ�`fd_out`������<font color=#dd0000>��һ�������ǹܵ��豸����Ҳ��Ŀǰ`splice`��һЩ�����ԡ�</font>



`flags`������

> **SPLICE_F_MOVE** ������ȥ�ƶ����ݶ����ǿ������ݡ�������Ƕ��ں˵�һ��С��ʾ������ں˲��ܴ�`pipe`�ƶ����ݻ���`pipe`�Ļ��治��һ����ҳ�棬��Ȼ��Ҫ�������ݡ�Linux�����ʵ����Щ���⣬���Դ�`2.6.21`��ʼ���ѡ������ã������Linux�汾Ӧ�û�ʵ�֡�
>
> **SPLICE_F_NONBLOCK** ��`splice` �������ᱻ������Ȼ��������ļ�������û�б�����Ϊ���ɱ�������ʽ�� I/O ����ô���� splice �п�����Ȼ��������
>
> **SPLICE_F_MORE**�� �����`splice`���û��и�������ݡ�



splice����������Linux����Ĺܵ����������ƣ� ��������һ��������ҪΪ�ܵ���



## �ļ��ֶ�

 Kafka �Ķ��� topic ����Ϊ�˶���� partition, ÿ�� partition �ַ�Ϊ�˶�� segment������һ�������е���Ϣʵ�����Ǳ����� N ���Ƭ���ļ��С� 

![](img/kafka4.webp)

ͨ���ֶεķ�ʽ��ÿ���ļ��������Ƕ�һ��С�ļ��Ĳ������ǳ���㣬ͬʱҲ�����˲��д��������� 



## �������� batch

�ͻ��˷�����Ϣ��`kafka`��������ʱ������һ���ڴ滺����Ƶġ� Ҳ����˵����Ϣ����д��һ���ڴ滺���У�Ȼ��ֱ��������Ϣ�����һ��Batch���Ż�һ������ͨ�Ű�Batch���͹�ȥ�� ����ͼ��ʾ��

![](img/kafka11.webp)

���꣺�ڴ滺����ɵ�Ƶ��GC������ν����



�����ڴ滺����Ƶı��⣬��ʵ���ǰѶ�����Ϣ���һ��Batch��һ�������������һ��Batch���߶��Batch��

����ÿ���������󶼿��Է��ͺܶ����ݹ�ȥ��������һ����Ϣһ���������󡣴Ӷ�������������������λʱ���ڷ��͵���������

�����������ˣ�һ��Batch�е����ݣ���ȡ����Ȼ���װ�ڵײ��������ͨ�����緢�ͳ�ȥ����Kafka�������� �ȵ����Batch������ݶ����͹�ȥ�ˣ�����Batch�������Ӧ����ô���� 

Ҫ֪������ЩBatch������ݴ�ʱ�ɻ��ڿͻ��˵�JVM���ڴ��ﰡ����ô��ʱ�Ӵ���ʵ�ֲ��棬һ���᳢�Ա����κα���ȥ������ЩBatch��Ӧ�����ݣ�Ȼ���Դ���JVM�Զ����յ���Щ�ڴ�������

�������ϵ���JVM�����������Ϳ��Բ��ϵ�������Ѿ����ͳɹ���Batch�ˣ�Ȼ��Ϳ��Բ��ϵ��ڳ����µ��ڴ�ռ��ú����µ�������ʹ�á�

->  ʵ���������е�ʱ��һ���������⣬**�������⣬����JVM GC���⡣** ���£�

![](img/kafka12.webp)

Kafkaʵ�ֵĻ���ػ���

![](img/kafka13.webp)

���ã�

```properties
# �����߿����ڻ���ȴ����͵��������ļ�¼���ڴ����ֽ�����
# �����¼���͵��ٶȱȷ��͵����������ٶȿ죬��ô�����߽�����max.block��֮�������׳�һ���쳣��
# ������Ӧ�ô��¶�Ӧ�������߽�ʹ�õ����ڴ棬������Ӳ���ƣ���Ϊ������ʹ�õĲ��������ڴ涼���ڻ��塣
# һЩ������ڴ潫����ѹ��(���������ѹ��)�Լ�ά�������е�����
buffer.memory = 33554432 # 32M
max.block.ms = 60000

batch.size = 16384 # 16k
linger.ms = 0
```



## ����ѹ��

 Kafka֧�ֶ�����Ϣѹ����ʽ��gzip��snappy��lz4�� 



## ҳ����

�����ʵʱ��linux����ϵͳ�����ṩ���Ż���

[https://www.jianshu.com/p/3b6612318c23](https://www.jianshu.com/p/3b6612318c23)

[https://juejin.im/post/5d59638c518825291e3dd77f](https://juejin.im/post/5d59638c518825291e3dd77f)


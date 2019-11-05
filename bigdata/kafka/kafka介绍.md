[Kafka ���Ž���-��������](<https://lotabout.me/2018/kafka-introduction/>)

[kafka����](<https://kafka.apache.org/>)

[confluent](<https://www.confluent.io/>)



# ��ظ���

`massage`�� kafka��������Ĵ��ݶ����й̶���ʽ�� 

`topic`�����⣬���� һ����Ϣ����page view��click��Ϊ�ȡ�

`producer`�� ������Ϣ�����壬�����Ƿ�������־��Ϣ�ȡ�

`consumer`�� ����producer����������Ϣ�����塣

`Consumer Group`��ÿ�� Consumer ����һ���ض��� Consumer Group����Ϊÿ�� Consumer ָ�� group name������ָ�� group name ������Ĭ�ϵ� group����

`broker`�� ��Ϣ�����㣬���broker���kafka��Ⱥ��

`partition`�� topic��������飬ÿ��partition����һ��������С�

`segment`�� �����С��ȵĶ������һ��partition��

`offset`�� һ�����������ڶ�λ��׷�ӵ�������ÿһ����Ϣ�����кţ����ֵΪ64λ��long��С��19λ�����ַ����ȡ�

- topic��partition��segment��offset�Ĺ�ϵ��

![](img/kafka1.webp)

- ÿ��topic���Է�Ϊ���partition��һ��partition�൱��һ����Ŀ¼��ÿ��partition�����ж��**��С���**��segment�ļ������segment����message��ɵģ���ÿһ����segment��һ���ɴ�С��ȵ�message��ɡ�segment��С������������server.properties�ļ������á�offset���ڶ�λλ�ڶ����Ψһ��Ϣ��

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

  

- segment
  - segment��index��data�ļ���ɣ������ļ��ɶԳ��֣��ֱ�洢���������ݡ�
  - segment�ļ��������򣺶������е�partition��˵��segment���ƴ�0��ʼ��֮���ÿһ��segment����Ϊ��һ��segment�ļ����һ����Ϣ��offsetֵ��

- offset

���ڷ����е�һ��offset�������345552��ôȥ������Ӧ��message�أ�

> ���ҵ���message���ڵ�segment�ļ���ͨ�����ֲ��ҵķ�ʽѰ��С�ڵ���345552��offset�������S��segment����Ҫ�����S����345552��S��һ��segment�����һ��message��Ϊ�������SС��345552�����α�����ǰsegment�����ҵ���

- topic �� partition

![](img/kafka2.png)

- kafka��Ϣ��������

```properties
# ����ô���ֿ��ܵ� delivery guarantee��
At most once ��Ϣ���ܻᶪ�����������ظ�����
At least one ��Ϣ�����ᶪ�������ܻ��ظ�����
Exactly once ÿ����Ϣ�϶��ᱻ����һ���ҽ�����һ�Σ��ܶ�ʱ�������û�����Ҫ�ġ�
```

- offset���µķ�ʽ

```properties
�Զ��ύ������enable.auto.commit=true�����µ�Ƶ�ʸ��ݲ�����auto.commit.interval.ms�����������ַ�ʽҲ����Ϊ��at most once����fetch����Ϣ��Ϳ��Ը���offset�������Ƿ����ѳɹ���

�ֶ��ύ������enable.auto.commit=false�����ַ�ʽ��Ϊ��at least once����fetch����Ϣ�󣬵���������ٵ��÷�����consumer.commitSync()�����ֶ�����offset���������ʧ�ܣ���offsetҲ������£�������Ϣ�ᱻ�ظ�����һ�Ρ�
```

- ��Ϣ�ַ�

  > Kafka ������������ݵ�Ԫ������Ϣ����һ����Ϣ��ʵ���� Key + Value ��ɵģ�Key �ǿ�ѡ��ɴ���ֵ��Value Ҳ���Դ���ֵ�����ڷ���һ����Ϣʱ�����ǿ���ָ����� Key����ô Producer ����� Key �� partition �������жϵ�ǰ������ϢӦ�÷��Ͳ��洢���ĸ� partition �У�����͸���Ƭ�������ƣ������ǿ��Ը�����Ҫ������չ Producer �� partition ���ƣ�Ĭ���㷨�� hash ȡ %����

  ��չ�Լ��� partition��

  ```java
  
  package dongguabai.kafka.partition;
   
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

eg:
$ kafka-topics --delete --topic test --zookeeper hadoop-cluster01:2181

#ɾ��zookeeper�и�topic��ص�Ŀ¼���
$ rm -r /kafka/config/topics/test0
$ rm -r /kafka/brokers/topics/test0
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
      Topic: test-demo        Partition: 0    Leader: 1       Replicas: 1,5   Isr: 1,5
      Topic: test-demo        Partition: 1    Leader: 2       Replicas: 2,1   Isr: 2,1
      Topic: test-demo        Partition: 2    Leader: 3       Replicas: 3,2   Isr: 3,2
```



> PartitionCount:3 ��ʾ�û�����3��������0��1��2����Leader1��2��3��ʾ3�������ֱ�Ϊbroker1��2��3��Replicas��ʾ�÷����ı���broker id



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

# broker 5
[root@spark-slave1 ~]# ls /home/data/kafka/kafkadata/ | grep test-demo
test-demo-0

# broker 3
[root@spark-slave2 ~]# ls /home/data/kafka/kafkadata/ | grep test-demo
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

eg:

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
kafka-consumer-groups  --bootstrap-server 172.16.0.200:9092 --group group_test  --delete
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
  1. һ��partitionֻ�ܱ�һ�����������ѣ�һ�������߿���ͬʱ���Ѷ��partition����ˣ�������õ�partition������С��consumer���������ͻ������������Ѳ������ݡ����ԣ��Ƽ�partition������һ��Ҫ����ͬʱ���е�consumer��������
  2. ����partition���������ڼ�Ⱥbroker������������leader partition�Ϳ��Ծ��ȵķֲ��ڸ���broker�У�����ʹ�ü�Ⱥ���ؾ��⡣



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

�����ļ��б��Ǳ�����Kafka broker������һ��ʵ�飬����һ��topicXXX����1 partition������ÿ��segment��СΪ500MB,������producer��Kafka brokerд���������,����ͼ2��ʾsegment�ļ��б�����˵��������2������

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

�û����ȹ��������͵���Ϣ����ProducerRecord��

Ȼ�����KafkaProducer#send�������з��͡�

KafkaProducer���յ���Ϣ�����ȶ���������л���Ȼ���ϱ��ػ����Ԫ������Ϣһ���͸�partitionerȥȷ��Ŀ����������׷��д�뵽�ڴ��е���Ϣ�����(accumulator)����ʱKafkaProducer#send�����ɹ����ء�ͬʱ��KafkaProducer�л���һ��ר�ŵ�Sender IO�̸߳��𽫻�����е���Ϣ�����η��͸���Ӧ��broker�������������Ϣ�����߼���



 **�ص㣺**

 �ܹ����������̣߳�ִ��KafkaPrducer#send�߼����̡߳������ǳ�֮Ϊ���û����̡߳���

?									ִ�з����߼���IO�̡߳������ǳ�֮Ϊ��Sender�̡߳��� 

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

2��ͬ�����ͣ�����ʹ��send()���ӷ�����Ϣ�� ���᷵��һ��Future���󣬵���get()�������еȴ��� �Ϳ���֪����Ϣ�Ƿ��ͳɹ���

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

 ����KafkaProducer#send�߼��ĵ�һ������Ϊ��������Ϣ�������л�������Ŀ�����������ͼ��ʾ�� 

![](img/kafka6.webp)

����ͼ��ʾ��һ������`topic`��"`test`"����Ϣ����"`message`"����Ϣ�����л�֮����`KafkaProducer`�����Ԫ����(�����`topic`��������Ϣ��)��ͬ���������`Partitioner`ʵ�������Ŀ������ļ��㡣

#### ׷��д����Ϣ������(accumulator)

producer����ʱ�ᴴ��һ��Ĭ��32MB(��buffer.memory����ָ��)��accumulator��������ר�ű�������͵���Ϣ������֮ǰ�ڡ��ؼ��������������ᵽ��linger.ms��batch.size�Ȳ���֮�⣬�����ݽṹ�л�������һ���ر���Ҫ�ļ�����Ϣ����Ϣ������Ϣ(batches)���ü��ϱ�������һ��HashMap������ֱ𱣴���ÿ��topic�����µ�batch���У���ǰ��˵�������ǰ���topic�������з���ġ�����������ͬ��������Ϣ�����ڶ�Ӧ�����µ�batch�����С��ٸ��򵥵����ӣ�������ϢM1, M2�����͵�test��0���������ڲ�ͬ��batch��M3���͵�test��1��������ôbatches�а�������Ϣ���ǣ�{"test-0" -> [batch1, batch2], "test-1" -> [batch3]}��
 ����topic�����µ�batch�����б���������ɸ���Ϣ���Ρ�ÿ��batch������Ҫ��3�����������
 compressor: ����ִ��׷��д�����
 batch����������batch.size�������ƣ���Ϣ������׷��д�뵽�ĵط�
 thunks��������Ϣ�ص��߼��ļ���
 ��һ����Ŀ�ľ��ǽ������͵���Ϣд����Ϣ������У�������������ͼ��ʾ��

![](img/kafka7.webp)

 ��һ��ִ�����֮�������Ͻ�KafkaProducer.send������ִ������ˣ��û����߳�������������ǵȴ�Sender�̷߳�����Ϣ��ִ�з��ؽ���ˡ� 

#### Sender�߳�Ԥ������Ϣ����

��ʱ����Sender�̵߳ǳ��ˡ��ϸ���˵��Sender�߳���KafkaProducer�������һֱ���������� �����Ĺ������̻������������ģ�

- ������ѯ������Ѱ��**�����÷���׼���ķ���** ��
- ����ѯ��õĸ���batch����Ŀ��������ڵ�leader broker���з��飻
- ��������batchͨ���ײ㴴����**Socket����**���͸�����broker��
- �ȴ��������˷���response������

Ϊ��˵���ϵķ��㣬���ǻ���ͼ�ķ�ʽ������Sender�̵߳Ĺ���ԭ��

![](img/kafka8.webp)

####  

#### Sender�̴߳���response

��ͼ��Sender�̻߳ᷢ��PRODUCE�������Ӧ��broker��broker�������֮���Ͷ�Ӧ��PRODUCE response��һ��Sender�߳̽��յ�response������(������Ϣ����˳��)����batch�еĻص�����������ͼ��ʾ��

![](img/kafka9.webp)

������һ����producer������Ϣ�Ϳ���������100%����ˡ�ͨ����4�����ǿ��Կ����°汾producer�����¼���ȫ���첽���̡�**����ڵ���producerǰ���Ǿ���Ҫ���������ƿ�����������û����̻߳�����Sender�߳�**��
 ����KafkaProducer���̰߳�ȫ�ģ������ʹ���������ֻ�����ʹ�÷�����



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
#	acks=all�������߻�ȴ����и����ɹ�д�����Ϣ�����ַ�ʽ���ȫ�ģ��ܹ���֤��Ϣ����ʧ�������ӳ�Ҳ�����ġ�
request.required.acks = 0, 1, n, -1/all

# ���������߻��巢�͵���Ϣ���ڴ��С�����Ӧ�õ���send�������ٶȴ��������߷��͵��ٶȣ�
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

 Kafka��֤������˳��Ҳ����˵�������������һ����˳������Ϣ��Kafka��ĳ����������ôKafka�ڷ����ڲ����ִ�˳�򣬶���������Ҳ����ͬ����˳�����ѡ����ǣ�Ӧ�õ���send������˳���ʵ�ʷ�����Ϣ��˳��һ����һ�µġ��ٸ����ӣ����retries������Ϊ0����max.in.flights.requests.per.session��������1����ô�п��ܵ�һ��������Ϣд��ʧ�ܣ����ǵڶ���������Ϣд��ɹ���Ȼ���һ��������Ϣ����д��ɹ�����ô���˳������ġ���ˣ������Ҫ��֤��Ϣ˳�򣬽�������max.in.flights.requests.per.sessionΪ1�����������ڵ�һ��������Ϣ����ʧ������ʱ���ڶ���������Ϣ��Ҫ�ȴ��� 



### ���л�

 [http://www.dengshenyu.com/%E5%88%86%E5%B8%83%E5%BC%8F%E7%B3%BB%E7%BB%9F/2017/11/12/kafka-producer.html](http://www.dengshenyu.com/�ֲ�ʽϵͳ/2017/11/12/kafka-producer.html)



## Consumers

- Kafka�ṩ������consumer api����Ϊ`high-level api`��`sample-api`��

  - **Sample-api** ��һ���ײ��API��ͨ��ֱ�Ӳ����ײ�API��ȡ���ݵķ�ʽ��ȡKafka�е����ݣ���Ҫ���и���������ƫ���������ԡ��ŵ㣺�ɲ�����ǿ��ȱ�㣺������Զ��ԱȽϸ��ӡ�(����ࣺSimpleConsumer) 
  - **High Level Consumer API**���߶ȳ����Kafka������API�����ײ�����ȡ���ݡ�����offset������ƫ�����Ȳ������ε���ֱ�ӽ������������Ĵ������ṩ����д�������Ա���ŵ��ǣ������򵥣�ȱ�㣺�ɲ�����̫��޷������Լ���ҵ�񳡾�ѡ����ʽ��(����ࣺConsumerConnector)

- **��kafka�У���ǰ����������Ϣ��offsetֵ����consumer��ά����**����ˣ�consumer�����Լ�������ζ�ȡkafka�е����ݡ����磬consumer����ͨ������offsetֵ���������������ѹ������ݡ�������û�б����ѣ�kafka�ᱣ������һ��ʱ�䣬���ʱ��������**������**�ģ�ֻ�е��˹���ʱ�䣬kafka�Ż�ɾ����Щ���ݡ�

- High-level API��װ�˶Լ�Ⱥ��һϵ��broker�ķ��ʣ�����͸��������һ��topic�����Լ�ά������������Ϣ��״̬����ÿ�����ѵĶ�����һ����Ϣ��

  High-level API��֧���������ʽ����topic�����consumers��ͬһ����������ôkafka���൱��һ��������Ϣ���񣬶�����consumer�����������Ӧpartition�е����ݡ���consumers�в�ͬ����������ô��ʱkafka���൱��һ���㲥���񣬻��topic�е�������Ϣ�㲥��ÿ��consumer��

  

  High level api��Low level api�����consumer���Եģ���producer�޹ء�

  

  High level api��consumer����partition��offsite�Ǵ���zookeeper�ϡ�High level api ����������һ���߳�ȥÿ��һ��ʱ�䣬offsite�Զ�ͬ����zookeeper�ϡ����仰˵�����ʹ����High level api�� ÿ��messageֻ�ܱ���һ�Σ�һ����������message֮��������consumer�Ĵ����Ƿ�ok��High level api������һ���̻߳��Զ��İ�offiste+1ͬ����zookeeper�ϡ����consumer��ȡ���ݳ������⣬offsiteҲ����zookeeper��ͬ������ˣ����consumer����ʧ���ˣ������ִ����һ�����������ǲ��Ե���Ϊ����ˣ�Best Practice��һ��consumer����ʧ�ܣ�ֱ��������conusmer group��Exception��ֹ��������������һ�������Ƕ�ʧ�ˣ���Ϊ��zookeeper�����offsite�Ѿ�+1�ˡ����ٴ�����conusmer group��ʱ���Ѿ�����һ����ʼ��ȡ�����ˡ�

  

  Low level api��consumer����partition��offsite��consumer�Լ��ĳ�����ά��������ͬ����zookeeper�ϡ�����Ϊ��kafka manager�ܹ�����ļ�أ�һ��Ҳ���ֶ���ͬ����zookeeper�ϡ������ĺô���һ����ȡĳ��message��consumerʧ���ˣ�����message��offsite�����Լ�ά�������ǲ���+1���´���������ʱ�򣬻�������offsite��ʼ����������������exactly once�������ݵ�׼ȷ���б�֤��

- Consumer��Partition�Ĺ�ϵ�� **Consumer Rebalance** ����
  - ���consumer��partition�࣬���˷ѣ���Ϊkafka���������һ��partition���ǲ��������ģ�����consumer����Ҫ����partition��
  - ���consumer��partition�٣�һ��consumer���Ӧ�ڶ��partitions��������Ҫ�������consumer����partition��������ᵼ��partition��������ݱ�ȡ�Ĳ�����
  - ���consumer�Ӷ��partition�������ݣ�����֤���ݼ��˳���ԣ�kafkaֻ��֤��һ��partition������������ģ������partition�����������˳����в�ͬ
  - ����consumer��broker��partition�ᵼ��rebalance������rebalance��consumer��Ӧ��partition�ᷢ���仯
  - High-level�ӿ��л�ȡ�������ݵ�ʱ���ǻ�block��



# ��ϢͶ������

kafka֧��3����ϢͶ������
	

- At most once�����һ�Σ���Ϣ���ܻᶪʧ���������ظ�

  > \>1. ���á�enable.auto.commit�� Ϊ true.
  >
  > \>2. ���� ��auto.commit.interval.ms�� Ϊһ����С��ֵ.

- At least once������һ�Σ���Ϣ���ᶪʧ�����ܻ��ظ�

  > 1. ���á�enable.auto.commit�� Ϊ false ����
  >
  > ���á�enable.auto.commit�� Ϊ true �����á�auto.commit.interval.ms�� Ϊһ���ϴ��ֵ.

- Exactly once��ֻ��һ�Σ���Ϣ����ʧ���ظ���ֻ������һ��



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



# kafka�ٶȿ�



## ˳���д

Kafka ����Ϣ�ǲ���׷�ӵ��ļ��еģ��������ʹ�����Գ�����ô��̵�˳���д������



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

### ��ͳ�ļ�IO

 ��ͳģʽ�£��ļ�����Ĳ������̣�����һ������Ҫ���ļ����ݷ��͵����硣������̷������û��ռ䣬�ļ�������socket����Ӳ����Դ������֮����һ���ں˿ռ䣬�ڲ���ϵͳ�ڲ�����������Ϊ:



> 1������sendfileʵ��Zero Copy����read�������ļ����ݱ�copy���ں˻�����
>
> 2��read�������أ��ļ����ݴ��ں˻�����copy���û�������
>
> 3��write�������ã����ļ����ݴ��û�������copy���ں���socket��صĻ�������
>
> 4�����ݴ�socket������copy�����Э�����档



![](img/kafka2.webp)

����ϸ���Ǵ�ͳread/write��ʽ���������ļ�����ķ�ʽ�����ǿ��Կ�������������̵��У��ļ�����ʵ�����Ǿ������Ĵ�copy������

> Ӳ�̡�>�ں�buf��>�û�buf��>socket��ػ�������>Э������



### sendfile

��kafka���õġ�**�㿽��(zero-copy)**��ϵͳ���û��ƣ������������û����������Ŀ���������һ�����̿ռ���ڴ�ռ��ֱ��ӳ�䣬���ݲ��ٸ��Ƶ����û�̬��������ϵͳ�������л�����2�Σ���������һ������

![](img/kafka3.webp)

 sendfileϵͳ���ã��Լ������Ϻ����������ļ�֮������ݴ��䡣sendfile�����벻�����������ݸ��ƣ����������������л��� 

```c++
sendfile(socket, file, len);
```

�����������£�

> 1��sendfileϵͳ���ã��ļ����ݱ�copy���ں˻�����
>
> 2���ٴ��ں˻�����copy���ں���socket��صĻ�����
>
> 3�������socket��صĻ�����copy��Э������



## �ļ��ֶ�

 Kafka �Ķ��� topic ����Ϊ�˶���� partition, ÿ�� partition �ַ�Ϊ�˶�� segment������һ�������е���Ϣʵ�����Ǳ����� N ���Ƭ���ļ��С� 

![](img/kafka4.webp)

 ͨ���ֶεķ�ʽ��ÿ���ļ��������Ƕ�һ��С�ļ��Ĳ������ǳ���㣬ͬʱҲ�����˲��д��������� 



## �������� batch



## ����ѹ��



## ҳ����
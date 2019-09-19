# Kafka

[Kafka ���Ž���-��������](<https://lotabout.me/2018/kafka-introduction/>)

[kafka����](<https://kafka.apache.org/>)

[confluent](<https://www.confluent.io/>)

## 1.��ظ���

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

- ÿ��topic���Է�Ϊ���partition��һ��partition�൱��һ����Ŀ¼��ÿ��partition�����ж����С��ȵ�segment�ļ������segment����message��ɵģ���ÿһ����segment��һ���ɴ�С��ȵ�message��ɡ�segment��С������������server.properties�ļ������á�offset���ڶ�λλ�ڶ����Ψһ��Ϣ��

  

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
# ����ô���ֿ��ܵ�delivery guarantee��
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

  

## 2.ʵ������

### 2.1����һ�� test2 test-demo��ע������� partitions ����Ϊ 3����

```shell
[root@spark-master ~]# kafka-topics --create --partitions 3 --replication-factor 2  --zookeeper spark-master:2181 --topic test-demo

Created topic "test-demo".
```

### 2.2�鿴`test-demo`

```shell
[root@spark-master ~]# kafka-topics --describe  --zookeeper spark-master:2181 | grep test-demo
Topic:test-demo PartitionCount:3        ReplicationFactor:2     Configs:
      Topic: test-demo        Partition: 0    Leader: 1       Replicas: 1,5   Isr: 1,5
      Topic: test-demo        Partition: 1    Leader: 2       Replicas: 2,1   Isr: 2,1
      Topic: test-demo        Partition: 2    Leader: 3       Replicas: 3,2   Isr: 3,2
```

> PartitionCount:3 ��ʾ�û�����3��������0��1��2����Leader1��2��3��ʾ3�������ֱ�Ϊbroker1��2��3��Replicas��ʾ�÷����ı���broker id

### 2.3����kafka����Ŀ¼�鿴

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

### 2.4�������ݣ�producer��

```shell
[root@spark-slave0 ~]# kafka-console-producer --broker-list spark-slave1:9092 --topic test-demo
>hello:world
>question:what is your name?
>answer:jams
```

### 2.5�������ݣ�consumer��

```shell
[root@spark-slave0 ~]# kafka-console-consumer --bootstrap-server spark-slave1:9092 --topic test-demo [--consumer-property group.id=group_test] --from-beginning
question:what is your name?
answer:jams
hello:world
```

### 2.6�鿴����ƫ������offset��

```shell
[root@spark-slave0 confluent-4.0.0]# bin/kafka-consumer-groups --bootstrap-server spark-slave1:9092 --describe --group group_test
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

```shell
$ bin/kafka-run-class kafka.tools.GetOffsetShell --broker-list spark-slave1:9092 --topic test-demo --time -1

test-demo:2:2
test-demo:1:2
test-demo:0:4
```

### 2.7�����е���offset

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

�鿴kafka�����ļ�.log

```shell
# --print-data-log �Ǳ�ʾ�鿴��Ϣ���ݵģ����Ӵ����ǲ鿴������ϸ����Ϣ���ݡ����Ҫ�鿴���log�ļ������ö��ŷָ���
$ kafka-run-class kafka.tools.DumpLogSegments --files /home/data/kafka/kafkadata/test-demo-1/00000000000000000000.log --print-data-log
```

�鿴kafka�����ļ�.index

```shell
$ kafka-run-class kafka.tools.DumpLogSegments --files /home/data/kafka/kafkadata/test-demo-1/00000000000000000000.index
```



## 3.kafka�ļ��洢����

### 3.1topic��partition�洢�ֲ�

- ͬһ��topic���ж����ͬpartition��ÿ��partitionΪһ��Ŀ¼��partiton��������Ϊtopic����+�������

- Partition��һ��Queue�Ľṹ��ÿ��Partition�е���Ϣ��������ģ���������Ϣ������׷�ӵ�Partition�ϣ����е�ÿһ����Ϣ����������һ��Ψһ��offsetֵ��
- Kafka��Ⱥ�ᱣ�����е���Ϣ��������Ϣ��û�б����ѣ����ǿ���**�趨��Ϣ�Ĺ���ʱ��**��ֻ�й��ڵ����ݲŻᱻ�Զ�������ͷŴ��̿ռ䡣
- Kafkaֻά����Partition�е�offsetֵ����Ϊ���offsite��ʶ�����partition��message���ѵ������ˡ�Consumerÿ����һ����Ϣ��offset�ͻ��1����ʵ��Ϣ��״̬��ȫ����Consumer���Ƶģ�Consumer���Ը��ٺ��������offsetֵ�������Ļ�Consumer�Ϳ��Զ�ȡ����λ�õ���Ϣ��
- Kafka�е�topic����partition����ʽ��ŵģ�ÿһ��topic��������������partition������Partition���������������topic��message��������Producer����������ʱ���ᰴ��һ��������������ǿ����Զ���ģ�����Ϣ������topic�ĸ���partition�С����潫�ĸ���������partitionΪ��λ�ģ�����ֻ��һ��partition�ĸ����ᱻѡ�ٳ�leader��Ϊ��д�á�
- �������partitionֵ
  1. һ��partitionֻ�ܱ�һ�����������ѣ�һ�������߿���ͬʱ���Ѷ��partition����ˣ�������õ�partition������С��consumer���������ͻ������������Ѳ������ݡ����ԣ��Ƽ�partition������һ��Ҫ����ͬʱ���е�consumer��������
  2. ����partition���������ڼ�Ⱥbroker������������leader partition�Ϳ��Ծ��ȵķֲ��ڸ���broker�У�����ʹ�ü�Ⱥ���ؾ��⡣

### 3.2partiton���ļ��洢��ʽ (partition��linux�������Ͼ���һ��Ŀ¼)

- ÿ��partion(Ŀ¼)�൱��һ�������ļ���ƽ�����䵽�����С���segment(��)�����ļ��С���ÿ����segment file��Ϣ������һ����ȣ��������Է���old segment file���ٱ�ɾ����
- ÿ��partitonֻ��Ҫ֧��˳���д�����ˣ�segment�ļ�����������**��������ò���**������

### 3.3partiton��segment�ļ��洢�ṹ

producer��message��ĳ��topic��message�ᱻ���ȵķֲ������partition�ϣ����������û�ָ���Ļص��������зֲ�����kafka broker�յ�message����Ӧpartition�����һ��segment����Ӹ���Ϣ����ĳ��segment�ϵ���Ϣ�����ﵽ����ֵ����Ϣ����ʱ�䳬����ֵʱ��segment�ϵ���Ϣ�ᱻflush�����̣�ֻ��flush�������ϵ���Ϣconsumer�������ѣ�segment�ﵽһ���Ĵ�С�󽫲���������segmentд���ݣ�broker�ᴴ���µ�segment��



ÿ��part���ڴ��ж�Ӧһ��index����¼ÿ��segment�еĵ�һ����Ϣƫ�ơ�

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

### ����˵����

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

### 3.4��partition�����ͨ��offset����message

�����ȡoffset=368776��message����Ҫͨ������2��������ҡ�

- ��һ������segment file

  ����ͼ2Ϊ��������00000000000000000000.index��ʾ�ʼ���ļ�����ʼƫ����(offset)Ϊ0.�ڶ����ļ� 00000000000000368769.index����Ϣ����ʼƫ����Ϊ368770 = 368769 + 1.ͬ�����������ļ�00000000000000737337.index����ʼƫ����Ϊ737338=737337 + 1�����������ļ��������ƣ�����ʼƫ����������������Щ�ļ���ֻҪ����offset **���ֲ���**�ļ��б��Ϳ��Կ��ٶ�λ�������ļ���

  ��offset=368776ʱ��λ��00000000000000368769.index|log

- �ڶ���ͨ��segment file����messageͨ����һ����λ��segment file����offset=368776ʱ�����ζ�λ��00000000000000368769.index��Ԫ��������λ�ú� 00000000000000368769.log������ƫ�Ƶ�ַ��Ȼ����ͨ��00000000000000368769.log˳�����ֱ�� offset=368776Ϊֹ��

segment index file��ȡϡ�������洢��ʽ�������������ļ���С��ͨ��mmap����ֱ���ڴ������ϡ������Ϊ�����ļ���ÿ����Ӧmessage����һ��Ԫ����ָ��,�� �ȳ���������ʡ�˸���Ĵ洢�ռ䣬������������Ҫ���ĸ����ʱ�䡣

## 4.Kafka ������-������

### 4.1Producers

- Producersֱ�ӷ�����Ϣ��broker�ϵ�leader partition������Ҫ�����κ��н������·��ת����Ϊ��ʵ��������ԣ�kafka��Ⱥ�е�ÿ��broker��������Ӧproducer�����󣬲�����topic��һЩԪ��Ϣ����ЩԪ��Ϣ������Щ�����Ǵ��ģ�topic��leader partition�����ģ��ֽ׶���Щleader partition�ǿ���ֱ�ӱ����ʵġ�

- **Producer�ͻ����Լ���������Ϣ�����͵���Щpartition��**ʵ�ֵķ�ʽ������������䡢ʵ��һ��������ؾ����㷨������ָ��һЩ�����㷨��Kafka�ṩ�˽ӿڹ��û�ʵ���Զ����partition���û�����Ϊÿ����Ϣָ��һ��partitionKey��ͨ�����key��ʵ��һЩhash�����㷨�����磬��userid��Ϊpartitionkey�Ļ�����ͬuserid����Ϣ���ᱻ���͵�ͬһ��partition��

- ��`Batch`�ķ�ʽ�������ݿ��Լ������ߴ���Ч�ʣ�kafka Producer ���Խ���Ϣ���ڴ����ۼƵ�һ����������Ϊһ��batch��������Batch��������С����ͨ��Producer�Ĳ������ƣ�����ֵ��������Ϊ�ۼƵ���Ϣ����������500�������ۼƵ�ʱ��������100ms�������ۼƵ����ݴ�С(64KB)��ͨ������batch�Ĵ�С�����Լ�����������ʹ���IO�Ĵ�������Ȼ�������������Ҫ��Ч�ʺ�ʱЧ�Է�����һ��Ȩ�⡣

- Producers�����첽�Ĳ��е���kafka������Ϣ������ͨ��producer�ڷ�������Ϣ֮���õ�һ��future��Ӧ�����ص���offsetֵ���߷��͹����������Ĵ����������и��ǳ���Ҫ�Ĳ�����acks��,�������������producerҪ��leader partition �յ�ȷ�ϵĸ������������acks��������Ϊ0����ʾproducer����ȴ�broker����Ӧ�����ԣ�producer�޷�֪����Ϣ�Ƿ��ͳɹ��������п��ܻᵼ�����ݶ�ʧ����ͬʱ��acksֵΪ0��õ�����ϵͳ��������

  ��acks����Ϊ1����ʾproducer����leader partition�յ���Ϣʱ�õ�broker��һ��ȷ�ϣ��������и��õĿɿ��ԣ���Ϊ�ͻ��˻�ȴ�ֱ��brokerȷ���յ���Ϣ��������Ϊ-1��producer�������б��ݵ�partition�յ���Ϣʱ�õ�broker��ȷ�ϣ�������ÿ��Եõ���ߵĿɿ��Ա�֤��

- Kafkaû���޶�������Ϣ�Ĵ�С���������Ƽ���Ϣ��С��Ҫ����1MB,ͨ��һ����Ϣ��С����1~10k

### 4.2Consumers

- Kafka�ṩ������consumer api����Ϊ`high-level api`��`sample-api`��

  - **Sample-api** ��һ���ײ��API��ͨ��ֱ�Ӳ����ײ�API��ȡ���ݵķ�ʽ��ȡKafka�е����ݣ���Ҫ���и���������ƫ���������ԡ��ŵ㣺�ɲ�����ǿ��ȱ�㣺������Զ��ԱȽϸ��ӡ�(����ࣺSimpleConsumer) 
  - **High Level Consumer API**���߶ȳ����Kafka������API�����ײ�����ȡ���ݡ�����offset������ƫ�����Ȳ������ε���ֱ�ӽ������������Ĵ������ṩ����д�������Ա���ŵ��ǣ������򵥣�ȱ�㣺�ɲ�����̫��޷������Լ���ҵ�񳡾�ѡ����ʽ��(����ࣺConsumerConnector)

- **��kafka�У���ǰ����������Ϣ��offsetֵ����consumer��ά����**����ˣ�consumer�����Լ�������ζ�ȡkafka�е����ݡ����磬consumer����ͨ������offsetֵ���������������ѹ������ݡ�������û�б����ѣ�kafka�ᱣ������һ��ʱ�䣬���ʱ��������**������**�ģ�ֻ�е��˹���ʱ�䣬kafka�Ż�ɾ����Щ���ݡ�

- High-level API��װ�˶Լ�Ⱥ��һϵ��broker�ķ��ʣ�����͸��������һ��topic�����Լ�ά������������Ϣ��״̬����ÿ�����ѵĶ�����һ����Ϣ��

  High-level API��֧���������ʽ����topic�����consumers��ͬһ����������ôkafka���൱��һ��������Ϣ���񣬶�����consumer�����������Ӧpartition�е����ݡ���consumers�в�ͬ����������ô��ʱkafka���൱��һ���㲥���񣬻��topic�е�������Ϣ�㲥��ÿ��consumer��

  

  High level api��Low level api�����consumer���Եģ���producer�޹ء�

  

  High level api��consumer����partition��offsite�Ǵ���zookeeper�ϡ�High level api ����������һ���߳�ȥÿ��һ��ʱ�䣬offsite�Զ�ͬ����zookeeper�ϡ����仰˵�����ʹ����High level api�� ÿ��messageֻ�ܱ���һ�Σ�һ����������message֮��������consumer�Ĵ����Ƿ�ok��High level api������һ���̻߳��Զ��İ�offiste+1ͬ����zookeeper�ϡ����consumer��ȡ���ݳ������⣬offsiteҲ����zookeeper��ͬ������ˣ����consumer����ʧ���ˣ������ִ����һ�����������ǲ��Ե���Ϊ����ˣ�Best Practice��һ��consumer����ʧ�ܣ�ֱ��������conusmer group��Exception��ֹ��������������һ�������Ƕ�ʧ�ˣ���Ϊ��zookeeper�����offsite�Ѿ�+1�ˡ����ٴ�����conusmer group��ʱ���Ѿ�����һ����ʼ��ȡ�����ˡ�

  

  Low level api��consumer����partition��offsite��consumer�Լ��ĳ�����ά��������ͬ����zookeeper�ϡ�����Ϊ��kafka manager�ܹ�����ļ�أ�һ��Ҳ���ֶ���ͬ����zookeeper�ϡ������ĺô���һ����ȡĳ��message��consumerʧ���ˣ�����message��offsite�����Լ�ά�������ǲ���+1���´���������ʱ�򣬻�������offsite��ʼ����������������exactly once�������ݵ�׼ȷ���б�֤��

- Consumer��Partition�Ĺ�ϵ��
  - ���consumer��partition�࣬���˷ѣ���Ϊkafka���������һ��partition���ǲ��������ģ�����consumer����Ҫ����partition��
  - ���consumer��partition�٣�һ��consumer���Ӧ�ڶ��partitions��������Ҫ�������consumer����partition��������ᵼ��partition��������ݱ�ȡ�Ĳ�����
  - ���consumer�Ӷ��partition�������ݣ�����֤���ݼ��˳���ԣ�kafkaֻ��֤��һ��partition������������ģ������partition�����������˳����в�ͬ
  - ����consumer��broker��partition�ᵼ��rebalance������rebalance��consumer��Ӧ��partition�ᷢ���仯
  - High-level�ӿ��л�ȡ�������ݵ�ʱ���ǻ�block��
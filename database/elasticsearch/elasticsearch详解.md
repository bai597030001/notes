# 安装使用

## 1.下载解压



## 2.修改配置

1、max file descriptors [4096] for elasticsearch process is too low, increase to at least [65536]

　　每个进程最大同时打开文件数太小，可通过下面2个命令查看当前数量

```shell
ulimit -Hn
ulimit -Sn
```

　　修改/etc/security/limits.conf文件，增加配置，用户退出后重新登录生效

```shell
*               soft    nofile          65536
*               hard    nofile          65536
```



2、max number of threads [3818] for user [es] is too low, increase to at least [4096]

　　问题同上，最大线程个数太低。修改配置文件/etc/security/limits.conf（和问题1是一个文件），增加配置

```shell
*               soft    nproc           4096
*               hard    nproc           4096
```



3、max virtual memory areas vm.max_map_count [65530] is too low, increase to at least [262144]

　　修改/etc/sysctl.conf文件，增加配置vm.max_map_count=262144

```shell
vi /etc/sysctl.confsysctl -p
```

　　执行命令sysctl -p生效



## 3.启动暂停

```shell
$ /opt/elasticsearch-6.4.3/bin/elasticsearch [-d]

$ kill -9 pid
```



## 集群健康

```shell
$ curl -X GET "localhost:9200/_cat/health?v"
```

响应：

```shell
epoch      timestamp cluster       status node.total node.data shards pri relo init unassign pending_tasks max_task_wait_time active_shards_percent
1533625274 15:01:14  elasticsearch green           1         1      0   0    0    0        0             0                  -                100.0%
```

我们可以看到，我们命名为“elasticsearch”的集群现在是green状态。

无论何时我们请求集群健康时，我们会得到green, yellow, 或者 red 这三种状态。

- Green ： everything is good（一切都很好）（所有功能正常）
- Yellow ： 所有数据都是可用的，但有些副本还没有分配（所有功能正常）
- Red ： 有些数据不可用（部分功能正常）

从上面的响应中我们可以看到，集群"elasticsearch"总共有1个节点，0个分片因为还没有数据。



## 节点列表

请求：

```shell
$ curl -X GET "localhost:9200/_cat/nodes?v"
```

响应：

```shell
ip        heap.percent ram.percent cpu load_1m load_5m load_15m node.role master name
127.0.0.1           15          53   0    0.03    0.03     0.05 mdi       *      Px524Ts
```

可以看到集群中只有一个节点，它的名字是“Px524Ts”



## 查看全部索引

请求：

```shell
$ curl -X GET "localhost:9200/_cat/indices?v"
```

响应：

```shell
health status index uuid pri rep docs.count docs.deleted store.size pri.store.size
```

上面的输出意味着：我们在集群中没有索引



```shell
health status index                           uuid                   pri rep docs.count docs.deleted store.size pri.store.size
green  open   .monitoring-kibana-6-2019.12.29 Z37-9foKQ2WC8yPm6qup8w   1   0       9550            0      1.3mb          1.3mb
green  open   pms                             Fte4N2fNS8GvF095Z-70UA   1   0          0            0       261b           261b
green  open   .monitoring-es-6-2019.12.29     kVDiJXNGQMKZN5O4zwWyPQ   1   0      21520           15      7.9mb          7.9mb
```


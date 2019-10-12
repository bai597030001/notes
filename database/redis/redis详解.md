# 通用命令

## 服务相关

```shell
$ BGREWRITEAOF # 异步执行一个 AOF（AppendOnly File） 文件重写操作

$ BGSAVE 
在后台异步保存当前数据库的数据到磁盘

$ CLIENT KILL [ip:port] [ID client-id] 
关闭客户端连接

$ CLIENT LIST 
获取连接到服务器的客户端连接列表

$ CLIENT GETNAME 
获取连接的名称

$ CLIENT PAUSE timeout 
在指定时间内终止运行来自客户端的命令

$ CLIENT SETNAME connection-name 
设置当前连接的名称

$ CLUSTER SLOTS 
获取集群节点的映射数组

$ COMMAND 
获取 Redis 命令详情数组

$ COMMAND COUNT 
获取 Redis 命令总数

$ TIME 
返回当前服务器时间

$ COMMAND INFO command-name [command-name ...] 
获取指定 Redis 命令描述的数组

$ config get *
$ CONFIG SET parameter value 
获取/修改 redis 配置参数，无需重启

$ DBSIZE 
返回当前数据库的 key 的数量

$ DEBUG OBJECT key 
获取 key 的调试信息

$ DEBUG SEGFAULT 
让 Redis 服务崩溃

$ FLUSHALL 
删除所有数据库的所有key

$ FLUSHDB 
删除当前数据库的所有key

$ INFO [section] 
获取 Redis 服务器的各种信息和统计数值

$ lastsave 
返回最近一次 Redis 成功将数据保存到磁盘上的时间，以 UNIX 时间戳格式表示

$ MONITOR # 有高手的文章说这个会急剧降低redis性能，只能在测试环境使用。
实时打印出 Redis 服务器接收到的命令，调试用

$ ROLE 
返回主从实例所属的角色

$ SAVE 
同步保存数据到硬盘

$ SHUTDOWN [NOSAVE] [SAVE] 
异步保存数据到硬盘，并关闭服务器

$ SLAVEOF host port 
将当前服务器转变为指定服务器的从属服务器(slave server)

$ SLOWLOG subcommand [argument] 
管理 redis 的慢日志

$ SYNC 
用于复制功能(replication)的内部命令
```





# 数据类型

`Redis key`值是二进制安全的，这意味着可以用任何二进制序列作为`key`值，从形如”`foo`”的简单字符串到一个`JPEG`文件的内容都可以。空字符串也是有效`key`值。



```shell
$ EXISTS

$ DEL

# 返回key对应的值的存储类型
$ TYPE

$ EXPIRE #设置超时时间
$ PERSIST#去除超时时间
$ TTL # 查看key对应的值剩余存活时间

$ rename oldkey newkey
# 向key的字符串追加拼接
$ append key value
```





## string

```shell
# 值可以是任何种类的字符串（包括二进制数据），例如你可以在一个键下保存一副jpeg图片。值的长度不能超过512 MB。
$ set mykey someValue
$ get mykey

$ mget
$ mset

# 若存在则不操作
$ setnx key value
# 如果其中一个key已经存在了，则都不设置。这些操作都是原子的
$ msetnx key1 value1 key2 value2 ... keyN valueN



# 设置key的值为value，并在timeout秒后失效，key将被删除
$ setex keyName timeout timeValue
```

`SET` 命令有些有趣的操作，例如，当`key`存在时`SET`会失败，或相反的，当`key`不存在时它只会成功。



```shell
# String当作整数递增/递减
$ INCR/INCRBY
$ DECR/DECRBY
```

**注意**：该操作是原子操作，就是说即使多个客户端对同一个key发出[INCR](http://www.redis.cn/commands/incr.html)命令，也决不会导致竞争的情况。



```shell
# 为key设置新值并且返回原值
$ GETSET
```





## hash



```shell
$ HSET key field value:key是对象名，field是属性，value是值；
	
$ HMSET key field value [field value ...]:同时设置多个属性

$ HGET key field：获取该对象的该属性

$ HMGET key field value [field value ...]：获取多个属性值

$ HGETALL key:获取对象的所有信息

$ HKEYS key：获取对象的所有属性

$ HVALS key：获取对象的所有属性值

$ HDEL key field：删除对象的该属性

$ HEXISTS key field:查看对象是否存在该属性

$ HINCRBY key field value:原子自增操作，只能是integer的属性值可以使用；

$ HLEN key: 获取属性的个数。
```





## list

`Redis lists`基于`Linked Lists`实现。



```shell
$ lpush 1,2,3,......
$ rpush 1,2,3,......
$ lrange 0, -1
# LRANGE 带有两个索引，一定范围的第一个和最后一个元素。这两个索引都可以为负来告知Redis从尾部开始计数，因此-1表示最后一个元素，-2表示list中的倒数第二个元素，以此类推。

# 从list中删除元素并同时返回删除的值
$ lpop
$ rpop

# 获取keyList的长度大小
$ llen keyList

# 获取该索引下的元素
$ index key index

# 删除count个value。（count为正数,从头开始，删除count个value元素；count为负，则从尾部向头删除|count|个value元素；count为0，则所有的元素为value的都删除）
$ lrem key count value

# 截取指定长度
$ ltrim mylist 0 2
$ rtrim mylist 0 2

# 源队列srckey，目标队列dstkey，将srckey的最后一个移除，并放到dstkey的第一个。
$ rpoplpush srckey dstkey
```



适用场景：

- list可被用来实现聊天系统。还可以作为不同进程间传递消息的队列。（关键是，你可以每次都以原先添加的顺序访问数据。）



## set

- 无序且唯一集合



```shell
$ sadd key value : 向set添加元素
	
$ srem key value ：从set中移除元素

$ smembers key : 取出所有set元素

$ sismember key value: 查看value是否存在set中

$ sunion key1 key2 ... keyN:将所有key合并后取出来，相同的值只取一次

$ scard key : 获取set中元素的个数

$ srandmember key: 随机取出一个

$ sdiff key1 key2 ... keyN：获取第一set中不存在后面几个set里的元素。

$ sdiffstore dstkey key1 key2 ... keyN：和sdiff相同，获取key1中不存在其他key里的元素，但要存储到dstkey中。

$ sinter key1 key2 ... keyN:取出这些set的交集

$ sintersotre dstkey key1 key2 ... keyN：取出这些key的交集并存储到dstkey

$ smove srckey dstkey member：将元素member从srckey中转移到dstkey中，这个操作是原子的。
```





## sorted set

- 有序且唯一集合



```shell
$ zadd key score member：向有序set中添加元素member，其中score为分数，默认升序；

$ zrange key start end [WITHSCORES]:获取按score从低到高索引范围内的元素，索引可以是负数，
-1表示最后一个，-2表示倒数第二个，即从后往前。withscores可选，表示获取包括分数。

$ zrecrange key start end [WITHSCORES]：同上，但score从高到低排序。

$ zcount key min max：获取score在min和max范围内的元素的个数

$ zcard key:获取集合中元素的个数。

$ zincrby key increment member:根据元素，score原子增加increment.

$ zremrangebyscore key min max:清空集合内的score位于min和max之间的元素。

$ zrank key member:获取元素的索引（照score从低到高排列）。

$ zrem key member:移除集合中的该元素

$ zsocre key member:获取该元素的score
```





# redis-replication







# transactions

`Redis` 事务可以一次执行多个命令， 并且带有以下两个重要的保证：

- 批量操作在发送 EXEC 命令前被放入队列缓存。 
- 收到 EXEC 命令后进入事务执行，事务中任意命令执行失败，其余的命令依然被执行。
- 在事务执行过程，其他客户端提交的命令请求不会插入到事务执行命令序列中。



一个事务从开始到执行会经历以下三个阶段： 
- 开始事务。
- 命令入队。
- 执行事务。



```shell
$ redis 127.0.0.1:6379> MULTI
OK

$ redis 127.0.0.1:6379> SET book-name "Mastering C++ in 21 days"
QUEUED

$ redis 127.0.0.1:6379> GET book-name
QUEUED

$ redis 127.0.0.1:6379> SADD tag "C++" "Programming" "Mastering Series"
QUEUED

$ redis 127.0.0.1:6379> SMEMBERS tag
QUEUED

$ redis 127.0.0.1:6379> EXEC
1) OK
2) "Mastering C++ in 21 days"
3) (integer) 3
4) 1) "Mastering Series"
2) "C++"
3) "Programming"
```



单个 Redis 命令的执行是原子性的，但 Redis 没有在事务上增加任何维持原子性的机制，所以 Redis 事务的执行并不是原子性的。



事务可以理解为一个打包的批量执行脚本，但批量指令并非原子化的操作，中间某条指令的失败不会导致前面已做指令的回滚，
也不会造成后续的指令不做。



事务相关命令

```shell
$ DISCARD 	取消事务，放弃执行事务块内的所有命令
		
$ EXEC 		执行所有事务块内的命令

$ MULTI 		标记一个事务块的开始

$ UNWATCH 	取消 WATCH 命令对所有 key 的监视

$ WATCH key [key ...] 	监视一个(或多个) key ，如果在事务执行之前这个(或这些) key 被其他命令所改动，那么事务将被打断
```





# persistence

redis提供了RDB（Redis DataBase）和AOF（Append Only File）两种持久化方式

## RDB

RDB，简而言之，就是在不同的时间点，将redis存储的数据生成快照并存储到磁盘等介质上；



RDB方式，是将redis某一时刻的数据持久化到磁盘中，是一种快照式的持久化方法。


redis在进行数据持久化的过程中，会先将数据写入到一个临时文件中，待持久化过程都结束了，
才会用这个临时文件替换上次持久化好的文件。正是这种特性，让我们可以随时来进行备份，
因为快照文件总是完整可用的。

对于RDB方式，redis会单独创建（fork）一个子进程来进行持久化，而主进程是不会进行任何IO操作的，
这样就确保了redis极高的性能。

如果需要进行大规模数据的恢复，且对于数据恢复的完整性不是非常敏感，那RDB方式要比AOF方式更加的高效。
虽然RDB有不少优点，但它的缺点也是不容忽视的。如果你对数据的完整性非常敏感，
那么RDB方式就不太适合你，因为即使你每5分钟都持久化一次，当redis故障时，
仍然会有近5分钟的数据丢失。所以，redis还提供了另一种持久化方式，那就是AOF。



## AOF

AOF，则是换了一个角度来实现持久化，那就是将redis执行过的所有写指令记录下来，
在下次redis重新启动时，只要把这些写指令从前到后再重复执行一遍，就可以实现数据恢复了。



AOF方式是将执行过的写指令记录下来，在数据恢复时按照从前到后的顺序再将指令都执行一遍，就这么简单。

通过配置redis.conf中的appendonly yes就可以打开AOF功能。如果有写操作（如SET等），redis就会被追加到AOF文件的末尾。

默认的AOF持久化策略是每秒钟fsync一次（fsync是指把缓存中的写指令记录到磁盘中），
因为在这种情况下，redis仍然可以保持很好的处理性能，即使redis故障，也只会丢失最近1秒钟的数据。

如果在追加日志时，恰好遇到磁盘空间满、inode满或断电等情况导致日志写入不完整，也没有关系，
redis提供了redis-check-aof工具，可以用来进行日志修复。



## 如何选择RDB和AOF

其实RDB和AOF两种方式也可以同时使用，在这种情况下，如果redis重启的话，
则会优先采用AOF方式来进行数据恢复，这是因为AOF方式的数据恢复完整度更高。

如果你没有数据持久化的需求，也完全可以关闭RDB和AOF方式，这样的话，
redis将变成一个纯内存数据库，就像memcache一样。



官方的建议是两个同时使用。这样可以提供更可靠的持久化方案。





# sentinel







# cluster







# redis-api







# 基于redis实现分布式锁


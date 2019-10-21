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



- 相关命令

```shell
# 异步执行一个 AOF（AppendOnly File） 文件重写操作(可以对 AOF 文件进行重建，将生成一个新的 AOF 文件， 这个文件包含重建当前数据集所需的最少命令。)
$ BGREWRITEAOF

#在后台异步保存当前数据库的数据到磁盘 rdb
$ BGSAVE
$ SAVE
```



- 相关配置

```shell
appendonly yes # 打开aof配置

save 60 1000 # 在满足“ 60 秒内有至少有 1000 个键被改动”这一条件时， 自动保存一次数据集
save 900 1
save 300 10
save 60 10000
# 几个条件之间是 "或" 的关系
```



## RDB

在默认情况下， `Redis` 将数据库快照保存在名字为 `dump.rdb`的二进制文件中。你可以对 `Redis` 进行设置， 让它在“ N 秒内数据集至少有 M 个改动”这一条件被满足时， 自动保存一次数据集。你也可以通过调用 `SAVE`或者 `BGSAVE` ， 手动让 `Redis` 进行数据集保存操作。

比如说， 以下设置会让 `Redis` 在满足“ 60 秒内有至少有 1000 个键被改动”这一条件时， 自动保存一次数据集:

```
save 60 1000
```

这种持久化方式被称为快照 `snapshotting`.



### 工作方式

当 `Redis` 需要保存 `dump.rdb` 文件时， 服务器执行以下操作:

- `Redis` 调用`forks`. 同时拥有父进程和子进程。
- 子进程将数据集写入到一个临时 `RDB` 文件中。
- 当子进程完成对新 `RDB` 文件的写入时，`Redis` 用新 `RDB` 文件替换原来的 `RDB` 文件，并删除旧的 R`D`B 文件。

这种工作方式使得 `Redis` 可以从写时复制（copy-on-write）机制中获益。



## AOF

快照功能并不是非常耐久（`durable`）： 如果 `Redis` 因为某些原因而造成故障停机， 那么服务器将丢失最近写入、且仍未保存到快照中的那些数据。 从 1.1 版本开始， `Redis` 增加了一种完全耐久的持久化方式： `AOF` 持久化。

你可以在配置文件中打开`AOF`方式:

```shell
appendonly yes
```

从现在开始， 每当 `Redis` 执行一个改变数据集的命令时（比如 `SET`）， 这个命令就会被追加到 `AOF` 文件的末尾。这样的话， 当 `Redis` 重新启时， 程序就可以通过重新执行 `AOF` 文件中的命令来达到重建数据集的目的。



### 日志重写

因为 `AOF` 的运作方式是不断地将命令追加到文件的末尾， 所以随着写入命令的不断增加， `AOF` 文件的体积也会变得越来越大。举个例子， 如果你对一个计数器调用了 100 次 `INCR` ， 那么仅仅是为了保存这个计数器的当前值， `AOF` 文件就需要使用 100 条记录（entry）。然而在实际上， 只使用一条 `SET` 命令已经足以保存计数器的当前值了， 其余 99 条记录实际上都是多余的。

为了处理这种情况， `Redis` 支持一种有趣的特性： 可以在不打断服务客户端的情况下， **对 `AOF` 文件进行重建（rebuild）**。执行 `BGREWRITEAOF` 命令， `Redis` 将生成一个新的 `AOF` 文件， 这个文件包含重建当前数据集所需的最少命令。`Redis` 2.2 需要自己手动执行 `BGREWRITEAOF` 命令； `Redis` 2.4 则可以自动触发 `AOF` 重写， 具体信息请查看 2.4 的示例配置文件。



### 刷盘配置

你可以配置 `Redis` 多久才将数据 `fsync` 到磁盘一次。有三种方式：

- 每次有新命令追加到 `AOF` 文件时就执行一次 `fsync` ：非常慢，也非常安全
- 每秒 `fsync` 一次：足够快（和使用 `RDB` 持久化差不多），并且在故障时只会丢失 1 秒钟的数据。
- 从不 `fsync` ：将数据交给操作系统来处理。更快，也更不安全的选择。

推荐（并且也是默认）的措施为每秒 `fsync` 一次， 这种 `fsync` 策略可以兼顾速度和安全性。



```shell
# 三种配置

appendfsync no # don't fsync, just let the OS flush the data when it wants. Faster.
appendfsync always # fsync after every write to the append only log. Slow, Safest.
appendfsync everysec # fsync only one time every second. Compromise.
```



### AOF文件损坏了怎么办

服务器可能在程序正在对 `AOF` 文件进行写入时停机， 如果停机造成了 `AOF` 文件出错（corrupt）， 那么 `Redis` 在重启时会拒绝载入这个 `AOF` 文件， 从而确保数据的一致性不会被破坏。当发生这种情况时， 可以用以下方法来修复出错的 `AOF` 文件：

- 为现有的 `AOF` 文件创建一个备份。

- 使用 `Redis` 附带的 `redis-check-aof` 程序，对原来的 `AOF` 文件进行修复:

  ```shell
  $ redis-check-aof –fix
  ```

- （可选）使用 `diff -u` 对比修复后的 `AOF` 文件和原始 `AOF` 文件的备份，查看两个文件之间的不同之处。

- 重启 `Redis` 服务器，等待服务器载入修复后的 `AOF` 文件，并进行数据恢复。



### 工作方式

`AOF` 重写和 `RDB` 创建快照一样，都巧妙地利用了写时复制机制:

- `Redis` 执行 `fork`() ，现在同时拥有父进程和子进程。
- 子进程开始将新 `AOF` 文件的内容写入到临时文件。
- 对于所有新执行的写入命令，父进程一边将它们累积到一个内存缓存中，一边将这些改动追加到现有 `AOF` 文件的末尾,这样样即使在重写的中途发生停机，现有的 `AOF` 文件也还是安全的。
- 当子进程完成重写工作时，它给父进程发送一个信号，父进程在接收到信号之后，将内存缓存中的所有数据追加到新 `AOF` 文件的末尾。
- 搞定！现在 `Redis` 原子地用新文件替换旧文件，之后所有命令都会直接追加到新 `AOF` 文件的末尾。



## RDB方式切换为AOF方式

在 `Redis` 2.2 或以上版本，可以在不重启的情况下，从 `RDB` 切换到 `AOF` ：

- 为最新的 `dump.rdb` 文件创建一个备份。
- 将备份放到一个安全的地方。
- 执行以下两条命令:
- `$ redis-cli config set appendonly yes`
- `$ redis-cli config set save “”`
- 确保写命令会被正确地追加到 `AOF` 文件的末尾。



执行的第一条命令开启了 `AOF` 功能： `Redis` **会阻塞**直到初始 `AOF` 文件创建完成为止， 之后 `Redis` 会继续处理命令请求， 并开始将写入命令追加到 `AOF` 文件末尾。

执行的第二条命令用于关闭 `RDB` 功能。 这一步是可选的， 如果你愿意的话， 也可以同时使用 `RDB` 和 `AOF` 这两种持久化功能。

**重要**:别忘了在 `redis.conf` 中打开 `AOF` 功能！ 否则的话， 服务器重启之后， 之前通过 `CONFIG SET` 设置的配置就会被遗忘， 程序会按原来的配置来启动服务器。



## AOF和RDB之间的相互作用

在版本号大于等于 2.4 的 `Redis` 中， `BGSAVE` 执行的过程中， 不可以执行 `BGREWRITEAOF` 。 反过来说， 在 `BGREWRITEAOF` 执行的过程中， 也不可以执行 `BGSAVE`。这可以防止两个 `Redis` 后台进程同时对磁盘进行大量的 I/O 操作。

如果 `BGSAVE` 正在执行， 并且用户显示地调用 `BGREWRITEAOF` 命令， 那么服务器将向用户回复一个 `OK` 状态， 并告知用户， `BGREWRITEAOF` 已经被预定执行： 一旦 `BGSAVE` 执行完毕， `BGREWRITEAOF` 就会正式开始。 当 `Redis` 启动时， 如果 `RDB` 持久化和 `AOF` 持久化都被打开了， 那么程序会优先使用 `AOF` 文件来恢复数据集， 因为 `AOF` 文件所保存的数据通常是最完整的。



## 备份redis数据

`Redis` 对于数据备份是非常友好的， 因为你可以在服务器运行的时候对 `RDB` 文件进行复制： `RDB` 文件一旦被创建， 就不会进行任何修改。 当服务器要创建一个新的 `RDB` 文件时， 它先将文件的内容保存在一个临时文件里面， 当临时文件写入完毕时， 程序才使用 `rename`(2) **原子地**用临时文件替换原来的 `RDB` 文件。

这也就是说， 无论何时， 复制 `RDB` 文件都是绝对安全的。

- 创建一个定期任务（`cron job`）， 每小时将一个 `RDB` 文件备份到一个文件夹， 并且每天将一个 `RDB` 文件备份到另一个文件夹。
- 确保快照的备份都带有相应的日期和时间信息， 每次执行定期任务脚本时， 使用 `find` 命令来删除过期的快照： 比如说， 你可以保留最近 48 小时内的每小时快照， 还可以保留最近一两个月的每日快照。
- 至少每天一次， 将 `RDB` 备份到你的数据中心之外， 或者至少是备份到你运行 `Redis` 服务器的物理机器之外。



# sentinel







# cluster







# redis-api







# 基于redis实现分布式锁


[Toc]



# 通用命令

## 服务相关

```shell
$ BGREWRITEAOF # 异步执行一个 AOF（AppendOnly File） 文件重写操作
$ BGSAVE # 在后台异步保存当前数据库的数据到磁盘 rdb
$ SAVE #同步保存数据到硬盘 rdb

$ CLIENT KILL [ip:port] [ID client-id] #关闭客户端连接
$ CLIENT LIST #获取连接到服务器的客户端连接列表
$ CLIENT GETNAME #获取连接的名称
$ CLIENT PAUSE timeout #在指定时间内终止运行来自客户端的命令
$ CLIENT SETNAME connection-name #设置当前连接的名称

$ CLUSTER SLOTS #获取集群节点的映射数组
$ ROLE #返回主从实例所属的角色
$ SLAVEOF host port #将当前服务器转变为指定服务器的从属服务器(slave server)

$ maxmemory 100mb

$ COMMAND #获取 Redis 命令详情数组
$ COMMAND COUNT #获取 Redis 命令总数

$ COMMAND INFO command-name [command-name ...] #获取指定 Redis 命令描述的数组

$ config get *
$ CONFIG SET parameter value #获取/修改 redis 配置参数，无需重启

$ DBSIZE #返回当前数据库的 key 的数量

$ DEBUG OBJECT key #获取 key 的调试信息

$ debug segfault #让 Redis 服务崩溃

$ FLUSHALL #删除所有数据库的所有key
$ FLUSHDB #删除当前数据库的所有key

$ INFO [section] #取 Redis 服务器的各种信息和统计数值 eg: info memory

$ lastsave #最近一次 Redis 成功将数据保存到磁盘上的时间，以 UNIX 时间戳格式表示

$ MONITOR # 有高手的文章说这个会急剧降低redis性能，只能在测试环境使用。
#实时打印出 Redis 服务器接收到的命令，调试用

$ SHUTDOWN [NOSAVE] [SAVE] #异步保存数据到硬盘，并关闭服务器

$ SLOWLOG subcommand [argument] #管理 redis 的慢日志

$ SYNC #用于复制功能(replication)的内部命令
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
$ setpx	# 单位：毫秒

->
$ set file:9527 ${random_value} NX EX ${timeout}
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



## list

`Redis lists`基于`Linked Lists`实现。



```shell
$ lpush mylist  1,2,3,......
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



### 适用场景：

- list可被用来实现聊天系统。还可以作为不同进程间传递消息的队列。（关键是，你可以每次都以原先添加的顺序访问数据。）



### List上的阻塞操作

可以使用Redis来实现生产者和消费者模型，如使用LPUSH和RPOP来实现该功能。但会遇到这种情景：list是空，这时候消费者就需要轮询来获取数据，这样就会增加redis的访问压力、增加消费端的cpu时间，而很多访问都是无用的。为此redis提供了阻塞式访问 [BRPOP](http://www.redis.cn/commands/brpop.html) 和 [BLPOP](http://www.redis.cn/commands/blpop.html) 命令。 消费者可以在获取数据时指定如果数据不存在阻塞的时间，如果在时限内获得数据则立即返回，如果超时还没有数据则返回null, 0表示一直阻塞。

同时redis还会为所有阻塞的消费者以先后顺序排队。

如需了解详细信息请查看 [RPOPLPUSH](http://www.redis.cn/commands/rpoplpush.html) 和 [BRPOPLPUSH](http://www.redis.cn/commands/brpoplpush.html)。



#### BLPOP/BRPOP

```shell
$ BLPOP key [key ...] timeout
```

阻塞式列表的弹出原语。 它是命令 `LPOP`的阻塞版本，这是因为当给定列表内没有任何元素可供弹出的时候， 连接将被 `BLPOP`命令阻塞。 当给定多个 key 参数时，按参数 `key` 的先后顺序依次检查各个列表，弹出第一个非空列表的头元素。

`BLPOP`命令引起客户端阻塞并且设置了一个非零的超时参数 `timeout` 的时候， 若经过了指定的 `timeout` 仍没有出现一个针对某一特定 `key` 的 `push` 操作，则客户端会解除阻塞状态并且返回一个 `nil` 的多组合值`(multi-bulk value`)。

**`timeout` 参数表示的是一个指定阻塞的最大秒数的整型值。**当 `timeout` 为 0 是表示阻塞时间无限制。



#### RPOPLPUSH/BRPOPLPUSH

原子性地返回并移除存储在 source 的列表的最后一个元素（列表尾部元素）， 并把该元素放入存储在 destination 的列表的第一个元素位置（列表头部）。

例如：假设 source 存储着列表 a,b,c， destination存储着列表 x,y,z。 执行 RPOPLPUSH 得到的结果是 source 保存着列表 a,b ，而 destination 保存着列表 c,x,y,z。

如果 source 不存在，那么会返回 nil 值，并且不会执行任何操作。 如果 source 和 destination 是同样的，那么这个操作等同于移除列表最后一个元素并且把该元素放在列表头部， 所以这个命令也可以当作是一个旋转列表的命令。



**示例**：

```shell
redis> RPUSH mylist "one"
(integer) 1
redis> RPUSH mylist "two"
(integer) 2
redis> RPUSH mylist "three"
(integer) 3
redis> RPOPLPUSH mylist myotherlist
"three"
redis> LRANGE mylist 0 -1
1) "one"
2) "two"
redis> LRANGE myotherlist 0 -1
1) "three"
redis> 
```



**模式：安全的队列**

Redis通常都被用做一个处理各种后台工作或消息任务的消息服务器。 一个简单的队列模式就是：生产者把消息放入一个列表中，等待消息的消费者用 [RPOP](http://www.redis.cn/commands/rpop.html) 命令（用轮询方式）， 或者用 BRPOP 命令（如果客户端使用阻塞操作会更好）来得到这个消息。

然而，因为消息有可能会丢失，所以这种队列并是不安全的。例如，当接收到消息后，出现了网络问题或者消费者端崩溃了， 那么这个消息就丢失了。

RPOPLPUSH (或者其阻塞版本的 [BRPOPLPUSH](http://www.redis.cn/commands/brpoplpush.html)） 提供了一种方法来避免这个问题：消费者端取到消息的同时把该消息放入一个正在处理中的列表。 当消息被处理了之后，该命令会使用 LREM 命令来移除正在处理中列表中的对应消息。

另外，可以添加一个客户端来监控这个正在处理中列表，如果有某些消息已经在这个列表中存在很长时间了（即超过一定的处理时限）， 那么这个客户端会把这些超时消息重新加入到队列中。



### key 的自动创建和删除

在我们的例子中，我们没有在推入元素之前创建空的 list，或者在 list 没有元素时删除它。在 list 为空时删除 key，并在用户试图添加元素（比如通过 `LPUSH`）而键不存在时创建空 list，是 Redis 的职责。

这不光适用于 lists，还适用于所有包括多个元素的 Redis 数据类型 – Sets, Sorted Sets 和 Hashes。

基本上，我们可以用三条规则来概括它的行为：

1. 当我们向一个聚合数据类型中添加元素时，如果目标键不存在，就在添加元素前创建空的聚合数据类型。

   ```shell
   > del mylist
   (integer) 1
   > lpush mylist 1 2 3
   (integer) 3
   ```

2. 当我们从聚合数据类型中移除元素时，如果值仍然是空的，键自动被销毁。

   ```shell
   > lpush mylist 1 2 3
   (integer) 3
   > exists mylist
   (integer) 1
   > lpop mylist
   "3"
   > lpop mylist
   "2"
   > lpop mylist
   "1"
   > exists mylist
   (integer) 0
   ```

   所有的元素被弹出之后， key 不复存在。

3. 对一个空的 key 调用一个只读的命令，比如 `LLEN` （返回 list 的长度），或者一个删除元素的命令，将总是产生同样的结果。该结果和对一个空的聚合类型做同个操作的结果是一样的。

   ```shell
   > del mylist
   (integer) 0
   > llen mylist
   (integer) 0
   > lpop mylist
   (nil)
   ```

   

## set

- 无序且唯一集合



增删改查：

```shell
$ sadd key value : 向set添加元素
	
$ srem key value ：从set中移除元素
$ spop 删除一个随机元素，把它返回给客户端

$ smembers key : 取出所有set元素

$ scard key : 获取set中元素的个数

$ srandmember key: 随机取出一个

$ sismember key value: 查看value是否存在set中

$ smove srckey dstkey member：将元素member从srckey中转移到dstkey中，这个操作是原子的。
```



取交集并集：

```shell
$ sinter key1 key2 ... keyN:取出这些set的交集
$ sintersotre dstkey key1 key2 ... keyN：取出这些key的交集并存储到dstkey

$ sunion key1 key2 ... keyN:将所有key合并后取出来，相同的值只取一次
$ sunionstore destKey key1 key2 ...keyN # 通常用于对多个集合取并集，并把结果存入另一个 set 中
```



```shell
$ sdiff key1 key2 ... keyN：获取第一set中不存在后面几个set里的元素。
$ sdiffstore dstkey key1 key2 ... keyN：和sdiff相同，获取key1中不存在其他key里的元素，但要存储到dstkey中。
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



事务可以理解为一个打包的批量执行脚本，但批量指令并非原子化的操作，中间某条指令的失败不会导致前面已做指令的回滚，也不会造成后续的指令不做。



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

 `Sentinel` 是一个管理多个 `Redis` 实例的工具，它可以实现对 `Redis` 的 **监控**、**通知**、**自动故障转移**。 





# cluster

Redis 集群是一个提供在**多个Redis间节点间共享数据**的程序集。

Redis 集群通过分区来提供**一定程度的可用性**,在实际环境中当某个节点宕机或者不可达的情况下继续处理命令. Redis 集群的优势:

- 自动分割数据到不同的节点上。
- 整个集群的部分节点失败或者不可达的情况下能够继续处理命令。



##  Redis集群的数据分片

Redis 集群有16384个哈希槽,每个key通过CRC16校验后对16384取模来决定放置哪个槽.集群的每个节点负责一部分hash槽,举个例子,比如当前集群有3个节点,那么:

- 节点 A 包含 0 到 5500号哈希槽.
- 节点 B 包含5501 到 11000 号哈希槽.
- 节点 C 包含11001 到 16384号哈希槽.

> 这种结构很容易添加或者删除节点. 比如如果我想新添加个节点D, 我需要从节点 A, B, C中得部分槽到D上. 如果我想移除节点A,需要将A中的槽移到B和C节点上,然后将没有任何槽的A节点从集群中移除即可. 由于从一个节点将哈希槽移动到另一个节点并不会停止服务,所以无论添加删除或者改变某个节点的哈希槽的数量都不会造成集群不可用的状态.



## Redis集群的主从复制模型

为了使在部分节点失败或者大部分节点无法通信的情况下集群仍然可用，所以集群使用了主从复制模型,每个节点都会有N-1个复制品.

在我们例子中具有A，B，C三个节点的集群,在没有复制模型的情况下,如果节点B失败了，那么整个集群就会以为缺少5501-11000这个范围的槽而不可用.

然而如果在集群创建的时候（或者过一段时间）我们为每个节点添加一个从节点A1，B1，C1,那么整个集群便有三个master节点和三个slave节点组成，这样在节点B失败后，集群便会选举B1为新的主节点继续服务，整个集群便不会因为槽找不到而不可用了

不过当B和B1 都失败后，集群是不可用的.



## Redis一致性保证

Redis 并不能保证数据的**强一致性**. 这意味这在实际中集群在特定的条件下可能会丢失写操作.

第一个原因是因为集群是用了**异步复制**. 写操作过程:

- 客户端向主节点B写入一条命令.
- 主节点B向客户端回复命令状态.
- 主节点将写操作复制给他得从节点 B1, B2 和 B3.

主节点对命令的复制工作发生在返回命令回复之后， 因为如果每次处理命令请求都需要等待复制操作完成的话， 那么主节点处理命令请求的速度将极大地降低 —— 我们必须在性能和一致性之间做出权衡。 注意：Redis 集群可能会在将来提供同步写的方法。 Redis 集群另外一种可能会丢失命令的情况是集群出现了网络分区， 并且一个客户端与至少包括一个主节点在内的少数实例被孤立。

举个例子 假设集群包含 A 、 B 、 C 、 A1 、 B1 、 C1 六个节点， 其中 A 、B 、C 为主节点， A1 、B1 、C1 为A，B，C的从节点， 还有一个客户端 Z1 假设集群中发生网络分区，那么集群可能会分为两方，大部分的一方包含节点 A 、C 、A1 、B1 和 C1 ，小部分的一方则包含节点 B 和客户端 Z1 .

Z1仍然能够向主节点B中写入, 如果网络分区发生时间较短,那么集群将会继续正常运作,如果分区的时间足够让大部分的一方将B1选举为新的master，那么Z1写入B中得数据便丢失了.

注意， 在网络分裂出现期间， 客户端 Z1 可以向主节点 B 发送写命令的最大时间是有限制的， 这一时间限制称为**节点超时时间（node timeout）**， 是 Redis 集群的一个重要的配置选项：



## Redis集群搭建

详见**`hadoop`集群环境搭建**



## 集群重新分片

 重新分片并不会对正在运行的集群程序产生任何影响 。 重新分片操作基本上就是将某些节点上的哈希槽移动到另外一些节点上面 



```shell
$ /opt/redis-5.0.4/src/redis-cli --cluster reshard 172.16.0.202:7003

>>> Performing Cluster Check (using node 172.16.0.202:7003)
M: c2dd9d90c50a332d1f54a6711a950b9df99ce114 172.16.0.202:7003
   slots:[11423-16383] (4961 slots) master
M: 7e08c6d5061b40e6c5b42b53fcc2c429b7d6a5de 172.16.0.201:7002
   slots:[0-499],[5461-11422] (6462 slots) master
M: 2f0c36bbe4dd57d4c005017b632ca94f2ef05562 172.16.0.200:7001
   slots:[500-5460] (4961 slots) master
[OK] All nodes agree about slots configuration.
>>> Check for open slots...
>>> Check slots coverage...
[OK] All 16384 slots covered.
How many slots do you want to move (from 1 to 16384)? 1000

all
```



```shell
$ 172.16.0.200:7001> cluster slots
1) 1) (integer) 0
   2) (integer) 499
   3) 1) "172.16.0.201"
      2) (integer) 7002
      3) "7e08c6d5061b40e6c5b42b53fcc2c429b7d6a5de"
2) 1) (integer) 5461
   2) (integer) 11422
   3) 1) "172.16.0.201"
      2) (integer) 7002
      3) "7e08c6d5061b40e6c5b42b53fcc2c429b7d6a5de"
3) 1) (integer) 500
   2) (integer) 5460
   3) 1) "172.16.0.200"
      2) (integer) 7001
      3) "2f0c36bbe4dd57d4c005017b632ca94f2ef05562"
4) 1) (integer) 11423
   2) (integer) 16383
   3) 1) "172.16.0.202"
      2) (integer) 7003
      3) "c2dd9d90c50a332d1f54a6711a950b9df99ce114"
```



## 测试故障转移

 要触发一次故障转移， 最简单的办法就是令集群中的某个主节点进入下线状态。 

```shell
172.16.0.200:7001> cluster nodes

7e08c6d5061b40e6c5b42b53fcc2c429b7d6a5de 172.16.0.201:7002@17002 master - 0 1571786666162 4 connected 0-499 5461-11422
2f0c36bbe4dd57d4c005017b632ca94f2ef05562 172.16.0.200:7001@17001 myself,master - 0 1571786662000 1 connected 500-5460
c2dd9d90c50a332d1f54a6711a950b9df99ce114 172.16.0.202:7003@17003 master - 0 1571786666000 3 connected 11423-16383
```



 后我们可以通过向端口号为7002 的主节点发送 **DEBUG SEGFAULT** 命令， 让这个主节点崩溃： 

```shell
$ redis-cli -p 7002 debug segfault
Error: Server closed the connection
```



 从 consistency-test 的这段输出可以看到， 集群在执行故障转移期间， 总共丢失了 578 个读命令和 577 个写命令， 但是并没有产生任何数据不一致。

这听上去可能有点奇怪， 因为在教程的开头我们提到过， Redis 使用的是异步复制， 在执行故障转移期间， 集群可能会丢失写命令。但是在实际上， 丢失命令的情况并不常见， 因为 Redis **几乎是同时**执行将命令回复发送给客户端， 以及将命令复制给从节点这两个操作， 所以实际上造成命令丢失的时间窗口是非常小的。不过， 尽管出现的几率不高， 但丢失命令的情况还是有可能会出现的， 所以我们对 Redis 集群不能提供强一致性的这一描述仍然是正确的。现在， 让我们使用 cluster nodes 命令,查看集群在执行故障转移操作之后， 主从节点的布局情况： 



## 手动故障转移

>  有的时候在主节点没有任何问题的情况下强制手动故障转移也是很有必要的，比如想要升级主节点的Redis进程，我们可以通过故障转移将其转为slave再进行升级操作来避免对集群的可用性造成很大的影响。 



Redis集群使用 `CLUSTER FAILOVER`命令来进行故障转移，不过要**在被转移的主节点的从节点上执行该命令** 手动故障转移比主节点失败自动故障转移更加**安全**，因为手动故障转移时客户端的切换是在确保新的主节点完全复制了失败的旧的主节点数据的前提下下发生的，所以避免了数据的丢失。

执行手动故障转移时从节点日志如下:

```shell
$ redis-cli -p 7002 cluster failover

# Manual failover user request accepted.
# Received replication offset for paused master manual failover: 347540
# All master replication stream processed, manual failover can start.
# Start of election delayed for 0 milliseconds (rank #0, offset 347540).
# Starting a failover election for epoch 7545.
# Failover election won: I'm the new master.
```

 其基本过程如下：客户端不再链接我们淘汰的主节点，同时主节点向从节点发送复制偏移量,从节点得到复制偏移量后故障转移开始,接着通知主节点进行配置切换,当客户端在旧的master上解锁后重新连接到新的主节点上。 



## 添加一个新节点

添加新的节点的基本过程就是添加一个空的节点然后移动一些数据给它。

有两种情况：添加一个主节点和添加一个从节点（添加从节点时需要将这个新的节点设置为集群中某个节点的复制） 



 两种情况第一步都是要添加 一个空的节点。（新建节点，开启redis服务，比如：7006）



### 添加主节点



```shell
# 第一个参数是新节点的地址，第二个参数是任意一个已经存在的节点的IP和端口
$ /opt/redis-5.0.4/src/redis-cli  --cluster add-node 127.0.0.1:7006 127.0.0.1:7000

$ redis 127.0.0.1:7006> cluster nodes
```



新节点现在已经连接上了集群， 成为集群的一份子， 并且可以对客户端的命令请求进行转向了， 但是和其他主节点相比， 新节点还有两点区别：

- 新节点没有包含任何数据， 因为它没有包含任何哈希槽.
- 尽管新节点没有包含任何哈希槽， 但它仍然是一个主节点， 所以在集群需要将某个从节点升级为新的主节点时， 这个新节点不会被选中。

接下来， 只要使用 redis-trib 程序， 将集群中的某些哈希桶移动到新节点里面， 新节点就会成为真正的主节点了。



### 添加从节点

 有两种方法添加从节点，可以像添加主节点一样使用redis-cli命令，也可以像下面的例子一样使用 –slave选项: 

```shell
$ /opt/redis-5.0.4/src/redis-cli  --cluster add-node --slave 127.0.0.1:7006 127.0.0.1:7000
```

此处的命令和添加一个主节点命令类似，此处并没有指定添加的这个从节点的主节点，这种情况下系统会在其他的复制集中的主节点中随机选取一个作为这个从节点的主节点。



你可以通过下面的命令指定主节点:

```shell
$ /opt/redis-5.0.4/src/redis-cli  --cluster add-node --slave --master-id 3c3a0c74aae0b56170ccb03a76b60cfe7dc1912e 127.0.0.1:7006 127.0.0.1:7000
```



 也可以使用`cluster replicate`命令添加 。（ 这个命令也可以改变一个从节点的主节点。 ）

 例如，要给主节点 127.0.0.1:7005添加一个从节点，该节点哈希槽的范围1423-16383, 节点 ID 3c3a0c74aae0b56170ccb03a76b60cfe7dc1912e,我们需要链接新的节点（已经是空的主节点）并执行命令: 

```shell
$ redis 127.0.0.1:7006> cluster replicate 3c3a0c74aae0b56170ccb03a76b60cfe7dc1912e
```



## 移除一个节点

```shell
# 第一个参数是任意一个节点的地址,第二个节点是你想要移除的节点地址。
$ /opt/redis-5.0.4/src/redis-cli  del-node 127.0.0.1:7000 `<node-id>`
```



 主从节点都用这种方式移除。但是移除主节点前，需要**确保这个主节点是空的**. 如果不是空的,需要将这个节点的数据重新分片到其他主节点上. 



## 从节点的迁移

 在Redis集群中会存在改变一个从节点的主节点的情况，需要执行如下命令 : 

```shell
$ redis 127.0.0.1:7006> CLUSTER REPLICATE <master-node-id>
```



简短的概况一下从节点迁移

- 集群会在有从节点数量最多的主节点上进行从节点的迁移.
- 要在一个主节点上添加多个从节点.
- 参数来控制从节点迁移 replica-migration-barrier:你可以仔细阅读redis.conf 。



## 主节点修复

当redis集群中一台master节点down机后（无slave情况下），再次启动连接现有集群会出现问题，需要修复slot



```shell
$ /opt/redis-5.0.4/src/redis-cli  --cluster fix 172.16.0.200:7001
```



# redis-api

- jedis
- redisson



Jedis是Redis的Java实现的客户端，其API提供了比较全面的Redis命令的支持。Redission也是Redis的客户端，相比于Jedis功能简单。Jedis简单使用阻塞的I/O和redis交互，Redission通过Netty支持非阻塞I/O。Jedis最新版本2.9.0是2016年的快3年了没有更新，而Redission最新版本是2018.10月更新。

Redission封装了锁的实现，其继承了java.util.concurrent.locks.Lock的接口，让我们像操作我们的本地Lock一样去操作Redission的Lock





# 发布订阅



# redis内存淘汰策略



```shell
# 设置maxmemory为0代表没有内存限制。对于64位的系统这是个默认值，对于32位的系统默认内存限制为3GB。
maxmemory 100mb
```

 当指定的内存限制大小达到时，需要选择不同的行为，也就是**策略**。 Redis可以仅仅对命令返回错误，这将使得内存被使用得更多，或者回收一些旧的数据来使得添加数据时可以避免内存限制。 



## 删除过期键对象

由于Redis进程内保存了大量的键，维护每个键的过期时间去删除键会消耗大量的CPU资源，对于单线程的Redis来说成本很高。所以Redis采用**惰性删除 + 定时任务删除**机制来实现过期键的内存回收。

**惰性删除**：当客户端读取键时，如果键带有过期时间并且已经过期，那么会执行删除操作并且查询命令返回空。这种机制是为了节约CPU成本，不需要单独维护一个TTL链表来处理过期的键。但是这种删除机制会导致内存不能及时得到释放，所以将结合下面的定时任务删除机制一起使用。

**定时任务删除**：Redis内部维护一个定时任务，用于随机获取一些带有过期属性的键，并将其中过期的键删除。来删除一些过期的冷数据。

在兼顾CPU和内存的的考虑下，Redis使用惰性删除 + 定时任务删除机制相结合，来删除过期键对象。



## 回收策略

LRU（ Least Recently Used ）

当`maxmemory`限制达到的时候Redis会使用的行为由 Redis的`maxmemory-policy`配置指令来进行配置。

以下的策略是可用的:

- **noeviction**:返回错误。当内存限制达到并且客户端尝试执行会让更多内存被使用的命令（大部分的写入指令，但DEL和几个例外）
- **allkeys-lru**: 尝试回收最少使用的键（LRU），使得新添加的数据有空间存放。
- **volatile-lru**: 尝试回收最少使用的键（LRU），但仅限于在过期集合的键,使得新添加的数据有空间存放。
- **allkeys-random**: 回收随机的键使得新添加的数据有空间存放。
- **volatile-random**: 回收随机的键使得新添加的数据有空间存放，但仅限于在过期集合的键。
- **volatile-ttl**: 回收在过期集合的键，并且优先回收存活时间（TTL）较短的键,使得新添加的数据有空间存放。

一般的经验规则:

- 使用**allkeys-lru**策略：当你希望你的请求符合一个幂定律分布，也就是说，你希望部分的子集元素将比其它其它元素被访问的更多。如果你不确定选择什么，这是个很好的选择。.
- 使用**allkeys-random**：如果你是循环访问，所有的键被连续的扫描，或者你希望请求分布正常（所有元素被访问的概率都差不多）。
- 使用**volatile-ttl**：如果你想要通过创建缓存对象时设置TTL值，来决定哪些对象应该被过期。

**allkeys-lru** 和 **volatile-random**策略对于当你想要单一的实例实现缓存及持久化一些键时很有用。不过一般运行两个实例是解决这个问题的更好方法。

为了键设置过期时间也是需要消耗内存的，所以使用**allkeys-lru**这种策略更加高效，因为没有必要为键取设置过期时间当内存有压力时。



## **回收进程如何工作**

理解回收进程如何工作是非常重要的:

- 一个客户端运行了新的命令，添加了新的数据。
- Redi检查内存使用情况，如果大于maxmemory的限制, 则根据设定好的策略进行回收。
- 一个新的命令被执行，等等。
- 所以我们不断地穿越内存限制的边界，通过不断达到边界然后不断地回收回到边界以下。

如果一个命令的结果导致大量内存被使用（例如很大的集合的交集保存到一个新的键），不用多久内存限制就会被这个内存使用量超越。



## 近似LRU算法

Redis的LRU算法并非完整的实现。这意味着Redis并没办法选择最佳候选来进行回收，也就是最久未被访问的键。相反它会尝试运行一个近似LRU的算法，通过对少量keys进行取样，然后回收其中一个最好的key（被访问时间较早的）。

不过从Redis 3.0算法已经改进为回收键的候选池子。这改善了算法的性能，使得更加近似真是的LRU算法的行为。

Redis LRU有个很重要的点，你通过调整每次回收时检查的采样数量，以实现**调整算法的精度**。这个参数可以通过以下的配置指令调整:

```
maxmemory-samples 5
```



# redis相关问题



## redis热点key

### 热点key产生原因

1. 用户消费的数据远大于生产的数据（热卖商品、热点新闻、热点评论、明星直播）。 

2. 请求分片集中，超过单 Server 的性能极限。 



### 热点key发现

- 通过客户端（eg：Jedis）编码时进行统计

- 通过代理统计

   像Twemproxy、Codis这些基于代理的Redis分布式架构，所有客户端的请求都是通过代理端完成的 

  ![](img/redis1.png)

- redis服务端monitor统计



### 热点key的处理

- 本地缓存

   对于数据一致性不是那么高的业务，可以将热点key缓存到业务机器的本地缓存中 。

-  迁移热点key 

   以redis cluster为例，我们可以将热点key所在的slot单独迁移到一个新的redis分片。这样这个热点key即使qps很高，也不会影响到整个集群的其他业务。 



## redis实现分布式锁

在多线程操作中，对于共享数据的访问修改，可以通过java提供的线程同步手段（synchorized，lock等）实现。但是在分布式环境中，访问共享数据的进程运行在不同的jvm上，所以只能通过分布式锁来保证共享数据访问的一致性。



###  分布式锁的几种实现方案 

- 基于数据库实现分布式锁
- 基于redis实现分布式锁
- 基于zookeeper实现分布式锁 



### 分布式锁实现需求/注意事项

- 互斥性。 可以保证在分布式部署的应用集群中， 在任意时刻， 同一个方法在同一时间只能被一台机器上的一个线程执行 -> **setnx保证**

- 不会发送死锁。即使一个客户端持有锁的期间崩溃而没有主动释放锁，也需要保证后续其他客户端能够加锁成功。-> **set时设置锁的过期时间（注意需要和set保证原子操作）**

  ```shell
  # redis2.8之后redis支持nx和ex操作是同一原子操作
  $ set key value ex 5 nx
  ```

- 加锁和解锁必须是同一个客户端，客户端自己不能把别人加的锁给释放了。 -> **set的值包含当前客户端的唯一标识（用UUID生成）**

  ```shell
  # 解锁操作需要比较唯一标识是否相等，相等再执行删除操作。这2个操作可以采用Lua脚本方式使2个命令的原子性。
  
  if redis.call("get",KEYS[1]) == ARGV[1] then
      return redis.call("del",KEYS[1])
  else
      return 0
  end
  ```

- 容错性。只要大部分的Redis节点正常运行，客户端就可以进行加锁和解锁操作。 -> 类比Redisson的RedLock思想，同时给集群中的多个master上锁，只有多数上锁成功，才返回上锁成功。

  

### 单机

单机版的redis想要实现分布式锁，需要注意上述分布式锁实现需求/注意事项的前三点。

示例：[ https://wudashan.cn/2017/10/23/Redis-Distributed-Lock-Implement/ ]( https://wudashan.cn/2017/10/23/Redis-Distributed-Lock-Implement/ )



### 集群

集群版的redis想要实现分布式锁，需要注意上述分布式锁实现需求/注意事项的四点。其中最主要的就是第四点：容错性。



 我们考虑如下场景: 

```
1.客户端1从Master获取了锁。
2.Master宕机了，存储锁的key还没有来得及同步到Slave上。
3.Slave升级为Master。
4.客户端2从新的Master获取到了对应同一个资源的锁。
5.客户端1和客户端2同时持有了同一个资源的锁，锁不再具有安全性。
```



 就此问题，Redis作者antirez写了RedLock算法来解决这种问题。 



#### RedLock锁的操作



**RedLock获取锁：**

- 获取当前时间。
- 按顺序依次向N个Redis节点执行获取锁的操作。这个获取操作跟前面基于单Redis节点的获取锁的过程相同，包含随机字符串my_random_value，也包含过期时间(比如PX 30000，即锁的有效时间)。为了保证在某个Redis节点不可用的时候算法能够继续运行，这个获取锁的操作还有一个超时时间(time out)，它要远小于锁的有效时间（几十毫秒量级）。客户端在向某个Redis节点获取锁失败以后，应该立即尝试下一个Redis节点。
- 计算整个获取锁的过程总共消耗了多长时间，计算方法是用当前时间减去第1步记录的时间。如果客户端从大多数Redis节点（>= N/2+1）成功获取到了锁，并且获取锁总共消耗的时间没有超过锁的有效时间(lock validity time)，那么这时客户端才认为最终获取锁成功；否则，认为最终获取锁失败。
- 如果最终获取锁成功了，那么这个锁的有效时间应该重新计算，它等于最初的锁的有效时间减去第3步计算出来的获取锁消耗的时间。
- 如果最终获取锁失败了（可能由于获取到锁的Redis节点个数少于N/2+1，或者整个获取锁的过程消耗的时间超过了锁的最初有效时间），那么客户端应该立即向所有Redis节点发起释放锁的操作（即前面介绍的单机Redis Lua脚本释放锁的方法）。



**RedLock释放锁：**

- 客户端向所有Redis节点发起释放锁的操作，不管这些节点当时在获取锁的时候成功与否。 



#### RedLock存在的问题



1. 如果有节点发生崩溃重启 



假设一共有5个Redis节点：A, B, C, D, E。设想发生了如下的事件序列：

```
客户端1成功锁住了A, B, C，获取锁成功（但D和E没有锁住）。
节点C崩溃重启了，但客户端1在C上加的锁没有持久化下来，丢失了。
节点C重启后，客户端2锁住了C, D, E，获取锁成功。
客户端1和客户端2同时获得了锁。
```

为了应对这一问题，antirez又提出了**延迟重启(delayed restarts)**的概念。也就是说，一个节点崩溃后，先不立即重启它，而是等待一段时间再重启，这段时间应该大于锁的有效时间(lock validity time)。这样的话，这个节点在重启前所参与的锁都会过期，它在重启后就不会对现有的锁造成影响。



2. 如果客户端长期阻塞导致锁过期 



![](img/redis-redlock1.png)

解释一下这个时序图：

​	客户端1在获得锁之后发生了很长时间的GC pause，在此期间，它获得的锁过期了，而客户端2获得了锁。当客户端1从GC pause中恢复过来的时候，它不知道自己持有的锁已经过期了，它依然向共享资源（上图中是一个存储服务）发起了写数据请求，而这时锁实际上被客户端2持有，因此两个客户端的写请求就有可能冲突（锁的互斥作用失效了）。



如何解决这个问题呢?引入了**fencing token**的概念：



![](img/redis-redlock2.png)

客户端1先获取到的锁，因此有一个较小的fencing token，等于33，而客户端2后获取到的锁，有一个较大的fencing token，等于34。客户端1从GC pause中恢复过来之后，依然是向存储服务发送访问请求，但是带了fencing token = 33。存储服务发现它之前已经处理过34的请求，所以会拒绝掉这次33的请求。这样就避免了冲突。

**但是其实这已经超出了Redis实现分布式锁的范围，单纯用Redis没有命令来实现生成Token。**



3.  时钟跳跃问题 



假设有5个Redis节点A, B, C, D, E。

```
客户端1从Redis节点A, B, C成功获取了锁（多数节点）。由于网络问题，与D和E通信失败。
节点C上的时钟发生了向前跳跃，导致它上面维护的锁快速过期。
客户端2从Redis节点C, D, E成功获取了同一个资源的锁（多数节点）。
客户端1和客户端2现在都认为自己持有了锁。
这个问题用Redis实现分布式锁暂时无解。而生产环境这种情况是存在的。
```



### Redisson实现原理

 实现原理：

[ https://juejin.im/post/5bf3f15851882526a643e207 ]( https://juejin.im/post/5bf3f15851882526a643e207 )

[ https://juejin.im/post/5bbb0d8df265da0abd3533a5 ]( https://juejin.im/post/5bbb0d8df265da0abd3533a5 )



![](img/redis-redisson1.png)



### Redisson使用及分布式锁实现

 [http://wuwenliang.net/2019/07/23/%E5%86%8D%E8%B0%88%E5%88%86%E5%B8%83%E5%BC%8F%E9%94%81%E4%B9%8B%E5%89%96%E6%9E%90Redis%E5%AE%9E%E7%8E%B0/?utm_source=tuicool&utm_medium=referral](http://wuwenliang.net/2019/07/23/再谈分布式锁之剖析Redis实现/?utm_source=tuicool&utm_medium=referral) 





## 单线程redis为什么快
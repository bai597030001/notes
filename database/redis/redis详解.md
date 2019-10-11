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
```





## string

```shell
# 值可以是任何种类的字符串（包括二进制数据），例如你可以在一个键下保存一副jpeg图片。值的长度不能超过512 MB。
$ set mykey someValue
$ get mykey

$ mget
$ mset
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

# 截取指定长度
$ ltrim mylist 0 2
$ rtrim mylist 0 2
```



适用场景：

- list可被用来实现聊天系统。还可以作为不同进程间传递消息的队列。（关键是，你可以每次都以原先添加的顺序访问数据。）



## set

## sorted set





redis-replication







transactions







persistence







sentinel







cluster







redis-api







基于redis实现分布式锁


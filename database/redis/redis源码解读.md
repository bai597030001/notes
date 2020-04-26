# 源码结构概览

第一阶段 阅读Redis的数据结构部分

- 内存分配 zmalloc.c和zmalloc.h
- 动态字符串 sds.h和sds.c
- 双端链表 adlist.c和adlist.h
- 字典 dict.h和dict.c
- 跳跃表 server.h文件里面关于zskiplist结构和zskiplistNode结构，以及t_zset.c中所有zsl开头的函数，比如 zslCreate、zslInsert、zslDeleteNode等等。
- 基数统计 hyperloglog.c 中的 hllhdr 结构， 以及所有以 hll 开头的函数



第二阶段 熟悉Redis的内存编码结构

- 整数集合数据结构 intset.h和intset.c
- 压缩列表数据结构 ziplist.h和ziplist.c



第三阶段 熟悉Redis数据类型的实现

- 对象系统 object.c
- 字符串键 t_string.c
- 列表建 t_list.c
- 散列键 t_hash.c
- 集合键 t_set.c
- 有序集合键 t_zset.c中除 zsl 开头的函数之外的所有函数
- HyperLogLog键 hyperloglog.c中所有以pf开头的函数



第四阶段 熟悉Redis数据库的实现

- 数据库实现 redis.h文件中的redisDb结构，以及db.c文件
- 通知功能 notify.c
- RDB持久化 rdb.c
- AOF持久化 aof.c

以及一些独立功能模块的实现

- 发布和订阅 redis.h文件的pubsubPattern结构，以及pubsub.c文件
- 事务 redis.h文件的multiState结构以及multiCmd结构，multi.c文件



第五阶段 熟悉客户端和服务器端的代码实现

- 事件处理模块 ae.c/ae_epoll.c/ae_evport.c/ae_kqueue.c/ae_select.c
- 网路链接库 anet.c和networking.c
- 服务器端 redis.c
- 客户端 redis-cli.c
- 这个时候可以阅读下面的独立功能模块的代码实现
- lua脚本 scripting.c
- 慢查询 slowlog.c
- 监视 monitor.c



第六阶段 这一阶段主要是熟悉Redis多机部分的代码实现

- 复制功能 replication.c
- Redis Sentinel sentinel.c
- 集群 cluster.c



关于测试方面的文件有：

- memtest.c 内存检测
- redis_benchmark.c 用于redis性能测试的实现。
- redis_check_aof.c 用于更新日志检查的实现。
- redis_check_dump.c 用于本地数据库检查的实现。
- testhelp.c 一个C风格的小型测试框架。



一些工具类的文件如下：

- bitops.c GETBIT、SETBIT 等二进制位操作命令的实现
- debug.c 用于调试时使用
- endianconv.c 高低位转换，不同系统，高低位顺序不同
- help.h 辅助于命令的提示信息
- lzf_c.c 压缩算法系列
- lzf_d.c 压缩算法系列
- rand.c 用于产生随机数
- release.c 用于发布时使用
- sha1.c sha加密算法的实现
- util.c 通用工具方法
- crc64.c 循环冗余校验
- sort.c SORT命令的实现
- 一些封装类的代码实现：
- bio.c background I/O的意思，开启后台线程用的
- latency.c 延迟类
- migrate.c 命令迁移类，包括命令的还原迁移等
- pqsort.c 排序算法类
- rio.c redis定义的一个I/O类
- syncio.c 用于同步Socket和文件I/O操作



# 源码详解



## 动态字符SDS

sds （Simple Dynamic String，简单动态字符串）是 Redis 底层所使用的字符串表示， 几乎所有的 Redis 模块中都用了 sds。



### SDS 与 C字符串区别



#### 常数时间获取字符串长度

C字符串需要遍历, 时间复杂度为O(n).

SDS直接获取,  时间复杂度为O(1).



####  防止缓冲区溢出

C语言不记录自身长度, 容易造成缓冲区溢出

```c
strcat(s1, s3)
```

SDS的空间分配策略完全杜绝了这种可能性.  当API需要对SDS进行修改时,  API会首先会检查SDS的空间是否满足条件, 如果不满足, API会自动对它动态扩展,   然后再进行修改, 这个过程是完全透明的.



#### 减少修改字符串带来的内存重分配次数

C语言对字符串修改后都需要手动重新分配内存; 当增加长度时需要扩展内存, 否则会产生缓冲区溢出;  当缩小长度时需要释放内存, 否则会产生内存泄露.

由于Redis频繁操作数据, 内存分配和释放耗时可能对性能造成影响, SSD避免了这种缺陷, 实现<font color=#00dd00>空间预分配和惰性空间释放两种优化策略</font>

##### 空间预分配

如果修改后len长度将小于 1 M, 这时分配给free的大小和len一样, 例如修改过后为13字节,  那么给free也是13字节 .    buf实际长度变成了  13 byte+ 13byte + 1byte = 27byte

如果修改后len长度将大于等于1 M, 这时分配给free的长度为 1 M,     例如修改过后为30M,  那么给free是1M .    buf实际长度变成了  30M + 1M + 1 byte

在修改时, 首先检查空间是不是够, 如果足够, 直接使用, 否则执行内存重分配.

##### 惰性空间释放

当缩短SDS长度时, 不进行内存释放, 而是记录到free字段中, 等待下次使用.  

与此同时, 也提供相应的API, 可以手动释放内存.



#### 二进制安全

C字符串只有末尾能保存空格， 中间如果有空格会被截取, 认作结束标识. 这样就不能保存图片, 音频视频等二进制数据了.

所有的SDS API会以二进制的方式处理SDS buf数组里面的数据, 程序不会对其中数据做任何限制, 过滤,修改和假设,  数据写入是什么样子, 读取出来就是什么样子.

> 例如 :  保留的数据中间出现'',  这是没有任何问题的. ,  因为它使用len而不是空字符判断结束.



### SDS实现



#### 结构定义

```c
/* Note: sdshdr5 is never used, we just access the flags byte directly.
 * However is here to document the layout of type 5 SDS strings. */
//小于一字节
struct __attribute__ ((__packed__)) sdshdr5 {
    unsigned char flags; /* 3 lsb of type, and 5 msb of string length */
    char buf[];
};
//一字节
struct __attribute__ ((__packed__)) sdshdr8 {
    uint8_t len; /* used 当前sds的长度*/
    uint8_t alloc; /* excluding the header and null terminator 为sds分配的内存大小*/
    unsigned char flags; /* 3 lsb of type, 5 unused bits 当前sds的类型*/
    char buf[];
};
/*
    1、节约内存：如SDS32可以节省3个字节
    2、buf指针引用：SDS返回给上层的，不是结构体首地址，而是 buf 指针地址，这样可以通过 buf[-1] 直接获得 flags ，来识别当前 sds 结构体的类型，从而获取整个结构体的任意一个部分
*/

//2字节
struct __attribute__ ((__packed__)) sdshdr16 {
    uint16_t len; /* used */
    uint16_t alloc; /* excluding the header and null terminator */
    unsigned char flags; /* 3 lsb of type, 5 unused bits */
    char buf[];
};

/*
	alloc字段:  空间预分配和惰性空间释放的设计思想
		
		空间预分配: 
			sdsavail()方法中，在获取字符串剩余可用空间的时候，就会使用到alloc字段。
			它记录了分配的总空间大小，方便我们在进行字符串追加操作的时候，判断是否需要额外分配空间。
			当前剩余的可用空间大小为alloc - len，即已分配总空间大小alloc - 当前使用的空间大小len
		
		惰性空间释放: 
			用于优化 SDS 的字符串截取或缩短操作。
			当 SDS 的 API 需要缩短 SDS 保存的字符串时，程序并不立即回收缩短后多出来的字节。
			这样一来，如果将来要对 SDS 进行增长操作的话，这些未使用空间就可能会派上用场。
*/

//4字节
struct __attribute__ ((__packed__)) sdshdr32 {
    uint32_t len; /* used */
    uint32_t alloc; /* excluding the header and null terminator */
    unsigned char flags; /* 3 lsb of type, 5 unused bits */
    char buf[];
};
//8字节
struct __attribute__ ((__packed__)) sdshdr64 {
    uint64_t len; /* used */
    uint64_t alloc; /* excluding the header and null terminator */
    unsigned char flags; /* 3 lsb of type, 5 unused bits */
    char buf[];
};

#define SDS_TYPE_5  0
#define SDS_TYPE_8  1
#define SDS_TYPE_16 2
#define SDS_TYPE_32 3
#define SDS_TYPE_64 4
#define SDS_TYPE_MASK 7
#define SDS_TYPE_BITS 3
#define SDS_HDR_VAR(T,s) struct sdshdr##T *sh = (void*)((s)-(sizeof(struct sdshdr##T)));
#define SDS_HDR(T,s) ((struct sdshdr##T *)((s)-(sizeof(struct sdshdr##T))))
#define SDS_TYPE_5_LEN(f) ((f)>>SDS_TYPE_BITS)
```



```c
// 结构体会按照其所有变量结构体做最小公倍数字节对齐。当使用 packed 修饰后，结构体会按照 1字节对齐。
_attribute__ ((__packed__)) 
```

1、节约内存：如SDS32可以节省3个字节
2、buf指针引用：SDS返回给上层的，不是结构体首地址，而是 buf 指针地址，这样可以通过 buf[-1] 直接获得 flags ，来识别当前 sds 结构体的类型，从而获取整个结构体的任意一个部分



> 以 SDS32 为例 ，修饰前按照 12（4x3）字节对齐，修饰后按照1字节对齐。

![](img/redis-sds1.png)



#### API

```c
//（1）创建一个包含给定c字符串的sds
sds sdsnew(const char *init);

//（2）为sds(也就是buf数组)分配指定空间
/* 创建一个sds字符串的核心函数 */
/* 会根据字符串长度来选择合适的SDS 类型，待数据填入完成后，会返回 SDS buf 的指针作为 SDS 的指针 */
sds sdsnewlen(const void *init, size_t initlen);

/* 扩容的核心函数 */
sds sdsMakeRoomFor(sds s, size_t addlen);

//（3）创建一个不包含任何内容的空字符串
sds sdsempty(void);

//（4）释放给定的sds
void sdsfree(sds);

//（5）创建一个给定sds的副本
sds sdsdup(sds);

//（6）清空sds保存的字符串内容
sds sdsclear(sds);

//（7）将给定c字符串拼接到另一个sds字符串的末尾
sds sdscat(sds,char *);

//（8）将给定sds字符串拼接到另一个sds字符串的末尾
sds sdscatsds(sds,sds);

//（9）将给定的c字符串复制到sds里面，覆盖原有的字符串
sds sdscpy(sds,char *);

//（10）保留sds给定区间内的数据
sds sdsrange(sds,int,int);

// 对给定字符或字符串进行分割
sds *sdssplitlen(const char *s, ssize_t len, const char *sep, int seplen, int *count)

//（11）从sds中移除所有在c字符串中出现过的字符
sds sdstrim(sds,const char *);

//（12）对比两个sds字符串是否相同
bool sdscmp(sds,sds);

// 获取sds可用的字节数   
/* sdsalloc() = sdsavail() + sdslen() */
size_t sdsavail(const sds s);

size_t sdslen(const sds s);
```



## 双向链表adlist



## 字典dict

hash

### 结构定义

Redis的字典实现主要依赖的数据结构包括三部分：dict，dictht，dictEntry节点。

dict中嵌入了2个dictht表，dictht表中的table字典存放着dictEntry

![](img/redis-dict1.webp)



![](img/redis-dict3.webp)

结构体的各个字段作用：

```properties
dict结构
    type：是用户自定义的函数列表，主要用于插入数据到字典时进行的一些操作，比如计算key哈希值的
 hashFunction 函数句柄。
    privdata：创建字典时指定的私有数据，一般提供给 type 字段中的函数句柄使用。
    ht[2]：类型为 dictht 结构的数组，这个数组有两个元素，而 dictht 结构主要用于保存数据，一
 般情况下只用ht[0],只有当字典扩容，缩容需要进行rehash时才会用到ht[1].
    rehashidx：rehash操作过程中最后一次处理的桶索引。
    iterators：用于迭代字典中的数据。

dictht结构
    table：类型为 dictEntry 结构指针的数组，用于保存数据，每个 key-value 的数据对通过类型为
 dictEntry 的结构来存储。
    size：table数组的大小。
    sizemark：用于取模，得到一个 0 ～ size 的索引值。恒等于size-1
    used：表示字典中有多少个元素。包含next单链表数据

dictEntry结构
    key：数据的键，主要用于查找数据。
    v：数据的值，数据主要通过这个字段来存储。
    next：用于解决Hash冲突，把冲突的key连接起来（拉链法）。
```



### 字典初始化

在redis-server启动中，整个数据库会先初始化一个空的字典用于存储整个数据库的键值对，初始化一个空字典，主要调用的是dict.h文件中的dictCreate函数

```c
/* Reset a hash table already initialized with ht_init().
 * NOTE: This function should only be called by ht_destroy(). */
static void _dictReset(dictht *ht)
{
    ht->table = NULL;
    ht->size = 0;
    ht->sizemask = 0;
    ht->used = 0;
}

/* 创建一个新的hash表 */
dict *dictCreate(dictType *type, void *privDataPtr)
{
    dict *d = zmalloc(sizeof(*d));  //96字节
    _dictInit(d,type,privDataPtr);  //结构体初始化值
    return d;
}

/* 初始化hash表 */
int _dictInit(dict *d, dictType *type, void *privDataPtr)
{
    _dictReset(&d->ht[0]);
    _dictReset(&d->ht[1]);
    d->type = type;
    d->privdata = privDataPtr;
    d->rehashidx = -1;
    d->iterators = 0;
    return DICT_OK;
}
```



dictCreate函数初始化一个空字典的主要步骤为：申请空间，调用_dictInit函数，给字典的各个字段赋予初始值。初始化后，一个字典内存占用情况如下图所示：

![](img/redis-dict2.webp)



Redis中怎么创建一个字典的

```c
------server.c------
/* Command table. sds string -> command struct pointer. */
dictType commandTableDictType = {
    dictSdsCaseHash,            /* hash function */
    NULL,                       /* key dup */
    NULL,                       /* val dup */
    dictSdsKeyCaseCompare,      /* key compare */
    dictSdsDestructor,          /* key destructor */
    NULL                        /* val destructor */
};

/*初始化服务端配置*/
void initServerConfig(void) {
    ........
    server.commands = dictCreate(&commandTableDictType,NULL);
    server.orig_commands = dictCreate(&commandTableDictType,NULL);
    ........
}
```

创建字典时，需要提供 dictType 参数，而这个参数主要定义了插入数据到字典时进行的一些操作，比如插入数据时key是否要进行复制的keyDup函数，那么我们来看看 dictType 的定义：

```c
typedef struct dictType {
    uint64_t (*hashFunction)(const void *key);  /*用于计算键的哈希值*/
    void *(*keyDup)(void *privdata, const void *key);  /*用于复制数据的键*/
    void *(*valDup)(void *privdata, const void *obj);  /*用于复制数据的值*/
    /*用于比较键是否相等*/
    int (*keyCompare)(void *privdata, const void *key1, const void *key2);
    void (*keyDestructor)(void *privdata, void *key);  /*用于释放复制出来的键的内存*/
    void (*valDestructor)(void *privdata, void *obj);  /*用于释放复制出来的值的内存*/
} dictType;
```



### 插入元素

redis-server启动后，再启动redis-client连上server，执行命令

```shell
$ 127.0.0.1:6379> set  hello  world
```



Server端收到命令后，会执行`void setKey(redisDb *db, robj *key, robj *val);`

根据之前介绍字典的特性，每个键必须是唯一的，主要流程如下:

```properties
1). 调用dictFind函数，查找键是否存在，则调用dbOverwrite函数修改键值对，否则调用dictAdd函数
添加元素
2). dbAdd会调用dict.h中的dictAdd函数插入键值对.
```

dictAdd函数

```c
/* 调用之前会查找key是否存在，不存在则调用dictAdd函数 */
int dictAdd(dict *d, void *key, void *val)
{
    /*添加键，字典中键已存在则返回NULL,否则添加键到新节点中，返回新节点*/
    dictEntry *entry = dictAddRaw(d,key,NULL); 
    if (!entry) return DICT_ERR;   /*键存在则返回错误*/
    dictSetVal(d, entry, val);  /*设置新值*/
    return DICT_OK;
}
```

而dictAdd() 函数主要还是通过调用 dictAddRaw() 函数来完成插入操作，dictAddRaw() 函数的代码如下：

```c
 /*入参字典，键，Hash表节点地址*/
dictEntry *dictAddRaw(dict *d, void *key, dictEntry **existing) 
{
    long index;
    dictEntry *entry;
    dictht *ht;
    /*该字典是否在进行rehash操作，如果是则执行一次rehash*/
    if (dictIsRehashing(d)) _dictRehashStep(d);
    /*查找键，找到则直接返回-1，并把老节点存入existing字段，否则把新节点的索引值返回，
    如果遇到Hash表容量不足，则进行扩容*/
    if ((index = _dictKeyIndex(d, key, dictHashKey(d,key), existing)) == -1)
        return NULL;
    /*是否进行rehash操作中，如果是则插入到散列表ht[1]中，否则插入到散列表ht[0]*/
    ht = dictIsRehashing(d) ? &d->ht[1] : &d->ht[0];
    /*申请新节点内存，插入散列表中，给新节点存入键信息*/
    entry = zmalloc(sizeof(*entry));
    entry->next = ht->table[index];
    ht->table[index] = entry;
    ht->used++;

    /* Set the hash entry fields. */
    dictSetKey(d, entry, key);
    return entry;
}
```



dictAddRaw() 函数主要完成以下几个工作：

```properties
1). 判断是否正在进行rehash操作（dictIsRehashing() 判断为真），如果是就调用 
_dictRehashStep() 函数进行rehash。

2). 通过调用 _dictKeyIndex() 函数计算key对应所在哈希表的位置（索引）index。

3).如果正在进行rehash，那么就使用ht数组的第二个哈希表，否则就用第一个

4).创建一个 dictEntry 结构用于保存数据的键和值。
```

dictAddRaw() 函数会返回一个类型为 dictEntry 结构的值，然后 dictAdd() 函数通过调用 dictSetVal() 函数设置其值。
插入元素，字典对应的内存占用结构如下图:

![](img/redis-dict4.webp)



### 字典扩容

当哈希表中的数据个数超过一定数量时，哈希冲突的链表过长，从而导致查询效率降低，这个时候就需要Rehash操作。Rehash操作是将哈希表的数组扩大，从而减少哈希冲突的比率。当然扩大哈希表的数组会导致之前的映射关系无效，所以需要把旧数据重新迁移到新的哈希表数组中。

Redis在插入数据到字典时，会通过 _dictExpandIfNeeded() 函数来判断是否需要进行Rehash操作

```c
static int _dictExpandIfNeeded(dict *d)
{
    if (dictIsRehashing(d)) return DICT_OK;

    if (d->ht[0].size == 0) return dictExpand(d, DICT_HT_INITIAL_SIZE);

    if (d->ht[0].used >= d->ht[0].size &&
        (dict_can_resize ||
         d->ht[0].used/d->ht[0].size > dict_force_resize_ratio))
    {
        return dictExpand(d, d->ht[0].used*2);
    }
    return DICT_OK;
}
/*

_dictExpandIfNeeded() 函数主要完成3个工作：

    1). 通过 dictIsRehashing() 来判断字典是否正在Rehash操作，如果是就直接返回OK，不需要再进行
    Rehash。
    2). 如果字典的第一个哈希表的大小为0，表示需要对第一个哈希表进行初始化。
    3). 如果第一个哈希表的元素个数大于等于哈希表的大小，那么就对第一个哈希表进行Rehash操作（把
    哈希表的大小扩大为原来的2倍）。
*/
```

进行Rehash操作通过调用 dictExpand() 函数来完成

```c
/*传入size = d->ht[0].used * 2 */
int dictExpand(dict *d, unsigned long size)
{
    /* the size is invalid if it is smaller than the number of
     * elements already inside the hash table */
    if (dictIsRehashing(d) || d->ht[0].used > size) 
        return DICT_ERR;

    dictht n; /* the new hash table */
    /*重新计算扩容后的值，必须为2的N次方幂*/
    unsigned long realsize = _dictNextPower(size);

    /* Rehashing to the same table size is not useful. */
    if (realsize == d->ht[0].size) return DICT_ERR;

    /* Allocate the new hash table and initialize all pointers to NULL */
    n.size = realsize;
    n.sizemask = realsize-1;
    n.table = zcalloc(realsize*sizeof(dictEntry*));
    n.used = 0;

    /* Is this the first initialization? If so it's not really a rehashing
     * we just set the first hash table so that it can accept keys. */
    if (d->ht[0].table == NULL) {
        d->ht[0] = n;
        return DICT_OK;
    }

    /* Prepare a second hash table for incremental rehashing */
    d->ht[1] = n;  /*扩容后的新内存放入ht[1]中*/
    d->rehashidx = 0; /*非-1，表示需要进行rehash*/
    return DICT_OK;
}
```

dictExpand() 函数比较简单，就是申请一个更大的哈希数组，如果第一个哈希表的哈希数组为空，那么就把第一个哈希表的哈希数组设置为新的哈希数组。否则将第二个哈希表的哈希数组设置为新的哈希数组。



扩容的主要流程如下:

```properties
1). 申请一块新内存后，初次申请是默认大小为4个dictEntry；非初次申请时，申请内存的大小为当前
Hash表容量的一倍。

2）把新申请的内存地址赋值给ht[1],并把字典的rehashidx标识从-1改为0，表示只有需要进行rehash
操作，此时字典的内存结构如下
```

![](img/redis-dict5.webp)



### 渐进式Rehash操作

从 dictExpand() 函数的实现来看，并没有在这里对数据进行Rehash操作，只是把哈希数组扩大2倍而已，那么Rehash操作在什么时候进行呢？对数据进行Rehash操作的触发点有很多个，如插入、删除和查找，当然最后都会调用 dictRehash() 函数来完成，我们来看看 dictRehash() 函数的实现： rehash 扩缩容操作都会进行触发。Redis的整个rehash实现

```c
/* Performs N steps of incremental rehashing. Returns 1 if there are still
 * keys to move from the old to the new hash table, otherwise 0 is returned.
 *
 * Note that a rehashing step consists in moving a bucket (that may have more
 * than one key as we use chaining) from the old to the new hash table, however
 * since part of the hash table may be composed of empty spaces, it is not
 * guaranteed that this function will rehash even a single bucket, since it
 * will visit at max N*10 empty buckets in total, otherwise the amount of
 * work it does would be unbound and the function may block for a long time. */
int dictRehash(dict *d, int n) {
    int empty_visits = n*10; /* Max number of empty buckets to visit. */
    if (!dictIsRehashing(d)) return 0;

    while(n-- && d->ht[0].used != 0) {
        dictEntry *de, *nextde;

        /* Note that rehashidx can't overflow as we are sure there are more
         * elements because ht[0].used != 0 */
        assert(d->ht[0].size > (unsigned long)d->rehashidx);
        while(d->ht[0].table[d->rehashidx] == NULL) {
            d->rehashidx++;
            if (--empty_visits == 0) return 1;
        }
        de = d->ht[0].table[d->rehashidx];
        /* Move all the keys in this bucket from the old to the new hash HT */
        while(de) {
            uint64_t h;

            nextde = de->next;
            /* Get the index in the new hash table */
            h = dictHashKey(d, de->key) & d->ht[1].sizemask;
            de->next = d->ht[1].table[h];
            d->ht[1].table[h] = de;
            d->ht[0].used--;
            d->ht[1].used++;
            de = nextde;
        }
        d->ht[0].table[d->rehashidx] = NULL;
        d->rehashidx++;
    }

    /* Check if we already rehashed the whole table... */
    if (d->ht[0].used == 0) {
        zfree(d->ht[0].table);
        d->ht[0] = d->ht[1];
        _dictReset(&d->ht[1]);
        d->rehashidx = -1;
        return 0;
    }

    /* More to rehash... */
    return 1;
}
```

dictRehash() 函数的第二个参数是指定了每次要对多少个槽进行Rehash（也就是冲突链表），Rehash操作就是遍历第一个哈希表的所有数据，然后重新计算key的哈希值，保存到第二个哈希表中，并且从第一个哈希表中删除。当第一个哈希表的元素个数为0时，就将第一个哈希表替换成第二个哈希表，并且完成Rehash操作。

![](img/redis-dict6.webp)



### 查找元素

Server端收到get命令后，最终要在字典中查找某个key键值对会执行dict.h中的dictFind()函数

```c
dictEntry *dictFind(dict *d, const void *key)
{
    dictEntry *he;
    uint64_t h, idx, table;

    if (d->ht[0].used + d->ht[1].used == 0) return NULL; /* dict is empty */
    if (dictIsRehashing(d)) _dictRehashStep(d);
    h = dictHashKey(d, key);    /*得到键的Hash值*/
    for (table = 0; table <= 1; table++) {  /*遍历查找Hash表  ht[0]与ht[1]*/
        idx = h & d->ht[table].sizemask;  /*根据Hash值获取到对应的索引值*/
        he = d->ht[table].table[idx];  /*获取值*/
        while(he) {   /*如果存在值则遍历该值中的单链表*/
            if (key==he->key || dictCompareKeys(d, key, he->key))
                return he;     /*找到与键相等的值，返回该元素*/
            he = he->next;
        }
        if (!dictIsRehashing(d)) return NULL;  /*如果未进行rehash操作，则只读取ht[0]*/
    }
    return NULL;
}
```

通过上面的介绍说明，dictFind() 函数的实现也比较容易理解，主要进行了如下操作:

```properties
1). 如果字典中第一个和第二个哈希表都为空，那么就返回NULL。
2). 如果判断正在进行Rehash操作，调用 _dictRehashStep() 对数据进行分步Rehash。
3). 根据键调用Hash函数取得其Hash值
4). 遍历字典的2个Hash表，读取索引对应的元素
5). 根据Hash值取到索引值
6). 根据索引在hash表中找到元素值，并先在第一个哈希表中查找是否存在，如果存在就返回key对应的
值。如果key不在第一个哈希表中，那么就要判断当前是否正在Rehash操作，如果是就在第二个哈希表中查
找key是否存在。因为在Rehash的过程中，key有可能被移动到第二个哈希表中。
7). 找不到则返回NULL
```

### 修改元素

```shell
$  set hello world2
```

Server端收到set命令后，会查询键是否已经在数据库中存在，存在则执行db.c文件中的dbOverwrite函数

```c
/* Overwrite an existing key with a new value. Incrementing the reference
 * count of the new value is up to the caller.
 * This function does not modify the expire time of the existing key.
 * The program is aborted if the key was not already present. */
void dbOverwrite(redisDb *db, robj *key, robj *val) {
    dictEntry *de = dictFind(db->dict,key->ptr);  /*查找键是否存在，返回存在的节点*/
    serverAssertWithInfo(NULL,key,de != NULL); /*不存在则中断执行*/
    dictEntry auxentry = *de;
    robj *old = dictGetVal(de);   /*获取老节点val字段值*/
    if (server.maxmemory_policy & MAXMEMORY_FLAG_LFU) {
        val->lru = old->lru;
    }
    dictSetVal(db->dict, de, val);   /*给节点设置新的值*/
    if (server.lazyfree_lazy_server_del) {
        freeObjAsync(old);
        dictSetVal(db->dict, &auxentry, NULL);
    }
    dictFreeVal(db->dict, &auxentry);   /*释放节点中旧val内存*/
}
```

### 删除元素

```shell
$ del hello
```

Server收到del命令后，删除键值对会执行dict.h文件中的dictDelete函数

```c
/* 查找并删除元素 */
static int dictDelete(dict *ht, const void *key) {
    unsigned int h;
    dictEntry *de, *prevde;

    if (ht->size == 0)
        return DICT_ERR;
    h = dictHashKey(ht, key) & ht->sizemask;
    de = ht->table[h];

    prevde = NULL;
    while(de) {
        if (dictCompareHashKeys(ht,key,de->key)) {  /*比对hash值*/
            /* Unlink the element from the list */
            if (prevde)
                prevde->next = de->next;
            else
                ht->table[h] = de->next;

            dictFreeEntryKey(ht,de);  /*释放该节点对应的键占用的内存*/
            dictFreeEntryVal(ht,de);  /*释放该节点对应的值占用的内存*/
            free(de);   /*释放本身占用内存*/
            ht->used--;  /*used -1*/
            return DICT_OK;
        }
        prevde = de;
        de = de->next;
    }
    return DICT_ERR; /* not found */
}
```

删除函数主要进行以下操作

```properties
1). 查找该键释放存在该字典中。
2). 存在则把该节点从单链表中删除。
3). 释放该节点对应键占用的内存，值占用的内存，以及本身占用的内存。
4). 给对应的Hash表的used字典减1操作
```



### API

```c
/* API */
dict *dictCreate(dictType *type, void *privDataPtr); /*初始化字典*/
int dictExpand(dict *d, unsigned long size);  /*字典扩容*/
int dictAdd(dict *d, void *key, void *val); /*添加键值对，已存在则不加*/
 /*添加key，并返回新添加的key对应的节点。若已存在，则存入existing字段中，并返回-1*/
dictEntry *dictAddRaw(dict *d, void *key, dictEntry **existing);
dictEntry *dictAddOrFind(dict *d, void *key); /*添加或者查找key*/
int dictReplace(dict *d, void *key, void *val);  /*添加键值对，若存在则修改，否则添加*/
int dictDelete(dict *d, const void *key);  /*删除元素*/
dictEntry *dictUnlink(dict *ht, const void *key);  /*删除key，但不释放内存*/
void dictFreeUnlinkedEntry(dict *d, dictEntry *he); /*释放dictUnlink函数删除key的内存*/
void dictRelease(dict *d);   /*释放字典*/
dictEntry * dictFind(dict *d, const void *key);  /*根据键查找元素*/
void *dictFetchValue(dict *d, const void *key);  /*根据键查找出值*/
int dictResize(dict *d);   /*扩缩容字典*/
dictIterator *dictGetIterator(dict *d); /*初始化普通迭代器*/
dictIterator *dictGetSafeIterator(dict *d);   /*初始化安全迭代器*/
dictEntry *dictNext(dictIterator *iter);  /*通过迭代器获取下一个节点*/
void dictReleaseIterator(dictIterator *iter);  /*释放迭代器*/
dictEntry *dictGetRandomKey(dict *d);  /*随机得到一个键*/
dictEntry *dictGetFairRandomKey(dict *d); 
/*随机得到一些键*/
unsigned int dictGetSomeKeys(dict *d, dictEntry **des, unsigned int count);
void dictGetStats(char *buf, size_t bufsize, dict *d);  /*读取字典的状态，使用情况等*/
uint64_t dictGenHashFunction(const void *key, int len); /*hash函数 字母大小写敏感*/
/*hash函数 字母大小写不敏感*/
uint64_t dictGenCaseHashFunction(const unsigned char *buf, int len);
void dictEmpty(dict *d, void(callback)(void*));  /*清空一个字典*/
void dictEnableResize(void); /*开启Resize*/
void dictDisableResize(void); /*关闭Resize*/
int dictRehash(dict *d, int n);  /*渐进式rehash, n为进行几步*/
int dictRehashMilliseconds(dict *d, int ms);   /*持续性rehash， ms为持续多久*/
void dictSetHashFunctionSeed(uint8_t *seed);  /*设置新的散列种子*/
uint8_t *dictGetHashFunctionSeed(void);  /*获取当前散列种子值*/
unsigned long dictScan(dict *d, unsigned long v, dictScanFunction *fn, 
dictScanBucketFunction *bucketfn, void *privdata);  /*间断性的迭代字段数据*/
uint64_t dictGetHash(dict *d, const void *key);  /*得到键的hash值*/
 /*使用指针+hash值去查找元素*/
dictEntry **dictFindEntryRefByPtrAndHash(dict *d, const void *oldptr, uint64_t hash); 
```


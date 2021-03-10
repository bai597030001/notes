

## MyISAM

- 1.优点：支持全文索引，访问速度快

- 2.缺点：不支持事务，也不支持外键

- 3.适用场景

	对事务完整性没有要求或者以SELECT、INSERT为主的应用基本都可以使用这个引擎来创建表。

- 4.存储方式

```

	.frm(存储表定义)  
	.MYD(MYData，存储数据)  
	.MYI(MYIndex，存储索引)

```

- 5.支持的表大小

```

	MySQL 5.0版本之前，MyISAM默认支持的表大小为4GB，若需要支持大于4GB的MyISAM表时，则需要指定MAX_ROWS和AVG_ROW_LENGTH属性。
	
	从MySQL5.0开始，MyISAM默认支持256T的单表数据。
```

- 5.表修复

```

	MyISAM表还有一个标志用来表明该数据表在上次使用后是不是被正常的关闭了。如果服务器以为当机或崩溃，这个标志可以用来判断数据表是否需要检查和修复。

	如果想让这种检查自动进行，可以在启动服务器时使用--myisam-recover现象。这会让服务器在每次打开一个MyISAM数据表是自动检查数据表的标志并进行必要的修复处理。

	MyISAM类型的表可能会损坏，可以使用CHECK TABLE语句来检查MyISAM表的健康，并用REPAIR TABLE语句修复一个损坏到MyISAM表
```

## InnoDB

- 特性

1.支持事务

2.支持行锁

3.支持外键 

4.支持类似于Oracle的行锁定读，即默认读取操作不会产生锁。

5.支持自动增长列AUTO_INCREMENT。自动增长列的值不能为空，且值必须唯一，且必须为主键。
	在执行插入操作时，若不指定自动增长列的值，或自动增长列的值为0或NULL，则插入的值为自动增长后的值。

6.创建的表的结构存储于.frm文件中。数据和索引存储在innodb_data_home和innodb_data_path表空间中(.ibd)。

7.对于表的数据存储，InnoDB存储引擎采用了聚集的方式，每张表的存储都是按主键顺序进行存放。
	若没有显示地在表定义时指定主键，InnoDB会为每一行生成一个6字节的ROWID，并以此作为主键。

8.InnoDB支持外键。外键所在的表为子表，外键依赖的表为父表。父表中被主表外键支持的字段必须为主键。
	当删除、更新浮标的某条信息时，子表也必须有相应的改变。

9.InnoDB通过多版本并发控制（MVCC）来获得高并发性，并实现了SQL标准的4种隔离机制，默认为REPEATABLE级别。
	同时使用一种被称为next-key locking的策略来避免幻读。InnoDB还提供了插入缓冲、二次写、自适应哈希索引、预读等高性能和高可用的功能。

适用场景

​	1.更新密集的表。InnoDB存储引擎特别适合处理多重并发的更新请求。

​	2.事务。InnoDB存储引擎是支持事务的标准MySQL存储引擎。

​	3.自动灾难恢复。与其它存储引擎不同，InnoDB表能够自动从灾难中恢复。

​	4.外键约束。MySQL支持外键的存储引擎只有InnoDB。

​	5.支持自动增加列AUTO_INCREMENT属性。

一般来说，如果需要事务支持，并且有较高的并发读取频率，InnoDB是不错的选择。



### InnoDB一棵B+树可以存放多少行数据

InnoDB 一棵 B + 树可以存放多少行数据？这个问题的简单回答是：约 2 千万

为什么是这么多呢？因为这是可以算出来的，要搞清楚这个问题，我们先从 InnoDB 索引数据结构、数据组织方式说起。

我们都知道计算机在存储数据的时候，有最小存储单元，这就好比我们今天进行现金的流通最小单位是一毛。

在计算机中磁盘存储数据最小单元是扇区，一个扇区的大小是 512 字节，而文件系统（例如 XFS/EXT4）他的最小单元是块，一个块的大小是 4k

而对于我们的 InnoDB 存储引擎也有自己的最小储存单元------页（Page），一个页的大小是 16K。

下面几张图可以帮你理解最小存储单元：

文件系统中一个文件大小只有 1 个字节，但不得不占磁盘上 4KB 的空间。

![img](https://mmbiz.qpic.cn/mmbiz_png/1J6IbIcPCLa4R1Ydibhv1JYVp4bwTY7kEWcRAqvJ6QqNgqHVw4iavbuR3pG78JdS3icafulQE5mXIYyZNIotmbtiaA/640?wx_fmt=png&tp=webp&wxfrom=5&wx_lazy=1&wx_co=1)

innodb 的所有数据文件（后缀为 ibd 的文件），他的大小始终都是 16384（16k）的整数倍。

![img](https://mmbiz.qpic.cn/mmbiz_png/1J6IbIcPCLa4R1Ydibhv1JYVp4bwTY7kEcwxRMz4doaB82tpFEjRAtGr9UZbxREMyADMXLonEQDmqpG2oM3DGMg/640?wx_fmt=png&tp=webp&wxfrom=5&wx_lazy=1&wx_co=1)

磁盘扇区、文件系统、InnoDB 存储引擎都有各自的最小存储单元。

![img](https://mmbiz.qpic.cn/mmbiz_png/1J6IbIcPCLa4R1Ydibhv1JYVp4bwTY7kEKSPz6ozRz2VTFyj2Qpnnia4aE4BTr69ibqDlrc3wA0LiamOqLdscEwWcw/640?wx_fmt=png&tp=webp&wxfrom=5&wx_lazy=1&wx_co=1)

在 MySQL 中我们的 InnoDB 页的大小默认是 16k，当然也可以通过参数设置：

![img](https://mmbiz.qpic.cn/mmbiz_png/1J6IbIcPCLa4R1Ydibhv1JYVp4bwTY7kEqgd6Vj5xe8T2TkqlykMgOvYJ4FW8Oa9VAK9vyU0DKsCNa0WAESibG2Q/640?wx_fmt=png&tp=webp&wxfrom=5&wx_lazy=1&wx_co=1)

数据表中的数据都是存储在页中的，所以一个页中能存储多少行数据呢？假设一行数据的大小是 1k，那么一个页可以存放 16 行这样的数据。

如果数据库只按这样的方式存储，那么如何查找数据就成为一个问题

因为我们不知道要查找的数据存在哪个页中，也不可能把所有的页遍历一遍，那样太慢了。

所以人们想了一个办法，用 B + 树的方式组织这些数据。如图所示：

![img](https://mmbiz.qpic.cn/mmbiz_png/1J6IbIcPCLa4R1Ydibhv1JYVp4bwTY7kEdBOH4QQibh7a33yEfKgr6DAUeJNib38m0k1l8yWDXte9ZQJ24lsojzOQ/640?wx_fmt=png&tp=webp&wxfrom=5&wx_lazy=1&wx_co=1)

我们先将数据记录按主键进行排序，分别存放在不同的页中（为了便于理解我们这里一个页中只存放 3 条记录，实际情况可以存放很多）

除了存放数据的页以外，还有存放键值 + 指针的页，如图中 page number=3 的页，该页存放键值和指向数据页的指针，这样的页由 N 个键值 + 指针组成。

当然它也是排好序的。这样的数据组织形式，我们称为索引组织表。

现在来看下，要查找一条数据，怎么查？

```
如：select * from user where id=5;
```

这里 id 是主键, 我们通过这棵 B + 树来查找，首先找到根页，你怎么知道 user 表的根页在哪呢？

其实每张表的根页位置在表空间文件中是固定的，即 page number=3 的页（这点我们下文还会进一步证明）

找到根页后通过二分查找法，定位到 id=5 的数据应该在指针 P5 指向的页中，那么进一步去 page number=5 的页中查找，同样通过二分查询法即可找到 id=5 的记录：

```
5    zhao2    27
```

现在我们清楚了 InnoDB 中主键索引 B + 树是如何组织数据、查询数据的，我们总结一下：

1、InnoDB 存储引擎的最小存储单元是页，页可以用于存放数据也可以用于存放键值 + 指针，在 B + 树中叶子节点存放数据，非叶子节点存放键值 + 指针。

2、索引组织表通过非叶子节点的二分查找法以及指针确定数据在哪个页中，进而在去数据页中查找到需要的数据；

那么回到我们开始的问题，通常一棵 B + 树可以存放多少行数据？

这里我们先假设 B + 树高为 2，即存在一个根节点和若干个叶子节点，那么这棵 B + 树的存放总记录数为：根节点指针数 * 单个叶子节点记录行数。

上文我们已经说明单个叶子节点（页）中的记录数 = 16K/1K=16。（这里假设一行记录的数据大小为 1k，实际上现在很多互联网业务数据记录大小通常就是 1K 左右）。

那么现在我们需要计算出非叶子节点能存放多少指针？

其实这也很好算，我们假设主键 ID 为 bigint 类型，长度为 8 字节，而指针大小在 InnoDB 源码中设置为 6 字节，这样一共 14 字节

我们一个页中能存放多少这样的单元，其实就代表有多少指针，即 16384/14=1170。

那么可以算出一棵高度为 2 的 B + 树，能存放 1170*16=18720 条这样的数据记录。

根据同样的原理我们可以算出一个高度为 3 的 B + 树可以存放：1170X1170X16=21902400 条这样的记录。

所以在 InnoDB 中 B + 树高度一般为 1-3 层，它就能满足千万级的数据存储。

在查找数据时一次页的查找代表一次 IO，所以通过主键索引查询通常只需要 1-3 次 IO 操作即可查找到数据。

> 怎么得到 InnoDB 主键索引 B + 树的高度？

上面我们通过推断得出 B + 树的高度通常是 1-3，下面我们从另外一个侧面证明这个结论。

在 InnoDB 的表空间文件中，约定 page number 为 3 的代表主键索引的根页，而在根页偏移量为 64 的地方存放了该 B + 树的 page level。

如果 page level 为 1，树高为 2，page level 为 2，则树高为 3。即 B + 树的高度 = page level+1；下面我们将从实际环境中尝试找到这个 page level。

在实际操作之前，你可以通过 InnoDB 元数据表确认主键索引根页的 page number 为 3，你也可以从《InnoDB 存储引擎》这本书中得到确认。

![img](https://mmbiz.qpic.cn/mmbiz_png/1J6IbIcPCLa4R1Ydibhv1JYVp4bwTY7kEicIw5qibLusqNtibI5hGAWJM3ZfW50a7tTfRnyibegexDX4MP9GB6eoGbA/640?wx_fmt=png&tp=webp&wxfrom=5&wx_lazy=1&wx_co=1)

![img](https://mmbiz.qpic.cn/mmbiz_jpg/1J6IbIcPCLa4R1Ydibhv1JYVp4bwTY7kEmibTvG86EjCiaczP2OP3vUXvau3wCDxBxLic7j6nP7DHGuseibk9sSXctQ/640?wx_fmt=jpeg&tp=webp&wxfrom=5&wx_lazy=1&wx_co=1)

可以看出数据库 dbt3 下的 customer 表、lineitem 表主键索引根页的 page number 均为 3，而其他的二级索引 page number 为 4。

关于二级索引与主键索引的区别请参考 MySQL 相关书籍，本文不在此介绍。

下面我们对数据库表空间文件做想相关的解析：

![img](https://mmbiz.qpic.cn/mmbiz_png/1J6IbIcPCLa4R1Ydibhv1JYVp4bwTY7kEXiaCia8s9YUB4hRU2gYOzVpWfOGxGibDz8010LUOPlZaiapGkADOiaU3zZw/640?wx_fmt=png&tp=webp&wxfrom=5&wx_lazy=1&wx_co=1)

因为主键索引 B + 树的根页在整个表空间文件中的第 3 个页开始，所以可以算出它在文件中的偏移量：16384*3=49152（16384 为页大小）。

另外根据《InnoDB 存储引擎》中描述在根页的 64 偏移量位置前 2 个字节，保存了 page level 的值

因此我们想要的 page level 的值在整个文件中的偏移量为：16384*3+64=49152+64=49216，前 2 个字节中。

接下来我们用 hexdump 工具，查看表空间文件指定偏移量上的数据：

![img](https://mmbiz.qpic.cn/mmbiz_png/1J6IbIcPCLa4R1Ydibhv1JYVp4bwTY7kE4HGW9mCxqibjwyFPgeWAjgp1FRAI5M5iaToSuHljCfPpWn1LhN191BicQ/640?wx_fmt=png&tp=webp&wxfrom=5&wx_lazy=1&wx_co=1)

linetem 表的 page level 为 2，B + 树高度为 page level+1=3；

region 表的 page level 为 0，B + 树高度为 page level+1=1；

customer 表的 page level 为 2，B + 树高度为 page level+1=3；

这三张表的数据量如下：

![img](https://mmbiz.qpic.cn/mmbiz_png/1J6IbIcPCLa4R1Ydibhv1JYVp4bwTY7kE47icoeDyGTd6msGmdCO93Jl0ziceHnxSnoZhTYMnaAKKjswicNTVTUrwQ/640?wx_fmt=png&tp=webp&wxfrom=5&wx_lazy=1&wx_co=1)

> 总结：

lineitem 表的数据行数为 600 多万，B + 树高度为 3，customer 表数据行数只有 15 万，B + 树高度也为 3。可以看出尽管数据量差异较大，这两个表树的高度都是 3

换句话说这两个表通过索引查询效率并没有太大差异，因为都只需要做 3 次 IO。那么如果有一张表行数是一千万，那么他的 B + 树高度依旧是 3，查询效率仍然不会相差太大。

region 表只有 5 行数据，当然他的 B + 树高度为 1。



> 最后回顾一道面试题

有一道 MySQL 的面试题，为什么 MySQL 的索引要使用 B + 树而不是其它树形结构? 比如 B 树？

现在这个问题的复杂版本可以参考本文；

他的简单版本回答是：

因为 B 树不管叶子节点还是非叶子节点，都会保存数据，这样导致在非叶子节点中能保存的指针数量变少（有些资料也称为扇出）

指针少的情况下要保存大量数据，只能增加树的高度，导致 IO 操作变多，查询性能变低；



> 小结

本文从一个问题出发，逐步介绍了 InnoDB 索引组织表的原理、查询方式，并结合已有知识，回答该问题，结合实践来证明。

当然为了表述简单易懂，文中忽略了一些细枝末节，比如一个页中不可能所有空间都用于存放数据，它还会存放一些少量的其他字段比如 page level，index number 等等，另外还有页的填充因子也导致一个页不可能全部用于保存数据。

关于二级索引数据存取方式可以参考 MySQL 相关书籍，他的要点是结合主键索引进行回表查询。



## MEMORY

- 1.优点： 速度快。为得到最快的响应时间，采用的逻辑存储介质是系统内存。

- 2.缺点： 

```

	虽然在内存中存储表数据确实会提供很高的性能，但当mysqld守护进程崩溃时，所有的Memory数据都会丢失。
	获得速度的同时也带来了一些缺陷。
	
	它要求存储在Memory数据表里的数据使用的是长度不变的格式，这意味着不能使用BLOB和TEXT这样的长度可变的数据类型，
	VARCHAR是一种长度可变的类型，但因为它在MySQL内部当做长度固定不变的CHAR类型，所以可以使用。

	只支持表锁，并发性能较差。
```

- 3.存储

	将表中的数据存储在内存中。表结构以文件存储于磁盘 .frm

- 4.支持的表大小

	MEMORY表的大小主要取决于两个参数，分别是max_rows和max_heap_table_size：

		max_rows可以在创建表时指定；
		max_heap_table_size的大小默认为16MB，可以按需要进行扩大。

- 5.支持的索引：同时支持散列索引和B树索引
	
	散列索引
	
		可以使用部分查询和通配查询，也可以使用<、>和>=等操作符方便数据挖掘。
	
	B树索引
	
		散列索引进行“相等比较”非常快，但是对“范围比较”的速度就慢多了，因此散列索引值适合使用在=和<>的操作符中，
		不适合在<或>操作符中，也同样不适合用在order by子句中。

	示例：

		下述代码创建了一个表，在username字段上使用了HASH散列索引。
		
		create table users
		(
		    id smallint unsigned not null auto_increment,
		    username varchar(15) not null,
		    pwd varchar(15) not null,
		    index using hash (username),
		    primary key (id)
		)engine=memory;
		
		下面的代码就创建一个表，使用BTREE索引。
				
		create table users
		(
		    id smallint unsigned not null auto_increment,
		    username varchar(15) not null,
		    pwd varchar(15) not null,
		    index using btree (username),
		    primary key (id)
		)engine=memory;

---

- 6.适用场景

```

	1.目标数据较小，而且被非常频繁地访问。在内存中存放数据，所以会造成内存的使用，
		可以通过参数max_heap_table_size控制Memory表的大小，设置此参数，就可以限制Memory表的最大大小。
	
	2.如果数据是临时的，而且要求必须立即可用，那么就可以存放在内存表中。
	
	3.存储在Memory表中的数据如果突然丢失，不会对应用服务产生实质的负面影响。

```

## MERGE

- MERGE存储引擎是一组MyISAM表的组合，这些MyISAM表结构必须完全相同。

```

	尽管其使用不如其它引擎突出，但是在某些情况下非常有用。说白了，Merge表就是几个相同MyISAM表的聚合器；

	Merge表中并没有数据，对Merge类型的表可以进行查询、更新、删除操作，这些操作实际上是对内部的MyISAM表进行操作。

```

- 使用场景
	
```

	对于服务器日志这种信息，一般常用的存储策略是将数据分成很多表，每个名称与特定的时间端相关。

	例如：可以用12个相同的表来存储服务器日志数据，每个表用对应各个月份的名字来命名。

	当有必要基于所有12个日志表的数据来生成报表，这意味着需要编写并更新多表查询，以反映这些表中的信息。

	与其编写这些可能出现错误的查询，不如将这些表合并起来使用一条查询，之后再删除Merge表，而不影响原来的数据，

	删除Merge表只是删除Merge表的定义，对内部的表没有任何影响。

```

## ARCHIVE

- 归档。在归档之后很多的高级功能就不再支持了，仅仅支持最基本的插入和查询两种功能。

- 在MySQL 5.5版以前，Archive是不支持索引，但是在MySQL 5.5以后的版本中就开始支持索引了。

- Archive拥有很好的压缩机制，它使用zlib压缩库，在记录被请求时会实时压缩，所以它经常被用来当做仓库使用。

# 如何选择合适的存储引擎？

选择标准可以分为：

	（1）是否需要支持事务；
	
	（2）是否需要使用热备；
	
	（3）崩溃恢复：能否接受崩溃；
	
	（4）是否需要外键支持；

然后按照标准，选择对应的存储引擎即可。
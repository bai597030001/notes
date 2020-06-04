# centos本地安装

## 安装

1.从[rpm包](<https://download.postgresql.org/pub/repos/yum/>)或[官网](<https://www.postgresql.org/download>)中下载rpm包

```shell
$ rpm -ivh pgdg-redhat-repo-latest.noarch.rpm
```

2.安装客户端

```shell
$ yum install postgresql12
```

3.安装服务端

```shell
$ yum install postgresql12-server
```

4.验证是否安装成功

```shell
$ rpm -aq| grep postgres
```



## 配置

初始化数据库

```shell
$ /usr/pgsql-12/bin/postgresql-12-setup initdb
```

启用开机自启动

```shell
$ systemctl enable postgresql-12
$ systemctl start postgresql-12
```

配置防火墙

```shell
$ firewall-cmd --permanent --add-port=5432/tcp  
$ firewall-cmd --permanent --add-port=80/tcp  
$ firewall-cmd --reload
```



```shell
$ su postgres  # 切换用户, postgres为安装postgresql后的默认用户

$ psql -U postgres # 登录数据库

$ ALTER USER postgres WITH PASSWORD 'postgres'  # 设置postgres用户密码为 postgres

$ \q # 退出
```



开启远程访问

```shell
$ vim /var/lib/pgsql/12/data/postgresql.conf

# 修改#listen_addresses = 'localhost'  为  listen_addresses='*'
# 当然，此处‘*’也可以改为任何你想开放的服务器IP
```



信任远程连接

```shell
$ vim /var/lib/pgsql/12/data/pg_hba.conf

# 修改如下内容，信任指定服务器连接
# IPv4 local connections:
host    all            all      127.0.0.1/32      trust
host    all            all      192.168.157.1/32（需要连接的服务器IP）  trust
```



重启

```shell
$ systemctl restart postgresql-12
```



# 数据类型

pg提供了丰富的数据类型，也可以自定义数据类型来满足业务需求。

## 数值类型

|   名称    | 描述                                   | 存储大小 | 范围                                                    |
| :-------: | -------------------------------------- | -------- | ------------------------------------------------------- |
| smallint  | 存储整数，小范围                       | 2字节    | -32768 至 +32767                                        |
|  integer  | 存储整数。使用这个类型可存储典型的整数 | 4字节    | -2147483648 至 +2147483647                              |
|  bigint   | 存储整数，大范围。                     | 8字节    | -9223372036854775808 至 9223372036854775807             |
|  decimal  | 用户指定的精度，精确                   | 变量     | 小数点前最多为131072个数字; 小数点后最多为16383个数字。 |
|  numeric  | 用户指定的精度，精确                   | 变量     | 小数点前最多为131072个数字; 小数点后最多为16383个数字。 |
|   real    | 可变精度，不精确                       | 4字节    | 6位数字精度                                             |
|  double   | 可变精度，不精确                       | 8字节    | 15位数字精度                                            |
|  serial   | 自动递增整数                           | 4字节    | 1 至 2147483647                                         |
| bigserial | 大的自动递增整数                       | 8字节    | 1 至 9223372036854775807                                |

## 字符串类型

|        数据类型         | 描述                                                         |
| :---------------------: | ------------------------------------------------------------ |
|       char(size)        | 这里`size`是要存储的字符数。固定长度字符串，右边的空格填充到相等大小的字符。 |
|     character(size)     | 这里`size`是要存储的字符数。 固定长度字符串。 右边的空格填充到相等大小的字符。 |
|      varchar(size)      | 这里`size`是要存储的字符数。 可变长度字符串。                |
| character varying(size) | 这里`size`是要存储的字符数。 可变长度字符串。                |
|          text           | 可变长度字符串。                                             |

## 日期/时间类型

|             名称              | 描述                   | 存储大小 | 最小值        | 最大值        |
| :---------------------------: | ---------------------- | -------- | ------------- | ------------- |
| timestamp [ (p) ] [不带时区 ] | 日期和时间(无时区)     | 8字节    | 4713 bc       | 294276 ad     |
|    timestamp [ (p) ]带时区    | 包括日期和时间，带时区 | 8字节    | 4713 bc       | 294276 ad     |
|             date              | 日期(没有时间)         | 4字节    | 4713 bc       | 5874897 ad    |
|   time [ (p) ] [ 不带时区 ]   | 时间(无日期)           | 8字节    | 00:00:00      | 24:00:00      |
|      time [ (p) ] 带时区      | 仅限时间，带时区       | 12字节   | 00:00:00+1459 | 24:00:00-1459 |
|  interval [ fields ] [ (p) ]  | 时间间隔               | 12字节   | -178000000年  | 178000000年   |

## 布尔类型

| 名称    | 描述                          | 存储大小 |
| ------- | ----------------------------- | -------- |
| boolean | 它指定`true`或`false`的状态。 | 1字节    |

## 货币类型

| 名称  | 描述     | 存储大小 | 范围                                           |
| ----- | -------- | -------- | ---------------------------------------------- |
| money | 货币金额 | 8字节    | -92233720368547758.08 至 +92233720368547758.07 |

## 几何类型

几何数据类型表示二维空间对象。最根本的类型：**点** - 形成所有其他类型的基础。

| 名称    | 存储大小   | 表示                   | 描述                        |
| ------- | ---------- | ---------------------- | --------------------------- |
| point   | 16字节     | 在一个平面上的点       | (x,y)                       |
| line    | 32字节     | 无限线(未完全实现)     | ((x1,y1),(x2,y2))           |
| lseg    | 32字节     | 有限线段               | ((x1,y1),(x2,y2))           |
| box     | 32字节     | 矩形框                 | ((x1,y1),(x2,y2))           |
| path    | 16+16n字节 | 封闭路径(类似于多边形) | ((x1,y1),…)                 |
| polygon | 40+16n字节 | 多边形(类似于封闭路径) | ((x1,y1),…)                 |
| circle  | 24字节     | 圆                     | `<(x，y)，r>`(中心点和半径) |

## 枚举类型

- 枚举类型是一个包含静态和值的有序集合的数据类型。

- PostgtesSQL中的枚举类型类似于 C 语言中的 enum 类型。

- 与其他类型不同的是枚举类型需要使用 CREATE TYPE 命令创建。

```shell
$ CREATE TYPE mood AS ENUM ('sad', 'ok', 'happy');

$ CREATE TYPE week AS ENUM ('Mon', 'Tue', 'Wed', 'Thu', 'Fri', 'Sat', 'Sun');
```

示例

```shell
CREATE TYPE mood AS ENUM ('sad', 'ok', 'happy');
CREATE TABLE person (
    name text,
    current_mood mood
);
INSERT INTO person VALUES ('Moe', 'happy');
SELECT * FROM person WHERE current_mood = 'happy';
 name | current_mood 
------+--------------
 Moe  | happy
(1 row)
```

## 网络地址类型

PostgreSQL 提供用于存储 IPv4 、IPv6 、MAC 地址的数据类型。

用这些数据类型存储网络地址比用纯文本类型好， 因为这些类型提供输入错误检查和特殊的操作和功能。

| 名字    | 存储空间     | 描述                    |
| :------ | :----------- | :---------------------- |
| cidr    | 7 或 19 字节 | IPv4 或 IPv6 网络       |
| inet    | 7 或 19 字节 | IPv4 或 IPv6 主机和网络 |
| macaddr | 6 字节       | MAC 地址                |

## 位串类型

位串就是一串 1 和 0 的字符串。它们可以用于存储和直观化位掩码。 我们有两种 SQL 位类型：bit(n) 和bit varying(n)， 这里的n是一个正整数。

bit 类型的数据必须准确匹配长度 n， 试图存储短些或者长一些的数据都是错误的。bit varying 类型数据是最长 n 的变长类型；更长的串会被拒绝。 写一个没有长度的bit 等效于 bit(1)， 没有长度的 bit varying 意思是没有长度限制。

## 文本搜索类型

全文检索即通过自然语言文档的集合来找到那些匹配一个查询的检索。

| 序号 |                         名字 & 描述                          |
| :--- | :----------------------------------------------------------: |
| 1    | **tsvector**。tsvector 的值是一个无重复值的 lexemes 排序列表， 即一些同一个词的不同变种的标准化。 |
| 2    | **tsquery**。tsquery 存储用于检索的词汇，并且使用布尔操作符 &(AND)，\|(OR)和!(NOT) 来组合它们，括号用来强调操作符的分组。 |

## JSON/JSONB类型

JSON数据类型可以用来存储 JSON数据， 这样的数据也可以存储为 text，但是 json 数据类型更有利于检查每个存储的数值是可用的 JSON 值。

JSONB类型 – JSON 的二进制形式，与 JSON 不同的是它删除了数据中的空格，保存对象的顺序不一样，存储层面做了优化，只有最后一个重复的键值保留。

JSONB通常是首选的格式因为它需要更少的空间存储对象，可以被索引，处理速度更快，因为它不需要被解析



[官方运算符使用说明](https://www.postgresql.org/docs/9.6/functions-json.html)



9.5以下需要整个记录更新，不可以单独修改某个值。

9.5以上可以单独更新某个值。



Pg 对 json 的支持已经比较成熟, 除了把结果转换成 json 外, 还可以:

- 按 json field 的内容作为查询条件
- 以 json field 的内容建立索引
- 把 json 和 result row 相互转换
- 甚至还能把 json 当成数据集和视图
- 甚至迭代 json 内容做循环



官方文档上说:

> 有两个JSON数据类型：json和jsonb。它们接受几乎 相同的值组作为输入。它们实际的主要差别是效率。json 数据类型存储输入文本的精确拷贝，处理函数必须在每个执行上重新解析；而jsonb数据以分解的二进制格式存储，这使得它由于添加了转换机制而在输入上稍微慢些，但是在处理上明显更快，因为不需要重新解析。jsonb也支持索引，这也是一个明显的优势。
>
> 因为json类型存储输入文本的精确拷贝，它将保存令牌间语义上无关紧要的空格，和JSON对象中键的顺序。另外，如果值中的一个JSON对象多次包含相同的键，那么保存所有的键/值对。（处理函数将最后一个值当做操作值。）相比之下， jsonb不保存空格，也不保存对象键的顺序，并且不保存重复对象键。如果在输入中指定了重复的键，那么只保存最后一个值。



## 数组类型

PostgreSQL 允许将字段定义成变长的多维数组。

数组类型可以是任何基本类型或用户定义类型，枚举类型或复合类型。

### 声明数组

```shell
$ CREATE TABLE sal_emp (
    name            text,
    pay_by_quarter  integer[],
    schedule        text[][]
);
# pay_by_quarter 为一位整型数组、schedule 为二维文本类型数组。

# 我们也可以使用 "ARRAY" 关键字
$ CREATE TABLE sal_emp (
   name text,
   pay_by_quarter integer ARRAY[4],
   schedule text[][]
);
```

### 插入值

插入值使用花括号 {}，元素在 {} 使用逗号隔开

```shell
$ INSERT INTO sal_emp
    VALUES ('Bill',
    '{10000, 10000, 10000, 10000}',
    '{{"meeting", "lunch"}, {"training", "presentation"}}');

$ INSERT INTO sal_emp
    VALUES ('Carol',
    '{20000, 25000, 25000, 25000}',
    '{{"breakfast", "consulting"}, {"meeting", "lunch"}}');
```

### 访问数组

```shell
# 访问数组的一个元素
$ SELECT name FROM sal_emp WHERE pay_by_quarter[1] <> pay_by_quarter[2];

 name
-------
 Carol
(1 row)

# 数组的下标数字是写在方括弧内的。
```

### 修改数组

```shell
$ UPDATE sal_emp SET pay_by_quarter = '{25000,25000,27000,27000}' WHERE name = 'Carol';

# 使用 ARRAY 构造器语法
$ UPDATE sal_emp SET pay_by_quarter = ARRAY[25000,25000,27000,27000]
    WHERE name = 'Carol';
```

### 数组中检索

要搜索一个数组中的数值，你必须检查该数组的每一个值。

```shell
$ SELECT * FROM sal_emp WHERE pay_by_quarter[1] = 10000 OR
                            pay_by_quarter[2] = 10000 OR
                            pay_by_quarter[3] = 10000 OR
                            pay_by_quarter[4] = 10000;
                            
# 可以用下面的语句找出数组中所有元素值都等于 10000 的行
$ SELECT * FROM sal_emp WHERE 10000 = ALL (pay_by_quarter);

# 也可以使用 generate_subscripts 函数
$ SELECT * FROM
   (SELECT pay_by_quarter,
           generate_subscripts(pay_by_quarter, 1) AS s
      FROM sal_emp) AS foo
 WHERE pay_by_quarter[s] = 10000;
```

## 复合类型

如果 PostgreSQL 提供的数据类型列表还不够，可以使用 `CREATE TYPE` 命令创建新的数据类型，例如复合类型，枚举，范围等。 

### 声明复合类型

```shell
$ CREATE TYPE complex AS (
    r       double precision,
    i       double precision
);

$ CREATE TYPE inventory_item AS (
    name            text,
    supplier_id     integer,
    price           numeric
);

# 定义了类型，我们就可以用它创建表

$ CREATE TABLE on_hand (
    item      inventory_item,
    count     integer
);

$ INSERT INTO on_hand VALUES (ROW('fuzzy dice', 42, 1.99), 1000);
```

### 复合类型值输入

要以文本常量书写复合类型值，在圆括弧里包围字段值并且用逗号分隔他们。 你可以在任何字段值周围放上双引号，如果值本身包含逗号或者圆括弧， 你必须用双引号括起。

```shell
# 一般格式
'( val1 , val2 , ... )'

# 示例
'("fuzzy dice",42,1.99)'
```

### 访问复合类型

要访问复合类型字段的一个域，我们写出一个点以及域的名字， 非常类似从一个表名字里选出一个字段。

实际上，因为实在太像从表名字中选取字段， 所以我们经常需要用圆括弧来避免分析器混淆。

比如，你可能需要从on_hand 例子表中选取一些子域：

```shell
# 错误示例：这样将不能工作，因为根据 SQL 语法，item是从一个表名字选取的， 而不是一个字段名字
$ SELECT item.name FROM on_hand WHERE item.price > 9.99; 

# 正确示例
$ SELECT item from on_hand;
$ SELECT (item).name FROM on_hand WHERE (item).price > 0.999;
$ SELECT (on_hand.item).name FROM on_hand WHERE (on_hand.item).price > 0.999;
```



## 范围类型

范围数据类型代表着某一元素类型在一定范围内的值。

PostgreSQL 内置的范围类型有：

- int4range — integer的范围
- int8range —bigint的范围
- numrange —numeric的范围
- tsrange —timestamp without time zone的范围
- tstzrange —timestamp with time zone的范围
- daterange —date的范围

此外，你可以定义你自己的范围类型。



# Data Size

PostgreSQL可以处理大量的数据。下面列出了当前的大小限制：

| **Limit**                 | **Value**                            |
| ------------------------- | ------------------------------------ |
| Maximum Database Size     | Unlimited                            |
| Maximum Table Size        | 32 TB                                |
| Maximum Row Size          | 1.6 TB                               |
| Maximum Field Size        | 1 GB                                 |
| Maximum Rows per Table    | Unlimited                            |
| Maximum Columns per Table | 250 - 1600 depending on column types |
| Maximum Indexes per Table | Unlimited                            |



# 数据完整性

支持的主键，约束，外键，唯一约束，非空约束，以及其它数据完整性特性确保只有合法的数据被存储。



# 索引

## 索引类型

PostgreSQL中有几种索引类型，如`B-tree`，`Hash`，`GiST`（[几何函数、地理位置相关](http://bigsec.net/b52/postgresql/functions-geometry.html)），`SP-GiST`和`GIN`（[倒排序索引，数组函数和操作符](http://bigsec.net/b52/postgresql/functions-array.html)）、`brin`、条件索引、函数索引。

> 默认情况下，`CREATE INDEX`命令使用`B-tree`索引。

### hash索引

等值查询

### GIST索引

通用搜索树，高效支持地理查询，KNN查询

### GIN索引

倒排索引

### brin索引

大幅优化顺序访问

### 条件索引

能大幅减小索引大小

### 函数索引

能优雅替代冗余字段



## 列存索引

PostgreSQL 列存索引（Column Store Index），CSI

该索引以列存形式组织数据，数据表的 INSERT/UPDATE/DELETE 操作均被同步到列存索引中

[列存索引](https://juejin.im/entry/5c7c9c55e51d4559875009ce)

> 在数据分析场景下，列存比行存更有优势。Oracle 从 12c 开始引入 in-memory组件，使其在数据实时分析和混合负载情况下的性能大幅提升，其主要特性是 In-Memory Column Store (IM column store) ，在存储中持久化的数据依然是行存，在内存中维护行存和列存两种模式的数据，行存用于 OLTP 场景，列存用于 OLAP 场景。SQL Server 同样支持 Columnstore indexes，其性能较行存有高达10倍的提升。
>
> PostgreSQL 在列存方面提出的一种 PostgreSQL 列存索引（Column Store Index）实现方法，该索引以列存形式组织数据，数据表的 INSERT/UPDATE/DELETE 操作均被同步到列存索引中，以下将从列存索引结构，并发控制，查询执行等方面介绍其如何增强 PostgreSQL 的 OLAP 处理能力。

### 创建列存索引

```shell
# 创建一个 CSI 索引，索引中每个列对应的数据存储在一个文件中，本例中的索引对应三个文件。

$ CREATE INDEX indexname ON tablename USING csi (a, b, c);
```

### 列存索引结构

[列存索引](https://juejin.im/entry/5c7c9c55e51d4559875009ce)





# fdw功能

`foreign-data wrapper`，就是将外部数据包装为sql可操作数据的模块。现在支持的外部数据源有上百种，从文件系统、到关系型数据库，nosql数据库、大数据平台都可以Wrap方式访问。[官方详细信息](https://wiki.postgresql.org/wiki/Foreign_data_wrappers)

需要下载编译安装对应的插件，如`mysql_fdw`，`mongo_fdw`等



# VACUUM

[参考链接](http://blog.itpub.net/31556440/viewspace-2375109/)

[官方中文使用手册关于VACUUM](https://docs.postgresql.tw/reference/sql-commands/vacuum)



# 命令行

```shell
$ su postgres  # 切换用户, postgres为安装postgresql后的默认用户

$ psql -U postgres # 登录数据库
```



```shell
# 列出所有数据库
$ \l

# 创建数据库
$ CREATE DATABASE database_name;

# 切换数据库
$ \c dbname

# 列出当前数据库的所有表
$ \d 

# 创建表
$ create table testcase(
    id INTEGER, 
    task_class INTEGER,
    age TEXT,
    PRIMARY KEY(id, task_class)
);

$ create table users(
	id serial,
	name varchar(20),
	sex	bool,
	action text,
	PRIMARY KEY(id)
);
$ insert into users(name,sex,action)values('tom',true,'hello postgres');
$ insert into users(name,sex,action)values('jerry',false,'hello world');

# 添加字段
$ alter table [表名] add column [字段名] [类型];

# 更改字段
$ alter table [表名] rename column [旧字段名] to [新字段名];

# 删除表
$ drop table 
# 清空表
$ delete from [表名]
$ TRUNCATE TABLE  [表名]

# 查看指定表的所有字段
$ \d  tablename
# 查看指定表的基本情况
$ \d+  tablename
```



```shell
# 表中插入一行数据
$ insert into [表名] (字段1,字段2) values (值1,值2);

# 表中删除一行数据
$ delete from [表名] where [该行特征];

# 修改表中数据
$ update [表名] set [目标字段名]=[目标值] where [该行特征]

# 查询
$ SELECT * from tablename;
```



# 创建模式/架构

在PostgreSQL中，`CREATE SCHEMA`语句用于创建模式/架构

```shell
$ CREATE SCHEMA schema_name;
```



使用模式/架构的优点：

> 模式有助于多用户使用一个数据库，而不会互相干扰。
>
> 它将数据库对象组织成逻辑组，使其更易于管理。
>
> 可以将第三方模式放入单独的模式中，以避免与其他对象的名称相冲突。



# java API

```xml
<dependency>
    <groupId>org.postgresql</groupId>
    <artifactId>postgresql</artifactId>
    <version>9.4.1212</version>
</dependency>
```



# 对比mysql

## pg优势

1.按照SQL 标准, 做 null 判断不能用 = null, 只能用 is null

> pg 可以设置 transform_null_equals 把 = null 翻译成 is null 

2.隔离级别

> MySQL 的事务隔离级别 repeatable read 并不能阻止常见的并发更新, 得加锁才可以, 但悲观锁会影响性能, 手动实现乐观锁又复杂. 
>
> Pg 的列里有隐藏的乐观锁 version 字段, 默认的 repeatable read 级别就能保证并发更新的正确性, 并且又有乐观锁的性能

3.OVER 子句

> MySQL 不支持 OVER 子句, 而 Pg 支持. OVER 子句能简单的解决 "每组取 top 5" 的这类问题.

4.pg实现文档数据库的功能

> 为了实现文档数据库的功能, 设计了 jsonb 的存储结构,性能优于 BSON.

5.pg有地理信息处理扩展 

> GIS 扩展可以用于游戏里的地形等，可以用 Pg 搭寻路服务器和地图服务器。因为它有丰富的几何类型

6.SQL 编程能力

> PG有极其强悍的 SQL 编程能力，有非常丰富的统计函数和统计语法支持，比如分析函数
>
> 这一点上MYSQL就差的很远，很多分析功能都不支持

7.索引

> PG 可以使用函数和条件索引，这使得PG数据库的调优非常灵活
>
> mysql就没有这个功能，条件索引在web应用中很重要。

8.复制特性

> 对于WEB应用来说，复制的特性很重要，mysql是异步复制；pgsql可以做到同步，异步，半同步复制。WAL段复制，流复制（v9出现，同步、半同步、异步），逻辑复制（v10出现：订阅/发布），触发器复制，第三方复制。
>
> mysql的同步是基于binlog复制，类似oracle golden gate,是基于stream的复制，做到同步很困难，这种方式更加适合异地复制，pgsql的复制基于wal，可以做到同步复制。同时，pgsql还提供stream复制。



## mysql优势

1.内存使用率

> mysql的innodb引擎，可以充分优化利用系统所有内存，超大内存下PG对内存使用的不那么充分

2.覆盖索引

> mysql innodb支持覆盖索引
>
> pg由于索引中完全没有版本信息，不能实现Coverage index scan（即查询只扫描索引，直接从索引中返回所需的属性，还需要访问表）。

3.用户量巨大，相关文档和社区较丰富



## 总结

PG具备更高的可靠性和稳定性，对数据一致性完整性的支持高于MySQL，因此PG更加适合严格的企业应用场景（比如金融、电信、ERP、CRM）；而MySQL查询速度较快，更加适合业务逻辑相对简单、数据可靠性要求较低的互联网场景（比如google、facebook、alibaba）。

PostgreSQL和MySQL是从底层设计原理开始就不一样的两种数据库，在数据量小的时候，数据库更趋于轻量化，MySQL会更适合。但是一旦数据量稍涨，计算量上升，PostgreSQL会是更好的选择。



# 堆表与索引组织表

Oracle支持堆表，也支持索引组织表

PostgreSQL只支持堆表，不支持索引组织表

Innodb只支持索引组织表

索引组织表的优势：

> 表内的数据就是按索引的方式组织，数据是有序的，如果数据都是按主键来访问，那么访问数据比较快。而堆表，按主键访问数据时，是需要先按主键索引找到数据的物理位置。

索引组织表的劣势：

> 索引组织表中上再加其它的索引时，其它的索引记录的数据位置不再是物理位置，而是主键值，所以对于索引组织表来说，主键的值不能太大，否则占用的空间比较大。
> 对于索引组织表来说，如果每次在中间插入数据，可能会导致索引分裂，索引分裂会大大降低插入的性能。所以对于使用innodb来说，我们一般最好让主键是一个无意义的序列，这样插入每次都发生在最后，以避免这个问题。
> 由于索引组织表是按一个索引树，一般它访问数据块必须按数据块之间的关系进行访问，而不是按物理块的访问数据的，所以当做全表扫描时要比堆表慢很多，这可能在OLTP中不明显，但在数据仓库的应用中可能是一个问题。
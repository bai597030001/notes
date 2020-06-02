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

PostgreSQL中有几种索引类型，如`B-tree`，`Hash`，`GiST`（[几何函数、地理位置相关](http://bigsec.net/b52/postgresql/functions-geometry.html)），`SP-GiST`和`GIN`（[倒排序索引，数组函数和操作符](http://bigsec.net/b52/postgresql/functions-array.html)）、brin等。

每种索引类型根据不同的查询使用不同的算法。 默认情况下，`CREATE INDEX`命令使用**B树**索引。

## 列存索引

PostgreSQL 列存索引（Column Store Index），CSI

该索引以列存形式组织数据，数据表的 INSERT/UPDATE/DELETE 操作均被同步到列存索引中

[列存索引](https://juejin.im/entry/5c7c9c55e51d4559875009ce)



# 命令行

登录

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

|                             特性                             |                          Postgresql                          |                            MySQL                             |
| :----------------------------------------------------------: | :----------------------------------------------------------: | :----------------------------------------------------------: |
|                             描述                             |      The world’s most **advanced** open source database      |      The world’s most **popular** open source database       |
|                             发展                             |           PostgreSQL is an open source **project**           |             MySQL is an open-source **product**              |
|                           实现语言                           |                              C                               |                            C、C++                            |
|                          图形化工具                          |                           PgAdmin                            |                       MySQL Workbench                        |
|                             ACID                             |                             Yes                              |                             Yes                              |
|                           存储引擎                           |                  **Single** storage engine                   | **Multiple** [storage engines](http://www.mysqltutorial.org/understand-mysql-table-types-innodb-myisam.aspx) e.g., InnoDB and MyISAM |
|                           全文检索                           |                             Yes                              |                             Yes                              |
| Drop a [temporary table](http://www.postgresqltutorial.com/postgresql-temporary-table/)（删除一个临时表） | No `TEMP` or `TEMPORARY` keyword in `DROP TABLE` statement（随着数据库的连接的断开而被删除） | MySQL supports the `TEMP` or `TEMPORARY`keyword in the `DROP TABLE` statement that allows you to remove the temporary table only.（需要手动删除） |
| [`DROP TABLE`](http://www.postgresqltutorial.com/postgresql-drop-table/)（删除表） | Support `CASCADE` option to drop table’s dependent objects e.g., tables, views, etc.,（级联操作：也就是更新、删除父表，将会同步更新、删除子表；而反过来则不变） |              Does not support `CASCADE` option               |
| [`TRUNCATE TABLE`](http://www.postgresqltutorial.com/postgresql-truncate-table/)（删除表） | PostgreSQL `TRUNCATE TABLE`supports more features like `CASCADE`, `RESTART IDENTITY`, `CONTINUE IDENTITY`, transaction-safe, etc.（对于移除表中的数据，delete是可以的，但是对于一个大表，truncate是更加有效的方式，因为truncate删除表中所有行的时候不需要扫表整个表） | [MySQL `TRUNCATE TABLE`](http://www.mysqltutorial.org/mysql-truncate-table/) does not support `CASCADE` and transaction safe i.e,. once data is deleted, it cannot be rolled back.（永久性删除，不可以撤销） |
|                          自动增加列                          | [`SERIAL`](http://www.postgresqltutorial.com/postgresql-serial/) | [`AUTO_INCREMENT`](http://www.mysqltutorial.org/mysql-sequence/) |
|                           解析功能                           |                             Yes                              |                              No                              |
| [Data types](http://www.postgresqltutorial.com/postgresql-data-types/) | Support many advanced types such as [array](http://www.postgresqltutorial.com/postgresql-array/), [hstore](http://www.postgresqltutorial.com/postgresql-hstore/), and user-defined type. |                      SQL-standard types                      |
| Unsigned [integer](http://www.postgresqltutorial.com/postgresql-integer/) |                              No                              |                             Yes                              |
| [Boolean type](http://www.postgresqltutorial.com/postgresql-boolean/) |                             Yes                              | Use `TINYINT(1)` internally for [Boolean](http://www.mysqltutorial.org/mysql-boolean/) |
|                     IP address data type                     |                             Yes                              |                              No                              |
|                         设置列默认值                         |           Support both constant and function call            | Must be a constant or `CURRENT_TIMESTAMP` for `TIMESTAMP` or `DATETIME` columns |
|               CTE（Common Table Expressions）                |                             Yes                              |                              No                              |
|                       `EXPLAIN`output                        |                        More detailed                         |                        Less detailed                         |
| [Materialized views](http://www.postgresqltutorial.com/postgresql-materialized-views/)（物化视图） | Yes（Postgresql将视图概念扩展到下一个级别，允许视图在物理上存储数据，我们将这些视图称为物化视图，物化视图会缓存复杂的查询结果，然后允许定期刷新此结果） |                              No                              |
| [CHECK constraint](http://www.postgresqltutorial.com/postgresql-check-constraint/)（检查约束） |                             Yes                              | No (MySQL ignores the [CHECK constraint](http://www.mysqltutorial.org/mysql-check-constraint/)) |
|                 Table inheritance（表继承）                  |                             Yes                              |                              No                              |
| Programming languages for [stored procedures](http://www.postgresqltutorial.com/postgresql-stored-procedures/) |   Ruby, Perl, Python, TCL, PL/pgSQL, SQL, JavaScript, etc.   | SQL:2003 syntax for [stored procedures](http://www.mysqltutorial.org/mysql-stored-procedure-tutorial.aspx) |
| [`FULL OUTER JOIN`](http://www.postgresqltutorial.com/postgresql-full-outer-join/)（全外连接） |                             Yes                              |                              No                              |
| [`INTERSECT`](http://www.postgresqltutorial.com/postgresql-intersect/) | Yes（Postgresql的INTERSECT运算符将两个或多个SELECT语句的结果集合并到一个结果集中） |                              No                              |
| [`EXCEPT`](http://www.postgresqltutorial.com/postgresql-tutorial/postgresql-except/) | Yes（Except运算符通过比较两个或多个quires的结果集来返回行，此返回行存在于第一查询子句而不存在第二查询子句中） |                              No                              |
|                 Partial indexes（部分索引）                  |                             Yes                              |                              No                              |
|                  Bitmap indexes（位图索引）                  |                             Yes                              |                              No                              |
|               Expression indexes（表达式索引）               |                             Yes                              |                              NO                              |
|                 Covering indexes（覆盖索引）                 | Yes (since version 9.2)[例子1](http://flacro.me/covering-index/)、[例子2](http://getby.cn/sql__db/2016/0928/93.html) | Yes. MySQL supports covering indexes that allow data to be retrieved by scanning the index alone without touching the table data. This is advantageous in case of large tables with millions of rows. |
|                Common table expression (CTE)                 |                             Yes                              | Yes. (since version 8.0, MySQL has supported [CTE](http://www.mysqltutorial.org/mysql-cte/)) |
| [Triggers](http://www.postgresqltutorial.com/postgresql-triggers/)（触发器） | Support triggers that can fire on most types of command, except for ones affecting the database globally e.g., roles and tablespaces. |                   Limited to some commands                   |
|                     Partitioning（分区）                     |                         RANGE, LIST                          | RANGE, LIST, HASH, KEY, and composite partitioning using a combination of RANGE or LIST with HASH or KEY subpartitions |
|                  Task Schedule（任务定时）                   |                           pgAgent                            | [Scheduled event](http://www.mysqltutorial.org/mysql-triggers/working-mysql-scheduled-event/) |
|              Connection Scalability（连接规模）              |         Each new connection is an OS process（进程）         |         Each new connection is an OS thread（线程）          |
|                  SQL compliant（SQL兼容性）                  |             PostgreSQL is largely SQL compliant.             | MySQL is partially SQL compliant. For example, it does not support check constraint. |
|                         Best suited                          | PostgreSQL performance is utilized when executing complex queries. | MySQL performs well in OLAP& OLTP systems when only read speeds are needed. |
|                       Support for JSON                       | Support JSON and other NoSQL features like native XML support. It also allows indexing JSON data for faster access. | MySQL has a JSON data type support but does not support any other NoSQL feature. |
|                        Default values                        |  The default values can be changed at the system level only  | The default values can be overwritten at the session level and the statement level |
|                        B-tree Indexes                        | B-tree indexes merged at runtime to evaluate are dynamically converted predicates. | Two or more B-tree indexes can be used when it is appropriate. |
|                      Object statistics                       |                 Very good object statistics                  |                Fairly good object statistics                 |



## MySQL的缺点

1.与系统目录相关的事务不符合ACID

2.有时服务器崩溃可能会破坏系统目录

3.没有可插入的身份验证模块阻止集中管理账户

4.不支持角色，因此很难为许多用户维护权限

5.存储过程不可缓存

6.用于程序或触发器的表始终是预先锁定的



## Postgresql的缺点

1.当前的外部解决方案需要很高的学习曲线

2.没有主要版本的升级工具

3.升级过程中需要双重存储

4.索引不能用于直接返回查询结果

5.不缓存查询执行计划

6.批量加载操作可能会受CPU限制



## 总结

PG具备更高的可靠性，对数据一致性完整性的支持高于MySQL，因此PG更加适合严格的企业应用场景（比如金融、电信、ERP、CRM）；而MySQL查询速度较快，更加适合业务逻辑相对简单、数据可靠性要求较低的互联网场景（比如google、facebook、alibaba）。

PostgreSQL和MySQL是从底层设计原理开始就不一样的两种数据库，在数据量小的时候，数据库更趋于轻量化，MySQL会更适合。但是一旦数据量稍涨，计算量上升，PostgreSQL会是更好的选择。
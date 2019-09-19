# MySQL 性能调优的方法

[参考链接](<https://www.jianshu.com/p/c7f497f30d7a>)

[参考链接](<https://linuxeye.com/379.html>)

**优化工具**

- 数据库层面

  - MySQL
  - mysqladmin：MySQL 客户端，可进行管理操作
  - mysqlshow：功能强大的查看 shell 命令
  - SHOW [SESSION | GLOBAL] variables：查看数据库参数信息
  - SHOW [SESSION | GLOBAL] STATUS：查看数据库的状态信息
  - information_schema：获取元数据的方法
  - SHOW ENGINE INNODB STATUS：Innodb 引擎的所有状态
  - SHOW PROCESSLIST：查看当前所有连接的 session 状态
  - explain：获取查询语句的执行计划
  - show index：查看表的索引信息
  - slow-log：记录慢查询语句
  - mysqldumpslow：分析 slowlog 文件的工具
  - Zabbix：监控主机、系统、数据库(部署 Zabbix 监控平台)
  - pt-query-digest：分析慢日志
  - MySQL slap：分析慢日志
  - sysbench：压力测试工具
  - MySQL profiling：统计数据库整体状态工具
  - Performance Schema：MySQL 性能状态统计的数据
  - workbench：管理、备份、监控、分析、优化工具(比较费资源)

- 系统层面

  CPU方面：vmstat、sar top、htop、nmon、mpstat。

  内存：free、ps-aux。

  IO 设备(磁盘、网络)：iostat、ss、netstat、iptraf、iftop、lsof。

## 1.选择合适的存储引擎: InnoDB

## 2.保证从内存中读取数据，将数据保存在内存中

### 2.1 足够大的 innodb_buffer_pool_size

- 推荐将数据完全保存在 innodb_buffer_pool_size ，即按存储量规划 innodb_buffer_pool_size 的容量。这样你可以完全从内存中读取数据，最大限度减少磁盘操作。

#### 2.1.1 如何确定 innodb_buffer_pool_size 足够大，数据是从内存读取而不是硬盘？

- 方法 1

```text
mysql> SHOW GLOBAL STATUS LIKE 'innodb_buffer_pool_pages_%';
+----------------------------------+--------+| Variable_name                    | Value  |
+----------------------------------+--------+
| Innodb_buffer_pool_pages_data    | 129037 || Innodb_buffer_pool_pages_dirty   | 362    |
| Innodb_buffer_pool_pages_flushed | 9998   || Innodb_buffer_pool_pages_free    | 0      |  !!!!!!!!
| Innodb_buffer_pool_pages_misc    | 2035   || Innodb_buffer_pool_pages_total   | 131072 |
+----------------------------------+--------+
6 rows in set (0.00 sec)
```

发现 Innodb_buffer_pool_pages_free 为 0，则说明 buffer pool 已经被用光，需要增大 innodb_buffer_pool_size



InnoDB 的其他几个参数：

```text
innodb_additional_mem_pool_size = 1/200 of buffer_pool
innodb_max_dirty_pages_pct 80%
```

- 方法 2

`iostat -d -x -k 1` 命令，查看硬盘的操作

#### 2.1.2服务器上是否有足够内存用来规划

执行 echo 1 > /proc/sys/vm/drop_caches 清除操作系统的文件缓存，可以看到真正的内存使用量。

### 2.2 数据预热

默认情况，只有某条数据被读取一次，才会缓存在 innodb_buffer_pool。所以，数据库刚刚启动，需要进行数据预热，将磁盘上的所有数据缓存到内存中。数据预热可以提高读取速度。



对于 InnoDB 数据库，可以用以下方法，进行数据预热:



1. 将以下脚本保存为 MakeSelectQueriesToLoad.sql

```text
SELECT DISTINCT
    CONCAT('SELECT ',ndxcollist,' FROM ',db,'.',tb,    ' ORDER BY ',ndxcollist,';') SelectQueryToLoadCache    FROM
    (        SELECT
            engine,table_schema db,table_name tb,
            index_name,GROUP_CONCAT(column_name ORDER BY seq_in_index) ndxcollist        FROM
        (            SELECT
                B.engine,A.table_schema,A.table_name,
                A.index_name,A.column_name,A.seq_in_index            FROM
                information_schema.statistics A INNER JOIN
                (                    SELECT engine,table_schema,table_name                    FROM information_schema.tables WHERE
                    engine='InnoDB'
                ) B USING (table_schema,table_name)            WHERE B.table_schema NOT IN ('information_schema','mysql')            ORDER BY table_schema,table_name,index_name,seq_in_index
        ) A        GROUP BY table_schema,table_name,index_name
    ) AAORDER BY db,tb
;
```

2.执行

```text
mysql -uroot -AN < /root/MakeSelectQueriesToLoad.sql > /root/SelectQueriesToLoad.sql
```

3.每次重启数据库，或者整库备份前需要预热的时候执行：

```text
mysql -uroot < /root/SelectQueriesToLoad.sql > /dev/null 2>&1
```

## 3. 定期优化重建数据库

- mysqlcheck -o –all-databases 会让 ibdata1 不断增大，真正的优化只有重建数据表结构：

```text
CREATE TABLE mydb.mytablenew LIKE mydb.mytable;INSERT INTO mydb.mytablenew SELECT * FROM mydb.mytable;ALTER TABLE mydb.mytable RENAME mydb.mytablezap;ALTER TABLE mydb.mytablenew RENAME mydb.mytable;DROP TABLE mydb.mytablezap;
```

## 4. 减少磁盘写入操作

### 4.1 使用足够大的写入缓存 innodb_log_file_size

但是需要注意如果用 1G 的 innodb_log_file_size ，假如服务器当机，需要 10 分钟来恢复。

推荐 innodb_log_file_size 设置为 0.25 * innodb_buffer_pool_size

### 4.2 innodb_flush_log_at_trx_commit

这个选项和写磁盘操作密切相关：

innodb_flush_log_at_trx_commit = 1 则每次修改写入磁盘
innodb_flush_log_at_trx_commit = 0/2 每秒写入磁盘

如果你的应用不涉及很高的安全性 (金融系统)，或者基础架构足够安全，或者 事务都很小，都可以用 0 或者 2 来降低磁盘操作。

## 5.分析查询日志和慢查询日志

## 6.为临时表分配足够的内存

在某些情况下，服务器在处理语句时会创建内部临时表。临时表用于内部操作如GROUP BY和distinct，还有一些ORDER BY查询以及UNION和FROM子句（派生表）中的子查询。这些都是在内存中创建的内存表。

内存中临时表的最大大小由tmp_table_size和max_heap_table_size中较小的值确定。如果临时表的大小超过这个阈值，则将其转换为磁盘上的InnoDB或MyISAM表

当增加max_heap_table_size和tmp_table_sizevariables的大小时，一定要监视服务器的内存使用情况，因为内存中的临时表可能会增加达到服务器内存容量的风险。

一般来说，32M到64M是建议值，从这两个变量开始并根据需要进行调优。

```text
允许的最大值：tmp_table_size

内存表的最大大小：max_heap_table_size

创建的临时表总数：created_tmp_tables

在磁盘上创建的临时表：created_tmp_disk_tables
```

## 7.增加线程缓存大小

连接管理器线程将每个客户端连接与专用于它的线程关联，该线程负责处理该连接的身份验证和所有请求处理。因此，线程和当前连接的客户端之间是一对一的比例。确保线程缓存足够大以容纳所有传入请求是非常重要的。

线程缓存大小由thread_cache_size系统变量决定。默认值为0（无缓存），这将导致为每个新连接设置一个线程，并在连接终止时需要处理该线程。如果希望服务器每秒接收数百个连接请求，那么应该将thread_cache_size设置的足够高，以便大多数新连接可以使用缓存线程。可以在服务器启动或运行时设置max_connections的值。

- thread_cache_size：可以缓存的线程数。
- Threads_cached：缓存中的线程数。
- Threads_created：创建用于处理连接的线程。
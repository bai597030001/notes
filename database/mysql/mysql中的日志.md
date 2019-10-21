# mysql日志分类

MySQL中有六种日志文件，分别是：

​		重做日志（redo log）、回滚日志（undo log）、二进制日志（binlog）、

​		错误日志（errorlog）、慢查询日志（slow query log）、一般查询日志（general log），中继日志（relay log）。



- 日志刷新操作

以下操作会刷新日志文件，刷新日志文件时会关闭旧的日志文件并重新打开日志文件。对于有些日志类型，如二进制日志，刷新日志会滚动日志文件，而不仅仅是关闭并重新打开。

```mysql
mysql > FLUSH LOGS;
shell > mysqladmin flush-logs
shell > mysqladmin refresh
```



## 错误日志

错误日志MySQL服务启动和停止正确和错误的信息，还记录了`mysqld`实例运行过程中发生的错误事件信息。

可以使用" `--log-error=[file_name]` "来指定`mysqld`记录的错误日志文件，如果没有指定file_name，则默认的错误日志文件为datadir目录下的 `hostname.err` ，`hostname`表示当前的主机名。

也可以在`MariaDB/MySQL`配置文件中的`mysqld`配置部分，使用`log-error`指定错误日志的路径。

如果不知道错误日志的位置，可以查看变量`log_error`来查看。

```mysql
mysql> show variables like 'log_error';
+---------------+----------------------------------------+
| Variable_name | Value                                  |
+---------------+----------------------------------------+
| log_error     | /var/lib/mysql/node1.longshuai.com.err |
+---------------+----------------------------------------+
```



## 一般查询日志

查询日志分为一般查询日志和慢查询日志，它们是通过查询是否超出变量 long_query_time 指定时间的值来判定的。在超时时间内完成的查询是一般查询，可以将其记录到一般查询日志中，**但是建议关闭这种日志（默认是关闭的）**，超出时间的查询是慢查询，可以将其记录到慢查询日志中。

使用" --general_log={0|1} "来决定是否启用一般查询日志，使用" --general_log_file=file_name "来指定查询日志的路径。不给定路径时默认的文件名以 `hostname`.log 命名。

和查询日志有关的变量有：

```mysql
`long_query_time = 10 ``# 指定慢查询超时时长，超出此时长的属于慢查询，会记录到慢查询日志中``log_output={TABLE|FILE|NONE}  ``# 定义一般查询日志和慢查询日志的输出格式，不指定时默认为file`
```

TABLE表示记录日志到表中，FILE表示记录日志到文件中，NONE表示不记录日志。只要这里指定为NONE，即使开启了一般查询日志和慢查询日志，也都不会有任何记录。

和一般查询日志相关的变量有：

```mysql
`general_log=off ``# 是否启用一般查询日志，为全局变量，必须在global上修改。``sql_log_off=off ``# 在session级别控制是否启用一般查询日志，默认为off，即启用``general_log_file=``/mydata/data/hostname``.log  ``# 默认是库文件路径下主机名加上.log`
```

在MySQL 5.6以前的版本还有一个"log"变量也是决定是否开启一般查询日志的。在5.6版本开始已经废弃了该选项。

默认没有开启一般查询日志，也不建议开启一般查询日志。



## 慢查询日志

查询超出变量 long_query_time 指定时间值的为慢查询。但是查询获取锁(包括锁等待)的时间不计入查询时间内。

mysql记录慢查询日志是在查询执行完毕且已经完全释放锁之后才记录的，因此慢查询日志记录的顺序和执行的SQL查询语句顺序可能会不一致(例如语句1先执行，查询速度慢，语句2后执行，但查询速度快，则语句2先记录)。

注意，MySQL 5.1之后就支持微秒级的慢查询超时时长，对于DBA来说，一个查询运行0.5秒和运行0.05秒是非常不同的，前者可能索引使用错误或者走了表扫描，后者可能索引使用正确。

另外，指定的慢查询超时时长表示的是超出这个时间的才算是慢查询，等于这个时间的不会记录。

和慢查询有关的变量：

```mysql
`long_query_time=10 ``# 指定慢查询超时时长(默认10秒)，超出此时长的属于慢查询``log_output={TABLE|FILE|NONE} ``# 定义一般查询日志和慢查询日志的输出格式，默认为file``log_slow_queries={``yes``|no}    ``# 是否启用慢查询日志，默认不启用``slow_query_log={1|ON|0|OFF}  ``# 也是是否启用慢查询日志，此变量和log_slow_queries修改一个另一个同时变化``slow_query_log_file=``/mydata/data/hostname-slow``.log  ``#默认路径为库文件目录下主机名加上-slow.log``log_queries_not_using_indexes=OFF ``# 查询没有使用索引的时候是否也记入慢查询日志`
```

现在启用慢查询日志。

```mysql
mysql> set @@global.slow_query_log=on;
```

因为默认超时时长为10秒，所以进行一个10秒的查询。

```mysql
mysql> select sleep(10);
```

查看慢查询日志文件。这里看到虽然sleep了10秒，但是最后查询时间超出了847微秒，因此这里也记录了该查询。

```mysql
[root@xuexi data]# cat xuexi-slow.log 
/usr/local/mysql/bin/mysqld, Version: 5.6.35-log (MySQL Community Server (GPL)). started with:
Tcp port: 3306  Unix socket: /mydata/data/mysql.sock
Time                 Id Command    Argument
# Time: 170329  9:55:58
# User@Host: root[root] @ localhost []  Id:     1
# Query_time: 10.000847  Lock_time: 0.000000 Rows_sent: 1  Rows_examined: 0
use test;
SET timestamp=1490752558;
select sleep(10);
```

随着时间的推移，慢查询日志文件中的记录可能会变得非常多，这对于分析查询来说是非常困难的。好在提供了一个专门归类慢查询日志的工具mysqldumpslow。

```mysql
`[root@xuexi data]``# mysqldumpslow --help``  ``-d           debug ``  ``-``v`           `verbose：显示详细信息``  ``-t NUM       just show the ``top` `n queries：仅显示前n条查询``  ``-a           don``'t abstract all numbers to N and strings to '``S'：归类时不要使用N替换数字，S替换字符串``  ``-g PATTERN   ``grep``: only consider stmts that include this string：通过``grep``来筛选``select``语句。`
```

该工具归类的时候，默认会将**同文本但变量值不同的查询语句视为同一类，并使用N代替其中的数值变量，使用S代替其中的字符串变量**。可以使用-a来禁用这种替换。如：

```mysql
[root@xuexi data]# mysqldumpslow xuexi-slow.log 
Reading mysql slow query log from xuexi-slow.log
Count: 1  Time=10.00s (10s)  Lock=0.00s (0s)  Rows=1.0 (1), root[root]@localhost
  select sleep(N)

[root@xuexi data]#  mysqldumpslow -a xuexi-slow.log   
Reading mysql slow query log from xuexi-slow.log
Count: 1  Time=10.00s (10s)  Lock=0.00s (0s)  Rows=1.0 (1), root[root]@localhost
  select sleep(10)
```

显然，这里归类后的结果只是精确到0.01秒的，如果想要显示及其精确的秒数，则使用-d选项启用调试功能。

```mysql
[root@xuexi data]#  mysqldumpslow -d xuexi-slow.log   
Reading mysql slow query log from xuexi-slow.log
[[/usr/local/mysql/bin/mysqld, Version: 5.6.35-log (MySQL Community Server (GPL)). started with:
Tcp port: 3306  Unix socket: /mydata/data/mysql.sock
Time                 Id Command    Argument
# Time: 170329  9:55:58
# User@Host: root[root] @ localhost []  Id:     1
# Query_time: 10.000847  Lock_time: 0.000000 Rows_sent: 1  Rows_examined: 0
use test;
SET timestamp=1490752558;
select sleep(10);
]]
<<>>
<<# Time: 170329  9:55:58
# User@Host: root[root] @ localhost []  Id:     1
# Query_time: 10.000847  Lock_time: 0.000000 Rows_sent: 1  Rows_examined: 0
use test;
SET timestamp=1490752558;
select sleep(10);
>> at /usr/local/mysql/bin/mysqldumpslow line 97, <> chunk 1.
[[# Time: 170329  9:55:58
# User@Host: root[root] @ localhost []  Id:     1
# Query_time: 10.000847  Lock_time: 0.000000 Rows_sent: 1  Rows_examined: 0
use test;
SET timestamp=1490752558;
select sleep(10);
]]
{{  select sleep(N)}}

Count: 1  Time=10.00s (10s)  Lock=0.00s (0s)  Rows=1.0 (1), root[root]@localhost
  select sleep(N)
```

慢查询在SQL语句调优的时候非常有用，应该将它启用起来，且应该让慢查询阈值尽量小，例如1秒甚至低于1秒。就像一天执行上千次的1秒语句，和一天执行几次的20秒语句，显然更值得去优化这个1秒的语句。



## 二进制日志

[https://www.cnblogs.com/f-ck-need-u/p/9001061.html#blog5](https://www.cnblogs.com/f-ck-need-u/p/9001061.html#blog5)



## 重做日志

[https://www.cnblogs.com/f-ck-need-u/p/9010872.html](https://www.cnblogs.com/f-ck-need-u/p/9010872.html)





## 回滚日志

[https://www.cnblogs.com/f-ck-need-u/p/9010872.html](https://www.cnblogs.com/f-ck-need-u/p/9010872.html)
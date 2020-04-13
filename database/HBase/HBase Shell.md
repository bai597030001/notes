# HBase Shell 基本操作 查询过滤 命令

[原文链接](<http://www.52xjava.cn/2018/01/23/hbase-shell-command/>)

## 1. 一般命令

> 1. status 查看状态
> 2. version 查看版本

## 2.DDL（数据定义语言Data Definition Language）命令

###     1. 创建表

​        create ‘表名称’,’列名称1’，’列名称2’，’列名称3’ 

```shell
$ create 'student','info','grade'
```



###     2.列出所有的表

list 
![blob.png](http://www.52xjava.cn/wp-content/uploads/image/20180122/1516629147169951.png)

```shell
list ‘abc.*’ #显示abc开头的表
```



###     3.获得表的描述

describe ‘table_name’ 

![blob.png](http://www.52xjava.cn/wp-content/uploads/image/20180122/1516631978333788.png)

![blob.png](http://www.52xjava.cn/wp-content/uploads/image/20180122/1516631952977234.png)

```
`Table student is ENABLED``student``COLUMN FAMILIES DESCRIPTION``{``NAME => ``'grade'``, ``BLOOMFILTER => ``'ROW'``, ``# bloom filter的作用是对一个region下查找记录所在的hfile有用。一个region下hfile数量越多，bloom filter的作用越明显。``VERSIONS => ``'1'``, ``# 版本 需要历史版本数据的应用通常可以设置为 3``IN_MEMORY => ``'false'``, ``# 内存缓存``KEEP_DELETED_CELLS =>``'FALSE'``,  ``DATA_BLOCK_ENCODING => ``'NONE'``, ``TTL => ``'FOREVER'``, ``COMPRESSION => ``'NONE'``, ``# 文件压缩处理``MIN_VERSIONS => ``'0'``,``BLOCKCACHE => ``'true'``, ``BLOCKSIZE => ``'65536'``, ``REPLICATION_SCOPE => ``'0'``}`
```

###     4.删除一个列族 

alter,disable, enable

```
`disable ``'member'``# 删除列族时必须先将表给disable``alter ``'member'``,{NAME=>``'member_id'``,METHOD=>``'delete'``}``# 删除后继续``enable` `'member'``enable` `'member'`
```

###     5.删除表

disable 'table_name'drop 'table_name'

![blob.png](http://www.52xjava.cn/wp-content/uploads/image/20180122/1516633339818253.png)

###     6.查询表是否存在

```
exists 'table_name'1
```

###     7.判断表是否enabled

```
is_enabled 'table_name'1
```

###     8.更改表名

```
`//``快照 ``//``需要开启快照功能，在hbase-site.xml文件中添加如下配置项：` `<property>``<name>hbase.snapshot.enabled<``/name``>``<value>``true``<``/value``>``<``/property``>` `//``命令``hbaseshell> disable ``'tableName'``hbaseshell> snapshot ``'tableName'``, ``'tableSnapshot'``hbaseshell> clone_snapshot ``'tableSnapshot'``, ``'newTableName'``hbaseshell> delete_snapshot ``'tableSnapshot'``hbaseshell> drop ``'tableName'`
```

## 3.DML（data manipulation language）操作

###     1.插入

![blob.png](http://www.52xjava.cn/wp-content/uploads/image/20180122/1516633784366398.png)



```
在ns1:t1或者t1表里的r1行，c1列中插入值，ts1是时间
  # t是table 't1'表的引用
  t.put 'r1','c1','value',ts1,{ATTRIBUTES=>{'mykey'=>'myvalue'}}
```

![blob.png](http://www.52xjava.cn/wp-content/uploads/image/20180122/1516634129587016.png)

###     2.获取一条数据

get <table>,<rowkey>,[<family:column>,….]

![blob.png](http://www.52xjava.cn/wp-content/uploads/image/20180122/1516634324919975.png)

```
`# 获取一个id的所有数据``get ``'table_name'``,``'row_index'` `# 获取一个id,一个列族的所有数据``get ``'table_name'``,``'row_index'``,``'info'` `# 获取一个id,一个列族中一个列的所有数据``get ``'table_name'``,``'row_index'``,``'info:age'`
```

### ![blob.png](http://www.52xjava.cn/wp-content/uploads/image/20180122/1516634350841817.png)

###     3.更新一条记录

> 通过 put 覆盖列值
>
> ```
> put 'table_name','row_index','info:column','value'
> ```

###     4.通过timestrap来获取两个版本的数据

> ```
> `# 得到某个时间版本的记录``get``'table_name'``,``'row'``,{COLUMN=>``'info:column'``,TIMESTRAP=>1321586238965}` `# 得到另一个个时间版本的记录``get``'table_name'``,``'row'``,{COLUMN=>``'info:column'``,TIMESTRAP=>1321586271843}`
> ```

###     5.全表扫描

> scanner 相当于：select * from table_name
>
> ```
> scan <table>, {COLUMNS => [ <family:column>,.... ], LIMIT => num}
> # 另外，还可以添加STARTROW、TIMERANGE和FITLER等高级功能
> ```

![2018-01-22_234806.png](http://www.52xjava.cn/wp-content/uploads/image/20180122/1516636729160202.png)

> ```
> `scan ``'hbase:meta'``scan ``'hbase:meta'``,{COLUMNS => ``'info:regioninfo'``}``scan ``'ns1:t1'``,{COLUMNS=>[``'c1'``,``'c2'``],LIMIT=>10,STARTROW=>``'xyz'``}``scan ``'t1'``,{COLUMNS=>``'c1'``,TIMERANGE=>[1303668804,1303668904]}``scan ``'t1'``,{REVERSED=>``true``}``scan ``'t1'``,{``    ``ROWPREFIXFILTER=>``'row2'``,``    ``FILTER=>"(QualifierFilter(>=,``'binary:xyz'``)) ``    ``AND (TimestampsFilter(123,456))"}``scan ``'t1'``,{FILTER => org.apache.hadoop.hbase.filter.ColumnPaginationFilter.new(1,0)}``scan ``'t1'``,{CONSISTENCY=>``'TIMELINE'``}` `# 设置操作属性：``scan ``'t1'``,{COLUMNS => [``'c1'``,``'c2'``],ATTRIBUTES=>{``'mykey'``=>``'myvalue'``}}``scan ``'t1'``,{COLUMNS=>[``'c1'``,``'c2'``],AUTHORIZATIONS=>[``'PRIVATE'``,``'SECRET'``]}``有个额外的选项：CACHE_BLOCKS,默认为``true``还有个选项：RAW，返回所有cells（包括删除的markers和uncollected deleted cells，不能用来选择特定的columns，默认为default）``如：scan ``'t1'``,{RAW=>``true``,VERSIONS=>10}`
> ```



![blob.png](http://www.52xjava.cn/wp-content/uploads/image/20180122/1516636424146679.png)

###     6.删除记录

![blob.png](http://www.52xjava.cn/wp-content/uploads/image/20180123/1516637893349493.png)

> ```
> `# 删除指定rowkey的 'info:age' 字段``delete ``'table_name'``,``'row_index'``,``'info:age'` `# 删除整行``deleteall ``'table_name'``,``'row_index'`
> ```



![blob.png](http://www.52xjava.cn/wp-content/uploads/image/20180123/1516688107143330.png)

###     7.查询表中有多少行

> ```
> ` ``hbase> count ``'ns1:t1'`  `# namespace命名空间分组`` ``hbase> count ``'t1'`` ``hbase> count ``'t1'``, INTERVAL => 100000 ``# 每隔多少行显示一次count，默认是1000`` ``hbase> count ``'t1'``, CACHE => 1000 ``# 每次去取的缓存区大小，默认是10，调整该参数可提高查询速度`` ` ` ``hbase> count ``'t1'``, INTERVAL => 10, CACHE => 1000`
> ```

###     8.清空表

truncate 'students'
先删除表，再重建

> ```
> `truncate 日志：``hbase shell>truncate ``'students'``Truncating ``'students'` `table (it may take a ``while``):``- Disabling table...``- Dropping table...``- Creating table ...`
> ```

## 

## 5.高级 scan 查询

###   1.限制条件

限制查找列：

> ```
> scan ‘table_name’,{COLUMNS=> 'column-familyinfo’}  # 列族
> ```

> ```
> scan ‘table_name’,{COLUMNS=> 'info:regioninfo’}    # 字段
> ```

> ```
> scan ‘table_name’,{COLUMNS=>[‘c1’,’c2’]}    # 多个列、字段
> ```

![blob.png](http://www.52xjava.cn/wp-content/uploads/image/20180123/1516674444957685.png)

限制查找条数：

> ```
> scan 'table_name', {COLUMNS => [ 'c'], LIMIT => n}
> ```

![blob.png](http://www.52xjava.cn/wp-content/uploads/image/20180123/1516674905462693.png)

限制时间范围：

> ```
> scan 'table_name', {TIMERANGE=>[ minStamp, maxStamp]}
> ```

![blob.png](http://www.52xjava.cn/wp-content/uploads/image/20180123/1516674923338138.png)

###   2.FILTER 过滤器

####     1.**rowkey过滤**

#### PrefixFilter:

行键前缀过滤器

> ```
> scan 'table_name',{FILTER => "PrefixFilter('rowkey_prefix')"}
> ```

![blob.png](http://www.52xjava.cn/wp-content/uploads/image/20180123/1516675308103336.png)

####     2.**列族过滤**

#### QuanlifierFilter:

列名限定符过滤器

> ```
> scan 'table_name',{FILTER => "QualifierFilter(CompareOp,'BinaryComparator')"} 
> # 参数是关系比较运算符 和 二进制比较器
> ```

![blob.png](http://www.52xjava.cn/wp-content/uploads/image/20180123/1516676647182749.png)

#### ColumnPrexfixFilter:

列名前缀过滤器

> ```
> scan 'table_name',{FILTER => "ColumnPrefixFilter('colunm')"}
> ```

![blob.png](http://www.52xjava.cn/wp-content/uploads/image/20180123/1516687613520288.png)

#### MultipleColumnPrexfixFilter:

多个列名前缀过滤器

> ```
> scan 'table_name',{FILTER => "MultipleColumnPrefixFilter('c1','c2')" }
> ```

![blob.png](http://www.52xjava.cn/wp-content/uploads/image/20180123/1516687597946911.png)

####     3.**列值过滤**



#### SingleColumnValueFilter:

列值过滤器

> ```
> `# 需要导入类``import` `org.apache.hadoop.hbase.filter.SingleColumnValueFilter``import` `org.apache.hadoop.hbase.filter.CompareFilter``import` `org.apache.hadoop.hbase.filter.SubstringComparator` `scan ``'table_name'``,{FILTER => SingleColumnValueFilter.new(``  ``Bytes.toBytes(``'info'``),  ``# 列族``  ``Bytes.toBytes(``'column'``),    ``# 字段``  ``CompareFilter::CompareOp.valueOf(``'EQUAL'``), ``# 比较器``  ``Bytes.toBytes(``'my value'``)) ``# 值``}`
> ```

![blob.png](http://www.52xjava.cn/wp-content/uploads/image/20180123/1516685687974910.png)

参数

有两个参数类在各类Filter中常常出现。统一介绍下：

> ```
> （1）比较运算符 CompareFilter.CompareOp
> EQUAL                      相等
> GREATER                    大于
> GREATER_OR_EQUAL           大于等于
> LESS                       小于
> LESS_OR_EQUAL              小于等于
> NOT_EQUAL                  不等于
> 
> （2）比较器  
> BinaryComparator           匹配完整字节数组 
> BinaryPrefixComparator     匹配字节数组前缀 
> RegexStringComparator      正则表达式匹配
> SubstringComparator        子串匹配
> ```

####     4.**组合过滤器**

​    多个过滤器可以通过 AND OR 连接进行组合过滤

> ```
> `# 列如：``hbase(main):008:0> scan ``'emp'``, {FILTER => "(MultipleColumnPrefixFilter(``'sal'``,``'COMM'``,``'deptno'``))`` ``AND (SingleColumnValueFilter(``'empinfo'``,``'deptno'``,=,``'substring:20'``))"}`
> ```



![blob.png](http://www.52xjava.cn/wp-content/uploads/image/20180123/1516691372176442.png)



# 使用

```shell
scan "Measure_Report_C3", {COLUMNS=>'f:engineId', LIMIT=>10}

scan "Measure_Report_C3", {FILTER => "PrefixFilter('20180810')"}

scan "Measure_Report_C3", {COLUMNS=>'f:engineId', FILTER => "PrefixFilter('20190102')"}

scan "Measure_Report_C3", {FILTER => "ColumnPrefixFilter('engineId')",LIMIT=>5}

scan "Measure_Report_C3", {FILTER=>SingleColumnValueFilter.new(Bytes.toBytes('f'),Bytes.toBytes('engineId'),CompareFilter::CompareOp.valueOf('EQUAL'),Bytes.toBytes('27101'))}
```


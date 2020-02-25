# 1.mysql中的键

- mysql建表时的**4种KEY**

---
## 1.1 主键(primary key)

- 能够**唯一标识**表中某一行的属性或属性组，**不能有重复**的，**不允许为空**，用来保证数据完整性。

- 一个表只能有一个主键（**PRIMARY KEY**），但可以有多个唯一键（**UNIQUE KEY**）。

- 主键常常与外键构成参照完整性约束，防止出现数据不一致。

- 主键也是一个特殊的索引(唯一性索引)。

```mysql
CREATETABLE tablename ( [...], PRIMARY KEY (列的列表) )

ALTER TABLE tablename ADD PRIMARY KEY(列的列表)
```

## 1.2 唯一键(unique key)

- 一张表往往有很多字段需要唯一性，数据**不能重复**；但是一张表只能有一个字段为主键，那么唯一键(unique key)，就可以解决表中有**多个字段**需要唯一性约束的问题。

- 唯一键**默认值允许自动为空**，而且可以多个不同字段为空（空字段不参与唯一性比较）。

- 增加唯一键
	
```mysql
1.创建表时

    --第一种，字段增加
    create table my_unique(
    name varchar(4) unique key,
    number varchar(20) unique key
    )charset utf8;

--第二种，字段末尾增加
    create table my_unique(
    name varchar(4) ,
    number varchar(20) ,
    unique key name(name),--指定唯一约束名字
    unique key(number)--使用默认名字

    )charset utf8;

2.创建表后

-- 创建表，不带唯一约束
    create table my_unique(
    name varchar(4) ,
    number varchar(20) 
    )charset utf8;

--第一种，modify column
	alter table my_unique modify column name varchar(5) UNIQUE KEY;

--第二种 add constraint unique key

	alter TABLE my_unique add CONSTRAINT num_uk UNIQUE KEY(number);

--指定唯一约束名字为num_uk
```

## 1.3 外键（foreign key）

- 是另一表的主键, 外键**可以有重复**的, **可以是空值**，用来**和其他表建立联系**用的。所以说，如果谈到了外键一定是至少涉及到两张表

![](img/primaryForeignKey.png)

```mysql
[CONSTRAINT symbol] FOREIGN KEY [id] (从表的字段1) REFERENCES tbl_name (主表的字段2) 
[ON DELETE 
	{RESTRICT | CASCADE | SET NULL | NO ACTION}] 
[ON UPDATE 
	{RESTRICT | CASCADE | SET NULL | NO ACTION} 
```
中括号里的内容是可选项

- 解释如下

```
CONSTRAINT symbol：可以给这个外键约束起一个名字，有了名字，以后找到它就很方便了。如果不加此参数的话，系统会自动分配一个名字。

FOREIGN KEY：将从表中的字段1作为外键的字段。

REFERENCES：映射到主表的字段2。

ON DELETE后面的四个参数：代表的是当删除主表的记录时，所做的约定。

RESTRICT(限制)：如果你想删除的那个主表，它的下面有对应从表的记录，此主表将无法删除。

CASCADE（级联）：如果主表的记录删掉，则从表中相关联的记录都将被删掉。

SET NULL：将外键设置为空。

NO ACTION：什么都不做。
```
注：一般是**RESTRICT**和**CASCADE**用的最多。

### 建立外键一（建表）

```sql
	CREATE TABLE `dage` (
        `id` int(11) NOT NULL auto_increment,
        `name` varchar(32) default '',
        PRIMARY KEY  (`id`)
    )ENGINE=InnoDB DEFAULT CHARSET=latin1；

	CREATE TABLE `xiaodi` (
        `id` int(11) NOT NULL auto_increment,
        `dage_id` int(11) default NULL,
        `name` varchar(32) default '',
        PRIMARY KEY  (`id`),
        KEY `dage_id` (`dage_id`),
        CONSTRAINT `xiaodi_ibfk_1` FOREIGN KEY (`dage_id`) REFERENCES `dage` (`id`)
    ) ENGINE=InnoDB DEFAULT CHARSET=latin1；
```

### 建立外键一（修改表）

```sql
	mysql> show create table xiaodi;
	CONSTRAINT `xiaodi_ibfk_1` FOREIGN KEY (`dage_id`) REFERENCES `dage` (`id`)

	mysql> alter table xiaodi drop foreign key xiaodi_ibfk_1; 
	Query OK, 1 row affected (0.04 sec)
	Records: 1  Duplicates: 0  Warnings: 

	mysql> alter table xiaodi add foreign key(dage_id) references dage(id) on delete cascade on update cascade;
	Query OK, 1 row affected (0.04 sec)
	Records: 1  Duplicates: 0  Warnings: 0
```

## 1.3 普通键（KEY）

- 建立索引

```sql

	CREATE TABLE `C3TimeOut_AlarmInfo` (
		`AffairID` bigint(20) NOT NULL AUTO_INCREMENT,
		`LineID` int(11) DEFAULT NULL,
		`EngineID` int(11) NOT NULL,
		`TrainNum` varchar(255) COLLATE utf8_bin DEFAULT NULL,
		`EngineBureauID` int(11) DEFAULT NULL,
		`MsIsdn` bigint(20) NOT NULL,
		`CTCS_ID` int(11) NOT NULL,
		`CalledNum` bigint(20) DEFAULT NULL,
		`StartTime` varchar(255) COLLATE utf8_bin NOT NULL,
		`StopTime` varchar(255) COLLATE utf8_bin DEFAULT NULL,
		`DiscBtsName` varchar(255) COLLATE utf8_bin DEFAULT NULL,
		`FindType` smallint(6) NOT NULL,
		`Fcod` varchar(255) COLLATE utf8_bin DEFAULT NULL,
		`CauseCategory` varchar(255) COLLATE utf8_bin DEFAULT NULL,
		`PriData` varchar(1024) COLLATE utf8_bin DEFAULT NULL,
		`AData` varchar(512) COLLATE utf8_bin DEFAULT NULL,
		`AbisData` varchar(2048) COLLATE utf8_bin DEFAULT NULL,
		`AnalyseSynth` varchar(512) COLLATE utf8_bin DEFAULT NULL,
		`TimeoutAck` smallint(6) DEFAULT NULL,
		`IsGuessValue` smallint(6) DEFAULT NULL,
		PRIMARY KEY (`AffairID`),
		KEY `MsIsdn` (`MsIsdn`),
		KEY `Fcod` (`Fcod`),
		KEY `StartTime` (`StartTime`,`StopTime`),
		KEY `EngineBureauID` (`EngineBureauID`),
		KEY `LineID` (`LineID`),
		CONSTRAINT `EngineBureauID` FOREIGN KEY (`EngineBureauID`) REFERENCES `GBIAP_common_config`.`BureauInfoTable` (`BureauNum`) ON UPDATE CASCADE,
		CONSTRAINT `Fcod` FOREIGN KEY (`Fcod`) REFERENCES `GBIAP_common_config`.`FcodInfo` (`Fcod`) ON UPDATE CASCADE,
		CONSTRAINT `LineID` FOREIGN KEY (`LineID`) REFERENCES `GBIAP_common_config`.`LineInfoTable` (`LineID`) ON UPDATE CASCADE
	) ENGINE=InnoDB AUTO_INCREMENT=51 DEFAULT CHARSET=utf8 COLLATE=utf8_bin ROW_FORMAT=DYNAMIC;
```

# 2.mysql 的索引

## 索引类型

PRIMARY 主键。 就是 唯一 且 不能为空。

INDEX 索引，普通的

UNIQUE 唯一索引。 不允许有重复。

	（PRIMARY, INDEX, UNIQUE 这3种是一类）

FULLTEXT 是全文索引，用于在一篇文章中，检索文本信息的。

## 2.1 索引(index) 

- 用来快速地寻找那些具有特定值的记录。主要是为了检索的方便，是为了加快访问速度， 按一定的规则创建的，一般起到排序作用。

```mysql
CREATE INDEX <索引的名字> ON tablename (列的列表); 

ALTER TABLE tablename ADD INDEX [索引的名字] (列的列表); 

CREATE TABLE tablename ( [...], INDEX [索引的名字] (列的列表) )
```
## 2.2 唯一性索引

- 这种索引和前面的“普通索引”基本相同，但有一个区别：索引列的所有值都只能出现一次，即必须唯一。

```

	CREATE UNIQUE INDEX <索引的名字> ON tablename (列的列表)

	ALTER TABLE tablename ADD UNIQUE [索引的名字] (列的列表)

	CREATE TABLE tablename ( [...], UNIQUE [索引的名字] (列的列表) )
```

## 2.3 全文索引

- MySQL从3.23.23版开始支持全文索引和全文检索。

- 在MySQL中，全文索引的索引类型为FULLTEXT。全文索引可以在VARCHAR或者TEXT类型的列上创建。  
	
	它可以通过CREATE TABLE命令创建，也可以通过ALTER TABLE或CREATE INDEX命令创建。

## 2.4 联合索引

### 2.4.1 建立联合索引

- alert table test add INDEX `sindex` (`aaa`,`bbb`,`ccc`) 

- 建表语句建立

```sql
CREATE TABLE index5(
    id INT,
    name VARCHAR(20),
    sex CHAR(4),
    INDEX index5_ns(name,sex)
);
```

### 2.4.2 最左前缀匹配原则

- 最左优先，在检索数据时从联合索引的最左边开始匹配

- 示例讲解：

	对列col1、列col2和列col3建一个联合索引
	
	> KEY test_col1_col2_col3 on test(col1,col2,col3);
	
	联合索引 test_col1_col2_col3 实际建立了(col1)、(col1,col2)、(col,col2,col3)三个索引。
	
	> SELECT * FROM test WHERE col1=“1” AND clo2=“2” AND clo4=“4”
	
	上面这个查询语句执行时会依照最左前缀匹配原则，检索时会使用索引(col1,col2)进行数据匹配。

注意：索引的字段可以是任意顺序的，如：

	SELECT * FROM test WHERE col1=“1” AND clo2=“2”
	
	SELECT * FROM test WHERE col2=“2” AND clo1=“1”

# 3.主键与索引对比

- 主键一定是唯一性索引，唯一性索引并不一定就是主键。

- 一个表中可以有多个唯一性索引，但只能有一个主键。

- 主键列不允许空值，而唯一性索引列允许空值。

- 主键可以被其他字段作外键引用，而索引不能作为外键引用。

## 主键与外键的使用条件

- 1.两张表必须都是InnoDB表，并且它们没有临时表。  
	注：InnoDB是数据库的引擎。MySQL常见引擎有两种：InnoDB和MyISAM，后者不支持外键。

- 2.建立外键关系的对应列必须具有相似的InnoDB内部数据类型。

- 3.建立外键关系的对应列必须建立了索引。

- 4.假如显式的给出了CONSTRAINT symbol，那symbol在数据库中必须是唯一的。假如没有显式的给出，InnoDB会自动的创建。

## 主键、外键和索引的区别

| 主键        | 外键           |	索引  |
| ------------- |:-------------:| -----:|
| 唯一标识一条记录，不能有重复的，不允许为空     | 表的外键是另一表的主键, 外键可以有重复的, 可以是空值 | 该字段没有重复值，但可以有一个空值 |
| 用来保证数据完整性      | 用来和其他表建立联系用的      |  是提高查询排序的速度 |
| 主键只能有一个 | 一个表可以有多个外键      |    一个表可以有多个惟一索引 |


# 4.存储引擎中索引的类型

## 非聚簇索引

- 索引指向数据所在行（磁盘）的位置

```
以myisam为例，一个数据表table中，它是有table.frm、table.myd以及table.myi组成。

table.myd记录了数据，table.myi记录了索引的数据。在用到索引时，先到table.myi（索引树）中进行查找，

取到数据所在table.myd的行位置，拿到数据。所以myisam引擎的索引文件和数据文件是独立分开的，则称之为非聚簇索引。
```


## 聚簇索引

- 索引指向主键对数据的引用（既存储主键索引值，又存储行数据）

```
以innodb为例，在一个数据table中，它的数据文件和索引文件是同一个文件。

即在查询过程中，找到了索引，便找到了数据文件。在innodb中，既存储主键索引值，又存储行数据，称之为聚簇索引。
```

- 图示

![](img/index1.png)

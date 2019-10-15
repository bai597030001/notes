# 事务基本操作

```mysql
# 关闭自动提交
set autocommit = 0;

# 开启事务
start transaction
begin

# 保存点(savepoint 允许在事务中创建一个保存点，一个事务中可以有多个 savepoint)
savepoint identifier

# 删除一个事务的保存点
release savepoint identifier

# 回滚
rollback
rollback work

# 把事务回滚到保存点
rollback to identifier

# 提交事务
commit
```



# mysql事务隔离级别



![](img/mysql-tx1.webp)



## 隔离级别

### 读未提交



### 读已提交



### 可重复读

mysql默认隔离级别，InnoDB通过多版本并发控制（MVCC，Multiversion Concurrency Control）机制解决了该隔离级别下的幻读问题。



### 串行化



## 设置事务隔离级别

### 修改配置文件

> my.ini文件中使用transaction-isolation选项来设置服务器的缺省事务隔离级别
> READ-UNCOMMITTED  
> READ-COMMITTED  
> REPEATABLE-READ  
> SERIALIZABLE  

> 例如: transaction-isolation = READ-COMMITTED



### 命令动态设置隔离级别 

> SET [GLOBAL | SESSION] TRANSACTION ISOLATION LEVEL <isolation-level>
> 其中的<isolation-level>可以是：  
> READ UNCOMMITTED  
> READ COMMITTED  
> REPEATABLE READ  
> SERIALIZABLE  
> 例如： SET TRANSACTION ISOLATION LEVEL REPEATABLE READ;



## 查看事务隔离级别

查看系统隔离级别：
	`select @@global.tx_isolation;`
查看当前会话隔离级别
	`select @@tx_isolation;`



# 事务并发存在的问题



## 脏读

- 一个事务读取了另外一个事务未提交的数据
- 示例

| 时间 | 转账事务A                       | 取款事务B                |
| ---- | ------------------------------- | ------------------------ |
| t1   |                                 | 开始事务                 |
| t2   | 开始事务                        |                          |
| t3   |                                 | 查询账户余额为1000元     |
| t4   |                                 | 取出500元把余额改为500元 |
| t5   | 查询账户余额为500元（**脏读**） |                          |
| t6   |                                 | 撤销事务余额恢复为1000元 |
| t7   | 汇入100元把余额改为600元        |                          |
| t8   | 提交事务                        |                          |



## 不可重复读:

- 在一个事务内读取表中的数据，多次读取结果不同。（一个事务读到了其他事务已经提交的update/delete的数据。（这个不一定是错误，只是某些场合不对））
- 在一个事务内，多次读同一数据。在这个事务还没有结束时，另外一个事务也访问(update/delete)该同一数据。那么，在第一个事务中的两次读数据之间，由于第二个事务的修改，那么第一个事务两次读到的的数据可能是不一样的。这样就发生了在一个事务内两次读到的数据是不一样的，因此称为是不可重复读。
- 示例

| 时间 | 查询事务A                               | 取款事务B                |
| ---- | --------------------------------------- | ------------------------ |
| t1   |                                         | 开始事务                 |
| t2   | 开始事务                                |                          |
| t3   |                                         | 查询账户余额为1000元     |
| t4   | 查询账户余额为1000元                    |                          |
| t5   |                                         | 取出100元把余额改为900元 |
| t6   |                                         | 提交事务                 |
| t7   | 查询账户余额为900元（和T4读取的不一致） |                          |



## 虚读(幻读)

- 在一个事务内读取到了别的事务插入的(insert的)数据，导致前后读取不一致。（一般是行影响，多了一行）
- 当前事务还没有结束，其他事务操作了当前事务操作的数据(insert)，导致当前事务多次读取数据不一致。
- 示例

| 时间 | 统计事务A                               | 转账事务B                     |
| ---- | --------------------------------------- | ----------------------------- |
| t1   |                                         | 开始事务                      |
| t2   | 开始事务                                |                               |
| t3   | 统计总存款数为10000元                   |                               |
| t4   |                                         | 新增一个存款账户，存款为100元 |
| t5   |                                         | 提交事务                      |
| t6   | 再次统计总存款数为10100元（**幻象读**） |                               |



## 丢失更新

- 两个事务对同一数据进行更新，后者会覆盖先者的更新。
- 示例

| 时间 | 取款事务A                        | 转账事务B                 |
| ---- | -------------------------------- | ------------------------- |
| t1   | 开始事务                         |                           |
| t2   |                                  | 开始事务                  |
| t3   | 查询账户余额为1000元             |                           |
| t4   |                                  | 查询账户余额为1000元      |
| t5   |                                  | 汇入100元把余额改为1100元 |
| t6   |                                  | 提交事务                  |
| t7   | 取出100元将余额改为900元         |                           |
| t8   | 撤销事务                         |                           |
| t9   | 余额恢复为1000元（**丢失更新**） |                           |



# mysql的ACID

一般来说，事务是必须满足4个条件（ACID）：：原子性（Atomicity）、一致性（Consistency）、隔离性（Isolation，又称独立性）、持久性（Durability）。



**原子性**：一个事务（transaction）中的所有操作，要么全部完成，要么全部不完成，不会结束在中间某个环节。事务在执行过程中发生错误，会被回滚（Rollback）到事务开始前的状态，就像这个事务从来没有执行过一样。  

**一致性**：一致性表示事务完成后，符合逻辑运算。在事务开始之前和事务结束以后，数据库的完整性没有被破坏。这表示写入的资料必须完全符合所有的预设规则，这包含资料的精确度、串联性以及后续数据库可以自发性地完成预定的工作。

**隔离性**：数据库允许多个并发事务同时对其数据进行读写和修改的能力，隔离性可以防止多个事务并发执行时由于交叉执行而导致数据的不一致。事务隔离分为不同级别，包括读未提交（Read uncommitted）、读提交（read committed）、可重复读（repeatable read）和串行化（Serializable）。

**持久性**：事务处理结束后，对数据的修改就是永久的，即便系统故障也不会丢失。



**READ UNCOMMITTED**

​	事物A和事物B，事物A未提交的数据，事物B可以读取到，这里读取到的数据叫做“脏数据”。

**READ COMMITTED**

​	事物A和事物B，事物A提交的数据，事物B才能读取到  

**REPEATABLE READ** 

​	事务A和事务B，事务A提交之后的数据，事务B读取不到，事务B是可重复读取数据的。  

**SERIALIZABLE**

​	事务A和事务B，事务A在操作数据库时，事务B只能排队等待 。



## mysql如何保证原子性

Undo Log是实现事务原子性的保障，它的原理很简单，为了满足事务的原子性，在操作任何数据之前，首先将数据备份到一个地方（这个存储备份数据的地方称为Undo Log）。然后再进行数据的修改，如果出现了错误或者用户执行了rollback语句，系统可以利用Undo Log中的备份将数据恢复到事务开始之前的状态
在MySQL数据库的InnoDB存储引擎中，还用Undo Log来实现多版本并发控制（简称：MVCC）



## mysql如何保证一致性

事务一旦完成，该事务对数据库所做的所有修改都会持久的保存到数据库中。为了保证持久性，数据库系统会将修改后的数据完全记录到持久的存储上（即Redo Log）
和Undo Log相反，Redo Log记录的是新数据的备份。在事务提交前，只要将Redo Log持久化即可，不需要将数据持久化。当系统崩溃时，虽然数据没有持久化，但是Redo Log已经持久化。系统可以根据Redo Log的内容，将所有数据恢复到最新的状态
所以，Undo Log保证事务的原子性，Redo Log保证事务的持久性
Undo + Redo的设计主要考虑的是提升IO性能（IO：输入Input和输出Output的首字母缩写）
事务的持久性并不代表事务中做的数据修改以后不能改变，是和临时对比来说的



## mysql如何保证隔离性



## mysql如何保证持久性



# InnoDB之MVCC

- 多版本并发控制(`Multi-Version Concurrency Control`, `MVCC`)是`MySQL`中基于**乐观锁**理论实现隔离级别的方式，用于实现**读已提交**和**可重复读取**隔离级别的实现。
- MVCC只在 `READ COMMITTED` 和 `REPEATABLE READ` 两个隔离级别下工作。其他两个隔离级别不能和MVCC兼容, 因为 `READ UNCOMMITTED` 总是读取最新的数据行, 而不是符合当前事务版本的数据行。而 `SERIALIZABLE` 则会对所有读取的行都加锁。
- MVCC是被Mysql中 `事务型存储引擎InnoDB` 所支持的;



## 相关概念

系统版本号：一个递增的数字，每开始一个新的事务，系统版本号就会自动递增。

事务版本号：事务开始时的系统版本号。



InnoDB下每行数据的**隐藏列**：

​	创建版本号(6-byte `DB_TRX_ID`)：创建一行数据时，将当前系统版本号作为创建版本号赋值

​	删除版本号(7-byte `DB_ROLL_PT`))：删除一行数据时，将当前系统版本号作为删除版本号赋值

​	行ID(6-byte `DB_ROW_ID` )：这个行ID随着插入新行而单调增加。如果InnoDB自动生成一个聚集索引，那么该索引将包含行ID值。否则，DB_ROW_ID列不会出现在任何索引中。



```mysql
SELECT * FROM INFORMATION_SCHEMA.INNODB_TRX;
```



- SELECT

select时读取数据的规则为：创建版本号 `<=` 当前事务版本号 `&&` 删除版本号为空或 `>` 当前事务版本号。只有符合这两个条件的记录，才能返回作为查询结果

> 创建版本号 `<=` 当前事务版本号：
>
> ​		保证取出的数据不会有后启动的事物中创建的数据。
>
> 删除版本号为空或 `>` 当前事务版本号：
>
> ​		保证了至少在该事物开启之前数据没有被删除，是应该被查出来的数据。



- INSERT

> `insert`时将当前的系统版本号赋值给创建版本号字段。



- UPDATE

> 插入一条新纪录，保存当前事务版本号为行创建版本号，同时保存当前事务版本号到原来删除的行，实际上这里的更新是通过`delete`和`insert`实现的。



- DELETE

> 删除时将当前的系统版本号赋值给删除版本号字段，标识该行数据在那一个事物中会被删除，即使实际上在未commit前该数据没有被删除。根据`select`的规则后开启的事务也不会查询到该数据。



- 快照读和当前读

  - 快照读

    当执行select操作时，innodb默认会执行快照读，会记录下这次select后的结果，之后select 的时候就会返回这次快照的数据，即使其他事务提交了不会影响当前select的数据，这就实现了可重复读了。

  - 当前读

    对于会对数据修改的操作(update、insert、delete)都是采用当前读的模式。在执行这几个操作时会读取最新的记录，即使是别的事务提交的数据也可以查询到。

    select的当前读需要手动的加锁：

    ```mysql
    select * from table where ? lock in share mode;
    select * from table where ? for update;
    ```



## MVCC保证读一致性



**示例**

```mysql
create table mvcctest( 
id int primary key not null auto_increment, 
name varchar(20));

insert into mvcctest values(NULL,'mi');
insert into mvcctest values(NULL,'kong');
```



假设系统初始事务ID为1；

| ID   | NAME | 创建时间 | 过期时间  |
| ---- | ---- | -------- | --------- |
| 1    | mi   | 1        | undefined |
| 2    | kong | 1        | undefined |



`transaction1`：

```mysql
start transaction;
select * from mvcctest;  //(1)
select * from mvcctest;  //(2)
commit
```



- 假设当执行事务1的过程中，准备执行语句(2)时，开始执行事务2：

### SELECT

`transaction2`：

```mysql
start transaction;
insert into mvcctest values(NULL,'qu');
commit;
```



| ID   | NAME | 创建时间 | 过期时间  |
| ---- | ---- | -------- | --------- |
| 1    | mi   | 1        | undefined |
| 2    | kong | 1        | undefined |
| 3    | qu   | 3        | undefined |

事务2执行完毕，开始执行事务1 语句(2)，由于事务1只能查询创建时间小于等于2的，所以事务2新增的记录在事务1中是查不出来的，这就**通过乐观锁的方式避免了幻读的产生**



### UPDATE

- 假设当执行事务1的过程中，准备执行语句(2)时，开始执行事务2：

`transaction3`：

```mysql
start transaction;
update mvcctest set name = 'fan' where id = 2;
commit;
```

InnoDB执行UPDATE，实际上是新插入了一行记录，并保存其创建时间为当前事务的ID，同时保存当前事务ID到要UPDATE的行的删除时间



| ID   | NAME | 创建时间 | 过期时间  |
| ---- | ---- | -------- | --------- |
| 1    | mi   | 1        | undefined |
| 2    | kong | 1        | 4         |
| 2    | fan  | 4        | undefined |

事务3执行完毕，开始执行事务1 语句(2)，由于事务1只能查询创建时间小于等于2的，所以事务修改的记录在事务1中是查不出来的，这样就保证了事务在两次读取时读取到的数据的状态是一致的



### DELETE

- 假设当执行事务1的过程中，准备执行语句(2)时，开始执行事务4：

`transaction4`：

```mysql
start transaction;
delete from mvcctest where id = 2;
commit;
```



| ID   | NAME | 创建时间 | 过期时间  |
| ---- | ---- | -------- | --------- |
| 1    | mi   | 1        | undefined |
| 2    | kong | 1        | 5         |

事务4执行完毕，开始执行事务1 语句(2)，由于事务1只能查询创建时间小于等于2、并且过期时间大于等于2，所以id=2的记录在事务1的 语句2中，也是可以查出来的，这样就保证了事务在两次读取时读取到的数据的状态是一致的



# InnoDB之LBCC

- Lock-Based Concurrency Control，基于锁的并发控制
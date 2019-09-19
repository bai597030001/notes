# Mysql show processlist 排查问题

[官网](https://dev.mysql.com/doc/refman/5.7/en/show-processlist.html)

- mysql show full processlist 用来查看当前线程处理情况 

### show full processlist


### select id, db, user, host, command, time, state, info from information_schema.processlist order by time desc

### 通过navicat中的【工具】=> 【服务器监控】进行查看

	Id：链接mysql 服务器线程的唯一标识，可以通过kill来终止此线程的链接。
	User：当前线程链接数据库的用户
	Host：显示这个语句是从哪个ip 的哪个端口上发出的。可用来追踪出问题语句的用户
	db: 线程链接的数据库，如果没有则为null
	Command: 显示当前连接的执行的命令，一般就是休眠或空闲（sleep），查询（query），连接（connect）
	Time: 线程处在当前状态的时间，单位是秒
	State：显示使用当前连接的sql语句的状态，很重要的列，后续会有所有的状态的描述，请注意，state只是语句执行中的某一个状态，一个 sql语句，已查询为例，可能需要经过copying to tmp table，Sorting result，Sending data等状态才可以完成
	Info: 线程执行的sql语句，如果没有语句执行则为null。这个语句可以使客户端发来的执行语句也可以是内部执行的语句 

## kill id

[官网](https://dev.mysql.com/doc/refman/5.7/en/kill.html)

- 查询执行时间超过2分钟的线程，然后拼接成 kill 语句

```sql

	select concat('kill ', id, ';')
	from information_schema.processlist
	where command != 'Sleep'
	and time > 2*60
	order by time desc
```
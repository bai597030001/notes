# Mysql Row_Format 参数讲解 

- 动态表

	一张表里面存在varchar、text以及其变形、blob以及其变形的字段。

	该表的 row_format是dynamic，就是说每条记录所占用的字节是动态的。其优点节省空间，缺点增加读取的时间开销

- 静态表

	与动态表相反，表中以char代替varchar等动态字段。

	该表 row_format为fixed,即每条记录占用字节一样。优点读取快，缺点浪费部分空间

---

> 做搜索查询量大的表一般都以空间来换取时间，设计成静态表。

### row_format还有其他一些值

	DEFAULT

	FIXED

	DYNAMIC

	COMPRESSED

	REDUNDANT

	COMPACT

- 修改行格式

	ALTER TABLE table_name ROW_FORMAT = DEFAULT

- 修改过程导致：

	fixed--->dynamic: 这会导致CHAR变成VARCHAR  
	dynamic--->fixed: 这会导致VARCHAR变成CHAR
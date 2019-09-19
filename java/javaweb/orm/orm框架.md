# ORM框架

### ORM框架

- **ORM框架**( 对象-关系映射（Object/Relation Mapping，简称ORM）)

> 它是一种解决问题的思路，是一种思想。它的实质就是在关系型数据库和业务实体对象之间作一个映射，这样，我们在具体的操作业务对象的时候，就不需要再去和复杂的SQL语句打交道，只需简单的操作对象的属性和方法。或者说，ORM，就是内存中的对象与数据库中的数据间的映射关系。


- **Hibernate**：Hibernate JPA是**JPA的一种底层实现**。是一个**持久化框架和ORM框架**。

- **mybatis** 是一个**ORM框架**。
  
```java

		spring-data-jpa会依赖引入spring-jdbc，spring-tx，spring-orm等jar包

- Hibernate 和 mybatis。Hibernate是符合JPA规范的完备的ORM框架；Mybatis更偏向于sql mapping，是一个持久层的sql mapping框架。

- spring提供了对hibernate的支持

- Spring并没有提供对mybatis的集成支持。但mybatis提供了compile将 MyBatis 代码无缝地整合到 Spring 中。 

```xml

	group: 'org.mybatis' 
	name: 'mybatis-spring' 
	version: 'x.x.x'

### JPA

- 1.**jpa**：JPA(Java Persistence Api)，即：Java持久层API。是JDK 5.0注解或XML描述对象－关系表的映射关系，并将运行期的实体对象持久化到数据库中。本质上就是一种**ORM规范**，所谓规范即只定义标准规则（如注解、接口），不提供实现。

> JPA通过JDK 5.0注解或XML描述对象－关系表的映射关系，并将运行期的实体对象持久化到数据库中。

### JPA与ORM的关系

- JPA的实现思想即是ORM。

- JPA并不是一种框架，而是一类框架的总称，基于ORM思想实现的框架都称为JPA框架。

- 常用的JPA框架：

		Hibernate(JBoos开源)  
		Open JPA(apache开源)  
		Spring Data JPA(Spring框架中的子模块)  
		EclipseLink(由Oracle提供，后捐献给了Eclipse社区)  
		Toplink

- **Spring Data Jpa**：是在JPA的规范下，提供了respository层(数据访问层)的实现。是**对JPA的再次封装和抽象**，底层还是要使用JPA的具体实现，如Hibernate JPA作为JPA规范的实现。  

#### 注意点

	JPA框架底层都是ORM思想，其框架具有ORM全部的优缺点。
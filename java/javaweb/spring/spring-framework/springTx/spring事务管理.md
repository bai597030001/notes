# 核心接口

![](img/springTransaction.png)

- Spring并不直接管理事务，而是提供了多种事务管理器，他们将事务管理的职责委托给Hibernate或者JTA等持久化机制所提供的相关平台框架的事务来实现。 

- Spring事务管理器的接口是`org.springframework.transaction.PlatformTransactionManager`，通过这个接口，Spring为各个平台如JDBC、Hibernate等都提供了对应的事务管理器，但是具体的实现就是各个平台自己的事情了。

```java
public interface PlatformTransactionManager()...{  
    // 由TransactionDefinition得到TransactionStatus对象
    TransactionStatus getTransaction(TransactionDefinition definition) throws TransactionException; 
    // 提交
    Void commit(TransactionStatus status) throws TransactionException;  
    // 回滚
    Void rollback(TransactionStatus status) throws TransactionException;  
} 
```

- 从这里可知具体的事务管理机制对Spring来说是透明的，它并不关心那些，那些是对应各个平台需要关心的，所以Spring事务管理的一个优点就是为不同的事务API提供一致的编程模型，如JTA、JDBC、Hibernate、JPA。


- 下面分别介绍各个平台框架实现事务管理的机制:

## JDBC事务

- 如果应用程序中直接使用JDBC来进行持久化，`DataSourceTransactionManager`会为你处理事务边界。为了使用`DataSourceTransactionManager`，你需要使用如下的XML将其装配到应用程序的上下文定义中：

```xml
<bean id="transactionManager" class="org.springframework.jdbc.datasource.DataSourceTransactionManager">
    <property name="dataSource" ref="dataSource" />
</bean>
```

- 实际上，`DataSourceTransactionManager`是通过调用`java.sql.Connection`来管理事务，而后者是通过`DataSource`获取到的。通过调用连接的`commit()`方法来提交事务，同样，事务失败则通过调用`rollback()`方法进行回滚。

## Hibernate事务

```xml
<bean id="transactionManager" class="org.springframework.orm.hibernate3.HibernateTransactionManager">
    <property name="sessionFactory" ref="sessionFactory" />
</bean>
```

- `sessionFactory`属性需要装配一个Hibernate`的`session工厂，`HibernateTransactionManager`的实现细节是它将事务管理的职责委托给`org.hibernate.Transaction`对象，而后者是从`Hibernate Session`中获取到的。当事务成功完成时，`HibernateTransactionManager`将会调用`Transaction`对象的`commit()`方法，反之，将会调用`rollback()`方法。

## Java持久化API事务（JPA）

- Hibernate多年来一直是事实上的Java持久化标准，但是现在Java持久化API作为真正的Java持久化标准进入大家的视野。如果你计划使用JPA的话，那你需要使用Spring的JpaTransactionManager来处理事务。你需要在Spring中这样配置JpaTransactionManager：

```xml
<bean id="transactionManager" class="org.springframework.orm.jpa.JpaTransactionManager">
    <property name="sessionFactory" ref="sessionFactory" />
</bean>
```

- JpaTransactionManager只需要装配一个JPA实体管理工厂（javax.persistence.EntityManagerFactory接口的任意实现）。JpaTransactionManager将与由工厂所产生的JPA EntityManager合作来构建事务。

##  Java原生API事务

- 如果你没有使用以上所述的事务管理，或者是跨越了多个事务管理源（比如两个或者是多个不同的数据源），你就需要使用JtaTransactionManager：

```xml
<bean id="transactionManager" class="org.springframework.transaction.jta.JtaTransactionManager">
    <property name="transactionManagerName" value="java:/TransactionManager" />
</bean>
```

- `JtaTransactionManager`将事务管理的责任委托给`javax.transaction.UserTransaction`和`javax.transaction.TransactionManager`对象，其中事务成功完成通过`UserTransaction.commit()`方法提交，事务失败通过`UserTransaction.rollback()`方法回滚。

# 事务属性

- 事务管理器接口`PlatformTransactionManager`通过`getTransaction(TransactionDefinition definition)`方法来得到事务，这个方法里面的参数是`TransactionDefinition`类，这个类就定义了一些基本的事务属性。 
那么什么是事务属性呢？事务属性可以理解成事务的一些基本配置，描述了事务策略如何应用到方法上。事务属性包含了5个方面，如图所示：

![](img/springTransaction1.png)

```java
public interface TransactionDefinition {
    int getPropagationBehavior(); // 返回事务的传播行为
    int getIsolationLevel(); // 返回事务的隔离级别，事务管理器根据它来控制另外一个事务可以看到本事务内的哪些数据
    int getTimeout();  // 返回事务必须在多少秒内完成
    boolean isReadOnly(); // 事务是否只读，事务管理器能够根据这个返回值进行优化，确保事务是只读的
}
```

- 我们可以发现`TransactionDefinition`正好用来定义事务属性，下面详细介绍一下各个事务属性。

## 传播行为

```properties
propagation_requierd		=如果当前没有事务，就新建一个事务，如果已存在一个事务中，加入到这个事务中，这是Spring默认的选择。

propagation_supports		=支持当前事务，如果没有当前事务，就以非事务方法执行。

propagation_mandatory		=使用当前事务，如果没有当前事务，就抛出异常。

propagation_required_new	=新建事务，如果当前存在事务，把当前事务挂起。

propagation_not_supported	=以非事务方式执行操作，如果当前存在事务，就把当前事务挂起。

propagation_never			=以非事务方式执行操作，如果当前事务存在则抛出异常。

propagation_nested			=如果当前存在事务，则在嵌套事务内执行。如果当前没有事务，则执行与propagation_required类似的操作。
```

## 隔离级别

```properties
ISOLATION_DEFAULT			=使用后端数据库默认的隔离级别

ISOLATION_READ_UNCOMMITTED	=最低的隔离级别，允许读取尚未提交的数据变更，可能会导致脏读、幻读或不可重复读

ISOLATION_READ_COMMITTED	=允许读取并发事务已经提交的数据，可以阻止脏读，但是幻读或不可重复读仍有可能发生

ISOLATION_REPEATABLE_READ	=对同一字段的多次读取结果都是一致的，除非数据是被本身事务自己所修改，可以阻止脏读和不可重复读，但幻读仍有可能发生

ISOLATION_SERIALIZABLE		=最高的隔离级别，完全服从ACID的隔离级别，确保阻止脏读、不可重复读以及幻读，也是最慢的事务隔离级别，因为它通常是通过完全锁定事务相关的数据库表来实现的
```

# 事务几种实现方式

- 编程式事务管理对基于 POJO 的应用来说是唯一选择。我们需要在代码中调用beginTransaction()、commit()、rollback()等事务管理相关的方法，这就是编程式事务管理。

- 基于 TransactionProxyFactoryBean 的声明式事务管理

- 基于 @Transactional 的声明式事务管理

- 基于Aspectj AOP 配置事务

---

## 编程式事务管理

## TransactionProxyFactoryBean的声明式事务管理

## Aspectj xml 声明式事务管理

## @Transactional 的声明式事务管理



# spring事务失效的场景

## 数据库引擎不支持事务

> 从 MySQL 5.5.5 开始的默认存储引擎是：InnoDB，之前默认的都是：MyISAM

## 注解所在的类没有被 Spring 管理

```java
// @Service
public class OrderServiceImpl implements OrderService {
    @Transactional
    public void updateOrder(Order order) {
        // update order
    }
}
```

如果此时把 `@Service` 注解注释掉，这个类就不会被加载成一个 Bean，那这个类就不会被 Spring 管理了，事务自然就失效了。

## 注解所在的方法不是 public 的

> `@Transactional` 只能用于 public 的方法上，否则事务不会失效，如果要用在非 public 方法上，可以开启 `AspectJ` 代理模式。

## 发生了自身调用

```java
// update方法上面没有加 @Transactional 注解，调用有 @Transactional 注解的 updateOrder 方法，updateOrder 方法上的事务不管用，不生效！！！
@Service
public class OrderServiceImpl implements OrderService {

    public void update(Order order) {
        updateOrder(order);
    }

    @Transactional
    public void updateOrder(Order order) {
        // update order
    }

}

// update 方法上加了 @Transactional，updateOrder 加了 REQUIRES_NEW 新开启一个事务，那么新开的事务也不管用！！！
@Service
public class OrderServiceImpl implements OrderService {

    @Transactional
    public void update(Order order) {
        updateOrder(order);
    }

    @Transactional(propagation = Propagation.REQUIRES_NEW)
    public void updateOrder(Order order) {
        // update order
    }

}
```

> 因为它们发生了自身调用，就调该类自己的方法，而没有经过 Spring 的代理类，默认只有在外部调用事务才会生效



解决方案之一就是在的类中注入自己，用注入的对象再调用另外一个方法



另一个解决方案就是用Aop代理对象



在spring的配置中，我们只需要添加标签`<aop:aspectj-autoproxy expose-proxy="true"/>` 或者 `<aop:config expose-proxy="true">`

并且在代码的调用中要求使用代理对象去调用即可：

```java
((ServiceA ) AopContext.currentProxy()).insert();
```



## 数据源没有配置事务管理器

```java
@Bean
public PlatformTransactionManager transactionManager(DataSource dataSource) {
    return new DataSourceTransactionManager(dataSource);
}
```



## propagation配置不当

`@Transactional(propagation = Propagation.NOT_SUPPORTED) `表示不以事务运行，当前若存在事务则挂起



## 异常被吃了

```java
// @Service
public class OrderServiceImpl implements OrderService {

    @Transactional
    public void updateOrder(Order order) {
        try {
            // update order
        } catch {

        }
    }
}
```

把异常吃了，然后又不抛出来，那么事务是无法回滚的



## 异常类型错误

```java
// @Service
public class OrderServiceImpl implements OrderService {

    @Transactional
    public void updateOrder(Order order) {
        try {
            // update order
        } catch {
            throw new Exception("更新错误");
        }
    }
}
```

这样事务也是不生效的，因为默认回滚的是：RuntimeException，如果你想触发其他异常的回滚，需要在注解上配置一下，如：

```java
@Transactional(rollbackFor = Exception.class)
```

这个配置仅限于 `Throwable` 异常类及其子类。
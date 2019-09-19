# spring注解之@Bean
---
## 作用 
> Spring的@Bean注解用于告诉方法，产生一个Bean对象，然后这个Bean对象交给Spring管理。  
> 产生这个Bean对象的方法Spring只会调用一次，随后Spring会将这个Bean对象放在自己的IOC容器中。

## 基于xml方式的bean声明

- spring3.0之前，没有注解的方式，如果要声明一个bean交给spring容器管理，只能通过xml配置文件的方式进行配置。

示例：

bean.xml
	
```xml
<?xml version="1.0" encoding="UTF-8"?>
<beans xmlns="http://www.springframework.org/schema/beans"
       xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
       xsi:schemaLocation="http://www.springframework.org/schema/beans http://www.springframework.org/schema/beans/spring-beans.xsd">

    <bean id="person" class="com.myspringboot.helloworld.model.Person">
        <property name="name" value="赵六"/>
        <property name="age" value="26"/>
    </bean>
</beans>
```

使用:

```java
ApplicationContext context = new ClassPathXmlApplicationContext("bean.xml");
Person xmlBeanPerson = context.getBean(Person.class);
System.out.println(xmlBeanPerson);
......
```

## 基于注解方式声明并注入bean

- spring3.0之后，支持使用注解方式来声明一个bean，并交给spring容器管理。

示例：

```java
config/MyMvcConfig.java

@Configuration
public class MyMvcConfig {

    @Bean
    public Person getPerson() {
        return new Person();
    }
}
```

> @Configuration告诉spring，这个类是一个spring配置类。  
> @Bean告诉spring，这个方法会产生一个Bean对象，然后spring会管理这个Bean对象。

## @Bean定义

```java
@Target({ElementType.METHOD, ElementType.ANNOTATION_TYPE})
@Retention(RetentionPolicy.RUNTIME)
@Documented
public @interface Bean {
    @AliasFor("name")
    String[] value() default {};

    @AliasFor("value")
    String[] name() default {};

    /** @deprecated */
    @Deprecated
    Autowire autowire() default Autowire.NO;

    boolean autowireCandidate() default true;

    String initMethod() default "";

    String destroyMethod() default "(inferred)";
}
```

- ElementType.METHOD, ElementType.ANNOTATION_TYPE：可以在使用在方法上，以及一个注释类型声明  

- value -- bean别名和name是相互依赖关联的，value,name如果都使用的话值必须要一致  

- name -- bean名称，如果不写会默认为注解的方法名称  

- autowire -- 自定装配默认是不开启的，建议尽量不要开启，因为自动装配不能装配基本数据类型、字符串、数组等，这是自动装配设计的局限性，以及自动装配不如显示依赖注入精确  

- initMethod -- bean的初始化之前的执行方法，该参数一般不怎么用，因为可以完全可以在代码中实现  

- destroyMethod -- bean销毁执行的方法

- bean的别名与描述

	```java
	@Configuration
	public class AppConfig {
	
	​```
	@Description("Provides a basic example of a bean")//描述
	@Bean(name = { "dataSource", "subsystemA-dataSource", "subsystemB-dataSource" })//别名
	public DataSource dataSource() {
	    // instantiate, configure and return DataSource bean...
	}
	​```
	
	}
	```
	
	


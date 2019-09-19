# spring中Ioc容器的使用
---

- 在使用中，如果我们不适用springmvc,springboot等，而只是想单纯的使用spring的Ioc容器来帮助我们管理依赖关系，怎么办呢？



## BeanFactory

- BeanFactory为IOC容器提供了基础功能，Spring文档中提到，当前该类仅仅是为了向后兼容老的版本，除非你有更好的原因否则就应该使用第二种容器。

## ApplicationContext

- 通过API文档可以知道，ApplicationContext是BeanFactory的子接口，并且从文档中也可以看到ApplicaionContext除了包含有BeanFactory的所有功能还支持了更多的功能。

## ApplicationContext的实现有四种方式：

```

     FileSystemXmlApplicationContext：加载配置文件的时候采用的是项目的路径。

     ClassPathXmlApplicationContext：加载配置文件的时候根据ClassPath位置。

     XmlWebApplicationContext：在Web环境下初始化监听器的时候会加载该类。

     AnnotationConfigApplicationContext：根据注解的方式启动Spring 容器。
```

### AnnotationConfigApplicationContext

```java

	package com.qls.test;
	
	import com.qls.impl.BraveKnight;
	import com.qls.impl.SlayDragonQuest;
	import com.qls.inter.Knight;
	import com.qls.inter.Query;
	import org.springframework.context.annotation.Bean;
	import org.springframework.context.annotation.Configuration;
	
	/**
	 * @Bean indicated that a method produced a bean to be managed by the spring container;
	 */
	@Configuration
	public class KnightConfig {
	    @Bean(name = "knight")
	    public Knight knight(){
	        return  new BraveKnight(quest());
	    }
	    @Bean(name = "quest")
	    public Query quest(){
	        return  new SlayDragonQuest(System.out);
	    }
	}
```

```java

	package com.qls.test;
	
	import com.qls.inter.Knight;
	import org.springframework.context.annotation.AnnotationConfigApplicationContext;
	
	public class Test4 {
	    public static void main(String[] args) {
	        AnnotationConfigApplicationContext ac = new AnnotationConfigApplicationContext(KnightConfig.class);
	        Knight knight = ac.getBean(Knight.class);
	        knight.embarkOnQuest();
	    }
	}
```

### ClassPathXmlApplicationContext/FileSystemXmlApplicationContext


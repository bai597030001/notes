# Spring事件监听机制

- spring的事件监听机制是基于java的事件机制实现的。

[原文链接](https://baijiahao.baidu.com/s?id=1617119527751442850&wfr=spider&for=pc)

# 1.java的事件机制

- java中的事件机制一般包括3个部分：EventObject，EventListener和Source。

## 1.1EventObject

- java.util.EventObject是事件状态对象的基类，它封装了事件源对象以及和事件相关的信息。所有java的事件类都需要继承该类。

## 1.2EventListener

- java.util.EventListener是一个标记接口，就是说该接口内是没有任何方法的。所有事件监听器都需要实现该接口。事件监听器注册在事件源上，当事件源的属性或状态改变的时候，调用相应监听器内的回调方法。

## 1.3Source

- 事件源不需要实现或继承任何接口或类，它是事件最初发生的地方。因为事件源需要注册事件监听器，所以事件源内需要有相应的盛放事件监听器的容器。



**java的事件机制是一个观察者模式。**



# 2.spring的事件

[原文链接](https://blog.csdn.net/wyj_0926/article/details/81410508)

[示例demo](https://www.cnblogs.com/winkey4986/p/5279502.html)

- Spring 的事件（Application Event）为 **Bean 与 Bean 之间的消息通信**提供了支持。当一个 Bean 处理完一个任务之后，希望另一个 Bean 知道并能做相应的处理，这时我们就需要让另一个 Bean 监听当前 Bean 所发送的事件。*（观察者模式）*

- Spring 的事件需要遵循以下流程：

  - 1. 自定义事件，集成 ApplicationEvent。

  - 2. 定义事件监听器，实现 ApplicationListener。

  - 3. 使用容器（ApplicationContext）发布事件

    - ApplicationEventPublisher是Spring的事件发布接口，ApplicationContext实现了该接口
    - ApplicationEventMulticaster就是Spring事件机制中的事件广播器，默认实现SimpleApplicationEventMulticaster

- 在Spring中通常是ApplicationContext本身担任监听器注册表的角色，在其子类AbstractApplicationContext中就聚合了事件广播器ApplicationEventMulticaster和事件监听器ApplicationListnener，并且提供注册监听器的addApplicationListnener方法。

- 其**执行的流程**大致为：

  - 当一个事件源产生事件时，它通过事件发布器ApplicationEventPublisher发布事件，然后事件广播器ApplicationEventMulticaster会去事件注册表ApplicationContext中找到事件监听器ApplicationListnener，并且逐个执行监听器的onApplicationEvent方法，从而完成事件监听器的逻辑。

## 2.1代码示例

- 1.自定义事件，必须继承ApplicationEvent，实现构造方法。

```java

public class UserEvent extends ApplicationEvent {

    private BaseBean baseBean;

    /**
     * 重写构造函数
     * @param source 发生事件的对象
     */
    public UserEvent(Object source, BaseBean baseBean) {
        super(source);
        this.baseBean = baseBean;

    }
    public BaseBean getBaseBean() {
        return baseBean;
    }
    public void setBaseBean(BaseBean baseBean) {
        this.baseBean = baseBean;
    }
}
```

- 1.2事件监听器，监听方法的实现有很多种，可以使用注解；也可以实现接口ApplicationListener

```java
@Component
public class UserBeanListener {

    /**
     * 监听实现方法
     * @param event 监听事件
     */
    @EventListener
    public void register(UserEvent event) {
        System.out.println("UserBeanListener: " + event.getBaseBean().getName());
    }
}

```

- 1.3发布事件
  - ApplicationEventPublisher是Spring的事件发布接口，ApplicationContext实现了该接口
  - ApplicationEventMulticaster就是Spring事件机制中的事件广播器，默认实现SimpleApplicationEventMulticaster

```java
@Service
public class RegisterService {

    @Autowired
    private ApplicationContext applicationContext;

    public void register() {
        /*
         * 这里的UserA 和 UserB 都是BaseBean的子类
         */
        UserA a = new UserA();
        a.setName("a");
        UserB b = new UserB();
        b.setName("b");
        // 发布事件  UserEvent
        applicationContext.publishEvent(new UserEvent(this, a));
        // 发布事件  UserEvent
        applicationContext.publishEvent(new UserEvent(this, b));
    }
}
```

## 2.2注解实现（@EventListener）

- 除了通过实现接口，还可以使用@EventListener 注解，实现对任意的方法都能监听事件。

```java
@Configuration
@ComponentScan(value = "com.learn")
public class Config {
@EventListener(classes={ApplicationEvent.class})
public void listen(ApplicationEvent event){
	System.out.println("事件触发："+event.getClass().getName());
	}
} 
```

- 在任意方法上标注@EventListener 注解，指定 classes，即需要处理的事件类型，一般就是 ApplicationEven 及其子类，可以设置多项。
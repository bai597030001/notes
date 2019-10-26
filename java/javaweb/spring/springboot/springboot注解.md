# @ConfigurationProperties

- 使用情景：我们想把配置文件的信息，读取并自动封装成实体类。这样我们在代码里面使用就轻松方便多了。

## 使用

yaml：

```yaml
server:
  port: 9090

person:
  name: 张三
  age: 18
  address: 海淀
  map:
    k1: v1
    k2: v2
  list:
    - value1
    - value2
    - value3
```


实体类

```java
@ConfigurationProperties("person")
@Component
public class Person {

    private String name;

    private Integer age;

    private String address;

    private Map<String, Object> map;

    private List<Object> list;
	
	......//getter,setter...
}
```

使用：

```java
@Autowired
private Person person;

@Test
public void configTestYaml() {
    System.out.println(person);
}
```

> 输出： Person{name='王五', age=24, address='丰台', map={k1=v1, k2=v2}, list=[{val1, val2}]}

- 还可以把@ConfigurationProperties直接定义在@bean的注解上，这时bean实体类就不用@Component和@ConfigurationProperties了

```java
	@Configuration
	public class MyMvcConfig {
	
	    @Bean
	    @ConfigurationProperties("person")
	    public Person getPerson() {
	        return new Person();
	    }
	}


- 然后在使用的时候直接注入即可

```

```java
@Autowired  
private Person person;

@Test
public void configTestYaml() {
    System.out.println(person);
}
```

## 声明

```java
@Target({ElementType.TYPE, ElementType.METHOD})
@Retention(RetentionPolicy.RUNTIME)
@Documented
public @interface ConfigurationProperties {
    @AliasFor("prefix")
    String value() default "";

    @AliasFor("value")
    String prefix() default "";

    boolean ignoreInvalidFields() default false;

    boolean ignoreUnknownFields() default true;
}
```



# @Value


# 比较


							@ConfigurationProperties					@value
	功能						批量注入配置文件中的属性						一个个指定
	松散绑定（松散语法）		支持											不支持
	SpEL					不支持										支持
	JSR303数据校验			支持											不支持
	复杂类型封装				支持											不支持


### 松散语法

- 属性命名规则

  person.firstName:使用标准方式  
  person.first-name:大写用-   
  person.first_name:大写用_  
  PERSON_FIRST_NAME: 系统属性推荐使用这种写法

### JSR303数据校验

- @Validated注解

  eg: @NotNull, @Email

### SpEL

application.properties文件中  
userAge = 12   //可以  
userAge = #{2*6}  //EL表达式不支持


# 4.使用原则

在某个业务逻辑中需要获取一下配置文件中的某项值，使用@Value；  

如果专门编写了一个javaBean来和配置文件进行映射，我们就直接使用@ConfigurationProperties；



# @ImportSource

- 作用：导入Spring的配置文件，让配置文件里面的内容生效

### 以前的做法：

> 我们自己定义一个类  假如叫做HelloService.java  
> 然后定义一个xml文件  就叫beans.xml

```xml
<?xml version="1.0" encoding="UTF‐8"?>
<beans xmlns="http://www.springframework.org/schema/
	beans"xmlns:xsi="http://www.w3.org/2001/XMLSchema‐instance"xsi:schemaLocation="http://www.springframework.org/schema/beanshttp://www.springframework.org/schema/beans/spring‐beans.xsd">
	<bean id="helloService" class="com.atguigu.springboot.service.HelloService">
</bean>
</beans>
```

然后我们去test中测试ioc中有没有这个容器

```java
@RunWith(SpringRunner.class)
@SpringBootTestpublic 
class Springboot02ApplicationTests {		
	@Autowired	ApplicationContext ioc; 	
	@Test	
	public void testHelloService(){		
		boolean b = ioc.containsBean("helloService");		
		System.out.println(b);	
	}
}
```

结果显示不存在

- 然后我们把@ImportResource标注在一个配置类上

  @ImportResource(locations = {"classpath:beans.xml"})
  		@SpringBootApplicationpublic class Springboot02Application { 	
  		public static void main(String[] args) {		
  		SpringApplication.run(Springboot02Application.class, args);	
  		}
  	}

我们再测试一下，就说ioc容器中存在。

- **以上就是以前的做法，但是SpringBoot推荐的做法是给容器中添加组件的方式**

- 推荐使用全注解的方式  
  1.创建一个config包。  
  2.创建一个配置类MyAppConfig.java  
  添加一个注解  

```java
	@Configuration
	public class MyAppConfig  {
	
	    @Bean
	    public Person getPerson() {
	        return new Person();
	    }
	}


```



# @PropertySource注解

- @PropertySource注解: 加载指定的配置文件

```java
	@Component  
	@PropertySource(value = {"classpath:person.properties"})  
	@ConfigurationProperties(prefix = "person")  
	@Validated  
	public class Person { 
	
	    @NotNull
	    private String name;
	
	    private Integer age;
	
	    private String address;
	
	    private Map<String, Object> map;
	
	    private List<Object> list;
		
		...

```

- 将全局配置文件(application.properties,application.yaml)中person对应的内容拷贝到person.properties文件中，然后将全局配置文件中关于person的信息都**注销**掉。就会读取person.properties配置文件中的信息。
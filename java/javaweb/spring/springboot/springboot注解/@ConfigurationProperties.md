# 1.springboot之@ConfigurationProperties

- 使用情景：我们想把配置文件的信息，读取并自动封装成实体类。这样我们在代码里面使用就轻松方便多了。

## 使用
 
yaml：

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


实体类

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

使用：

	@Autowired
    private Person person;

	@Test
    public void configTestYaml() {
        System.out.println(person);
    }

> 输出： Person{name='王五', age=24, address='丰台', map={k1=v1, k2=v2}, list=[{val1, val2}]}

- 还可以把@ConfigurationProperties直接定义在@bean的注解上，这时bean实体类就不用@Component和@ConfigurationProperties了

```

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

	@Autowired  
    private Person person;

	@Test
    public void configTestYaml() {
        System.out.println(person);
    }

## 声明

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


# 2.springboot之@Value


# 3.比较


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
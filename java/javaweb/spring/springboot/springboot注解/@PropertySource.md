# springboot之@PropertySource注解

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

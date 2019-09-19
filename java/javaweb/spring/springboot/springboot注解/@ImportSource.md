# springboot之@ImportSource

- 作用：导入Spring的配置文件，让配置文件里面的内容生效

### 以前的做法：

> 我们自己定义一个类  假如叫做HelloService.java  
> 然后定义一个xml文件  就叫beans.xml

	<?xml version="1.0" encoding="UTF‐8"?>
	<beans xmlns="http://www.springframework.org/schema/
		beans"xmlns:xsi="http://www.w3.org/2001/XMLSchema‐instance"xsi:schemaLocation="http://www.springframework.org/schema/beanshttp://www.springframework.org/schema/beans/spring‐beans.xsd">
		<bean id="helloService" class="com.atguigu.springboot.service.HelloService">
	</bean>
	</beans>

然后我们去test中测试ioc中有没有这个容器

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


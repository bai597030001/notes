## SpringBoot配置(Profile不同环境配置)  

### properties格式

- Profile是Spring对不同环境提供不同配置功能的支持，可以通过激活、指定参数等方式快速切换环境

- 一般我们在开发的时候有测试环境，开发环境，生产环境等。

- 在编写多个配置文件的时候，文件名字是application-(profile).properties/yml

	eg: application-test.properties, application-prod.properties, application-dev.properties

- 我们只需要在application-.properties/yaml中指定激活配置文件即可:

	spring.profiles.active=prod

### yaml/yml格式

- 文档快

```

	#激活哪个环境
	spring:
	  profiles:
	    active: test
	#测试环境
	---
	server:
	  port: 808
	spring:
	  profiles: test
	#开发环境
	---
	server:
	  port: 8082
	spring:
	  profiles: dev

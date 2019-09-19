# 1.SqlMapConf.xml配置
---

## mybatis的配置文件中配置内容和顺序如下

- 1.properties(属性)

	`<properties resource="jdbc.properties">`  
		`<property name="jdbc.username" value="root1"/>`  
		`<property name="jdbc.password" value="root"/>`  
	`</properties>`


- 2.typeAlises(别名)

	`<typeAliases>`
		`<!-- 单个别名定义 -->`
		`<!-- <typeAlias type="com.itheima.mybatis.pojo.User" alias="user"/> -->`
		`<!-- 别名包扫描器(推荐使用此方式)，整个包下的类都被定义别名，别名为类名，不区分大小写-->`
		`<package name="com.itheima.mybatis.pojo"/>`
	`</typeAliases>`


- 3.mapper(映射配置)

	`<mappers>`  
		`<!-- 第一种方式，加载 resource-->  `  
		`<mapper resource="mapper/user.xml"/>  
		`<!-- <mapper resource="mapper/UserMapper.xml"/> -->  `  
		
		`<!-- 第二种方式，class扫描器要求：  `
			 `1、映射文件与接口同一目录下  `
			 `2、映射文件名必需与接口文件名称一致  `
		` -->  `
		`<!-- <mapper class="com.itheima.mybatis.mapper.UserMapper"/> -->  `  
		
		`<!-- 第三种方式，包扫描器要求(推荐使用此方式)：  `
			 `1、映射文件与接口同一目录下  `
			 `2、映射文件名必需与接口文件名称一致  `
		`-->  `
		`<package name="com.itheima.mybatis.mapper"/>  `
	`</mappers>`

## 编写mapper接口的四个开发规范
- 在mapper.xml中，使namespace等于mapper接口的地址（完全限定名）
- mapper.java接口中的方法名和mapper.xml中statement的id一致
- mapper.java接口中方法的输入参数类型和mapper.xml中statement的parameterType指定的类型一致
- mapper.java接口中方法返回值类型和mapper.xml中statement的resultType指定的类型一致

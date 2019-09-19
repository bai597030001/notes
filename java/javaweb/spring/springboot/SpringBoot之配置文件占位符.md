# springboot之配置文件占位符

### 使用SpringBoot提供的的一些随机数

	${random.value}、${random.int}、${random.long}
	
	${random.int(10)}、${random.int[1024,65536]}

### 使用我们这个配置文件中自己在前面定义的值

	person.last-name=张三

	person.dog.name=${person.last-name}小狗

- 如果前面没有这个值，可以用默认值：

	${app.name:金毛}
# mybatis新建简单工程注意事项

---

### mybatis新建工程共有以下几个步骤

	1. idea新建maven工程
	2. 新建包名: com.mybatis.demo, entity,mapper
	3. entity中定义实体类User
	4. mapper中配置实体类User对应的mabatis的sql查询配置文件
	5. 在resources目录下新建mybatis的总的配置文件mybatis-config.xml

### 重点注意事项

- mybatis的配置文件mybatis-config.xml中,需要将实体类对应的配置文件注册进来(java对象与数据库之间的xml文件)

		<!-- 注册映射文件：java对象与数据库之间的xml文件路径！ -->
	    <mappers>
	        <!--当用包名+文件名的全路径配置resource时,会报错:
	            Could not find resource com.mybatis.demo.mapper.BtsKiloModuleMapper.xml-->
	        <!--<package name="com.mybatis.demo.mapper.BtsKiloModuleMapper.xml"/>-->
	        <mapper resource="com.mybatis.demo.mapper.BtsKiloModuleMapper.xml"/>
	
	        <!--必须将xml配置文件单独放入resources文件夹中-->
	        <!--<mapper resource="mapper/BtsKiloModuleMapper.xml"/>-->
	    </mappers>

- 当在上述配置中配置完以后,会出现编译错误,是由于映射文件没有注册成功导致的。经查找发现，在生成的classes目录下并不包含实体类对用的映射文件配置类。这是由于maven在编译打包的时候，不会将src/java下的任何xml配置文件打包到classes目录。

### 解决办法

- 1.在pom.xml中新增以下配置，maven则会在编译打包时将源码路径下的xml配置文件添加到classes/下与com同级的mapper路径下，即:target/classes/mapper/User.xml。此时mabatis的配置文件中注册实体类文件的路径应该写成:

	<mappers>
        <mapper resource="mapper/User.xml"/>
    </mappers>

	pom.xml中新增配置:

		<build>

	        <resources>
	            <resource>
					<!--此处路径到java目录即可,否则打包的classes文件路径会出错,会将mapper目录单独打包到classes下-->
	                <directory>src/main/java/com/mybatis/demo</directory>
	                <includes>
	                    <include>**/*.xml</include>
	                    <include>**/*.properties</include>
	                </includes>
	            </resource>
	        </resources>
	
	    </build>

- 2.在resources目录下新建mapper目录，用于存放实体类的映射文件。这样maven打包的时候也会将在:target/classes/mapper下存放各个实体类的xml配置文件。注意，这种方式无需在pom.xml中配置实体类编译存放。
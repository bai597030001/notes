# 原生mybatis

启动初始化相关：主要就是一个数据库信息配置文件 --> 文件中同时指定需要加载的mapper文件 --> mapper文件中定义domain实体对象、mapper接口类、接口对应的sql语句定义。

使用时：通过读取xml配置，生成sessionFactory对象，用该对象创建出sqlSession，进行增删改查操作。



# mybatis类型处理器

在项目开发中经常会遇到一个问题：

当我们在javabean中自定义了枚举类型或者其它某个类型，但是在数据库中存储时往往需要转换成数据库对应的类型，并且在从数据库中取出来时也需要将数据库类型转换为javabean中的对应类型。比如：javabean中字段类型为Date，数据库中存储的是varchar类型；javabean中字段类型是Enum，数据库中存储的是String或者Integer。
因为有大量类似数据的转换，手动转换类型进行存储和查询已经过于麻烦。MyBatis为我们提供了解决办法：TypeHandler类型处理器。

## TypeHandler

MyBatis 中的 TypeHandler 类型处理器用于 JavaType 与 JdbcType 之间的转换，用于 PreparedStatement 设置参数值和从 ResultSet 或 CallableStatement 中取出一个值。



## BaseTypeHandler 

实际开发中，我们可以继承 org.apache.ibatis.type.BaseTypeHandler 类型来实现自定义类型处理器。这个类型是抽象类型，实现了 TypeHandler 的方法进行通用流程的封装，做了异常处理，并定义了几个类似的抽象方法



## 类型转换器还可以通过注解配置 java 类型和 jdbc 类型：

@MappedTypes：注解配置 java 类型
@MappedJdbcTypes：注解配置 jdbc 类型



## 配置TypeHandler到程序中

1.在Mapper.xml中声明

如果在 mybatis-config.xml 配置文件中没有配置 typeHandler，可以在各个映射文件中显示配置需要使用的类型处理器，也可以实现类型转换的功能。

```xml
<insert id="insert" parameterType="studentDO" keyProperty="id" useGeneratedKeys="true">
    insert into t_student (name, sex, selfcard_no, note)
    values (
        #{name,jdbcType=VARCHAR},
        #{sex,jdbcType=TINYINT,typeHandler=com.yjw.demo.mybatis.common.type.SexEnumTypeHandler},
        #{selfcardNo,jdbcType=BIGINT},
        #{note,jdbcType=VARCHAR}
    )
</insert>
```



2.在mybatis配置文件中设置

```xml
<?xml version="1.0" encoding="UTF-8" ?>
<!DOCTYPE configuration PUBLIC "-//mybatis.org//DTD Config 3.0//EN"
        "http://mybatis.org/dtd/mybatis-3-config.dtd">
<configuration>
    <typeHandlers>
        <typeHandler javaType="com.yjw.demo.mybatis.common.constant.Sex"
                     jdbcType="TINYINT"
                     handler="com.yjw.demo.mybatis.common.type.SexEnumTypeHandler"/>
    </typeHandlers>
</configuration>
```

mybatis-config.xml 文件配置 typeHandler，通过显示的指定 javaType 和 jdbcType 实现类型处理器的自动发现，比如在调用如下 insert 配置的时候就不需要显示的指定 typeHandler，就可以实现类型转换的功能。

```xml
<insert id="insert" parameterType="studentDO" keyProperty="id" useGeneratedKeys="true">
    insert into t_student (name, sex, selfcard_no, note)
    values (
        #{name,jdbcType=VARCHAR},
        #{sex,jdbcType=TINYINT},
        #{selfcardNo,jdbcType=BIGINT},
        #{note,jdbcType=VARCHAR}
    )
</insert>
```



3.在springboot的yml配置文件中设置类型处理器所在的包名

```yaml
mybatis:
  type-handlers-package: com.xxx.handler
```









# mybatis使用事项



## 生成的class包中无xml文件

pom.xml中新增配置:

```xml
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
```



## mapper(映射配置)

```xml
<mappers>
	<!-- 第一种方式，加载 resource-->
	<mapper resource="mapper/user.xml"/>
    
    <!-- 第二种方式，class扫描器要求：
	1、映射文件与接口同一目录下
	2、映射文件名必需与接口文件名称一致 -->
    <mapper class="com.itheima.mybatis.mapper.UserMapper"/>
    
    <!-- 第三种方式，包扫描器要求(推荐使用此方式)
         1、映射文件与接口同一目录下
         2、映射文件名必需与接口文件名称一致 -->
    <package name="com.itheima.mybatis.mapper"/>
</mappers>
```





## parametertype多个参数处理

### 单个参数

```java
public List<XXBean> getXXBeanList(@param("id")String id);  

<select id="getXXXBeanList" parameterType="java.lang.String" resultType="XXBean">

　　select t.* from tableName t where t.id= #{id}  

</select>  
```

其中方法名和ID一致，#{}中的参数名与方法中的参数名一致， 这里采用的是@Param这个参数，实际上@Param这个最后会被Mabatis封装为map类型的。

select 后的字段列表要和bean中的属性名一致， 如果不一致的可以用 as 来补充。

### 多个参数

#### 方案1

```java
public List<XXXBean> getXXXBeanList(String xxId, String xxCode);  

<select id="getXXXBeanList" resultType="XXBean">不需要写parameterType参数

　　select t.* from tableName where id = #{0} and name = #{1}  

</select>  
```

由于是多参数那么就不能使用parameterType， 改用#｛index｝是第几个就用第几个的索引，索引从0开始

#### 方案2

```java
public List<XXXBean> getXXXBeanList(@Param("id")String id, @Param("code")String code);  

<select id="getXXXBeanList" resultType="XXBean">

　　select t.* from tableName where id = #{id} and name = #{code}  

</select>  
```

由于是多参数那么就不能使用parameterType， 这里用@Param来指定哪一个

#### Map封装多参数

```java
public List<XXXBean> getXXXBeanList(HashMap map);  

<select id="getXXXBeanList" parameterType="hashmap" resultType="XXBean">

　　select 字段... from XXX where id=#{xxId} code = #{xxCode}  

</select>  
```

其中hashmap是mybatis自己配置好的直接使用就行。map中key的名字是那个就在#{}使用那个，map如何封装就不用了我说了吧。 

#### List封装in

```java
public List<XXXBean> getXXXBeanList(List<String> list);  

<select id="getXXXBeanList" resultType="XXBean">
　　select 字段... from XXX where id in
　　<foreach item="item" index="index" collection="list" open="(" separator="," close=")"> 
　　　　#{item}  
　　</foreach>  
</select>  
```

foreach 最后的效果是select 字段... from XXX where id in ('1','2','3','4') 

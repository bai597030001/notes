# mybatis parametertype 多个参数的处理

## 1.单个参数

	public List<XXBean> getXXBeanList(@param("id")String id);  

	<select id="getXXXBeanList" parameterType="java.lang.String" resultType="XXBean">
	
	　　select t.* from tableName t where t.id= #{id}  
	
	</select>  

其中方法名和ID一致，#{}中的参数名与方法中的参数名一致， 这里采用的是@Param这个参数，实际上@Param这个最后会被Mabatis封装为map类型的。

select 后的字段列表要和bean中的属性名一致， 如果不一致的可以用 as 来补充。

## 2.多个参数

### 2.1方案1

	public List<XXXBean> getXXXBeanList(String xxId, String xxCode);  

	<select id="getXXXBeanList" resultType="XXBean">不需要写parameterType参数
	
	　　select t.* from tableName where id = #{0} and name = #{1}  
	
	</select>  

由于是多参数那么就不能使用parameterType， 改用#｛index｝是第几个就用第几个的索引，索引从0开始

### 2.2方案2

	public List<XXXBean> getXXXBeanList(@Param("id")String id, @Param("code")String code);  

	<select id="getXXXBeanList" resultType="XXBean">
	
	　　select t.* from tableName where id = #{id} and name = #{code}  
	
	</select>  

由于是多参数那么就不能使用parameterType， 这里用@Param来指定哪一个

## 3.Map封装多参数

	public List<XXXBean> getXXXBeanList(HashMap map);  

	<select id="getXXXBeanList" parameterType="hashmap" resultType="XXBean">
	
	　　select 字段... from XXX where id=#{xxId} code = #{xxCode}  
	
	</select>  

其中hashmap是mybatis自己配置好的直接使用就行。map中key的名字是那个就在#{}使用那个，map如何封装就不用了我说了吧。 

## 4.List封装in

	public List<XXXBean> getXXXBeanList(List<String> list);  

	<select id="getXXXBeanList" resultType="XXBean">
	　　select 字段... from XXX where id in
	　　<foreach item="item" index="index" collection="list" open="(" separator="," close=")">  
	　　　　#{item}  
	　　</foreach>  
	</select>  

foreach 最后的效果是select 字段... from XXX where id in ('1','2','3','4') 


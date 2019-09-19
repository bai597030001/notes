# spring JdbcTemplate
---

## spring框架一站式框架，

- 1.针对javaee三层，每一层都有解决技术。

- 2.在dao层，使用jdbcTemplate

## spring对不同的持久化层技术都进行了封装

	ORM持久化技术 | 模板类
	---------| -------------
	JDBC 			| org.springframework.jdbc.core.JdbcTemplate
	Hibernate 		| org.springframework.orm.hibernate5.HibernateTemplate
	IBatis(MyBatis) | org.springframework.orm.ibatis.SqlMapClientTemplate
	JPA				| org.springframework.orm.jpa.JpaTemplate

- jdbcTemplate对jdbc进行封装。和dbutils使用很相似，对数据进行crud操作。

## JdbcTemplate中RowMapper及其实现

```java

    @Test
    public void origJdbcTemplateTest() {
        List<User> list = jdbcTemplate.query("SELECT * FROM user;", new RowMapper<User>() {
            public User mapRow(final ResultSet resultSet, int i) throws SQLException {
                return new User() {{
                    setId(resultSet.getInt("id"));
                    setName(resultSet.getString("name"));
                    setAge(resultSet.getInt("age"));
                }};
            }
        });

        System.out.println(list);

        System.out.println("=============================================================");

        list = jdbcTemplate.query("SELECT * FROM user;", new BeanPropertyRowMapper<User>(User.class));

        System.out.println(list);
    }
```

- 其中，可以自定义RowMapper<JavaBean>{...}来实现，也可以使用spring提供的实现

eg: BeanPropertyRowMapper<User>(User.class)



[博客1](https://www.cnblogs.com/wanggd/p/3140506.html)

[博客2](http://blog.csdn.net/dyllove98/article/details/7772463)
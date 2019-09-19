# 1.@Primary

- 自动装配时当出现多个Bean候选者时，被注解为@Primary的Bean将作为首选者，否则将抛出异常 

# 2.@Qualifier

- 合格者，通过这个标示，表明了哪个实现类才是我们所需要的
- 需要注意的是@Qualifier的参数名称必须为我们之前定义的bean的名称之一
- 定义

```java
@Configuration
public class DataBaseConfig {

    @Bean(name = "gbiapDataSource")
    @ConfigurationProperties(prefix = "lj.datasource.gbiap")
    public DataSource gbiapDataSource() {
        return new DruidDataSource();
    }

    @Bean(name = "gbiapCommonConfigDataSource")
    @ConfigurationProperties(prefix = "lj.datasource.gbiap-common-config")
    public DataSource gbiapCommonConfigDataSource() {
        return new DruidDataSource();
    }

    @Bean(name = "gbiapCountResultDataSource")
    @ConfigurationProperties(prefix = "lj.datasource.gbiap-count-result")
    public DataSource gbiapCountResultDataSource() {
        return new DruidDataSource();
    }


    @Bean(name = "gbiapJdbcTemplate")
    public JdbcTemplate gbiapJdbcTemplate(@Qualifier("gbiapDataSource") DataSource ctaDataSource) {
        return new JdbcTemplate(ctaDataSource);
    }

    @Bean(name = "gbiapCommonConfigJdbcTemplate")
    public JdbcTemplate gbiapCommonConfigJdbcTemplate(@Qualifier("gbiapCommonConfigDataSource") DataSource gbiapCommonConfigJdbcTemplate) {
        return new JdbcTemplate(gbiapCommonConfigJdbcTemplate);
    }

    @Bean(name = "gbiapCountResultJdbcTemplate")
    public JdbcTemplate gbiapCountResultJdbcTemplate(@Qualifier("gbiapCountResultDataSource") DataSource gbiapCountResultJdbcTemplate) {
        return new JdbcTemplate(gbiapCountResultJdbcTemplate);
    }
}
```

- 使用

```java
@Repository
public class BureauInfoDao {

    @Autowired
    @Qualifier("gbiapCommonConfigJdbcTemplate")
    //上边两行效果等同于：@Resource(name = "gbiapCommonConfigJdbcTemplate")
    private JdbcTemplate jdbcTemplate;

    @Autowired
    private RowMapper<BureauInfoEntity> rowMapper;

    private static final String TABLE_NAME = "BureauInfoTable";

    private List<BureauInfoEntity> getTableInfo() {
        String sql = "select * from " + TABLE_NAME;
        return jdbcTemplate.query(sql, rowMapper);
    }
}
```



# 3.@Resource

- 默认按名称装配，当找不到与名称匹配的bean才会按类型装配。 

# 4. @Autowired

- 默认按类型装配，如果我们想使用按名称装配，可以结合@Qualifier注解一起使用 

> @Autowired  
> @Qualifier(“personDaoBean”)  
> 存在多个实例配合使用

# 小结
- @Autowired + @Qualifier == @Resource
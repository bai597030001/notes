# SpringCloud Config

Spring Cloud Config项目是一个解决分布式系统的配置管理方案。它包含了Client和Server两个部分，**server提供配置文件的存储、以接口的形式将配置文件的内容提供出去，client通过接口获取数据、并依据此数据初始化自己的应用**。



我们开始在 Spring Boot 项目中集成 Spring Cloud Config，并以 github 作为配置存储。除了 git 外，还可以用数据库、svn、本地文件等作为存储。主要从以下三块来说一下 Config 的使用。

## 1.基础版的配置中心（不集成 Eureka）

### config server

pom.xml

```xml
<properties>
    <java.version>1.8</java.version>
    <spring-cloud.version>Greenwich.SR2</spring-cloud.version>
</properties>

<dependencies>
    <dependency>
        <groupId>org.springframework.cloud</groupId>
        <artifactId>spring-cloud-config-server</artifactId>
    </dependency>
</dependencies>

<dependencyManagement>
    <dependencies>
        <dependency>
            <groupId>org.springframework.cloud</groupId>
            <artifactId>spring-cloud-dependencies</artifactId>
            <version>${spring-cloud.version}</version>
            <type>pom</type>
            <scope>import</scope>
        </dependency>
    </dependencies>
</dependencyManagement>

<repositories>
    <repository>
        <id>spring-milestones</id>
        <name>Spring Milestones</name>
        <url>https://repo.spring.io/milestone</url>
        <snapshots>
            <enabled>false</enabled>
        </snapshots>
    </repository>
    <repository>
        <id>repository.springframework.maven.release</id>
        <name>Spring Framework Maven Release Repository</name>
        <url>http://maven.springframework.org/milestone/</url>
    </repository>
    <repository>
        <id>org.springframework</id>
        <url> http://maven.springframework.org/snapshot</url>
    </repository>
    <repository>
        <id>spring-milestone</id>
        <name>Spring Maven MILESTONE Repository</name>
        <url>http://repo.spring.io/libs-milestone</url>
    </repository>
    <repository>
        <id>spring-release</id>
        <name>Spring Maven RELEASE Repository</name>
        <url>http://repo.spring.io/libs-release</url>
    </repository>
</repositories>
```



application.yaml

```yaml
spring:
  application.name: cloud-config-server
  profiles:
    active: native
  cloud:
    config:
      server:
        # native.search-locations: file:/home/baijd/spring_cloud/cloud_config
        native.search-locations: E:/temp
        # prefix: /config # set the prefix of Config Server Rest API
```



开启注解

```java
@SpringBootApplication
@EnableConfigServer
public class CloudConfigServerApplication {

    public static void main(String[] args) {
        SpringApplication.run(CloudConfigServerApplication.class, args);
    }

}
```



浏览器访问：按照config server访问规则进行访问

```properties
/{application}/{profile}[/{label}]
/{application}-{profile}.yml
/{label}/{application}-{profile}.yml
/{application}-{profile}.properties
/{label}/{application}-{profile}.properties
```

如：

http://localhost:8080/cloud-config-server/application-dev.properties

http://localhost:8080/cloud-config-server-application-dev.properties



### config client

```xml
<properties>
    <java.version>1.8</java.version>
    <spring-cloud.version>Greenwich.SR2</spring-cloud.version>
</properties>

<dependencies>
    <dependency>
        <groupId>org.springframework.boot</groupId>
        <artifactId>spring-boot-starter-web</artifactId>
    </dependency>

    <dependency>
        <groupId>org.springframework.cloud</groupId>
        <artifactId>spring-cloud-starter-config</artifactId>
    </dependency>

    <dependency>
        <groupId>org.springframework.boot</groupId>
        <artifactId>spring-boot-starter-actuator</artifactId>
    </dependency>
</dependencies>

<dependencyManagement>
    <dependencies>
        <dependency>
            <groupId>org.springframework.cloud</groupId>
            <artifactId>spring-cloud-dependencies</artifactId>
            <version>${spring-cloud.version}</version>
            <type>pom</type>
            <scope>import</scope>
        </dependency>
    </dependencies>
</dependencyManagement>

<repositories>
    <repository>
        <id>spring-milestones</id>
        <name>Spring Milestones</name>
        <url>https://repo.spring.io/milestone</url>
        <snapshots>
            <enabled>false</enabled>
        </snapshots>
    </repository>
    <repository>
        <id>repository.springframework.maven.release</id>
        <name>Spring Framework Maven Release Repository</name>
        <url>http://maven.springframework.org/milestone/</url>
    </repository>
    <repository>
        <id>org.springframework</id>
        <url> http://maven.springframework.org/snapshot</url>
    </repository>
    <repository>
        <id>spring-milestone</id>
        <name>Spring Maven MILESTONE Repository</name>
        <url>http://repo.spring.io/libs-milestone</url>
    </repository>
    <repository>
        <id>spring-release</id>
        <name>Spring Maven RELEASE Repository</name>
        <url>http://repo.spring.io/libs-release</url>
    </repository>
</repositories>
```



`bootstrap.yaml`

```yaml
spring:
  application.name: cloud-config-client
  cloud:
    config:
      uri: http://localhost:8080/
      fail-fast: true
      enabled: true # 开启配置
      profile: dev  #版本
```

```yaml
server:
  port: 10001
```



```java
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.beans.factory.annotation.Value;
import org.springframework.boot.SpringApplication;
import org.springframework.boot.autoconfigure.SpringBootApplication;
import org.springframework.core.env.Environment;
import org.springframework.web.bind.annotation.GetMapping;
import org.springframework.web.bind.annotation.RestController;

@RestController
@SpringBootApplication
public class CloudConfigClientApplication {

    @Value("${test.word}")
    private String configData;

    @Autowired
    private Environment environment;

    @GetMapping(value = "show")
    public String getConfigData(){
        return configData;
        //return environment.getProperty("test.world");
    }
    
    public static void main(String[] args) {
        SpringApplication.run(CloudConfigClientApplication.class, args);
    }

}
```



`E:/temp/cloud-config-client-dev.properties`

```properties
test.word: cloud config client, hello world
```



浏览器访问：http://localhost:10001/show



### 注意点

1.注意要将调用配置服务的信息，配置到`bootstrap.yml`中，不然，只会跑到默认的配置服务http://localhost:8888

2.<font color=#dd0000>配置文件的名称：客户端应用名 + 版本。如：`cloud-config-client-dev.properties`</font>





## 2.结合 Eureka 版的配置中心

按照上边的方式，SpringCloudConfigClient只能调用固定的一个Server，一旦Server出现异常，正式调用将会中断。
解决方法是：写多个Server注册到Eureka-Server上，然后配置相同Service-Name，然后利用负载均衡处理。

### eureka server

```xml
<properties>
    <java.version>1.8</java.version>
    <java.version>1.8</java.version>
    <project.build.sourceEncoding>UTF-8</project.build.sourceEncoding>
    <project.reporting.outputEncoding>UTF-8</project.reporting.outputEncoding>
    <spring-cloud.version>Greenwich.SR2</spring-cloud.version>
</properties>

<dependencies>

    <dependency>
        <groupId>org.springframework.boot</groupId>
        <artifactId>spring-boot-starter-web</artifactId>
    </dependency>

    <dependency>
        <groupId>de.codecentric</groupId>
        <artifactId>spring-boot-admin-starter-server</artifactId>
        <version>2.1.5</version>
    </dependency>
    <dependency>
        <groupId>de.codecentric</groupId>
        <artifactId>spring-boot-admin-dependencies</artifactId>
        <version>2.1.5</version>
        <type>pom</type>
        <scope>import</scope>
    </dependency>

    <dependency>
        <groupId>org.springframework.cloud</groupId>
        <artifactId>spring-cloud-starter-netflix-eureka-server</artifactId>
    </dependency>

</dependencies>

<dependencyManagement>
    <dependencies>

        <dependency>
            <groupId>de.codecentric</groupId>
            <artifactId>spring-boot-admin-dependencies</artifactId>
            <version>2.1.5</version>
        </dependency>

        <dependency>
            <groupId>org.springframework.cloud</groupId>
            <artifactId>spring-cloud-dependencies</artifactId>
            <version>${spring-cloud.version}</version>
            <type>pom</type>
            <scope>import</scope>
        </dependency>

    </dependencies>
</dependencyManagement>
```



```yaml
spring:
  application:
    name: eureka-server
server:
  port: 8761
eureka:
  instance:
    hostname: localhost
    # 在多机器独立部署的情况下是没有问题的，配置 prefer-ip-address 为 ture，
    # 代表发现服务时候优先按照 IP 去搜寻，对于多集群而言，可以保证尽快准确搜索到服务。
    # 而对于单机部署来说，IP 地址都是相同的，这会导致其余注册中心出现在
    # unavailable-replicas (不可用副本) 中。所以单机部署时候不建议开启这个
    # 参数（默认值为 false），多机部署时候可以开启。
    prefer-ip-address: false
    lease-renewal-interval-in-seconds: 5 # heartbeat interval
    lease-expiration-duration-in-seconds: 15 # offline timeout
  client:
    service-url:
      defaultZone: http://${eureka.instance.hostname}:${server.port}/eureka/
    register-with-eureka: true # 设置为 false,代表不向注册中心注册自己
    fetch-registry: false #false表示自己端就是注册中心，我的职责就是维护服务实例，并不需要去检索服务
management:
  endpoints:
    web:
      exposure:
        include: "*"
  endpoint:
    health:
      show-details: ALWAYS
```



```java
@SpringBootApplication
@EnableEurekaServer
public class CloudEurekaServerApplication {

    public static void main(String[] args) {
        SpringApplication.run(CloudEurekaServerApplication.class, args);
    }

}
```





### config server

```xml
<properties>
    <java.version>1.8</java.version>
    <spring-cloud.version>Greenwich.SR2</spring-cloud.version>
</properties>

<dependencies>
    <dependency>
        <groupId>org.springframework.cloud</groupId>
        <artifactId>spring-cloud-config-server</artifactId>
    </dependency>

    <dependency>
        <groupId>org.springframework.cloud</groupId>
        <artifactId>spring-cloud-starter-netflix-eureka-client</artifactId>
    </dependency>

    <dependency>
        <groupId>org.springframework.boot</groupId>
        <artifactId>spring-boot-starter-actuator</artifactId>
    </dependency>

</dependencies>

<dependencyManagement>
    <dependencies>
        <dependency>
            <groupId>org.springframework.cloud</groupId>
            <artifactId>spring-cloud-dependencies</artifactId>
            <version>${spring-cloud.version}</version>
            <type>pom</type>
            <scope>import</scope>
        </dependency>
    </dependencies>
</dependencyManagement>

<repositories>
    <repository>
        <id>spring-milestones</id>
        <name>Spring Milestones</name>
        <url>https://repo.spring.io/milestone</url>
        <snapshots>
            <enabled>false</enabled>
        </snapshots>
    </repository>
    <repository>
        <id>repository.springframework.maven.release</id>
        <name>Spring Framework Maven Release Repository</name>
        <url>http://maven.springframework.org/milestone/</url>
    </repository>
    <repository>
        <id>org.springframework</id>
        <url> http://maven.springframework.org/snapshot</url>
    </repository>
    <repository>
        <id>spring-milestone</id>
        <name>Spring Maven MILESTONE Repository</name>
        <url>http://repo.spring.io/libs-milestone</url>
    </repository>
    <repository>
        <id>spring-release</id>
        <name>Spring Maven RELEASE Repository</name>
        <url>http://repo.spring.io/libs-release</url>
    </repository>
</repositories>
```



```yaml
spring:
  application.name: cloud-config-server
  profiles:
    #设置为本地启动的方式，而不是通过git
    active: native
  cloud:
    config:
      server:
        # native.search-locations: file:/home/baijd/spring_cloud/cloud_config
        # 配置文件所在目录，classpath（类路径）和（系统文件路径） file两种
        native.search-locations: file:E:/temp
        # prefix: /config # set the prefix of Config Server Rest API

eureka:
  instance:
    leaseRenewalIntervalInSeconds: 10 #表示eureka client发送心跳给server端的频率
    health-check-url-path: /actuator/health #健康检查页面的URL，相对路径，默认使用 HTTP 访问，如果需要使用 HTTPS则需要使用绝对路径配置

  client:
    registryFetchIntervalSeconds: 5 #表示eureka client间隔多久去拉取服务注册信息，默认为30秒
    service-url:
      defaultZone: ${EUREKA_SERVICE_URL:http://localhost:8761}/eureka/ #服务注册中心地址
```



```java
@SpringBootApplication
@EnableConfigServer
public class CloudConfigServerApplication {

    public static void main(String[] args) {
        SpringApplication.run(CloudConfigServerApplication.class, args);
    }

}
```





### config client

```xml
<properties>
    <java.version>1.8</java.version>
    <spring-cloud.version>Greenwich.SR2</spring-cloud.version>
</properties>

<dependencies>
    <dependency>
        <groupId>org.springframework.boot</groupId>
        <artifactId>spring-boot-starter-web</artifactId>
    </dependency>

    <dependency>
        <groupId>org.springframework.cloud</groupId>
        <artifactId>spring-cloud-starter-config</artifactId>
    </dependency>

    <dependency>
        <groupId>org.springframework.cloud</groupId>
        <artifactId>spring-cloud-starter-netflix-eureka-client</artifactId>
    </dependency>

    <dependency>
        <groupId>org.springframework.boot</groupId>
        <artifactId>spring-boot-starter-actuator</artifactId>
    </dependency>
</dependencies>

<dependencyManagement>
    <dependencies>
        <dependency>
            <groupId>org.springframework.cloud</groupId>
            <artifactId>spring-cloud-dependencies</artifactId>
            <version>${spring-cloud.version}</version>
            <type>pom</type>
            <scope>import</scope>
        </dependency>
    </dependencies>
</dependencyManagement>

<repositories>
    <repository>
        <id>spring-milestones</id>
        <name>Spring Milestones</name>
        <url>https://repo.spring.io/milestone</url>
        <snapshots>
            <enabled>false</enabled>
        </snapshots>
    </repository>
    <repository>
        <id>repository.springframework.maven.release</id>
        <name>Spring Framework Maven Release Repository</name>
        <url>http://maven.springframework.org/milestone/</url>
    </repository>
    <repository>
        <id>org.springframework</id>
        <url> http://maven.springframework.org/snapshot</url>
    </repository>
    <repository>
        <id>spring-milestone</id>
        <name>Spring Maven MILESTONE Repository</name>
        <url>http://repo.spring.io/libs-milestone</url>
    </repository>
    <repository>
        <id>spring-release</id>
        <name>Spring Maven RELEASE Repository</name>
        <url>http://repo.spring.io/libs-release</url>
    </repository>
</repositories>
```



`bootstrap.yaml`

```yaml
spring:
  application.name: cloud-config-client
  cloud:
    config:
      uri: http://localhost:8080/
      fail-fast: true
      enabled: true # 开启配置
      profile: dev  #版本
      label: ""     #git配置的分支信息，master类似的

eureka:
  instance:
    leaseRenewalIntervalInSeconds: 10 #表示eureka client发送心跳给server端的频率
    health-check-url-path: /actuator/health #健康检查页面的URL，相对路径，默认使用 HTTP 访问，如果需要使用 HTTPS则需要使用绝对路径配置

  client:
    registryFetchIntervalSeconds: 5 #表示eureka client间隔多久去拉取服务注册信息，默认为30秒
    service-url:
      defaultZone: ${EUREKA_SERVICE_URL:http://localhost:8761}/eureka/ #服务注册中心地址
```

```yaml
server:
  port: 10001
```



```java
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.beans.factory.annotation.Value;
import org.springframework.boot.SpringApplication;
import org.springframework.boot.autoconfigure.SpringBootApplication;
import org.springframework.core.env.Environment;
import org.springframework.web.bind.annotation.GetMapping;
import org.springframework.web.bind.annotation.RestController;

@RestController
@SpringBootApplication
public class CloudConfigClientApplication {

    @Value("${test.word}")
    private String configData;

    @Autowired
    private Environment environment;

    @GetMapping(value = "show")
    public String getConfigData(){
        return configData;
        //return environment.getProperty("test.world");
    }
    
    public static void main(String[] args) {
        SpringApplication.run(CloudConfigClientApplication.class, args);
    }

}
```



`E:/temp/cloud-config-client-dev.properties`

```properties
test.word: cloud config client, hello world
```



浏览器访问：http://localhost:10001/showWord



### 注意点

1.注意要将调用配置服务的信息，配置到`bootstrap.yml`中，不然，只会跑到默认的配置服务http://localhost:8888

2.<font color=#dd0000>配置文件的名称：客户端应用名 + 版本。如：`cloud-config-client-dev.properties`</font>



### 手动刷新

当修给了配置中心的配置文件后，是不能自动刷新的。根据官网，需要以post请求访问`http://localhost:9001/actuator/bus-refresh`

```http
POST http://localhost:9001/actuator/bus-refresh
Accept: */*
Cache-Control: no-cache
```

然后再请求配置，发现配置已经更新。



`/actuator/bus-env`，



## 3.实现配置的自动刷新

上述手动刷新Post请求，虽然可以不重启服务就更新配置，但是仍然是不可取的。

我们可以利用git的webhooks来达到自动更新配置。如果使用的是本地文件方式（没有使用git），则可以监测配置中心的目录，当有配置变更后，通过Httpclient或okhttp发送post请求到`http://localhost:9001/actuator/bus-refresh`，这样可以实现自动刷新配置的功能。



实现步骤：

```properties
1.提交代码/配置文件被修改，触发post请求给bus-refresh

2.server端接收到请求并发送给Spring Cloud Bus

3.Spring Cloud bus接到消息并通知给其它客户端

4.其它客户端接收到通知，请求Server端获取最新配置

至此，全部客户端均获取到最新的配置
```





### java文件监听

可以使用java7提供的WatchService，也可以使用commons-io包提供的。



```java
package com.example.cloudconfigclient.WatchFile;

import java.io.IOException;
import java.nio.file.*;

/**
 * 监听文件变化
 *
 * 要实现监听文件的变更然后马上做出响应，在Java7之前，除了不断的循环似乎没什么好办法，
 * 而且实现也很麻烦。Java7推出了一个WatchService来解决这个问题。
 *
 * WatchKey:
 *      此类代表着一个Object,(文件或者文件夹)注册到WatchService后，返回一个WatchKey类。
 *      反应当前所注册的Object状态的类。此类封装了不同事件的状态值，比如，当文件(文件夹)被修改或者被删除或者创建的时候，此类首先产生一个信号量,等待消费者来取并且该WatchKey将会进入到WatchService的队列中。如果WatchKey已经进入到队列当中，但是又有了变化，将不会再次进入到队列当中。
 *
 *      Java7WatchService.take()方法可以取到队列的WatchKey.
 *
 * WatchEvent:
 *      此类代表文件的一个具体的文件变更事件
 *
 * 原理:
 *      把一个Path注册到WatchService,并告诉它你感兴趣的变化事件，如果WatchService检测到变更，
 *      会以事件的形式发送到一个队列，你只需不断地从WatchService中等待事件的到来并确认是自己
 *      关注的事件，然后做相应的处理，最后需要reset表示已经处理了这个事件。
 *
 * 存在的问题:
 *      （1）当文件修改时，会被调用两次，即输出两个相同的修改。
 *      （2）不能对其子文件夹进行监控，只能提示目录被修改。
 *      （3）无法对文件类型进行过滤。
 */
public class Java7WatchService {

    public static void main(String[] a) {

        final Path path = Paths.get("E:/temp/cloud-config-client-dev.properties");

        try (java.nio.file.WatchService watchService = FileSystems.getDefault().newWatchService()) {
            //给path路径加上文件观察服务
            path.register(watchService, StandardWatchEventKinds.ENTRY_CREATE,StandardWatchEventKinds.ENTRY_MODIFY,StandardWatchEventKinds.ENTRY_DELETE);
            // start an infinite loop
            while (true) {
                final WatchKey key = watchService.take();

                for (WatchEvent<?> watchEvent : key.pollEvents()) {

                    final WatchEvent.Kind<?> kind = watchEvent.kind();

                    if (kind == StandardWatchEventKinds.OVERFLOW) {
                        continue;
                    }
                    //创建事件
                    if (kind == StandardWatchEventKinds.ENTRY_CREATE) {
                        System.out.println("create file event");
                    }
                    //修改事件
                    if (kind == StandardWatchEventKinds.ENTRY_MODIFY) {
                        System.out.println("modify file event");
                    }
                    //删除事件
                    if (kind == StandardWatchEventKinds.ENTRY_DELETE) {
                        System.out.println("delete file event");
                    }
                    // get the filename for the event
                    final WatchEvent<Path> watchEventPath = (WatchEvent<Path>) watchEvent;
                    final Path filename = watchEventPath.context();
                    // print it out
                    System.out.println(kind + " -> " + filename);

                }
                // reset the keyf
                boolean valid = key.reset();
                // exit loop if the key is not valid (if the directory was
                // deleted,for
                if (!valid) {
                    break;
                }
            }

        } catch (IOException | InterruptedException ex) {
            System.err.println(ex);
        }
    }
}
```



```xml
<!-- https://mvnrepository.com/artifact/commons-io/commons-io -->
<dependency>
    <groupId>commons-io</groupId>
    <artifactId>commons-io</artifactId>
    <version>2.6</version>
</dependency>
```



```java
package com.example.cloudconfigclient.WatchFile;

import org.apache.commons.io.filefilter.FileFilterUtils;
import org.apache.commons.io.filefilter.HiddenFileFilter;
import org.apache.commons.io.filefilter.IOFileFilter;
import org.apache.commons.io.monitor.FileAlterationListenerAdaptor;
import org.apache.commons.io.monitor.FileAlterationMonitor;
import org.apache.commons.io.monitor.FileAlterationObserver;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import java.io.File;
import java.util.concurrent.TimeUnit;

/**
 * 文件变化监听器
 *      在Apache的Commons-IO中有关于文件的监控功能的代码. 文件监控的原理如下：
 *      由文件监控类FileAlterationMonitor中的线程不停的扫描文件观察器FileAlterationObserver，
 *      如果有文件的变化，则根据相关的文件比较器，判断文件时新增，还是删除，还是更改。（默认为1000毫秒执行一次扫描）
 */
public class FileListener extends FileAlterationListenerAdaptor {

    private Logger log = LoggerFactory.getLogger(FileListener.class);
    /**
     * 文件创建执行
     */
    @Override
    public void onFileCreate(File file) {
        log.info("[新建]:" + file.getAbsolutePath());
    }
    /**
     * 文件创建修改
     */
    @Override
    public void onFileChange(File file) {
        log.info("[修改]:" + file.getAbsolutePath());
    }
    /**
     * 文件删除
     */
    @Override
    public void onFileDelete(File file) {
        log.info("[删除]:" + file.getAbsolutePath());
    }
    /**
     * 目录创建
     */
    @Override
    public void onDirectoryCreate(File directory) {
        log.info("[新建]:" + directory.getAbsolutePath());
    }
    /**
     * 目录修改
     */
    @Override
    public void onDirectoryChange(File directory) {
        log.info("[修改]:" + directory.getAbsolutePath());
    }
    /**
     * 目录删除
     */
    @Override
    public void onDirectoryDelete(File directory) {
        log.info("[删除]:" + directory.getAbsolutePath());
    }

    @Override
    public void onStart(FileAlterationObserver observer) {
        // TODO Auto-generated method stub
        super.onStart(observer);
    }

    @Override
    public void onStop(FileAlterationObserver observer) {
        // TODO Auto-generated method stub
        super.onStop(observer);
    }

    
    
    public static void main(String[] args) throws Exception {
        // 监控目录
        String rootDir = "E:\\temp";
        // 轮询间隔 5 秒
        long interval = TimeUnit.SECONDS.toMillis(1);
        // 创建过滤器
        IOFileFilter directories = FileFilterUtils.and(FileFilterUtils.directoryFileFilter(), HiddenFileFilter.VISIBLE);
        IOFileFilter files    = FileFilterUtils.and(FileFilterUtils.fileFileFilter(), FileFilterUtils.suffixFileFilter(".properties"));
        IOFileFilter filter = FileFilterUtils.or(directories, files);
        // 使用过滤器
        FileAlterationObserver observer = new FileAlterationObserver(new File(rootDir), filter);
        //不使用过滤器
        //FileAlterationObserver observer = new FileAlterationObserver(new File(rootDir));
        observer.addListener(new FileListener());
        //创建文件变化监听器
        FileAlterationMonitor monitor = new FileAlterationMonitor(interval, observer);
        // 开始监控
        monitor.start();
    }
}
```



### spring文件监听

结合commons-io，将bean注册给spring容器

```xml
<dependency>
    <groupId>commons-io</groupId>
    <artifactId>commons-io</artifactId>
    <version>2.6</version>
</dependency>
```





1.创建上述FileListener





2.添加监听器工厂，交给spring容器

```java
@Component
public class FileListenerFactory {

    // 设置监听路径
    private final String monitorDir = "E:/temp";

    // 设置轮询间隔
    private final long interval = TimeUnit.SECONDS.toMillis(1);

    public FileAlterationMonitor getMonitor() {
        // 创建过滤器
        IOFileFilter directories = FileFilterUtils.and(
                FileFilterUtils.directoryFileFilter(),
                HiddenFileFilter.VISIBLE);
        IOFileFilter files = FileFilterUtils.and(
                FileFilterUtils.fileFileFilter(),
                FileFilterUtils.suffixFileFilter(".properties"));
        IOFileFilter filter = FileFilterUtils.or(directories, files);

        // 装配过滤器
        // FileAlterationObserver observer = new FileAlterationObserver(new File(monitorDir));
        FileAlterationObserver observer = new FileAlterationObserver(new File(monitorDir), filter);

        // 向监听者添加监听器，并注入业务服务
        observer.addListener(new FileListener(listenerService));

        // 返回监听者
        return new FileAlterationMonitor(interval, observer);
    }
}
```



3.运行

> 这里以SpringBoot自启动任务为例，在应用启动后开始监听目标文件夹。由于监听器在独立的线程中执行，一旦异常发生将导致线程退出，所以如果希望监听线程不中断，应在线程中捕获所有异常。

```java
@Component
public class FileListenerRunner implements CommandLineRunner {

    @Autowired
    private FileListenerFactory fileListenerFactory;

    @Override
    public void run(String... args) throws Exception {
        // 创建监听者
        FileAlterationMonitor fileAlterationMonitor = fileListenerFactory.getMonitor();
        try {
            // do not stop this thread
            fileAlterationMonitor.start();
        } catch (Exception e) {
            e.printStackTrace();
        }
    }
}
```





## config server访问规则

Spring Cloud Config 有它的一套访问规则，我们通过这套规则在浏览器上直接访问就可以。

```properties
/{application}/{profile}[/{label}]
/{application}-{profile}.yml
/{label}/{application}-{profile}.yml
/{application}-{profile}.properties
/{label}/{application}-{profile}.properties
```

> {application} 就是应用名称，对应到配置文件上来，就是配置文件的名称部分，例如我上面创建的配置文件。
>
> {profile} 就是配置文件的版本，我们的项目有开发版本、测试环境版本、生产环境版本，对应到配置文件上来就是以 application-{profile}.yml 加以区分，例如application-dev.yml、application-sit.yml、application-prod.yml。
>
> {label} 表示 git 分支，默认是 master 分支，如果项目是以分支做区分也是可以的，那就可以通过不同的 label 来控制访问不同的配置文件了。


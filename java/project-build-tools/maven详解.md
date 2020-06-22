# settings.xml

https://www.cnblogs.com/yichenscc/p/10367481.html



## server

> 仓库的下载和部署是在`pom.xml`文件中的`repositories`和`distributionManagement`元素中定义的。然而，一般类似用户名、密码（**有些仓库访问是需要安全认证的**）等信息不应该在`pom.xml`文件中配置，这些信息可以配置在`settings.xml`中。

```xml
<servers>
    <server>
        <!--server的id（注意不是用户登陆的id），该id与distributionManagement中repository元素的id相匹配。 -->
        <id>server001</id>
        <!--鉴权用户名。鉴权用户名和鉴权密码表示服务器认证所需要的登录名和密码。 -->
        <username>my_login</username>
        <!--鉴权密码 。鉴权用户名和鉴权密码表示服务器认证所需要的登录名和密码。密码加密功能已被添加到2.1.0 +。详情请访问密码加密页面 -->
        <password>my_password</password>
        <!--鉴权时使用的私钥位置。和前两个元素类似，私钥位置和私钥密码指定了一个私钥的路径（默认是${user.home}/.ssh/id_dsa）以及如果需要的话，一个密语。将来passphrase和password元素可能会被提取到外部，但目前它们必须在settings.xml文件以纯文本的形式声明。 -->
        <privateKey>${usr.home}/.ssh/id_dsa</privateKey>
        <!--鉴权时使用的私钥密码。 -->
        <passphrase>some_passphrase</passphrase>
        <!--文件被创建时的权限。如果在部署的时候会创建一个仓库文件或者目录，这时候就可以使用权限（permission）。这两个元素合法的值是一个三位数字，其对应了unix文件系统的权限，如664，或者775。 -->
        <filePermissions>664</filePermissions>
        <!--目录被创建时的权限。 -->
        <directoryPermissions>775</directoryPermissions>
    </server>
</servers>
```



## mirror

> 为仓库列表配置的下载镜像列表

```xml
<mirrors>
    <!-- 给定仓库的下载镜像。 -->
    <mirror>
        <!-- 该镜像的唯一标识符。id用来区分不同的mirror元素。 -->
        <id>planetmirror.com</id>
        <!-- 镜像名称 -->
        <name>PlanetMirror Australia</name>
        <!-- 该镜像的URL。构建系统会优先考虑使用该URL，而非使用默认的服务器URL。 -->
        <url>http://downloads.planetmirror.com/pub/maven2</url>
        <!-- 被镜像的服务器的id。例如，如果我们要设置了一个Maven中央仓库（http://repo.maven.apache.org/maven2/）的镜像，就需要将该元素设置成central。这必须和中央仓库的id central完全一致。 -->
        <mirrorOf>central</mirrorOf>
    </mirror>
</mirrors>
```



## profiles

根据环境参数来调整构建配置的列表

> `settings.xml`中的`profile`元素是`pom.xml`中`profile`元素的**裁剪版本**。
>
> 它包含了`id`、`activation`、`repositories`、`pluginRepositories`和 `properties`元素。
>
> 这里的`profile`元素只包含这五个子元素是因为这里只关心构建系统这个整体（这正是`settings.xml`文件的角色定位），而非单独的项目对象模型设置。
>
> 如果一个`settings.xml`中的`profile`被激活，它的值会覆盖任何其它定义在`pom.xml`中带有相同`id`的`profile`。

```xml
<profiles>
    <profile>
        <!-- profile的唯一标识 -->
        <id>test</id>
        <!-- 自动触发profile的条件逻辑 -->
        <activation />
        <!-- 扩展属性列表 -->
        <properties />
        <!-- 远程仓库列表 -->
        <repositories />
        <!-- 插件仓库列表 -->
        <pluginRepositories />
    </profile>
</profiles>
```



### activation

自动触发`profile`的条件逻辑。

> 如`pom.xml`中的`profile`一样，`profile`的作用在于它能够在某些特定的环境中自动使用某些特定的值；这些环境通过`activation`元素指定。
>
> `activation`元素并不是激活`profile`的唯一方式。`settings.xml`文件中的`activeProfile`元素可以包含`profile`的`id`。
>
> `profile`也可以通过在命令行，使用-P标记和逗号分隔的列表来显式的激活（如，-P test）。

```xml
<activation>
    <!--profile默认是否激活的标识 -->
    <activeByDefault>false</activeByDefault>
    <!--当匹配的jdk被检测到，profile被激活。例如，1.4激活JDK1.4，1.4.0_2，而!1.4激活所有版本不是以1.4开头的JDK。 -->
    <jdk>1.5</jdk>
    <!--当匹配的操作系统属性被检测到，profile被激活。os元素可以定义一些操作系统相关的属性。 -->
    <os>
        <!--激活profile的操作系统的名字 -->
        <name>Windows XP</name>
        <!--激活profile的操作系统所属家族(如 'windows') -->
        <family>Windows</family>
        <!--激活profile的操作系统体系结构 -->
        <arch>x86</arch>
        <!--激活profile的操作系统版本 -->
        <version>5.1.2600</version>
    </os>
    <!--如果Maven检测到某一个属性（其值可以在POM中通过${name}引用），其拥有对应的name = 值，Profile就会被激活。如果值字段是空的，那么存在属性名称字段就会激活profile，否则按区分大小写方式匹配属性值字段 -->
    <property>
        <!--激活profile的属性的名称 -->
        <name>mavenVersion</name>
        <!--激活profile的属性的值 -->
        <value>2.0.3</value>
    </property>
    <!--提供一个文件名，通过检测该文件的存在或不存在来激活profile。missing检查文件是否存在，如果不存在则激活profile。另一方面，exists则会检查文件是否存在，如果存在则激活profile。 -->
    <file>
        <!--如果指定的文件存在，则激活profile。 -->
        <exists>${basedir}/file2.properties</exists>
        <!--如果指定的文件不存在，则激活profile。 -->
        <missing>${basedir}/file1.properties</missing>
    </file>
</activation>
```



***注：***

> 在maven工程的`pom.xml`所在目录下执行`mvn help:active-profiles`命令可以查看中央仓储的profile是否在工程中生效。



### properties

用来存放一些值。这些值可以在`pom.xml`中的任何地方使用标记`${X}`来使用，这里X是指属性的名称。属性有五种不同的形式，并且都能在`settings.xml`文件中访问。

```xml
<!-- 
  1. env.X: 在一个变量前加上"env."的前缀，会返回一个shell环境变量。例如,"env.PATH"指代了$path环境变量（在Windows上是%PATH%）。 
  2. project.x：指代了POM中对应的元素值。例如: <project><version>1.0</version></project>通过${project.version}获得version的值。 
  3. settings.x: 指代了settings.xml中对应元素的值。例如：<settings><offline>false</offline></settings>通过 ${settings.offline}获得offline的值。 
  4. Java System Properties: 所有可通过java.lang.System.getProperties()访问的属性都能在POM中使用该形式访问，例如 ${java.home}。 
  5. x: 在<properties/>元素中，或者外部文件中设置，以${someVar}的形式使用。
 -->
<properties>
    <user.install>${user.home}/our-project</user.install>
</properties>
```

***注：***

> 如果该profile被激活，则可以在`pom.xml`中使用`${user.install}`。



### repositories

远程仓库列表，它是maven用来填充构建系统本地仓库所使用的一组远程仓库。

```xml
<repositories>
    <!--包含需要连接到远程仓库的信息 -->
    <repository>
        <!--远程仓库唯一标识 -->
        <id>codehausSnapshots</id>
        <!--远程仓库名称 -->
        <name>Codehaus Snapshots</name>
        <!--如何处理远程仓库里发布版本的下载 -->
        <releases>
            <!--true或者false表示该仓库是否为下载某种类型构件（发布版，快照版）开启。 -->
            <enabled>false</enabled>
            <!--该元素指定更新发生的频率。Maven会比较本地POM和远程POM的时间戳。这里的选项是：always（一直），daily（默认，每日），interval：X（这里X是以分钟为单位的时间间隔），或者never（从不）。 -->
            <updatePolicy>always</updatePolicy>
            <!--当Maven验证构件校验文件失败时该怎么做-ignore（忽略），fail（失败），或者warn（警告）。 -->
            <checksumPolicy>warn</checksumPolicy>
        </releases>
        <!--如何处理远程仓库里快照版本的下载。有了releases和snapshots这两组配置，POM就可以在每个单独的仓库中，为每种类型的构件采取不同的策略。例如，可能有人会决定只为开发目的开启对快照版本下载的支持。参见repositories/repository/releases元素 -->
        <snapshots>
            <enabled />
            <updatePolicy />
            <checksumPolicy />
        </snapshots>
        <!--远程仓库URL，按protocol://hostname/path形式 -->
        <url>http://snapshots.maven.codehaus.org/maven2</url>
        <!--用于定位和排序构件的仓库布局类型-可以是default（默认）或者legacy（遗留）。Maven 2为其仓库提供了一个默认的布局；然而，Maven 1.x有一种不同的布局。我们可以使用该元素指定布局是default（默认）还是legacy（遗留）。 -->
        <layout>default</layout>
    </repository>
</repositories>
```



### pluginRepositories

发现插件的远程仓库列表。

> 和`repository`类似，只是`repository`是管理jar包依赖的仓库，`pluginRepositories`则是管理插件的仓库。
>
> maven插件是一种特殊类型的构件。由于这个原因，插件仓库独立于其它仓库。
>
> `pluginRepositories`元素的结构和`repositories`元素的结构类似。每个`pluginRepository`元素指定一个Maven可以用来寻找新插件的远程地址。



## activeProfiles

手动激活profiles的列表，按照`profile`被应用的顺序定义`activeProfile`。

> 该元素包含了一组`activeProfile`元素，每个`activeProfile`都含有一个profile id。任何在`activeProfile`中定义的profile id，不论环境设置如何，其对应的 `profile`都会被激活。如果没有匹配的`profile`，则什么都不会发生。

> 例如，env-test是一个activeProfile，则在pom.xml（或者profile.xml）中对应id的profile会被激活。如果运行过程中找不到这样一个profile，Maven则会像往常一样运行。

```xml
<activeProfiles>
    <!-- 要激活的profile id -->
    <activeProfile>env-test</activeProfile>
</activeProfiles>
```



# pom.xml

## scm

```xml
<!-- SCM(Source Control Management)标签允许你配置你的代码库，供Maven web站点和其它插件使用。 --> 
<scm> 
    <!-- SCM的URL,该URL描述了版本库和如何连接到版本库。--> 
    <connection> 
        scm:svn:http://svn.baidu.com/banseon/maven/abs
    </connection> 

    <!-- 给开发者使用的，类似connection元素。即该连接不仅仅只读 --> 
    <developerConnection> 
        scm:svn:http://svn.baidu.com/baijd/maven/baijd/dao
    </developerConnection> 

    <!-- 当前代码的标签，在开发阶段默认为HEAD --> 
    <tag></tag> 

    <!-- 指向项目的可浏览SCM库（例如ViewVC或者Fisheye）的URL。 --> 
    <url> http://svn.baidu.com/baijd </url> 
</scm> 
```

## profiles

> 在开发过程中，我们的软件会面对**不同的运行环境**，比如开发环境、测试环境、生产环境，而我们的软件在不同的环境中，有的配置可能会不一样，比如数据源配置、日志文件配置、以及一些软件运行过程中的基本配置，那每次我们将软件部署到不同的环境时，都需要修改相应的配置文件，这样来回修改，很容易出错，而且浪费劳动力。

maven提供了一种方便的解决这种问题的方案，就是profile功能。

```xml
<profiles>
    <profile>
        <!-- 本地开发环境 -->
        <id>dev</id>
        <properties>
            <profiles.active>dev</profiles.active>
        </properties>
        <activation>
            <!-- 设置默认激活这个配置 -->
            <activeByDefault>true</activeByDefault>
        </activation>
    </profile>
    <profile>
        <!-- 发布环境 -->
        <id>release</id>
        <properties>
            <profiles.active>release</profiles.active>
        </properties>
    </profile>
    <profile>
        <!-- 测试环境 -->
        <id>beta</id>
        <properties>
            <profiles.active>beta</profiles.active>
        </properties>
    </profile>
</profiles>
```

这里定义了三个环境，分别是dev（开发环境）、beta（测试环境）、release（发布环境），其中开发环境是默认激活的（activeByDefault为true），这样如果在不指定profile时默认是开发环境，也在package的时候显示指定你要选择哪个开发环境

// TODO

未完待续



## repositories



## dependencyManagement

- 管理依赖项版本

在maven多模块项目中，为了项目的正确运行，必须让所有的子项目使用依赖项的统一版本，必须确保应用的各个项目的依赖项和版本一致，才能保证测试的和发布的是相同的结果。

在我们项目顶层的POM文件中，我们会看到dependencyManagement元素。通过它元素来管理jar包的版本，让子项目中引用一个依赖而不用显示的列出版本号。Maven会沿着父子层次向上走，直到找到一个拥有dependencyManagement元素的项目，然后它就会使用在这个dependencyManagement元素中指定的版本号。



**这样做的好处：**

> 统一管理项目的版本号，确保应用的各个项目的依赖和版本一致，才能保证测试的和发布的是相同的成果，因此，在顶层pom中定义共同的依赖关系。同时可以避免在每个使用的子项目中都声明一个版本号，这样想升级或者切换到另一个版本时，只需要在父类容器里更新，不需要任何一个子项目的修改；如果某个子项目需要另外一个版本号时，只需要在dependencies中声明一个版本号即可。子类就会使用子类声明的版本号，不继承于父类版本号。



**与dependencies区别**

> dependencies即使在子项目中不写该依赖项，那么子项目仍然会从父项目中继承该依赖项（全部继承）
>
> dependencyManagement里只是声明依赖，并不实现引入，因此子项目需要显示的声明需要用的依赖。如果不在子项目中声明依赖，是不会从父项目中继承下来的；只有在子项目中写了该依赖项，并且没有指定具体版本，才会从父项目中继承该项，并且version和scope都读取自父pom;另外如果子项目中指定了版本号，那么会使用子项目中指定的jar版本。





## distributionManagement

我们知道，maven的依赖管理是基于版本管理的，对于发布状态的artifact，如果版本号相同，即使我们内部的镜像服务器上的组件比本地新，maven也不会主动下载的。如果我们在开发阶段都是基于正式发布版本来做依赖管理，那么遇到这个问题，就需要升级组件的版本号，可这样就明显不符合要求和实际情况了。

但是，如果是基于快照版本，那么问题就自热而然的解决了

> maven中的仓库分为两种，snapshot快照仓库和release发布仓库。
>
> snapshot快照仓库用于保存开发过程中的不稳定版本，release正式仓库则是用来保存稳定的发行版本。
>
> 定义一个组件/模块为快照版本，只需要在`pom`文件中在该模块的版本号后加上`-SNAPSHOT`即可(注意这里必须是大写)



`maven`会根据模块的版本号(`pom`文件中的`version`)中是否带有`-SNAPSHOT`来判断是快照版本还是正式版本。

如果是快照版本，那么在`mvn deploy`时会自动发布到快照版本库中，而使用快照版本的模块，在不更改版本号的情况下，直接编译打包时，`maven`会自动从镜像服务器上下载最新的快照版本。

如果是正式发布版本，那么在`mvn deploy`时会自动发布到正式版本库中，而使用正式版本的模块，在不更改版本号的情况下，编译打包时如果本地已经存在该版本的模块则不会主动去镜像服务器上下载。

```xml
<project xmlns="http://maven.apache.org/POM/4.0.0" xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
         xsi:schemaLocation="http://maven.apache.org/POM/4.0.0 http://maven.apache.org/maven-v4_0_0.xsd">
    <modelVersion>4.0.0</modelVersion>
    <groupId>net.aty.mybatis</groupId>
    <artifactId>mybatis-demo</artifactId>
    <packaging>jar</packaging>
    <version>${project.release.version}</version>

    <properties>
        <project.release.version>0.1-SNAPSHOT</project.release.version>
    </properties>

    <profiles>
        <profile>
            <id>release</id>
            <properties>
                <project.release.version>0.1</project.release.version>
            </properties>
        </profile>
    </profiles>
    <!--定义snapshots库和releases库的nexus地址-->
    <distributionManagement>
        <repository>
            <id>nexus-releases</id>
            <url>
                http://172.17.103.59:8081/nexus/content/repositories/releases/
            </url>
        </repository>
        <snapshotRepository>
            <id>nexus-snapshots</id>
            <url>
                http://172.17.103.59:8081/nexus/content/repositories/snapshots/
            </url>
        </snapshotRepository>
    </distributionManagement>
</project>
```

`pom`文件中`version`的定义是采用占位符的形式，这样的好处是可以根据不同的`profile`来替换版本信息，比如`maven`默认是使用`0.1-SNAPSHOT`作为该模块的版本。

>   1、如果在发布时使用`mvn deploy -P release` 的命令，那么会自动使用0.1作为发布版本，那么根据`maven`处理`snapshot`和`release`的规则，由于版本号后不带`-SNAPSHOT`故当成是正式发布版本，会被发布到`release`仓库；
>
> 2、如果发布时使用`mvn deploy`命令，那么就会使用默认的版本号`0.1-SNAPSHOT`，此时`maven`会认为是快照版本，会自动发布到快照版本库。  



在`distributionManagement`段中配置的是`snapshot`快照库和`release`发布库的地址，示例中采用nexus作为镜像服务器。对于版本库<font color=#dd0000>主要是id和url的配置</font>，配置完成后就可以通过`mvn deploy`进行发布了，当然了，如果你的镜像服务器需要用户名和密码，那么还需要在`maven`的`settings.xml`文件中做如下配置：

```xml
<server>
  <id>nexus-releases</id>
  <username>admin</username>
  <password>admin123</password>
</server>
<server>
  <id>nexus-snapshots</id>
  <username>admin</username>
  <password>admin123</password>
</server>
```

**注意**

这里<font color=#00dd00>配置的`server`的`id`必须和`pom`文件中的`distributionManagement`对应仓库的id保持一致</font>，maven在处理发布时会根据id查找用户名称和密码进行登录和文件的上传发布。



# 常用插件

## maven-compiler-plugin

```xml
<!-- maven编译插件 -->

<!-- 编译插件的版本(maven-compiler-plugin) -->
<plugin.compiler.version>3.6.1</plugin.compiler.version>

<plugin>
    <groupId>org.apache.maven.plugins</groupId>
    <artifactId>maven-compiler-plugin</artifactId>
    <version>${plugin.compiler.version}</version>
    <configuration>
        <!-- 编译版本 -->
        <source>${project.build.version}</source>
        <target>${project.build.version}</target>
        <!-- 编码方式 -->
        <encoding>${project.build.sourceEncoding}</encoding>
    </configuration>
</plugin>
```





## maven-surefire-plugin

```xml
<!-- mvn test插件版本 -->
<plugin.surefire.version>2.20.1</plugin.surefire.version>

<!--maven测试用例插件-->
<plugin>
    <groupId>org.apache.maven.plugins</groupId>
    <artifactId>maven-surefire-plugin</artifactId>
    <version>${plugin.surefire.version}</version>
</plugin>
```





## flatten-maven-plugin

```xml
<flatten-maven-plugin.version>1.1.0</flatten-maven-plugin.version>

<plugin>
    <groupId>org.codehaus.mojo</groupId>
    <artifactId>flatten-maven-plugin</artifactId>
    <version>${flatten-maven-plugin.version}</version>
    <configuration>
        <updatePomFile>true</updatePomFile>
        <flattenMode>resolveCiFriendliesOnly</flattenMode>
    </configuration>
    <executions>
        <execution>
            <id>flatten</id>
            <phase>process-resources</phase>
            <goals>
                <goal>flatten</goal>
            </goals>
        </execution>
        <execution>
            <id>flatten.clean</id>
            <phase>clean</phase>
            <goals>
                <goal>clean</goal>
            </goals>
        </execution>
    </executions>
</plugin>
```



从Maven 3.5.0-beta-1版本开始，就可以使用{sha1} 和 ${changelist}作为占位符来替换pom文件了。



### 单模块项目

在单模块项目中，情况比较简单，只使用了`${revision}`来替换版本。

还可以用另一种动态添加参数的方式来指定版本

```shell
$ $ mvn -Drevision=1.0.0-SNAPSHOT clean package
```

或者在(父)项目的properties中定义版本

```xml
<project>
  <modelVersion>4.0.0</modelVersion>
  
  <parent>
    <groupId>org.apache</groupId>
    <artifactId>apache</artifactId>
    <version>18</version>
  </parent>
  
  <groupId>org.apache.maven.ci</groupId>
  <artifactId>ci-parent</artifactId>
  <version>${revision}</version>

  <properties>
    <revision>1.0.0-SNAPSHOT</revision>
  </properties>
</project>
```

### 多模块项目

一个父项目和一个或多子模块。

#### 父pom

```xml
<project>
  <modelVersion>4.0.0</modelVersion>
  <parent>
    <groupId>org.apache</groupId>
    <artifactId>apache</artifactId>
    <version>18</version>
  </parent>
  <groupId>org.apache.maven.ci</groupId>
  <artifactId>ci-parent</artifactId>
  <version>${revision}</version>

  <properties>
    <revision>1.0.0-SNAPSHOT</revision>
  </properties>
  <modules>
    <module>child1</module>
    <module>child2</module>
    <module>child3</module>
  </modules>
</project>
```

#### 子模块

```xml
<project>
  <modelVersion>4.0.0</modelVersion>
  <parent>
    <groupId>org.apache.maven.ci</groupId>
    <artifactId>ci-parent</artifactId>
    <version>${revision}</version>
  </parent>
  
  <groupId>org.apache.maven.ci</groupId>
  <artifactId>ci-child</artifactId>

</project>
```

**注：多模块项目中子模块的版本应该使用父工程的版本，单独设置版本的话会导致版本混乱。**



#### 依赖

多模块工程结构下，会有很多模块依赖的情况，应该使用`${project.version}`来定义依赖（同父工程下的依赖）的版本



**注：定义依赖版本时，使用${revision}会导致构建失败，需要使用${project.version}。**



### install/deploy

如果使用以上设置来发布，必须使用`flatten-maven-plugin`插件

```xml
<project>
  <modelVersion>4.0.0</modelVersion>
  <parent>
    <groupId>org.apache</groupId>
    <artifactId>apache</artifactId>
    <version>18</version>
  </parent>
  <groupId>org.apache.maven.ci</groupId>
  <artifactId>ci-parent</artifactId>
  <name>First CI Friendly</name>
  <version>${revision}</version>
  ...
  <properties>
    <revision>1.0.0-SNAPSHOT</revision>
  </properties>

 <build>
  <plugins>
    <plugin>
      <groupId>org.codehaus.mojo</groupId>
      <artifactId>flatten-maven-plugin</artifactId>
      <version>1.1.0</version>
      <configuration>
          是否更新pom文件，此处还有更高级的用法
        <updatePomFile>true</updatePomFile>
        <flattenMode>resolveCiFriendliesOnly</flattenMode>
      </configuration>
      <executions>
        <execution>
          <id>flatten</id>
          <phase>process-resources</phase>
          <goals>
            <goal>flatten</goal>
          </goals>
        </execution>
        <execution>
          <id>flatten.clean</id>
          <phase>clean</phase>
          <goals>
            <goal>clean</goal>
          </goals>
        </execution>
      </executions>
    </plugin>
  </plugins>
  </build>
  <modules>
    <module>child1</module>
    ..
  </modules>
</project>
```



最终执行`mvn install/deploy`后，会将该模块的`pom`文件中的`${revision}`替换为实际的版本。



## maven-source-plugin

对于Maven工程的源代码，如果需要进行源文件的打包（jar包），可以利用`maven-source-plugin`插件来完成

```xml
<plugin>  
    <groupId>org.apache.maven.plugins</groupId>  
    <artifactId>maven-source-plugin</artifactId>  
    <version>3.0.0</version>  
    <configuration>  
        <attach>true</attach>  
    </configuration>  
    <executions>  
        <execution>  
            <phase>compile</phase>  
            <goals>  
                <goal>jar</goal>  
            </goals>  
        </execution>  
    </executions>  
</plugin>  
```

配置中指定了`phase`为`compile`，意思是在生命周期`compile`的时候就将源文件打包，即只要执行的`mvn`命令所属的生明周期包括`compile`这一阶段，就会将源代码打包。同样，`phase`还可以指定为`package`、`install`等。

**安装时，会同时将源码包安装到本地仓库，名为`xxx-sources.jar`**



## spring-boot-maven-plugin

```xml
<!-- springboot打包插件的版本(spring-boot-maven-plugin) -->
<plugin.spring.boot.version>2.3.1.RELEASE</plugin.spring.boot.version>

<!-- spring-boot-maven-plugin,springboot项目打包插件 -->
<!-- 使用该插件打包springboot项目，该插件可将所需要的依赖包一起打入目标jar包中 -->
<plugin>
    <groupId>org.springframework.boot</groupId>
    <artifactId>spring-boot-maven-plugin</artifactId>
    <version>${plugin.spring.boot.version}</version>
</plugin>
```


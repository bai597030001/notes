# 安装配置



## 启动暂停

```shell
$ activemq start
```



## 管理控制台

Activemq默认的管理后台端口是8161，默认管理用户名和密码均是admin。

Activemq默认的管理后台端口，我们可以在activemq的配置文件jetty.xml中查看到

```xml
<property name=”port” value=”8161″/>
```



Activemq默认管理用户名和密码均是admin，如果我们要修改默认用户名和密码的话，可以通过修改jetty-realm.properties文件来达到目的。

```shell
vim /usr/local/activemq/conf/jetty-realm.properties

admin: ilanni, admin

user: user, user
```



登录：http://192.168.199.170:8161
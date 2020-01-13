# 安装配置



## 管理界面

默认用户名密码 guest



## 配置rabbitmq账号密码

执行以下命令：

```shell
$ rabbitmq-plugins enable rabbitmq_management
```



添加新用户：

```shell
$ rabbitmqctl add_user admin admin
```



这里添加的用户名和密码为 admin

添加用户标签（超级管理员）：

```shell
$ rabbitmqctl set_user_tags admin administrator
```



添加成功后可以执行以下命令查看当前的用户列表：

```shell
$ rabbitmqctl list_users
```



## 启动暂停

```shell
$ service rabbitmq-server start #启动
$ rabbitmq-server -detached   #后台运行rabbitmq

$ service rabbitmq-server stop #停止

$ service rabbitmq-server restart #重启

$ service rabbitmq-server status #查看状态

$ service rabbitmq-server etc #查看有哪些命令可以使用

$ rabbitmq-plugins enable rabbitmq_management   #启动后台管理

$ rabbitmqctl status # 查看状态

$ rabbitmq-pluginsenable xxx # 开启某个插件
$ rabbitmq-pluginsdisablexxx # 关闭某个插件
```


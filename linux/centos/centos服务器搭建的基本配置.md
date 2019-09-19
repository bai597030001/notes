# centos7服务器搭建基本配置

- 包括：网络配置，主机名，防火墙，selinux，ssh免密登陆

## 网络配置

vi /etc/sysconfig/network-scripts/ifcfg-enp1s0

```bash
BOOTPROTO=static  #启用静态IP地址
ONBOOT=yes  #开启自动启用网络连接
IPADDR=172.16.1.131
GATEWAY=172.16.0.1
NETMASK=255.255.252.0
DNS1=61.233.9.9  #不上外网不用配
DNS2=211.98.2.4
```

- 启动、关闭、重启网络：
  - service network.service status/start/stop/restart
  - /bin/systemctl start/stop/restart network.service

## 主机名

- 从centos7开始，推荐修改/etc/hostname而不是/etc/sysconfig/network（如果两个都修改，以/etc/hostname为准）

## 防火墙

- 在旧版本的CentOS中，是使用 `iptables` 命令来设置防火墙的。但是，从CentOS7开始，默认就没有安装iptables，而是改用`firewall`来配置防火墙。
- firewall的配置文件是以xml的格式，存储在` /usr/lib/firewalld/` 和` /etc/firewalld/ `目录中。

查看防火墙状态：

```bash
firewall-cmd --state
```

开启/关闭/重启防火墙：

```bash
service firewalld start/stop/restart
/bin/systemctl start/stop/restart firewalld.service
```

启用/禁用防火墙：

```bash
systemctl enable/disable firewalld.service
```

- 防火墙打开特定端口（命令行操作）：

```bash
查看当前激活的区域有哪些：firewall-cmd --get-active-zones

结果：public
  interfaces: enp2s0f0

如果interfaces里面包含了需要打开端口的网卡，就选择在这个区域里配置打开端口。

例如：
对应的是网卡1，是我们需要的网卡，执行如下命令打开对应的端口
firewall-cmd --zone=public --add-port=3306/tcp --permanent

删除：
firewall-cmd --zone=public --remove-port=80/tcp --permanent

查看打开的端口（若有修改，则需要重启防火墙）：
firewall-cmd --zone=public --list-ports

firewall-cmd --zone=public --list-all
```

firewall-cmd命令参数说明：

```bash
–-zone：指定作用域。
–-add-port=80/tcp：添加的端口，格式为：端口/通讯协议。
–-permanent：表示永久生效，没有此参数重启后会失效。
```

- 防火墙打开特定端口（操作配置文件）：

/etc/firewalld/zones/public.xml 文件的默认内容为：

```xml
<?xml version="1.0" encoding="utf-8"?>
<zone>
  <short>Public</short>
  <description>For use in public areas. You do not trust the other computers on networks to not harm your computer. Only selected incoming connections are accepted.</description>
  <service name="dhcpv6-client"/>
  <service name="ssh"/>
</zone>
```

修改该配置文件，来添加3306端口。修改后的内容为：

```xml
<?xml version="1.0" encoding="utf-8"?>
<zone>
  <short>Public</short>
  <description>For use in public areas. You do not trust the other computers on networks to not harm your computer. Only selected incoming connections are accepted.</description>
  <service name="dhcpv6-client"/>
  <service name="ssh"/>
  <port protocol="tcp" port="3306"/>
</zone>
```

- CentOS7更改为iptables防火墙

[链接](<https://blog.csdn.net/lamp_yang_3533/article/details/76644105>)

## selinux

- 安全增强型 Linux（Security-Enhanced Linux）简称 SELinux，它是一个 Linux 内核模块，也是 Linux 的一个安全子系统。
- 从Linux2.6内核之后就将SELinux集成在了内核当中，因为SELinux是内核级别的，所以我们对于其配置文件的修改都是需要重新启动操作系统才能生效的。

关闭selinux：

```bash
vi /etc/selinux/config
```

将SELINUX=enforcing改为SELINUX=disabled
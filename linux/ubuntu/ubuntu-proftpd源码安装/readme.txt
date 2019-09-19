源码安装proftpd
	源码存放目录：/usr/src下
	指定安装目录为 /opt/proftpd, 需要先建立/opt/proftpd目录
	之后进行安装配置-> ./configure --prefix=/opt/proftpd
	make;
	chmod +x modules/glue.sh
	make install;
	
	此时完成了proftpd的安装，目录为/opt/proftpd下的所有文件
		可执行文件：/opt/proftpd/sbin/proftpd
		配置文件：	/opt/proftpd/etc/proftpd.conf (注意：添加代码：RootLogin	on)
	如果要将proftpd设置为开机启动，则打开 /etc/rc.local文件
	添加：/opt/proftpd/sbin/proftpd
	
	最后，进入源码目录清除一下吧
	make clean
	make distclean
	
ps:
	查看进程：	pgrep proftpd
	杀死进程：	pkill proftpd
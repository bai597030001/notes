Դ�밲װproftpd
	Դ����Ŀ¼��/usr/src��
	ָ����װĿ¼Ϊ /opt/proftpd, ��Ҫ�Ƚ���/opt/proftpdĿ¼
	֮����а�װ����-> ./configure --prefix=/opt/proftpd
	make;
	chmod +x modules/glue.sh
	make install;
	
	��ʱ�����proftpd�İ�װ��Ŀ¼Ϊ/opt/proftpd�µ������ļ�
		��ִ���ļ���/opt/proftpd/sbin/proftpd
		�����ļ���	/opt/proftpd/etc/proftpd.conf (ע�⣺��Ӵ��룺RootLogin	on)
	���Ҫ��proftpd����Ϊ������������� /etc/rc.local�ļ�
	��ӣ�/opt/proftpd/sbin/proftpd
	
	��󣬽���Դ��Ŀ¼���һ�°�
	make clean
	make distclean
	
ps:
	�鿴���̣�	pgrep proftpd
	ɱ�����̣�	pkill proftpd
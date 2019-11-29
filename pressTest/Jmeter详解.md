Java.NET.BindException: Address already in use: connect

原因：短时间内new socket操作很多,而socket.close()操作并不能立即释放绑定的端口,而是把端口设置为TIMEWAIT 状态,过段时间(默认240s)才释放,(用netstat -na可以看到),最后系统资源耗尽(windows上是耗尽了pool of ephemeral ports ,这段区间在1024-5000之间)
解决方法：在运行JMeter agent的机器上，添加注册表条目HKEY_LOCAL_MACHINE\SYSTEM\CurrentControlSet\Services\Tcpip\Parameters

 新建 -> DWORD(32位)值(D) -> name：TcpTimedWaitDe，value：30   –> 设置为30秒，默认是240秒 

 新建 -> DWORD(32位)值(D) -> name：MaxUserPort，value：65534（十进制） –> 设置最大连接数65534 


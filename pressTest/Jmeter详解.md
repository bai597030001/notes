# jmeter使用

## 新建任务

新建 `test plan` -> add -> Thread(User) -> Thread Group

对于新建的线程组 -> add ->Sampler -> Http Request

对于新建的线程组 -> add -> listener -> View Results Tree

对于新建的线程组 -> add -> listerner -> summary Report



保存：save test plan as -> XXX.jmx



## 测试并生成html报告：

### 方式一：利用已有.jtl文件生成报告

```shell
jmeter -g test.jtl -o /path
# -g：后跟test.jtl文件所在的路径
# -o：后跟生成的HTML文件存放的路径
```

注：如果是在Windows环境命令行运行，必须指定生成的HTML文件存放文件夹，否则会报错；如果是linux环境，如指定路径下不存在该文件夹，会生成对应的文件夹存放报告文件！



### 方式二：无.jtl文件生成测试报告

```shell
jmeter -n -t test.jmx -l test.jtl -e -o /path
# -n：以非GUI形式运行Jmeter 
# -t：source.jmx 脚本路径 
# -l：result.jtl 运行结果保存路径（.jtl）,此文件必须不存在 
# -e：在脚本运行结束后生成html报告 
# -o：用于存放html报告的目录
```



### 示例

```shell
.\bin\jmeter -n -t .\test-plan\DubboPriInterFaceTestPlan.jmx -l .\test-result\DubboPriInterfaceTestPlan.jtl -e -o .\test-report\DubboPriInterfaceTestReport
```



# 常见异常



Java.NET.BindException: Address already in use: connect

原因：短时间内new socket操作很多,而socket.close()操作并不能立即释放绑定的端口,而是把端口设置为TIMEWAIT 状态,过段时间(默认240s)才释放,(用netstat -na可以看到),最后系统资源耗尽(windows上是耗尽了pool of ephemeral ports ,这段区间在1024-5000之间)
解决方法：在运行JMeter agent的机器上，添加注册表条目HKEY_LOCAL_MACHINE\SYSTEM\CurrentControlSet\Services\Tcpip\Parameters

 新建 -> DWORD(32位)值(D) -> name：TcpTimedWaitDe，value：30   –> 设置为30秒，默认是240秒 

 新建 -> DWORD(32位)值(D) -> name：MaxUserPort，value：65534（十进制） –> 设置最大连接数65534 


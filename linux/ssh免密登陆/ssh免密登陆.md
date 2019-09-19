# ssh免密登陆配置

- 需要用到的命令：`ssh-keygen`，`ssh`，`scp`

## 1.1ssh-keygen命令及文件介绍

`ssh-keygen -t [rsa|dsa]`：（加密方式选 rsa|dsa，默认dsa）

- 通过命令`ssh-keygen -t rsa`生成之后会在用户的根目录生成一个 “.ssh”的文件夹。进入“.ssh”会生成以下几个文件：
  - authorized_keys	: 存放远程免密登录的公钥,主要通过这个文件记录多台机器的公钥
  - id_rsa                      : 生成的私钥文件
  - id_rsa.pub 		    : 生成的公钥文件
  - know_hosts 		 : 已知的主机公钥清单
- 注意：如果希望ssh公钥生效需满足至少下面两个条件
  - 1) .ssh目录的权限必须是700
  - 2) .ssh/authorized_keys文件权限必须是600

## 1.2配置多台服务器免密

- 需要分别在每个节点上创建一对密钥文件，密钥文件包括公钥文件`id_rsa.pub`和私钥文件 `id_rsa`。客户端在使用ssh登录到其他节点上的时候，ssh会发送私钥去和其他节点上的公钥去匹配，如果匹配成功，那么就会自动登录，而不需要输入密码，这就实现了无密码登录。  
- 所以，客户端要实现免密码登录到目标节点上的时候，就需要把公钥写入目标节点上的authorized_keys中。
- 具体步骤
  - 1）在6台机器上分别新建~/.ssh目录
  - 2）分别在机器上执行`ssh-keygen -t rsa`，生成公钥和私钥
  - 3）找一台主服务器，将其余节点的公钥文件内容复制到该主服务器的`authorized_keys`中
    - 方法一：将所有机器的`id_rsa.pub`文件复制到一台主机上，然后将所有`id_rsa.pub`内容追加到一个`id_rsa.pub`文件中。
    - `ssh-copy-id`
  - 4）将上述主服务器上包含所有节点公钥的authorized_keys文件分别复制到各个节点
  - 5）在主服务器上ssh连接其他节点的每台服务器，然后退出，这样可以将每个节点的公钥摘要写入到 `~/.ssh/known_hosts`中。
  - 6）将主服务器的`~/.ssh/known_hosts`文件scp到每个节点，替换掉原来的文件

## 2 SSH 公钥检查

[连接](<http://www.worldhello.net/2010/04/08/1026.html>)
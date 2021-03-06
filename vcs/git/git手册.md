# git代码管理

![](img/git1.png)



- Workspace:工作区，执行`git add *`命令就把改动提交到了暂存区，执行`git pull`命令将远程仓库的数据拉到当前分支并合并，执行`git checkout [branch-name]`切换分支
- Index:暂存区，执行`git commit -m '说明'` 命令就把改动提交到了仓库区（当前分支）
- Repository:本地仓库，执行`git push origin master`提交到远程仓库，执行`git clone 地址`将克隆远程仓库到本地
- Remote:远程仓库，就是类似github，coding等网站所提供的仓库



| 术语               | 定义                                                         |
| ------------------ | ------------------------------------------------------------ |
| 仓库（Repository） | 一个仓库包括了所有的版本信息、所有的分支和标记信息。在Git中仓库的每份拷贝都是完整的。仓库让你可以从中取得你的工作副本。 |
| 分支（Branches）   | 一个分支意味着一个独立的、拥有自己历史信息的代码线（code line）。你可以从已有的代码中生成一个新的分支，这个分支与剩余的分支完全独立。默认的分支往往是叫master。用户可以选择一个分支，选择一个分支执行命令`git checkout branch`. |
| 标记（Tags）       | 一个标记指的是某个分支某个特定时间点的状态。通过标记，可以很方便的切换到标记时的状态，例如2009年1月25号在testing分支上的代码状态 |
| 提交（Commit）     | 提交代码后，仓库会创建一个新的版本。这个版本可以在后续被重新获得。每次提交都包括作者和提交者，作者和提交者可以是不同的人 |
| 修订（Revision）   | 用来表示代码的一个版本状态。Git通过用SHA1 hash算法表示的id来标识不同的版本。每一个 SHA1 id都是160位长，16进制标识的字符串.。最新的版本可以通过HEAD来获取。之前的版本可以通过"HEAD~1"来获取，以此类推。 |



- #### 忽略特定的文件

  .`gitignore`

- #### 追踪空的文件夹

  `.gitkeep`

- 配置

```shell
# 显示当前的Git配置
$ git config --list

# 编辑Git配置文件，只是配置用户信息的话直接看下面两行命令即可
$ git config -e [--global]

# 设置提交代码时的用户信息，是否加上全局--global自行决定，一般是直接设置全局的。另外用户邮箱需要注意最好使用gmail,QQ也可以，需要和你远程仓库保持一致不然你的contribution是不会被记录在远程仓库的
$ git config [--global] user.name "[name]"
$ git config [--global] user.email "[email address]"
```

`Git的设置文件为`.gitconfig`，它可以在用户主目录下（全局配置），也可以在项目目录下（项目配置）。`



# git本地操作

- 创建仓库

```shell
# 在当前目录新建一个Git代码库
$ git init

# 新建一个目录，将其初始化为Git代码库
$ git init [project-name]

# 下载一个项目和它的整个代码历史（各个分支提交记录等）
$ git clone [url]
```

- 提交文件

```shell
# 添加当前目录的所有文件到暂存区
$ git add */./--all

# 提交暂存区到仓库区
$ git commit -m [message]
# 提交时显示所有diff信息
$ git commit -v
# 使用一次新的commit，替代上一次提交
# 如果代码没有任何新变化，则用来改写上一次commit的提交信息
$ git commit --amend -m [message]
# 重做上一次commit，并包括指定文件的新变化
$ git commit --amend [file1] [file2] ...

# 为远程Git更名为origin
$ git remote add origin git@github.com:abcd/tmp.git

# 推送此次修改，这是首次推送需要加上-u,之后推送就可以直接git push  origin master,origin是远程Git名字，这个可以自己定义，不过一般是用origin罢了，master是默认的分支，如果不在master分支提交需要写清楚分支名称
$ git push -u origin master
```

- 版本回退

```shell
# 在Git中，用 HEAD 表示当前版本; 上一个版本就是 HEAD^ 上上一个版本就是 HEAD^^; 当然往上100个版本写100个^比较容易数不过来，所以写成 HEAD~100
	
$ git reset --hard HEAD^

$ git reset --hard 1094a		# 版本号没必要写全，前几位就可以了
```

```shell
$ git reflog 用来记录你的每一次命令

$ git log --graph 查看分支合并图

$ git rm -r --cached 文件/文件夹名字 取消文件被版本控制

$ git merge --no-ff -m '合并描述' 分支名 不使用Fast forward方式合并，采用这种方式合并可以看到合并记录

$ git check-ignore -v 文件名 查看忽略规则
```



- 忽略已加入到版本库中的文件

```shell
$ git update-index --assume-unchanged file 忽略单个文件

$ git rm -r --cached 文件/文件夹名字 (. 忽略全部文件)
```



- 取消忽略文件

```shell
$ git update-index --no-assume-unchanged file
```



- 合并提交

```shell
# 在没有推送到远程之前，如果想合并多次的提交，并且修改提交信息

$ git rebase -i [SHA] # SHA 是上一次提交之前的那次提交的

$ git rebase -i HEAD~4 # 最后一个数字4代表压缩最后四次提交。
```

里边的提示：

```properties
pick：保留该commit（缩写:p）
reword：保留该commit，但我需要修改该commit的注释（缩写:r）
edit：保留该commit, 但我要停下来修改该提交(不仅仅修改注释)（缩写:e）
squash：将该commit和前一个commit合并（缩写:s）
fixup：将该commit和前一个commit合并，但我不要保留该提交的注释信息（缩写:f）
exec：执行shell命令（缩写:x）
drop：我要丢弃该commit（缩写:d）
```

示例：

```properties
pick fb29a79 新增dubbo,spring cloud等相关笔记
pick a8fe28b 新增spring cloud笔记
pick 242230e 更新spring cloud gateway
pick 10d24ff 更新网络相关 cloud相关
pick 81707ee 更新tomcat, io等
pick c0af253 更新部分笔记
squash f28e89e 修改git手册

# Rebase f7b3dda..f28e89e onto 10d24ff (7 commands)
#
# Commands:
# p, pick <commit> = use commit
# r, reword <commit> = use commit, but edit the commit message
# e, edit <commit> = use commit, but stop for amending
# s, squash <commit> = use commit, but meld into previous commit
# f, fixup <commit> = like "squash", but discard this commit's log message
# x, exec <command> = run command (the rest of the line) using shell
# b, break = stop here (continue rebase later with 'git rebase --continue')
# d, drop <commit> = remove commit
# l, label <label> = label current HEAD with a name
# t, reset <label> = reset HEAD to a label
# m, merge [-C <commit> | -c <commit>] <label> [# <oneline>]
# .       create a merge commit using the original merge commit's
# .       message (or the oneline, if no original merge commit was
# .       specified). Use -c <commit> to reword the commit message.
#
# These lines can be re-ordered; they are executed from top to bottom.
#
# If you remove a line here THAT COMMIT WILL BE LOST.
#
# However, if you remove everything, the rebase will be aborted.
#
# Note that empty commits are commented out
```





# git远程操作

## 提交本地仓库代码到远程仓库

github新建 `new repository`

Repository name: 仓库名称

Description(可选): 仓库描述介绍

Public, Private : 仓库权限（公开共享，私有或指定合作者）

Initialize this repository with a README: 添加一个README.md		-> 建议勾上

gitignore: 不需要进行版本管理的仓库类型，对应生成文件.gitignore

license: 证书类型，对应生成文件LICENSE

点击 Clone or dowload 会出现一个地址, copy 这个地址备用。

```http
https://github.com/bai597030001/network_optimize.git

git@github.com:bai597030001/network_optimize.git
```



### 本地已经建好了源码文件

即：文件夹并且已经有了代码,即本地仓库已经存在



- 1).将本地的仓库关联到GitHub

```shell
$ git remote add origin https://github.com/bai597030001/network_optimize.git
```

- 2).上传github之前pull一下

```shell
$ git pull origin master

# fatal: refusing to merge unrelated histories 错误
# 其实这个问题是因为 两个 根本不相干的 git 库， 一个是本地库， 一个是远端库，然后本地要去推送到远端， 远端觉得这个本地库跟自己不相干， 所以告知无法合并
# 使用这个强制的方法 --allow-unrelated-histories 把两段不相干的 分支进行强行合并

$ git pull origin master  --allow-unrelated-histories
```



- 3).上传代码到GitHub远程仓库

```shell
$ git push -u origin master

# -> remote: error: GH007: Your push would publish a private email address.
			
# 在GitHub的你账号网页上右上角，个人的登录退出的位置，找到setting：
		
# 		setting->emails->Keep my email address private，把这一项去掉勾选即可。
```

- 4).修改代码后再次提交

```shell
$ git commit -a -m "描述"
		
$ git pull origin master

$ git push -u origin master
```



### 本地仓库还没有代码

- 1).把github上面的仓库克隆到本地

```shell
$ git clone https://github.com/bai597030001/network_optimize.git(替换成你之前复制的地址)

# 这个步骤以后你的本地项目文件夹下面就会多出个文件夹，该文件夹名即为你github上面的项目名，如我多出了个 network_optimize 文件夹，我们把本地项目文件夹下的所有文件（除了新多出的那个文件夹不用），其余都复制到那个新多出的文件夹下。
		
# 接着继续输入命令 cd network_optimize, 进入 network_optimize 文件夹
```

- 2).提交

```shell
$ git add .
		
$ git commit  -m  "提交信息"  
		
$ git push -u origin master # （注：此操作目的是把本地仓库push到github上面，此步骤需要你输入帐号和密码）
```



## 本地与远程不一致如何同步

3.拉取 github 最新代码到本地

```shell
# 从远程获取最新版本到本地： 从远程的origin仓库的master分支下载到本地并新建一个分支temp
$ git fetch origin master:temp

# 比较本地的仓库和远程参考的区别
$ git diff temp

# 合并temp分支到master分支
$ git merge temp

# 删除此分支
$ git brach -d temp
```



# 分支管理

[原文链接](https://www.cnblogs.com/videring/articles/6962005.html)

```shell
$ git branch # 创建分支
$ git branch -b # 创建并切换到新建的分支上
$ git checkout # 切换分支
$ git branch # 查看分支列表
$ git branch -v # 查看所有分支的最后一次操作
$ git branch -vv # 查看当前分支
$ git branch -b ##  分支名 origin/分支名 创建远程分支到本地
$ git branch --merged#  # 查看别的分支和当前分支合并过的分支
$ git branch --no-merged # 查看未与当前分支合并的分支
$ git branch -d 分支名 # 删除本地分支
$ git branch -D 分支名 # 强行删除分支
$ git branch origin :分支名 # 删除远处仓库分支
```



```shell
$ git merge  # 分支名 合并分支到当前分支上

# Rebase 实际上就是取出一系列的提交记录，“复制”它们，然后在另外一个地方逐个的放下去。
# Rebase 的优势就是可以创造更线性的提交历史，这听上去有些难以理解。如果只允许使用 Rebase 的话，代码库的提交历史将会变得异常清晰。
$ git rebase
```



# 暂存

```shell
$ git stash # 暂存当前修改
$ git stash apply # 恢复最近的一次暂存
$ git stash apply stash@{0}
$ git stash pop # 恢复暂存并删除暂存记录
$ git stash list # 查看暂存列表
$ git stash drop # 暂存名(例：stash@{0}) 移除某次暂存
$ git stash clear # 清除暂存
$ git stash save # 查看对应的所有的修改
$ git stash show -p stash@{1} # 不输入对应的 stash@{} 内容则将最近的 stash 与当前分支做比较，如果加了则用指定的暂存
```



# 标签

```shell
$ git tag 标签名 添加标签(默认对当前版本)
$ git tag 标签名 commit_id 对某一提交记录打标签
$ git tag -a 标签名 -m '描述' 创建新标签并增加备注
$ git tag 列出所有标签列表
$ git show 标签名 查看标签信息
$ git tag -d 标签名 删除本地标签
$ git push origin 标签名 推送标签到远程仓库
$ git push origin --tags 推送所有标签到远程仓库
$ git push origin :refs/tags/标签名 从远程仓库中删除标签
```



# cherry-pick

使用 cherry-pick，可以从其他分支复制指定的一个或几个commit，然后导入到现在的分支。

```shell
$ git cherry-pick <commit id> # 单独合并一个提交
$ git cherry-pick -x <commit id> # 同上，不同点：保留原提交者信息。

# Git从1.7.2版本开始支持批量cherry-pick，就是一次可以cherry-pick一个区间的commit。

$ git cherry-pick <start-commit-id>..<end-commit-id>
$ git cherry-pick <start-commit-id>^..<end-commit-id>
# 前者表示把<start-commit-id>到<end-commit-id>之间(左开右闭，不包含start-commit-id)的提交cherry-pick到当前分支；
# 后者有"^"标志的表示把<start-commit-id>到<end-commit-id>之间(闭区间，包含start-commit-id)的提交cherry-pick到当前分支。

# 其中，<start-commit-id>到<end-commit-id>只需要commit-id的前6位即可，并且<start-commit-id>在时间上必须早于<end-commit-id>

# 注：以上合并，需要手动push代码。
```

## 使用的场合

- 把弄错分支的提交移动到正确的地方
- 把其他分支的提交添加到现在的分支

## 示例

例如，我们想把 learn-cherry-pick 这个分支上的第二个提交提取出来，然后添加到 master 上。

![](img/git-cherry-pick1.png)

从图右部分可以找到我们想提取 commit 的 commit id 为 `c3f0d9a`，在 master 分支上执行 cherry-pick

```shell
$ git checkout master
$ git pull
$ git cherry-pick c3f0d9a
[master 573066e] add a new line
 Date: Sun Jan 14 18:50:20 2018 +0800
 1 file changed, 1 insertion(+)
```

![](img/git-cherry-pick2.png)

cherry-pick 过程中也是可能会产生冲突的，解决冲突后先 add，然后使用 `git cherry-pick --continue`。
如果想放弃 cherry-pick，使用 `git cherry-pick --abort`

```shell
$ git cherry-pick --continue
$ git cherry-pick --quit
$ git cherry-pick --abort
```



# 回退操作

```shell
$ git reset --hard HEAD^ # 回退到上一个版本
$ git reset --hard ahdhs1(commit_id) # 回退到某个版本
$ git checkout -- file #撤销修改的文件(如果文件加入到了暂存区，则回退到暂存区的，如果文件加入到了版本库，则还原至加入版本库之后的状态)
$ git reset HEAD file # 撤回暂存区的文件修改到工作区
```



# 合并分支

假如我们现在在dev分支上，刚开发完项目，执行了下列命令

```shell
$ git  add .
$ git  commit -m '提交的备注信息'
$ git  push -u origin dev
```

现在想将dev分支合并到master分支：

```shell
# 1、首先切换到master分支上
$ git checkout master

# 2.如果是多人开发的话 需要把远程master上的代码pull下来
$ git pull origin master

# 3.然后把dev分支的代码合并到master上
$ git merge dev

# 4.解决冲突
不同分支的内容会被分开显示
<<<<<<<<<<
yyy
==========
xxx
>>>>>>>>>>

为了解决冲突你需要把不要的去掉，把要的保留下来。箭头和等号必须删掉。做完后保存退出，
执行 git merge --continue。会生成新的commit，merge结束。

# 4.然后查看状态及执行提交命令
git status

On branch master
Your branch is ahead of 'origin/master' by 12 commits.
  (use "git push" to publish your local commits)
nothing to commit, working tree clean

# 上面的意思就是你有12个commit，需要push到远程master上 
# 最后执行下面提交命令
$ git push origin master
```

其他

```shell
# 更新远程分支列表
$ git remote update origin --prune

# 查看所有分支
$ git branch -a

# 删除远程分支Chapater6
$ git push origin --delete Chapater6

# 删除本地分支 Chapater6
$ git branch -d  Chapater6
```



# 常见问题

`error : warning: LF will be replaced by CRLF`

```shell
# windows中的换行符为 CRLF， 而在linux下的换行符为LF，所以在执行add . 时出现提示

$ rm -rf .git  # 删除.git
$ git config --global core.autocrlf false  # 禁用自动转换  
```



# git/github 删除

`git rm`		删除文件

`git checkout --fileName`  从本地/远程仓库恢复文件到本地


github -> settings -> 底部 Delete this repository



# Git分支管理

## 分支命名

**master 分支**

master 为主分支，也是用于部署生产环境的分支，确保master分支稳定性， master 分支一般由develop以及hotfix分支合并，任何时间都不能直接修改代码

**develop 分支**

develop 为开发分支，始终保持最新完成以及bug修复后的代码，一般开发的新功能时，feature分支都是基于develop分支下创建的。

**feature 分支**

- 开发新功能时，以develop为基础创建feature分支。
- 分支命名: feature/ 开头的为特性分支， 命名规则: feature/user_module、 feature/cart_module

**release分支**

release 为预上线分支，发布提测阶段，会release分支代码为基准提测。当有一组feature开发完成，首先会合并到develop分支，进入提测时会创建release分支。如果测试过程中若存在bug需要修复，则直接由开发者在release分支修复并提交。当测试完成之后，合并release分支到master和develop分支，此时master为最新代码，用作上线。

**hotfix 分支**

分支命名: hotfix/ 开头的为修复分支，它的命名规则与feature分支类似。线上出现紧急问题时，需要及时修复，以master分支为基线，创建hotfix分支，修复完成后，需要合并到master分支和develop分支



![](img/git_branch1.jpg)



## 常见任务

增加新功能

```shell
(dev)$: git checkout -b feature/xxx            # 从dev建立特性分支
(feature/xxx)$: blabla                         # 开发
(feature/xxx)$: git add xxx
(feature/xxx)$: git commit -m 'commit comment'
(dev)$: git merge feature/xxx --no-ff          # 把特性分支合并到dev
```

修复紧急bug

```shell
(master)$: git checkout -b hotfix/xxx         # 从master建立hotfix分支
(hotfix/xxx)$: blabla                         # 开发
(hotfix/xxx)$: git add xxx
(hotfix/xxx)$: git commit -m 'commit comment'
(master)$: git merge hotfix/xxx --no-ff       # 把hotfix分支合并到master，并上线到生产环境
(dev)$: git merge hotfix/xxx --no-ff          # 把hotfix分支合并到dev，同步代码
```

测试环境代码

```shell
(release)$: git merge dev --no-ff             # 把dev分支合并到release，然后在测试环境拉取并测试
```

生产环境上线

```shell
(master)$: git merge release --no-ff          # 把release测试好的代码合并到master，运维人员操作
(master)$: git tag -a v0.1 -m '部署包版本名'  #给版本命名，打Tag
```

## 日志规范

在一个团队协作的项目中，开发人员需要经常提交一些代码去修复bug或者实现新的feature。

而项目中的文件和实现什么功能、解决什么问题都会渐渐淡忘，最后需要浪费时间去阅读代码。但是好的日志规范commit messages编写有帮助到我们，它也反映了一个开发人员是否是良好的协作者。

编写良好的Commit messages可以达到3个重要的目的：

- 加快review的流程
- 帮助我们编写良好的版本发布日志
- 让之后的维护者了解代码里出现特定变化和feature被添加的原因

目前，社区有多种 Commit message 的写法规范。来自Angular 规范是目前使用最广的写法，比较合理和系统化。如下图：

![](img/git_angular_log1.jpg)

### Commit messages基本语法

当前业界应用的比较广泛的是 Angular Git Commit Guidelines

“https://github.com/angular/angular.js/blob/master/DEVELOPERS.md#-git-commit-guidelines

具体格式为:

```xml
<type>: <subject>
<BLANK LINE>
<body>
<BLANK LINE>
<footer>
```

- type: 本次 commit 的类型，诸如 bugfix docs style 等

- scope: 本次 commit 波及的范围

- subject: 简明扼要的阐述下本次 commit 的主旨，在原文中特意强调了几点：

- - 使用祈使句，是不是很熟悉又陌生的一个词
  - 首字母不要大写
  - 结尾无需添加标点

body: 同样使用祈使句，在主体内容中我们需要把本次 commit 详细的描述一下，比如此次变更的动机，如需换行，则使用 |

footer: 描述下与之关联的 issue 或 break change

### Type的类别

- feat: 添加新特性
- fix: 修复bug
- docs: 仅仅修改了文档
- style: 仅仅修改了空格、格式缩进、都好等等，不改变代码逻辑
- refactor: 代码重构，没有加新功能或者修复bug
- perf: 增加代码进行性能测试
- test: 增加测试用例
- chore: 改变构建流程、或者增加依赖库、工具等

### Commit messages格式

```shell
# 标题行：50个字符以内，描述主要变更内容
#
# 主体内容：更详细的说明文本，建议72个字符以内。需要描述的信息包括:
#
# * 为什么这个变更是必须的? 它可能是用来修复一个bug，增加一个feature，提升性能、可靠性、稳定性等等
# * 他如何解决这个问题? 具体描述解决问题的步骤
# * 是否存在副作用、风险?
#
# 如果需要的化可以添加一个链接到issue地址或者其它文档
```


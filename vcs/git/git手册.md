# git�������

![](img/git1.png)



- Workspace:��������ִ��`git add *`����ͰѸĶ��ύ�����ݴ�����ִ��`git pull`���Զ�ֿ̲������������ǰ��֧���ϲ���ִ��`git checkout [branch-name]`�л���֧
- Index:�ݴ�����ִ��`git commit -m '˵��'` ����ͰѸĶ��ύ���˲ֿ�������ǰ��֧��
- Repository:���زֿ⣬ִ��`git push origin master`�ύ��Զ�ֿ̲⣬ִ��`git clone ��ַ`����¡Զ�ֿ̲⵽����
- Remote:Զ�ֿ̲⣬��������github��coding����վ���ṩ�Ĳֿ�



| ����               | ����                                                         |
| ------------------ | ------------------------------------------------------------ |
| �ֿ⣨Repository�� | һ���ֿ���������еİ汾��Ϣ�����еķ�֧�ͱ����Ϣ����Git�вֿ��ÿ�ݿ������������ġ��ֿ�������Դ���ȡ����Ĺ��������� |
| ��֧��Branches��   | һ����֧��ζ��һ�������ġ�ӵ���Լ���ʷ��Ϣ�Ĵ����ߣ�code line��������Դ����еĴ���������һ���µķ�֧�������֧��ʣ��ķ�֧��ȫ������Ĭ�ϵķ�֧�����ǽ�master���û�����ѡ��һ����֧��ѡ��һ����ִ֧������`git checkout branch`. |
| ��ǣ�Tags��       | һ�����ָ����ĳ����֧ĳ���ض�ʱ����״̬��ͨ����ǣ����Ժܷ�����л������ʱ��״̬������2009��1��25����testing��֧�ϵĴ���״̬ |
| �ύ��Commit��     | �ύ����󣬲ֿ�ᴴ��һ���µİ汾������汾�����ں��������»�á�ÿ���ύ���������ߺ��ύ�ߣ����ߺ��ύ�߿����ǲ�ͬ���� |
| �޶���Revision��   | ������ʾ�����һ���汾״̬��Gitͨ����SHA1 hash�㷨��ʾ��id����ʶ��ͬ�İ汾��ÿһ�� SHA1 id����160λ����16���Ʊ�ʶ���ַ���.�����µİ汾����ͨ��HEAD����ȡ��֮ǰ�İ汾����ͨ��"HEAD~1"����ȡ���Դ����ơ� |



- #### �����ض����ļ�

  .`gitignore`

- #### ׷�ٿյ��ļ���

  `.gitkeep`

- ����

```shell
# ��ʾ��ǰ��Git����
$ git config --list

# �༭Git�����ļ���ֻ�������û���Ϣ�Ļ�ֱ�ӿ��������������
$ git config -e [--global]

# �����ύ����ʱ���û���Ϣ���Ƿ����ȫ��--global���о�����һ����ֱ������ȫ�ֵġ������û�������Ҫע�����ʹ��gmail,QQҲ���ԣ���Ҫ����Զ�ֿ̲Ᵽ��һ�²�Ȼ���contribution�ǲ��ᱻ��¼��Զ�ֿ̲��
$ git config [--global] user.name "[name]"
$ git config [--global] user.email "[email address]"
```

`Git�������ļ�Ϊ`.gitconfig`�����������û���Ŀ¼�£�ȫ�����ã���Ҳ��������ĿĿ¼�£���Ŀ���ã���`



# git���ز���

- �����ֿ�

```shell
# �ڵ�ǰĿ¼�½�һ��Git�����
$ git init

# �½�һ��Ŀ¼�������ʼ��ΪGit�����
$ git init [project-name]

# ����һ����Ŀ����������������ʷ��������֧�ύ��¼�ȣ�
$ git clone [url]
```

- �ύ�ļ�

```shell
# ��ӵ�ǰĿ¼�������ļ����ݴ���
$ git add */./--all

# �ύ�ݴ������ֿ���
$ git commit -m [message]
# �ύʱ��ʾ����diff��Ϣ
$ git commit -v
# ʹ��һ���µ�commit�������һ���ύ
# �������û���κ��±仯����������д��һ��commit���ύ��Ϣ
$ git commit --amend -m [message]
# ������һ��commit��������ָ���ļ����±仯
$ git commit --amend [file1] [file2] ...

# ΪԶ��Git����Ϊorigin
$ git remote add origin git@github.com:abcd/tmp.git

# ���ʹ˴��޸ģ������״�������Ҫ����-u,֮�����;Ϳ���ֱ��git push  origin master,origin��Զ��Git���֣���������Լ����壬����һ������origin���ˣ�master��Ĭ�ϵķ�֧���������master��֧�ύ��Ҫд�����֧����
$ git push -u origin master
```

- �汾����

```shell
# ��Git�У��� HEAD ��ʾ��ǰ�汾; ��һ���汾���� HEAD^ ����һ���汾���� HEAD^^; ��Ȼ����100���汾д100��^�Ƚ�������������������д�� HEAD~100
	
$ git reset --hard HEAD^

$ git reset --hard 1094a		# �汾��û��Ҫдȫ��ǰ��λ�Ϳ�����
```

```shell
$ git reflog ������¼���ÿһ������

$ git log --graph �鿴��֧�ϲ�ͼ

$ git rm -r --cached �ļ�/�ļ������� ȡ���ļ����汾����

$ git merge --no-ff -m '�ϲ�����' ��֧�� ��ʹ��Fast forward��ʽ�ϲ����������ַ�ʽ�ϲ����Կ����ϲ���¼

$ git check-ignore -v �ļ��� �鿴���Թ���
```



- �����Ѽ��뵽�汾���е��ļ�

```shell
$ git update-index --assume-unchanged file ���Ե����ļ�

$ git rm -r --cached �ļ�/�ļ������� (. ����ȫ���ļ�)
```



- ȡ�������ļ�

```shell
$ git update-index --no-assume-unchanged file
```



- �ϲ��ύ

```shell
# ��û�����͵�Զ��֮ǰ�������ϲ���ε��ύ�������޸��ύ��Ϣ

$ git rebase -i [SHA] # SHA ����һ���ύ֮ǰ���Ǵ��ύ��

$ git rebase -i HEAD~4 # ���һ������4����ѹ������Ĵ��ύ��
```

��ߵ���ʾ��

```properties
pick��������commit����д:p��
reword��������commit��������Ҫ�޸ĸ�commit��ע�ͣ���д:r��
edit��������commit, ����Ҫͣ�����޸ĸ��ύ(�������޸�ע��)����д:e��
squash������commit��ǰһ��commit�ϲ�����д:s��
fixup������commit��ǰһ��commit�ϲ������Ҳ�Ҫ�������ύ��ע����Ϣ����д:f��
exec��ִ��shell�����д:x��
drop����Ҫ������commit����д:d��
```

ʾ����

```properties
pick fb29a79 ����dubbo,spring cloud����رʼ�
pick a8fe28b ����spring cloud�ʼ�
pick 242230e ����spring cloud gateway
pick 10d24ff ����������� cloud���
pick 81707ee ����tomcat, io��
pick c0af253 ���²��ֱʼ�
squash f28e89e �޸�git�ֲ�

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





# gitԶ�̲���

## �ύ���زֿ���뵽Զ�ֿ̲�

github�½� `new repository`

Repository name: �ֿ�����

Description(��ѡ): �ֿ���������

Public, Private : �ֿ�Ȩ�ޣ���������˽�л�ָ�������ߣ�

Initialize this repository with a README: ���һ��README.md		-> ���鹴��

gitignore: ����Ҫ���а汾����Ĳֿ����ͣ���Ӧ�����ļ�.gitignore

license: ֤�����ͣ���Ӧ�����ļ�LICENSE

��� Clone or dowload �����һ����ַ, copy �����ַ���á�

```http
https://github.com/bai597030001/network_optimize.git

git@github.com:bai597030001/network_optimize.git
```



### �����Ѿ�������Դ���ļ�

�����ļ��в����Ѿ����˴���,�����زֿ��Ѿ�����



- 1).�����صĲֿ������GitHub

```shell
$ git remote add origin https://github.com/bai597030001/network_optimize.git
```

- 2).�ϴ�github֮ǰpullһ��

```shell
$ git pull origin master

# fatal: refusing to merge unrelated histories ����
# ��ʵ�����������Ϊ ���� ��������ɵ� git �⣬ һ���Ǳ��ؿ⣬ һ����Զ�˿⣬Ȼ�󱾵�Ҫȥ���͵�Զ�ˣ� Զ�˾���������ؿ���Լ�����ɣ� ���Ը�֪�޷��ϲ�
# ʹ�����ǿ�Ƶķ��� --allow-unrelated-histories �����β���ɵ� ��֧����ǿ�кϲ�

$ git pull origin master  --allow-unrelated-histories
```



- 3).�ϴ����뵽GitHubԶ�ֿ̲�

```shell
$ git push -u origin master

# -> remote: error: GH007: Your push would publish a private email address.
			
# ��GitHub�����˺���ҳ�����Ͻǣ����˵ĵ�¼�˳���λ�ã��ҵ�setting��
		
# 		setting->emails->Keep my email address private������һ��ȥ����ѡ���ɡ�
```

- 4).�޸Ĵ�����ٴ��ύ

```shell
$ git commit -a -m "����"
		
$ git pull origin master

$ git push -u origin master
```



### ���زֿ⻹û�д���

- 1).��github����Ĳֿ��¡������

```shell
$ git clone https://github.com/bai597030001/network_optimize.git(�滻����֮ǰ���Ƶĵ�ַ)

# ��������Ժ���ı�����Ŀ�ļ�������ͻ������ļ��У����ļ�������Ϊ��github�������Ŀ�������Ҷ���˸� network_optimize �ļ��У����ǰѱ�����Ŀ�ļ����µ������ļ��������¶�����Ǹ��ļ��в��ã������඼���Ƶ��Ǹ��¶�����ļ����¡�
		
# ���ż����������� cd network_optimize, ���� network_optimize �ļ���
```

- 2).�ύ

```shell
$ git add .
		
$ git commit  -m  "�ύ��Ϣ"  
		
$ git push -u origin master # ��ע���˲���Ŀ���ǰѱ��زֿ�push��github���棬�˲�����Ҫ�������ʺź����룩
```



## ������Զ�̲�һ�����ͬ��

3.��ȡ github ���´��뵽����

```shell
# ��Զ�̻�ȡ���°汾�����أ� ��Զ�̵�origin�ֿ��master��֧���ص����ز��½�һ����֧temp
$ git fetch origin master:temp

# �Ƚϱ��صĲֿ��Զ�̲ο�������
$ git diff temp

# �ϲ�temp��֧��master��֧
$ git merge temp

# ɾ���˷�֧
$ git brach -d temp
```



# ��֧����

[ԭ������](https://www.cnblogs.com/videring/articles/6962005.html)

```shell
$ git branch # ������֧
$ git branch -b # �������л����½��ķ�֧��
$ git checkout # �л���֧
$ git branch # �鿴��֧�б�
$ git branch -v # �鿴���з�֧�����һ�β���
$ git branch -vv # �鿴��ǰ��֧
$ git brabch -b ##  ��֧�� origin/��֧�� ����Զ�̷�֧������
$ git branch --merged#  # �鿴��ķ�֧�͵�ǰ��֧�ϲ����ķ�֧
$ git branch --no-merged # �鿴δ�뵱ǰ��֧�ϲ��ķ�֧
$ git branch -d ��֧�� # ɾ�����ط�֧
$ git branch -D ��֧�� # ǿ��ɾ����֧
$ git branch origin :��֧�� # ɾ��Զ���ֿ��֧
```



```shell
$ git merge  # ��֧�� �ϲ���֧����ǰ��֧��

# Rebase ʵ���Ͼ���ȡ��һϵ�е��ύ��¼�������ơ����ǣ�Ȼ��������һ���ط�����ķ���ȥ��
# Rebase �����ƾ��ǿ��Դ�������Ե��ύ��ʷ��������ȥ��Щ������⡣���ֻ����ʹ�� Rebase �Ļ����������ύ��ʷ�������쳣������
$ git rebase
```



# �ݴ�

```shell
$ git stash # �ݴ浱ǰ�޸�
$ git stash apply # �ָ������һ���ݴ�
$ git stash apply stash@{0}
$ git stash pop # �ָ��ݴ沢ɾ���ݴ��¼
$ git stash list # �鿴�ݴ��б�
$ git stash drop # �ݴ���(����stash@{0}) �Ƴ�ĳ���ݴ�
$ git stash clear # ����ݴ�
$ git stash save # �鿴��Ӧ�����е��޸�
$ git stash show -p stash@{1} # �������Ӧ�� stash@{} ����������� stash �뵱ǰ��֧���Ƚϣ������������ָ�����ݴ�
```



# ��ǩ

```shell
$ git tag ��ǩ�� ��ӱ�ǩ(Ĭ�϶Ե�ǰ�汾)
$ git tag ��ǩ�� commit_id ��ĳһ�ύ��¼���ǩ
$ git tag -a ��ǩ�� -m '����' �����±�ǩ�����ӱ�ע
$ git tag �г����б�ǩ�б�
$ git show ��ǩ�� �鿴��ǩ��Ϣ
$ git tag -d ��ǩ�� ɾ�����ر�ǩ
$ git push origin ��ǩ�� ���ͱ�ǩ��Զ�ֿ̲�
$ git push origin --tags �������б�ǩ��Զ�ֿ̲�
$ git push origin :refs/tags/��ǩ�� ��Զ�ֿ̲���ɾ����ǩ
```



# cherry-pick

ʹ�� cherry-pick�����Դ�������֧����ָ����һ���򼸸�commit��Ȼ���뵽���ڵķ�֧��

```shell
$ git cherry-pick <commit id> # �����ϲ�һ���ύ
$ git cherry-pick -x <commit id> # ͬ�ϣ���ͬ�㣺����ԭ�ύ����Ϣ��

# Git��1.7.2�汾��ʼ֧������cherry-pick������һ�ο���cherry-pickһ�������commit��

$ git cherry-pick <start-commit-id>..<end-commit-id>
$ git cherry-pick <start-commit-id>^..<end-commit-id>
# ǰ�߱�ʾ��<start-commit-id>��<end-commit-id>֮��(���ұգ�������start-commit-id)���ύcherry-pick����ǰ��֧��
# ������"^"��־�ı�ʾ��<start-commit-id>��<end-commit-id>֮��(�����䣬����start-commit-id)���ύcherry-pick����ǰ��֧��

# ���У�<start-commit-id>��<end-commit-id>ֻ��Ҫcommit-id��ǰ6λ���ɣ�����<start-commit-id>��ʱ���ϱ�������<end-commit-id>

# ע�����Ϻϲ�����Ҫ�ֶ�push���롣
```

## ʹ�õĳ���

- ��Ū���֧���ύ�ƶ�����ȷ�ĵط�
- ��������֧���ύ��ӵ����ڵķ�֧

## ʾ��

���磬������� learn-cherry-pick �����֧�ϵĵڶ����ύ��ȡ������Ȼ����ӵ� master �ϡ�

![](img/git-cherry-pick1.png)

��ͼ�Ҳ��ֿ����ҵ���������ȡ commit �� commit id Ϊ `c3f0d9a`���� master ��֧��ִ�� cherry-pick

```shell
$ git checkout master
$ git pull
$ git cherry-pick c3f0d9a
[master 573066e] add a new line
 Date: Sun Jan 14 18:50:20 2018 +0800
 1 file changed, 1 insertion(+)
```

![](img/git-cherry-pick2.png)

cherry-pick ������Ҳ�ǿ��ܻ������ͻ�ģ������ͻ���� add��Ȼ��ʹ�� `git cherry-pick --continue`��
�������� cherry-pick��ʹ�� `git cherry-pick --abort`

```shell
$ git cherry-pick --continue
$ git cherry-pick --quit
$ git cherry-pick --abort
```



# ���˲���

```shell
$ git reset --hard HEAD^ # ���˵���һ���汾
$ git reset --hard ahdhs1(commit_id) # ���˵�ĳ���汾
$ git checkout -- file #�����޸ĵ��ļ�(����ļ����뵽���ݴ���������˵��ݴ����ģ�����ļ����뵽�˰汾�⣬��ԭ������汾��֮���״̬)
$ git reset HEAD file # �����ݴ������ļ��޸ĵ�������
```



# �ϲ���֧

��������������dev��֧�ϣ��տ�������Ŀ��ִ������������

```shell
$ git  add .
$ git  commit -m '�ύ�ı�ע��Ϣ'
$ git  push -u origin dev
```

�����뽫dev��֧�ϲ���master��֧��

```shell
# 1�������л���master��֧��
$ git checkout master

# 2.����Ƕ��˿����Ļ� ��Ҫ��Զ��master�ϵĴ���pull����
$ git pull origin master

# 3.Ȼ���dev��֧�Ĵ���ϲ���master��
$ git merge dev

# 4.�����ͻ
��ͬ��֧�����ݻᱻ�ֿ���ʾ
<<<<<<<<<<
yyy
==========
xxx
>>>>>>>>>>

Ϊ�˽����ͻ����Ҫ�Ѳ�Ҫ��ȥ������Ҫ�ı�����������ͷ�͵Ⱥű���ɾ��������󱣴��˳���
ִ�� git merge --continue���������µ�commit��merge������

# 4.Ȼ��鿴״̬��ִ���ύ����
git status

On branch master
Your branch is ahead of 'origin/master' by 12 commits.
  (use "git push" to publish your local commits)
nothing to commit, working tree clean

# �������˼��������12��commit����Ҫpush��Զ��master�� 
# ���ִ�������ύ����
$ git push origin master
```

����

```shell
# ����Զ�̷�֧�б�
$ git remote update origin --prune

# �鿴���з�֧
$ git branch -a

# ɾ��Զ�̷�֧Chapater6
$ git push origin --delete Chapater6

# ɾ�����ط�֧ Chapater6
$ git branch -d  Chapater6
```



# ��������

`error : warning: LF will be replaced by CRLF`

```shell
# windows�еĻ��з�Ϊ CRLF�� ����linux�µĻ��з�ΪLF��������ִ��add . ʱ������ʾ

$ rm -rf .git  # ɾ��.git
$ git config --global core.autocrlf false  # �����Զ�ת��  
```



# git/github ɾ��

`git rm`		ɾ���ļ�

`git checkout --fileName`  �ӱ���/Զ�ֿ̲�ָ��ļ�������


github -> settings -> �ײ� Delete this repository
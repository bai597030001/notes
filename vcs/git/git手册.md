# git�������

![](img/git1.png)



- Workspace:��������ִ��`git add *`����ͰѸĶ��ύ�����ݴ�����ִ��`git pull`���Զ�ֿ̲������������ǰ��֧���ϲ���ִ��`git checkout [branch-name]`�л���֧
- Index:�ݴ�����ִ��`git commit -m '˵��'` ����ͰѸĶ��ύ���˲ֿ�������ǰ��֧��
- Repository:�ֿ������򱾵زֿ⣩��ִ��`git push origin master`�ύ��Զ�ֿ̲⣬ִ��`git clone ��ַ`����¡Զ�ֿ̲⵽����
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
�������û���κ��±仯����������д��һ��commit���ύ��Ϣ
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
$ git branch ������֧
$ git branch -b �������л����½��ķ�֧��
$ git checkout �л���֧
$ git branch �鿴��֧�б�
$ git branch -v �鿴���з�֧�����һ�β���
$ git branch -vv �鿴��ǰ��֧
$ git brabch -b ��֧�� origin/��֧�� ����Զ�̷�֧������
$ git branch --merged �鿴��ķ�֧�͵�ǰ��֧�ϲ����ķ�֧
$ git branch --no-merged �鿴δ�뵱ǰ��֧�ϲ��ķ�֧
$ git branch -d ��֧�� ɾ�����ط�֧
$ git branch -D ��֧�� ǿ��ɾ����֧
$ git branch origin :��֧�� ɾ��Զ���ֿ��֧
$ git merge ��֧�� �ϲ���֧����ǰ��֧��
```



# �ݴ�

```shell
$ git stash �ݴ浱ǰ�޸�
$ git stash apply �ָ������һ���ݴ�
$ git stash pop �ָ��ݴ沢ɾ���ݴ��¼
$ git stash list �鿴�ݴ��б�
$ git stash drop �ݴ���(����stash@{0}) �Ƴ�ĳ���ݴ�
$ git stash clear ����ݴ�
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



# ���˲���

```shell
$ git reset --hard HEAD^ ���˵���һ���汾
$ git reset --hard ahdhs1(commit_id) ���˵�ĳ���汾
$ git checkout -- file�����޸ĵ��ļ�(����ļ����뵽���ݴ���������˵��ݴ����ģ�����ļ����뵽�˰汾�⣬��ԭ������汾��֮���״̬)
$ git reset HEAD file �����ݴ������ļ��޸ĵ�������
```



# ��������

`error : warning: LF will be replaced by CRLF`

```shell
# windows�еĻ��з�Ϊ CRLF�� ����linux�µĻ��з�ΪLF��������ִ��add . ʱ������ʾ

$ rm -rf .git  // ɾ��.git
$ git config --global core.autocrlf false  //�����Զ�ת��  
```



# git/github ɾ��

`git rm`		ɾ���ļ�

`git checkout --fileName`  �ӱ���/Զ�ֿ̲�ָ��ļ�������


github -> settings -> �ײ� Delete this repository
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



# 1.git���ز���

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
git reflog ������¼���ÿһ������
```



# 2.gitԶ�̲���

## 2.1�ύ���زֿ���뵽Զ�ֿ̲�

github�½� `new repository`

Repository name: �ֿ�����

Description(��ѡ): �ֿ���������

Public, Private : �ֿ�Ȩ�ޣ���������˽�л�ָ�������ߣ�

Initialize this repository with a README: ���һ��README.md		-> ���鹴��

gitignore: ����Ҫ���а汾����Ĳֿ����ͣ���Ӧ�����ļ�.gitignore

license: ֤�����ͣ���Ӧ�����ļ�LICENSE

��� Clone or dowload �����һ����ַ, copy �����ַ���á�

```
https://github.com/bai597030001/network_optimize.git
git@github.com:bai597030001/network_optimize.git
```



### 2.1.1�����Ѿ�������Դ���ļ�/�ļ��в����Ѿ����˴���,�����زֿ��Ѿ�����



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



### 2.1.2���زֿ⻹û�д���

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

## 2.2������Զ�̲�һ�����ͬ��

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



# 3.git��֧����

[ԭ������](https://www.cnblogs.com/videring/articles/6962005.html)

# 4.git��ǩ

# 5.��������

5.1 `error : warning: LF will be replaced by CRLF`

```shell
# windows�еĻ��з�Ϊ CRLF�� ����linux�µĻ��з�ΪLF��������ִ��add . ʱ������ʾ

$ rm -rf .git  // ɾ��.git
$ git config --global core.autocrlf false  //�����Զ�ת��  
```



# 6.git/github ɾ��

git rm		ɾ���ļ�

git checkout --fileName  �ӱ���/Զ�ֿ̲�ָ��ļ�������


github -> settings -> �ײ� Delete this repository
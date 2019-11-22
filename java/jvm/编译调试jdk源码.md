#  源码编译OpenJDK8

- 下载Ubuntu
- 下载OpenJdk源码
- 下载Boot JDK，一般要比当前要编译的版本低
- 安装必要的依赖
- configure && make



## 1.准备vmware虚拟机环境

- ubuntu14.04



## 2.下载OpenJdk源码

据原文说法，[OpenJDK](http://openjdk.java.net/) 使用Mercurial进行版本管理。另外一个名叫[AdoptOpenJDK project](https://adoptopenjdk.net/about.html).提供了OpenJDK的镜像，可以让我们用git下载。

站点的官网如下：https://adoptopenjdk.net/about.html 

主页上说他们的目标就是：

Provide a reliable source of OpenJDK binaries for all platforms, for the long term future.

据我的使用体验来说，之前编译过一次OpenJDK，各种报错，各种改源码才能编译通过。这次确实编译很顺，代码一句没改。

看起来，代码还是比较可靠的。

不扯别的了，直接clone搞下来吧，我这边是直接在/home/ckl目录下执行的shell：

```shell
git clone --depth 1 -b master https://github.com/AdoptOpenJDK/openjdk-jdk8u.git
```



## 3.下载Boot JDK

要编译jdk，需要先有jdk才行，而且版本要低于编译的版本，这里用jdk7



## 4.修改环境变量（/ect/profile） 

```shell
export JAVA_HOME=/usr/local/jdk1.7.0_80
export JRE_HOME=${JAVA_HOME}/jre
export CLASSPATH=.:${JAVA_HOME}/lib:${JRE_HOME}/lib
export PATH=${JAVA_HOME}/bin:$PATH
```



## 5.安装依赖

```
sudo apt install \
        libx11-dev \
        libxext-dev \
        libxrender-dev \
        libxtst-dev \
        libxt-dev \
        libcups2-dev \
        libfreetype6-dev \
        libasound2-dev
```



## 6.配置脚本、编译

在源码目录新建脚本build.sh：

```sh
bash ./configure --with-target-bits=64 --with-boot-jdk=/usr/local/jdk1.7.0_80/ --with-debug-level=slowdebug --enable-debug-symbols ZIP_DEBUGINFO_FILES=0
make all ZIP_DEBUGINFO_FILES=0
```

给build.sh增加可执行权限并执行：

```shell
chmod +x build.sh
./build.sh
```



# 用NetBeans调试JVM代码 

- 下载NetBeans
- 配置OpenJdk工程
- 配置Java工程
- Debug OpenJdk（即虚拟机源码）
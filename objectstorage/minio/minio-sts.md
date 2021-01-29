# minio sts

## docker安装启动minio

```shell
$ docker run -di -p 9000:9000 --name minio \
  -d --restart=always \ 
  -e "MINIO_ACCESS_KEY=minioadmin" \
  -e "MINIO_SECRET_KEY=minioadmin" \
  -v /mnt/monioData:/data \
  -v /mnt/monioConfig:/root/.minio \
  minio/minio server /data
```

浏览器输入 `http://10.201.4.70:9000` 使用minioadmin/minioadmin登录

## 使用minio client

```shell
$ docker pull minio/mc
$ docker run -it --entrypoint=/bin/sh minio/mc
```

输入mc

输入mc config



关联minio-serve

```shell
$ mc config host add minio http://10.0.98.222:9000 minioadmin minioadmin --api s3v4

$ mc admin info server minio
```

查看之前创建的bucket和上传的文件

```shell
$ mc ls minio
$ mc ls minio/yzl-storage-test
```

## 添加用户

```shell
# 添加用户
$ mc admin user add minio imaUser imaUserimaUser

# 禁用用户
$ mc admin user disable minio imaUser

# 启用用户
$ mc admin user enable minio imaUser

# 删除用户
$ mc admin user remove minio imaUser

# 用户列表
$ mc admin user list --json minio

# 某个用户详情
$ mc admin user info minio someuser
```

## 分配用户

```shell
$ mc admin user add minio imaUser imaUserimaUser

$ mc admin group add minio newgroup newuser

$ mc admin group info minio newgroup 

$ mc admin policy set minio readwrite group =newgroup 
```

使用imaUser/imaUserimaUser登录web端

## 赋予权限

```SHELL
# 给某个用户赋予权限
$ mc admin policy set minio readwrite user=imaUser

# 给某个用户组赋予权限
$ mc admin policy set minio readwrite group =newgroup
```


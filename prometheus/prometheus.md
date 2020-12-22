# prometheus监控

[官网](https://www.prometheus.wang/)

[github](https://github.com/prometheus/prometheus)

[链接](https://yunlzheng.gitbook.io/prometheus-book/)

[链接1](https://songjiayang.gitbooks.io/prometheus/content/)

[链接2](https://sb-woms.gitbook.io/sb/prometheus/springboot-prometheus-05)

[链接3](https://www.prometheus.wang/)

## 安装部署

prometheus.yaml

```yaml
# my global config
global:
  scrape_interval:     15s # Set the scrape interval to every 15 seconds. Default is every 1 minute.
  evaluation_interval: 15s # Evaluate rules every 15 seconds. The default is every 1 minute.
  # scrape_timeout is set to the global default (10s).

# Alertmanager configuration
alerting:
  alertmanagers:
  - static_configs:
    - targets:
      # - alertmanager:9093

# Load rules once and periodically evaluate them according to the global 'evaluation_interval'.
rule_files:
  # - "first_rules.yml"
  # - "second_rules.yml"

# A scrape configuration containing exactly one endpoint to scrape:
# Here it's Prometheus itself.
scrape_configs:
  # The job name is added as a label `job=<job_name>` to any timeseries scraped from this config.
  - job_name: 'prometheus'

    # metrics_path defaults to '/metrics'
    # scheme defaults to 'http'.

    static_configs:
    - targets: ['localhost:9090']
  
  # 采集node exporter监控数据
  - job_name: 'node'
    static_configs:
    - targets: ['localhost:9100']

  - job_name: 'example'
    metrics_path: '/actuator/prometheus'
    static_configs:
    - targets: ['localhost:10001']
  
  - job_name: 'jxm'
    static_configs:
    - targets: ['localhost:9200']
```



centos下grafana安装部署

```shell
$ wget https://dl.grafana.com/oss/release/grafana-7.3.5-1.x86_64.rpm
$ sudo rpm -ivh grafana-7.3.5-1.x86_64.rpm
$ grafana-server -config /etc/grafana/grafana.ini -homepath /usr/share/grafana
```

[http://localhost:3000](http://localhost:3000/)。 默认的账户为admin：admin

## 数据模型

### TSDB

### 格式

```txt
<metric name>{<label name>=<label value>, ...}
```

### Metrics类型

`Counter`: 一种累加的`metric`，如请求的个数，结束的任务数，出现的错误数等

`Gauge`: 常规的metric,如温度，可任意加减。其为瞬时的，与时间没有关系的，可以任意变化的数据。

`Histogram`: 柱状图，用于观察结果采样，分组及统计，如：请求持续时间，响应大小。其主要用于表示一段时间内对数据的采样，并能够对其指定区间及总数进行统计。**根据统计区间计算**

`Summary`: 类似`Histogram`，用于表示一段时间内数据采样结果，其直接存储quantile数据，而不是根据统计区间计算出来的。**不需要计算，直接存储结果**

## PromQL



## 配置

`global`: 主要有四个属性

- `scrape_interval`: 拉取 targets 的默认时间间隔。
- `scrape_timeout`: 拉取一个 target 的超时时间。
- `evaluation_interval`: 执行 rules 的时间间隔。
- `external_labels`: 额外的属性，会添加到拉取的数据并存到数据库中。



## Exporter

在Prometheus的架构设计中，Prometheus Server并不直接服务监控特定的目标，其主要任务负责数据的收集，存储并且对外提供数据查询支持。因此为了能够能够监控到某些东西，如主机的CPU使用率，我们需要使用到Exporter。Prometheus周期性的从Exporter暴露的HTTP服务地址（通常是/metrics）拉取监控样本数据。

从上面的描述中可以看出Exporter可以是一个相对开放的概念，其可以是一个独立运行的程序独立于监控目标以外，也可以是直接内置在监控目标中。只要能够向Prometheus提供标准格式的监控样本数据即可。

Prometheus 社区已经提供了很多 exporter, 详情请参考[这里](https://prometheus.io/docs/instrumenting/exporters/#exporters-and-integrations) 。



### node_exporter

为了能够采集到主机的运行指标如CPU, 内存，磁盘等信息。我们可以使用[Node Exporter](https://github.com/prometheus/node_exporter)。

> Node Exporter同样采用Golang编写，并且不存在任何的第三方依赖，只需要下载，解压即可运行。可以从https://prometheus.io/download/获取最新的node exporter版本的二进制包。

```shell
$ curl -OL https://github.com/prometheus/node_exporter/releases/download/v0.17.0/node_exporter-0.17.0.linux-amd64.tar.gz
$ tar -xzf node_exporter-0.17.0.linux-amd64.tar.gz
$ cd node_exporter-0.17.0.linux-amd64/
$ mv node_exporter /usr/local/bin/
```



vim /etc/rc.d/init.d/node_exporter

```bash
#!/bin/bash
#
# /etc/rc.d/init.d/node_exporter
#
#  Prometheus node exporter
#
#  description: Prometheus node exporter
#  processname: node_exporter

# Source function library.
. /etc/rc.d/init.d/functions

PROGNAME=node_exporter
PROG=/opt/prometheus/
USER=root
LOGFILE=/var/log/prometheus.log
LOCKFILE=/var/run/.pid

start() {
    echo -n "Starting : "
    cd /opt/prometheus/
    daemon --user root --pidfile="" " &> &"
    echo $(pidofproc $PROGNAME) >$LOCKFILE
    echo
}

stop() {
    echo -n "Shutting down $PROGNAME: "
    killproc $PROGNAME
    rm -f $LOCKFILE
    echo
}


case "$1" in
    start)
    start
    ;;
    stop)
    stop
    ;;
    status)
    status 
    ;;
    restart)
    stop
    start
    ;;
    reload)
    echo "Sending SIGHUP to $PROGNAME"
    kill -SIGHUP $(pidofproc $PROGNAME)#!/bin/bash
    ;;
    *)
        echo "Usage: service node_exporter {start|stop|status|reload|restart}"
        exit 1
    ;;
esac
```

运行node exporter

```bash
$ ./node_exporter
```

启动成功后，查看端口



### JMX Exporter

> JMX-Exporter 提供了两种用法:
>
> 1. **启动独立进程。** JVM 启动时指定参数，暴露 JMX 的 RMI 接口，JMX-Exporter 调用 RMI 获取 JVM 运行时状态数据，转换为 Prometheus metrics 格式，并暴露端口让 Prometheus 采集。
> 2. **JVM 进程内启动(in-process)。** JVM 启动时指定参数，通过 javaagent 的形式运行 JMX-Exporter 的 jar 包，进程内读取 JVM 运行时状态数据，转换为 Prometheus metrics 格式，并暴露端口让 Prometheus 采集。
>
> 官方不推荐使用第一种方式，一方面配置复杂，另一方面因为它需要一个单独的进程，而这个进程本身的监控又成了新的问题



jmx配置：（/home/baijd/prometheus/configuration.yml）

```yaml
---   
lowercaseOutputLabelNames: true
lowercaseOutputName: true
whitelistObjectNames: ["java.lang:type=OperatingSystem"]
blacklistObjectNames: []
rules:
  - pattern: 'java.lang<type=OperatingSystem><>(committed_virtual_memory|free_physical_memory|free_swap_space|total_physical_memory|total_swap_space)_size:'
    name: os_$1_bytes
    type: GAUGE
    attrNameSnakeCase: true
  - pattern: 'java.lang<type=OperatingSystem><>((?!process_cpu_time)\w+):'
    name: os_$1
    type: GAUGE
    attrNameSnakeCase: true
```

java应用启动命令：

```shell
$ java -javaagent:/home/baijd/prometheus/jmx_prometheus_javaagent-0.14.0.jar=9200:/home/baijd/prometheus/configuration.yml -jar example-0.0.1-SNAPSHOT.jar
```



然后，就可以添加 Grafana 监控面板：

jvm用的比较多的是 https://grafana.com/grafana/dashboards/8563 ，可以直接导入



## pushgateway 



## Alertmanager

告警能力在Prometheus的架构中被划分为两个部分，<font color=#00dd00>在Prometheus Server中定义告警规则</font>以及<font color=#00dd00>产生告警</font>，Alertmanager组件则用于处理这些由Prometheus产生的告警。Alertmanager即Prometheus体系中告警的统一处理中心。

Alertmanager提供了多种内置第三方告警通知方式，同时还提供了对Webhook通知的支持，通过Webhook用户可以完成对告警更多个性化的扩展



## Java客户端

```groovy
compile 'io.prometheus:simpleclient:0.5.0'
compile 'io.prometheus:simpleclient_hotspot:0.5.0'
compile 'io.prometheus:simpleclient_servlet:0.5.0'
compile 'io.prometheus:simpleclient_pushgateway:0.5.0'
compile 'io.prometheus:simpleclient_spring_web:0.5.0'
compile group: 'io.prometheus', name: 'simpleclient_spring_boot', version: '0.5.0' // springboot1.X
```

```xml
<!--springboot2.X-->
<dependency>
    <groupId>org.springframework.boot</groupId>
    <artifactId>spring-boot-starter-actuator</artifactId>
</dependency>
<dependency>
    <groupId>io.micrometer</groupId>
    <artifactId>micrometer-core</artifactId>
</dependency>
<dependency>
    <groupId>io.micrometer</groupId>
    <artifactId>micrometer-registry-prometheus</artifactId>
</dependency>
<dependency>
```


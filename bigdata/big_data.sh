export JAVA_HOME=/opt/jdk1.8.0_162
export SPARK_HOME=/opt/spark-2.3.0-bin-hadoop2.7
export HADOOP_HOME=/opt/hadoop-2.7.5
#export HADOOP_HOME=/opt/hadoop-3.0.0
export ZOOKEEPER_HOME=/opt/zookeeper-3.4.12
export KAFKA_HOME=/opt/confluent-4.0.0
export HBASE_HOME=/opt/hbase-1.2.6
export HADOOP_COMMON_LIB_NATIVE_DIR=$HADOOP_HOME/lib/native
export HADOOP_OPTS="-Djava.library.path=$HADOOP_HOME/lib:$HADOOP_COMMON_LIB_NATIVE_DIR"
export MYSQL_HOME=/opt/mysql-5.6.43-linux-glibc2.12-x86_64
export REDIS_HOME=/opt/redis-5.0.3
export KAFKA_HEAP_OPTS="-Xms8g -Xmx16g"
export DOTNET_HOME=/opt/dotnet-sdk-2.2.401-linux-x64

# PATH
export PATH+=:$JAVA_HOME/bin
export PATH+=:$SPARK_HOME/bin:$SPARK_HOME/sbin
export PATH+=:$HADOOP_HOME/bin:$HADOOP_HOME/sbin
export PATH+=:$ZOOKEEPER_HOME/bin
export PATH+=:$KAFKA_HOME/bin
export PATH+=:$HBASE_HOME/bin
export PATH+=:$MYSQL_HOME/bin
export PATH+=:$REDIS_HOME/src
export PATH+=:$DOTNET_HOME

# Zookeeper
export ZOO_LOG_DIR=/home/data/zookeeper/zkdatalog

# Hadoop
export HDFS_DATANODE_USER=root  
export HDFS_NAMENODE_USER=root  
export HDFS_SECONDARYNAMENODE_USER=root  
export HDFS_DATANODE_SECURE_USER=root
export HDFS_JOURNALNODE_USER=root
export HDFS_ZKFC_USER=root

# Spark
#export SPARK_MASTER_HOST=172.16.1.126
#export SPARK_WORKER_CORES=4 
#export SPARK_WORKER_MEMORY=16g
export HADOOP_CONF_DIR=$HADOOP_HOME/etc/hadoop
export SPARK_CLASSPATH=$HBASE_HOME/lib

export LD_LIBRARY_PATH+=/usr/local/lib64

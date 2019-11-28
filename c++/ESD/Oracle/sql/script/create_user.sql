第一步: 使用sys用户登录oracle
C:\Documents and Settings\Administrator>sqlplus
Enter user-name: sys as sysdba
Enter password: 
第二步:创建用户
SQL>create user test identified by test
default tablespace users
temporary tablespace temp;
第三步:给用户授权
SQL>grant resource,connect to test;
第四布:连接到test用户
connect test

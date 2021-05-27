# happy-chating

网络聊天室学习项目
受《linux高性能服务器编程》9.6节的启发，实现一个简单的聊天室，逐步丰富功能。

## 环境配置

本聊天室用到了sqlite，所以需要安装：

```bash
sudo apt-get install sqlite3  //安装sqlite3
sudo apt-get install libsqlite3-dev //安装对应的api
```

以上两项都安装后才可以使用

server文件夹中是服务器端代码，client文件夹中是客户端代码

## 使用方法

### 服务器

1. make
2. ./server

### 客户端

1. make
2. ./client 127.0.0.1 8000

# websocket服务器

# 编译工具 Cmake 开发工具Clion

### 项目结构
```
|-- websocket_server_win
    |-- |includes                 存放头文件
    |-- |lib                      工具库文件
    |-- |src                      存放源文件
    |-- |web                      测试用的HTML页面
    |-- main.cpp                  入口文件
    |-- websocket_server_win.exe  编译之后的文件，可以直接运行
```

### 项目设计
项目采用`socket`进行网络之间的进程通信，传输层采用TCP协议。  

- 客户端握手  
收到客户端的websocket连接请求之后，进行websocket握手，客户端将发送一个相当标准的 HTTP 请求：
```text
GET /chat HTTP/1.1
Host: example.com:8000
Upgrade: websocket
Connection: Upgrade
Sec-WebSocket-Key: dGhlIHNhbXBsZSBub25jZQ==
Sec-WebSocket-Version: 13
```
`HTTP 版本必须是 1.1 或更高，方法必须是GET`  
- 服务器握手响应  
取出客户端的`Sec-WebSocket-Key`的值，然后拼接上"`258EAFA5-E914-47DA-95CA-C5AB0DC85B11`"这段字符串，把结果用SHA-1编码，再用base64编码一次，把编码后的值放在`Sec-WebSocket-Accept`字段，并给客户端响应。
```text
HTTP/1.1 101 Switching Protocols
Upgrade: websocket
Connection: Upgrade
Sec-WebSocket-Accept: s3pPLMBiTxaQ9kYGzzhZRbK+xOo=
```
- 接收消息

- 解码消息
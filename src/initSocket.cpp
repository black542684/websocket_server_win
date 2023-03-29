//
// Created by Lenovo on 2023/3/28.
//
#include "../includes/initSocket.h"

void error_die(const char* str) {
    perror(str);
    exit(1);
}

SOCKET initSocket(unsigned short* port) {
    // 1. 网络通信初始化
    WSADATA data;
    int result = WSAStartup(
            MAKEWORD(1, 1),
            &data
    );
    if (result) { // 返回0表示成功，不为0表示失败
        error_die("WSAStartup");
    }

    // 2.创建套接字
    SOCKET server_socket = socket(
            PF_INET, // 套接字类型-IPV4
            SOCK_STREAM, // 数据流-TCP
            IPPROTO_TCP  // 协议
    );
    if (server_socket == -1) {
        // 打印错误提示并结束程序
        error_die("创建套接字");
    }

    // 3.设置端口可复用
    int opt = 1;
    result = setsockopt(
            server_socket,
            SOL_SOCKET,
            SO_REUSEADDR,
            (const char*)&opt,
            sizeof(opt)
            );
    if (result == -1) {
        error_die("设置socket");
    }

    // 4.配置服务器端的网络地址
    SOCKADDR_IN server_addr;
    memset(&server_addr, 0, sizeof(server_addr)); // 初始化为0
    server_addr.sin_family = AF_INET;// 套接字类型
    server_addr.sin_port = htons(*port); // 端口-小端转大端
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY); // ip地址-127.0.0.1  小端转大端



    // 5.绑定套接字
    result = ::bind(server_socket, (SOCKADDR*)&server_addr, sizeof(server_addr));
    if (result < 0) {
        error_die("绑定套接字");
    }

    // 6.动态分配一个端口
    if (*port == 0) {
        int nameLen = sizeof(server_addr);// 地址缓冲区长度
        // 获取一个套接口的本地名字
        result = getsockname(
                server_socket, // 标识一个已捆绑套接口的描述符号
                (SOCKADDR*)&server_addr, // 接收套接口的地址
                &nameLen // 地址缓冲区长度-字节
        );

        if (result < 0) {
            error_die("获取套接字信息");
        }
        *port = server_addr.sin_port;
    }

    // 7.创建监听队列
    if (listen(server_socket, 5) < 0) {
        error_die("监听套接字");
    }

    return server_socket;
}

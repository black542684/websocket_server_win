//
// Created by Lenovo on 2023/3/28.
//
#include <iostream>
#include <string>
#include "../includes/websocket.h"
#include "../includes/utils.h"
#include "../lib/sha1.hpp"
#include <map>


unsigned WINAPI accept_request(void* arg) {
    auto client = (SOCKET)arg; // 客户端套接字
    std::string request_line; // 请求行
    std::string buff; // 缓冲区
    char* outer_ptr = nullptr;

    std::map<std::string, std::string> request_head;  // 请求头

    // 获取请求行
    get_line(client, request_line);
    // 循环获取所有的请求头数据
    while (true) {
        get_line(client, buff);
        if (buff.length() == 0) {
            break;
        }

        // 以 : 分割字符串  (Content-tye: text/plain)
        char* tmp = strtok_s(
                (char *)buff.c_str(),
                ":",
                &outer_ptr
                );
        std::string key(tmp);
        tmp = strtok_s(nullptr, ":", &outer_ptr);
        std::string value(tmp);

        delete_space(key); // 去除两端空格
        delete_space(value); // 去除两端空格

        // 保存请求头信息
        if (!key.empty() && !value.empty()) {
            request_head.emplace(key, value);
        }

        buff.clear();
    }

    /**
     * 获取客户端发送的Sec-WebSocket-Key的值，
     * 然后用SHA1编码，再用base64编码一次
     */

    std::cout << "Sec-WebSocket-Key: "<< request_head["Sec-WebSocket-Key"] << std::endl;

    char hex[SHA1_HEX_SIZE];
    char base64[SHA1_BASE64_SIZE];

    // 把客户发送的 Sec-WebSocket-Key 和 "258EAFA5-E914-47DA-95CA-C5AB0DC85B11" 连接起来
    // 把结果用SHA-1编码，再用base64编码一次，就可以了
    std::string websocket_key = request_head["Sec-WebSocket-Key"];
    std::string magic = "258EAFA5-E914-47DA-95CA-C5AB0DC85B11";
    websocket_key += magic;

    sha1(websocket_key.c_str())
    .finalize()
    .print_hex(hex)
    .print_base64(base64);

    std::cout << "Sec-WebSocket-Accept: "<< base64 << std::endl;

    // 当服务器收到握手请求时，它应该发回一个特殊的响应，表明协议将从 HTTP 变为 WebSocket
    buff = "HTTP/1.1 101 Switching Protocols\r\n";
    send(client, buff.c_str(), buff.length(), 0);
    buff = "Upgrade: websocket\r\n";
    send(client, buff.c_str(), buff.length(), 0);
    buff = "Connection: Upgrade\r\n";
    send(client, buff.c_str(), buff.length(), 0);

    buff = "Sec-WebSocket-Accept: ";
    buff += base64;
    buff += "\r\n";
    send(client, buff.c_str(), buff.length(), 0);

    buff = "\r\n";
    send(client, buff.c_str(), buff.length(), 0);

    printf("握手结束\n");
    handleMessage(client);


    std::cout << "结束" << std::endl;
    return 0;
}


void handleMessage(SOCKET client) {
    printf("开始接收消息...\n");
    char c = '\0'; // 保存读取到的一个字符
    while (true) {
        int n = recv(client, &c, 1, 0);
        if (n > 0) {
            char s[10];
            itoa(c, s, 2);
            s[8] = '\0';
            printf("%s ", s);
        } else {
            break;
        }
    }
}
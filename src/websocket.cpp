//
// Created by Lenovo on 2023/3/28.
//
#include <iostream>
#include <string>
#include "../includes/websocket.h"
#include "../includes/utils.h"
#include "../lib/sha1.hpp"
#include <map>

using namespace std;


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

    // std::cout << "Sec-WebSocket-Key: "<< request_head["Sec-WebSocket-Key"] << std::endl;

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

    // std::cout << "Sec-WebSocket-Accept: "<< base64 << std::endl;

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

    printf("握手结束，开始正式通讯！\n");

    while (true) {
        handleMessage(client);
    }


    std::cout << "结束" << std::endl;
    return 0;
}


void handleMessage(SOCKET client) {
    // printf("开始接收消息...\n");
    char c = '\0'; // 保存读取到的一个字符

    recv(client, &c, 1, 0); // 读取第一个字节
    // 消息类型
    char opcode = c & (char)0x0F;
    parintMessageType(opcode);

    // 获取消息长度
    recv(client, &c, 1, 0);
    long long length = getMessageLength(c, client);

    // 获取解码消息的掩码字节-4个字节
    char mask[4] = {0};
    getMask(mask, client);

    // 获取消息体
    encodeMessageBody(mask, length, client);
}

void parintMessageType(char type) {
    switch (type) {
        case 0x0:
            cout << "延续帧" << endl;
            break;
        case 0x01:
            cout << "文本帧" << endl;
            break;
        case 0x02:
            cout << "二进制帧" << endl;
            break;
        case 0x03:
        case 0x04:
        case 0x05:
        case 0x06:
        case 0x07:
            cout << "保留给更多的非控制帧" << endl;
            break;
        case 0x08:
            cout << "连接关闭" << endl;
            break;
        case 0x09:
            cout << "ping" << endl;
            break;
        case 0x0A:
            cout << "pong" << endl;
            break;
        case 0x0B:
        case 0x0C:
        case 0x0D:
        case 0x0E:
        case 0x0F:
            cout << "为进一步的控制帧保留" << endl;
            break;

    }
}

long long getMessageLength(unsigned char len,  SOCKET client) {
    /**
     * 第二个字节
     * 第 1 位定义了是否"Payload data"进行了掩码计算，
     * 如果值设置为 1，那么在 Masking-Key 字段中会有一个掩码密钥，
     * 并且它可以用来进行"Payload data"的去掩码计算
     *
     * 剩下的 2 - 8 位组成一个二进制数：
     * 如果这个值在 0 与 125 之间，那么这个值就是消息的长度。
     * 如果这个值是 126，那么接下来的 2 个字节（16 位无符号整数）是消息长度。
     * 如果这个值是 127，那么接下来的 8 个字节（64 位无符号整数）是消息长度。
     */
    len = len & 127;
    long long length = 0;

    char c = '\0'; // 保存读取到的一个字符
    if (len <= 125) {
        length = len;
    } else if (len == 126) {
        recv(client, &c, 1, 0);
        length += c;
        length << 8;

        recv(client, &c, 1, 0);
        length += c;
    } else if (len == 127) {
        recv(client, &c, 1, 0);
        length += c;
        length << 8;

        recv(client, &c, 1, 0);
        length += c;
        length << 8;

        recv(client, &c, 1, 0);
        length += c;
        length << 8;

        recv(client, &c, 1, 0);
        length += c;
        length << 8;

        recv(client, &c, 1, 0);
        length += c;
        length << 8;

        recv(client, &c, 1, 0);
        length += c;
        length << 8;

        recv(client, &c, 1, 0);
        length += c;
        length << 8;

        recv(client, &c, 1, 0);
        length += c;
    }


    ::printf("消息长度: %lld \n", length);
    return length;
}

void getMask(char* mask, SOCKET client) {
    char c = '\0';
    for (int i = 0; i < 4; i++) {
        recv(client, &c, 1, 0);
        mask[i] = c;
    }
}

void encodeMessageBody(const char* mask, long long len, SOCKET client) {
    char* encoded = new char[len]; // 存放未解码的消息
    std::string decoded; // 存放解码后的消息
    char c = '\0';

    // 获取消息体
    for (int i = 0; i < len; i++) {
        recv(client, &c, 1, 0);
        encoded[i] = c;
    }

    // 对消息进行解码
    for (int i = 0; i < len; i++) {
        decoded.push_back(encoded[i] ^ mask[i % 4]);
    }

    cout << "客户端: " << decoded << endl;
}
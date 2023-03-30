//
// Created by Lenovo on 2023/3/28.
//

#ifndef WEBSOCKET_SERVER_WIN_WEBSOCKET_H
#define WEBSOCKET_SERVER_WIN_WEBSOCKET_H

#include <WinSock2.h>
#include <process.h>
// 处理客户端请求的线程函数
unsigned WINAPI accept_request(void* arg);

// 和客户端完成握手之后，接收和处理客户端的消息
void handleMessage(SOCKET client);

// 获取接收消息的类型
void parintMessageType(char type);

// 获取消息长度
long long getMessageLength(unsigned char len, SOCKET client);

// 获取解码消息的掩码
void getMask(char* mask, SOCKET client);

// 获取消息体长度并且解码
void encodeMessageBody(const char* mask, long long len, SOCKET client);

#endif //WEBSOCKET_SERVER_WIN_WEBSOCKET_H

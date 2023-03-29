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
#endif //WEBSOCKET_SERVER_WIN_WEBSOCKET_H

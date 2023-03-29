//
// Created by Lenovo on 2023/3/28.
//

#ifndef WEBSOCKET_SERVER_WIN_INITSOCKET_H
#define WEBSOCKET_SERVER_WIN_INITSOCKET_H
#include <WinSock2.h>
#pragma comment(lib, "Ws2_32.lib")
void error_die(const char* str);
SOCKET initSocket(unsigned short* port);
#endif //WEBSOCKET_SERVER_WIN_INITSOCKET_H

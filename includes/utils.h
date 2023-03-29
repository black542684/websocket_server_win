//
// Created by Lenovo on 2023/3/28.
//

#ifndef WEBSOCKET_SERVER_WIN_UTILS_H
#define WEBSOCKET_SERVER_WIN_UTILS_H
#include <WinSock2.h>
#include <string>
/*
* 读取请求的一行
*/
int get_line(SOCKET sock, std::string& buff);
/**
 * 去除字符串两端空格
 * @param s
 */
void delete_space(std::string& s);
#endif //WEBSOCKET_SERVER_WIN_UTILS_H

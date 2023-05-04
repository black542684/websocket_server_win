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

/**
 * 解析请求行
 * @param client  客户端
 * @param method  请求方法
 * @param path    请求路径
 * @param version 协议版本
 */
void formatRequestLine(SOCKET client, std::string& method, std::string& path, std::string& version);
#endif //WEBSOCKET_SERVER_WIN_UTILS_H

//
// Created by Lenovo on 2023/3/28.
//
#include "../includes/utils.h"

int get_line(SOCKET sock, std::string& buff) {
    char c = '\0'; // 保存读取到的一个字符
    int i = 0; // 下标-  buff[i]

    while (c != '\n') {
        // 从缓冲区中读取一个字符
        int n = recv(sock, &c, 1, 0);
        // recv函数返回其实际copy的字节数，大于0表示读取到了字符
        if (n > 0) {
            // 由于换行符是\r\n 这种格式，需要判断
            if (c == '\r') {
                // 预先读取，查看下一个字符是不是\n  但不会使套接字接收队列中的数据减少
                n = recv(sock, &c, 1, MSG_PEEK);
                if (n > 0 && c == '\n') {
                    n = recv(sock, &c, 1, 0); // 正常读取，下一个 \n
                }
                else {
                    c = '\n';
                }
            }
            if (c != '\r' && c != '\n') {
                i++;
                buff.push_back(c);
            }

        }
        else {
            c = '\n';
        }
    }

    return i;
}

void delete_space(std::string& s)
{
    if (s.empty())
    {
        return;
    }
    s.erase(0, s.find_first_not_of(" "));
    s.erase(s.find_last_not_of(" ") + 1);
}

void formatRequestLine(SOCKET client, std::string& method, std::string& path, std::string& version) {
    std::string request_line; // 请求行
    char* outer_ptr = nullptr;

    get_line(client, request_line); // 获取请求行

    // 以‘ ’分割请求行
    char* tmp = strtok_s(
            (char *)request_line.c_str(),
            " ",
            &outer_ptr
    );
    method = tmp;

    tmp = strtok_s(nullptr, " ", &outer_ptr);
    path = tmp;

    tmp = strtok_s(nullptr, " ", &outer_ptr);
    version = tmp;
}
#include <iostream>
#include "includes/initSocket.h"
#include "includes/websocket.h"

using namespace std;

int main() {


    unsigned short port = 8081; // 监听的端口号
    DWORD	threadId; // 线程ID

    SOCKET sock = initSocket(&port);
    cout << "websocket服务器初始化，正在监听" << port<< "端口..." << endl;

    struct sockaddr_in client_addr{};
    int client_addr_len  = sizeof(client_addr);

    // 循环等待客户端套接字
    while (true) {
        SOCKET client_sock = accept(sock, (struct sockaddr *)&client_addr, &client_addr_len);
        if (client_sock == -1) {
            error_die("接收客户端失败");
        }

        // 创建一个新线程处理客户端请求
        _beginthreadex(
                NULL,
                0,
                accept_request,
                (void*)client_sock,
                0,
                (unsigned*)&threadId
                );
    }

    // 关闭服务器套接字
    closesocket(sock);
    return 0;
}

#include <cstring>
#include <iostream>
using namespace std;
#include <arpa/inet.h>
#include <sys/un.h>
#include <unistd.h>

#define MAX_BUF_SIZE 1024

int main() {
    // 1.创建socket本地套接字
    int listen_fd = socket(AF_LOCAL, SOCK_STREAM, 0);
    if (-1 == listen_fd) {
        perror("socket");
        return -1;
    }

    // 2.绑定本地套接字文件
    struct sockaddr_un server_addr;
    // 地址族
    server_addr.sun_family = AF_LOCAL;
    // 套接字文件绑定了之后自动生成一个文件用于通信
    strcpy(server_addr.sun_path, "server.sock");

    int ret = bind(listen_fd, (struct sockaddr*)&server_addr, sizeof(server_addr));
    if (-1 == ret) {
        perror("bind");
        return -1;
    }

    // 3.监听
    ret = listen(listen_fd, 5);
    if (-1 == ret) {
        perror("listen");
        return -1;
    }

    // 4.等待客户端连接
    struct sockaddr_un client_addr;
    socklen_t client_addr_len = sizeof(client_addr);

    int connect_fd = accept(listen_fd, (struct sockaddr*)&client_addr, &client_addr_len);
    if (-1 == connect_fd) {
        perror("accept");
        return -1;
    }

    printf("client (socket filename : %s) has connected.\n", client_addr.sun_path);

    char buf[MAX_BUF_SIZE] = {0};
    // 5.开始通信
    while (1) {
        // 读
        bzero(buf, sizeof(buf));
        int len = recv(connect_fd, buf, sizeof(buf) - 1, 0);
        if (-1 == len) {
            if (errno == ECONNRESET)  // 报错处理
                goto CLOSE;
            perror("recv");
            return -1;
        }

        if (len > 0)
            printf("recv : %s", buf);
        else if (0 == len) {
        CLOSE:
            printf("client (socket filename : %s) has closed...\n", client_addr.sun_path);
            break;
        }

        // 写
        send(connect_fd, buf, strlen(buf), 0);
    }

    // 6.关闭连接
    close(connect_fd);
    close(listen_fd);

    return 0;
}

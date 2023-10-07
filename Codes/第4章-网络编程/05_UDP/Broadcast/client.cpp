#include <cstring>
#include <iostream>
using namespace std;
#include <arpa/inet.h>
#include <unistd.h>

#define MAX_BUF_SIZE 1024
#define MAX_IPV4_STRING 16

int main() {
    // 1.创建通信的socket套接字
    int socket_fd = socket(AF_INET, SOCK_DGRAM, 0);
    if (-1 == socket_fd) {
        perror("socket");
        return -1;
    }

    // 2.绑定端口信息，让发送方能够正确找到
    struct sockaddr_in client_addr;
    // 地址族
    client_addr.sin_family = AF_INET;
    // IP
    // inet_pton(AF_INET, "127.0.0.2", &client_addr.sin_addr.s_addr);  // 这行代码会出问题，但是我也不知道为什么
    client_addr.sin_addr.s_addr = INADDR_ANY;
    // 端口
    client_addr.sin_port = htons(10000);

    int ret = bind(socket_fd, (struct sockaddr*)&client_addr, sizeof(client_addr));
    if (-1 == ret) {
        perror("bind");
        return -1;
    }

    char buf[MAX_BUF_SIZE] = {0};

    // 2.开始通信
    while (1) {
        // 读数据
        recvfrom(socket_fd, buf, sizeof(buf) - 1, 0, nullptr, nullptr);
        printf("recv : %s", buf);
    }

    // 4.关闭套接字
    close(socket_fd);

    return 0;
}

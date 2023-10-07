#include <cstring>
#include <iostream>
using namespace std;
#include <arpa/inet.h>
#include <unistd.h>

#define MAX_BUF_SIZE 1024
#define MAX_IPV4_STRING 16

// 多播的IP地址
const char* Multicast_IP = "239.0.0.10";

int main() {
    // 1.创建通信的socket套接字
    int socket_fd = socket(AF_INET, SOCK_DGRAM, 0);
    if (-1 == socket_fd) {
        perror("socket");
        return -1;
    }

    // 加入多播组
    struct ip_mreq _optval;
    // 初始化
    _optval.imr_interface.s_addr = INADDR_ANY;
    inet_pton(AF_INET, Multicast_IP, &_optval.imr_multiaddr.s_addr);

    setsockopt(socket_fd, IPPROTO_IP, IP_ADD_MEMBERSHIP, &_optval, sizeof(_optval));

    // 2.绑定端口信息，让发送方能够正确找到
    struct sockaddr_in client_addr;
    // 地址族
    client_addr.sin_family = AF_INET;
    // IP
    // inet_pton(AF_INET, "127.0.0.2", &client_addr.sin_addr.s_addr);  // 和之前一样的问题
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

#include <cstring>
#include <iostream>
using namespace std;
#include <arpa/inet.h>
#include <unistd.h>

#define MAX_BUFFER_SIZE 1024
#define MAX_IPV4_STRING_SIZE 16

int main(int argc, char* const argv[]) {
    // 判断命令行参数
    if (argc < 3) {
        printf("usage: %s  <ip>  <port>.\n", argv[0]);
        return -1;
    }

    const char* server_ip = argv[1];
    const unsigned short server_port = atoi(argv[2]);

    // 1.创建socket套接字
    int listen_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (-1 == listen_fd) {
        perror("socket");
        return -1;
    }

    // 2.由于客户端需要主动连接，服务端需要绑定一个固定端口
    struct sockaddr_in server_addr;
    // 地址族
    server_addr.sin_family = AF_INET;
    // ip
    inet_pton(AF_INET, server_ip, &server_addr.sin_addr.s_addr);
    // 端口
    server_addr.sin_port = htons(server_port);

    int ret = bind(listen_fd, (struct sockaddr*)&server_addr, sizeof(server_addr));
    if (-1 == ret) {
        perror("bind");
        return -1;
    }

    printf("server has initalized.\n");

    // 3.开始监听
    ret = listen(listen_fd, 8);
    if (-1 == ret) {
        perror("listen");
        return -1;
    }

    // 4.接受客户端连接
    // 我们就简单的让客户端发送数据，服务端不断接受，并且是p2p
    struct sockaddr_in client_addr;
    socklen_t client_addr_len = sizeof(client_addr);

    int connect_fd = accept(listen_fd, (struct sockaddr*)&client_addr, &client_addr_len);
    if (-1 == connect_fd) {
        perror("accept");
        return -1;
    }

    // 获取客户端信息
    in_port_t client_port = ntohs(client_addr.sin_port);
    char client_ip[MAX_IPV4_STRING_SIZE] = {0};
    inet_ntop(AF_INET, &client_addr.sin_addr.s_addr, client_ip, sizeof(client_ip));

    char buf[MAX_BUFFER_SIZE] = {0};
    // 5.开始通信
    while (1) {
        bzero(buf, sizeof(buf));

        int len = recv(connect_fd, buf, 5, 0);
        if (-1 == len) {
            perror("recv");
            return -1;
        }

        if (len > 0)
            // printf("recv client(ip: %s,port:%d) message: %s", client_ip, client_port, buf);
            printf("%s", buf);
        else {  // 写端关闭
            printf("client(ip: %s,port:%d) has closed...\n", client_ip, client_port);
            break;
        }
    }

    // 6.关闭socket
    close(connect_fd);
    close(listen_fd);

    return 0;
}

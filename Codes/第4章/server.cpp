#include <cstring>
#include <iostream>
using namespace std;
#include <arpa/inet.h>
#include <unistd.h>

#define MAX_BUFFER_SIZE 1024
#define MAX_IPV4_SIZE 16

int main(int argc, char const* argv[]) {
    // 判断命令行参数
    if (argc < 3) {
        printf("usage: %s  <ip>  <port>\n", argv[0]);
        return -1;
    }

    // 存储IP和端口
    const char* server_ip = argv[0];
    unsigned short server_port = (unsigned short)atoi(argv[1]);

    // 1.创建socket套接字
    int listen_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (-1 == listen_fd) {
        perror("socket");
        return -1;
    }

    // 2.绑定socket地址让客户端方便连接
    struct sockaddr_in server_addr;
    // 地址族
    server_addr.sin_family = AF_INET;
    // IP
    inet_pton(AF_INET, server_ip, &server_addr.sin_addr.s_addr);
    // 端口
    server_addr.sin_port = htons(server_port);

    int ret = bind(listen_fd, (struct sockaddr*)&server_addr, sizeof(server_addr));
    if (-1 == ret) {
        perror("bind");
        return -1;
    }

    cout << "server has initialized." << endl;

    // 3.开始监听，并且设置请求队列的阈值
    ret = listen(listen_fd, 5);
    if (-1 == ret) {
        perror("listen");
        return -1;
    }

    // 4.接受连接，这里就处理一个客户端
    struct sockaddr_in client_addr;
    socklen_t client_addr_len = sizeof(client_addr);

    int connect_fd = accept(listen_fd, (struct sockaddr*)&client_addr, &client_addr_len);
    if (-1 == connect_fd) {
        perror("accpet");
        return -1;
    }

    // 处理并打印客户端的信息
    char client_ip[MAX_IPV4_SIZE] = {0};
    inet_ntop(AF_INET, &client_addr.sin_addr.s_addr, client_ip, sizeof(client_ip));

    unsigned short client_port = ntohs(client_addr.sin_port);

    printf("client(ip: %s,port: %d) has connected...\n", client_ip, client_port);

    char buf[MAX_BUFFER_SIZE] = {0};
    // 5.进行通信
    while (1) {
        bzero(buf, sizeof(buf));
        // 读
        int len = recv(connect_fd, buf, sizeof(buf) - 1, 0);
        if (-1 == len) {
            perror("recv");
            return -1;
        }
        if (len > 0)
            printf("recv data: %s", buf);
        else if (0 == len) {
            printf("client(ip: %s,port: %d) has closed...\n", client_ip, client_port);
            break;
        }

        // 写回去，返回大写的版本
        for (auto& ch : buf)
            ch = toupper(ch);

        send(connect_fd, buf, strlen(buf), 0);
    }

    // 6.关闭连接
    close(connect_fd);
    close(listen_fd);

    return 0;
}

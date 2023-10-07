#include <cstring>
#include <iostream>
using namespace std;
#include <arpa/inet.h>
#include <unistd.h>

#define IPV4_STRING_MAX 16
#define MAX_SIZE 1024

// 客户端从键盘录入数据
int main(int argc, char const* argv[]) {
    // 判断命令行参数个数
    if (argc != 3) {
        printf("usage : %s  <ip_address>  <port>\n", argv[0]);
        return -1;
    }

    const char* server_ip = argv[1];
    const short server_port = atoi(argv[2]);  // atoi()函数可以把合理的字符串转化为整数

    // 1.创建socket
    int listen_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (-1 == listen_fd) {
        perror("socket");
        return -1;
    }

    // 2.绑定IP和端口号
    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(server_port);  // 注意从主机字节序转换为网络字节序
    inet_pton(AF_INET, server_ip, &server_addr.sin_addr.s_addr);

    int ret = bind(listen_fd, (struct sockaddr*)&server_addr, sizeof(server_addr));
    if (-1 == ret) {
        perror("bind");
        return -1;
    }

    // 3.开始监听
    ret = listen(listen_fd, 8);
    if (-1 == ret) {
        perror("listen");
        return -1;
    }

    // 4.接受连接请求
    struct sockaddr_in client_addr;
    socklen_t client_addr_len = sizeof(client_addr);
    int connect_fd = accept(listen_fd, (struct sockaddr*)&client_addr, &client_addr_len);
    if (-1 == connect_fd) {
        perror("accept");
        return -1;
    }

    // 打印连接的客户端的信息
    char client_ip[IPV4_STRING_MAX] = {0};
    inet_ntop(AF_INET, &client_addr.sin_addr.s_addr, client_ip, sizeof(client_ip));
    printf("client has connected , ip : %s , port : %d\n", client_ip, ntohs(client_addr.sin_port));

    // 5.开始通信
    // 我们的要求是客户端发送什么，服务端都返回相同的值
    char buf[MAX_SIZE] = {0};
    while (1) {
        bzero(buf, sizeof(buf));
        // 读数据
        int len = read(connect_fd, buf, sizeof(buf) - 1);
        if (-1 == len) {
            perror("read");
            return -1;
        }
        if (len > 0)
            printf("recv data : %s", buf);
        else if (0 == len) {  // 客户端断开连接
            printf("client closed...\n");
            break;
        }

        // 写数据
        write(connect_fd, buf, strlen(buf));
    }

    // 6.关闭连接
    close(connect_fd);
    close(listen_fd);

    return 0;
}

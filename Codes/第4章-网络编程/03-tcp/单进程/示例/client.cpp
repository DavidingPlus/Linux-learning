#include <cstring>
#include <iostream>
using namespace std;
#include <arpa/inet.h>
#include <unistd.h>

#define MAX_SIZE 1024

static int count = 0;

int main(int argc, char const* argv[]) {
    // 判断命令行参数个数
    if (argc != 3) {
        printf("usage : %s  <ip_address>  <port>\n", argv[0]);
        return -1;
    }

    const char* server_ip = argv[1];
    const short server_port = atoi(argv[2]);  // atoi()函数可以把合理的字符串转化为整数

    // 1.创建socket
    int connect_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (-1 == connect_fd) {
        perror("socket");
        return -1;
    }

    // 2.建立连接
    struct sockaddr_in server_addr;
    // 地址族
    server_addr.sin_family = AF_INET;
    // 端口号
    server_addr.sin_port = htons(server_port);
    // IP地址
    inet_pton(AF_INET, server_ip, &server_addr.sin_addr.s_addr);

    int ret = connect(connect_fd, (struct sockaddr*)&server_addr, sizeof(server_addr));
    if (-1 == ret) {
        perror("connect");
        return -1;
    }
    // 连接成功，输出信息
    printf("connected successfully , waiting for communication.\n");

    // 3.开始通信
    // 我们要求客户端可以从键盘录入值进行通信
    char buf[MAX_SIZE] = {0};
    while (1) {
        bzero(buf, sizeof(buf));
        // 写数据
        fgets(buf, sizeof(buf), stdin);  // 这里有一个问题，服务器在阻塞读的时候服务端如果强制退出比如 ctrl + c，就会出问题，这样客户端不会停止，因为他还在写的部分阻塞，没有在读的部分，但是问题不大，因为实际开发当中我们不会对服务器做这样的操作

        // 增加退出功能
        if (strcmp(buf, "quit\n") == 0 || strcmp(buf, "QUIT\n") == 0)
            return 0;

        printf("send : %s", buf);
        write(connect_fd, buf, strlen(buf));

        // 读数据
        bzero(buf, sizeof(buf));
        int len = read(connect_fd, buf, sizeof(buf) - 1);
        if (-1 == len) {
            perror("len");
            return -1;
        }
        if (len > 0)
            printf("recv : %s", buf);
        else if (0 == len) {
            // 服务端关闭了
            printf("server closed...\n");
            break;
        }
    }

    // 4.关闭连接
    close(connect_fd);

    return 0;
}

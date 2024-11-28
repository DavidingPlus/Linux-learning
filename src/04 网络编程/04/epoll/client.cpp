#include <cstring>
#include <iostream>
using namespace std;
#include <arpa/inet.h>
#include <unistd.h>

#define MAX_BUF_SIZE 1024

int main() {
    // 1.创建套接字
    int connect_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (-1 == connect_fd) {
        perror("socket");
        return -1;
    }

    // 2.建立连接
    struct sockaddr_in server_addr;
    // 地址族
    server_addr.sin_family = AF_INET;
    // 端口
    server_addr.sin_port = htons(9999);
    // IP
    inet_pton(AF_INET, "127.0.0.2", &server_addr.sin_addr.s_addr);

    int ret = connect(connect_fd, (struct sockaddr*)&server_addr, sizeof(server_addr));
    if (-1 == ret) {
        perror("connect");
        return -1;
    }

    printf("connected successfully , waiting for communicating.\n");

    char buf[MAX_BUF_SIZE] = {0};
    // 3.开始通信
    while (1) {
        // 写
        bzero(buf, sizeof(buf));
        fgets(buf, sizeof(buf), stdin);

        // 增加退出功能
        if (strcmp(buf, "quit\n") == 0 || strcmp(buf, "QUIT\n") == 0)
            goto END;

        write(connect_fd, buf, strlen(buf));
        printf("send : %s", buf);

        // 读
        bzero(buf, sizeof(buf));
        int len = read(connect_fd, buf, sizeof(buf) - 1);
        if (-1 == len) {
            perror("read");
            return -1;
        }
        if (len > 0)
            printf("recv : %s", buf);
        else if (0 == len) {  // 说明写端关闭，也就是服务端关闭
            printf("server has closed...\n");
            break;
        }
    }

END:
    // 4.关闭连接
    close(connect_fd);

    return 0;
}

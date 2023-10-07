#include <cstring>
#include <iostream>
using namespace std;
#include <arpa/inet.h>
#include <unistd.h>

#define MAX_BUFFER_SIZE 1024

int main(int argc, char* const argv[]) {
    // 判断命令行参数
    if (argc < 3) {
        printf("usage: %s  <ip>  <port>.\n", argv[0]);
        return -1;
    }

    const char* server_ip = argv[1];
    const unsigned short server_port = atoi(argv[2]);

    // 1.创建socket套接字
    int connect_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (-1 == connect_fd) {
        perror("socket");
        return -1;
    }

    // 2.开始连接
    struct sockaddr_in server_addr;
    // 地址族
    server_addr.sin_family = AF_INET;
    // ip
    inet_pton(AF_INET, server_ip, &server_addr.sin_addr.s_addr);
    // port
    server_addr.sin_port = htons(server_port);

    int ret = connect(connect_fd, (struct sockaddr*)&server_addr, sizeof(server_addr));
    if (-1 == ret) {
        perror("connect");
        return -1;
    }

    // 3.开始通信
    char buf[MAX_BUFFER_SIZE] = {0};
    // 每隔一秒向服务端发送一次数据
    while (1) {
        static int count = 0;
        bzero(buf, sizeof(buf));
        sprintf(buf, "hello,this is client, count:%d\n", count++);
        printf("send: %s", buf);

        send(connect_fd, buf, strlen(buf), 0);
        sleep(1);
    }

    // 4.关闭连接
    close(connect_fd);

    return 0;
}

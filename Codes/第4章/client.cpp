#include <cstring>
#include <iostream>
using namespace std;
#include <arpa/inet.h>
#include <unistd.h>

#define MAX_BUFFER_SIZE 5

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
    int connect_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (-1 == connect_fd) {
        perror("socket");
        return -1;
    }

    // 2.与服务端尝试建立连接
    struct sockaddr_in server_addr;
    // 地址族
    server_addr.sin_family = AF_INET;
    // IP
    inet_pton(AF_INET, server_ip, &server_addr.sin_addr.s_addr);
    // 端口
    server_addr.sin_port = htons(server_port);

    int ret = connect(connect_fd, (struct sockaddr*)&server_addr, sizeof(server_addr));
    if (-1 == ret) {
        perror("connect");
        return -1;
    }

    cout << "connected successfully, waiting for communicate." << endl;

    // 3.开始通信
    char buf[MAX_BUFFER_SIZE] = {0};
    while (1) {
        // 写
        bzero(buf, sizeof(buf));
        fgets(buf, sizeof(buf), stdin);
        send(connect_fd, buf, strlen(buf), 0);
        printf("send: %s", buf);

        // 读
        bzero(buf, sizeof(buf));
        int len = recv(connect_fd, buf, sizeof(buf) - 1, 0);
        if (-1 == len) {
            perror("recv");
            return -1;
        }
        if (len > 0)
            printf("recv: %s", buf);
        else if (0 == len) {
            cout << "server has closed..." << endl;
            break;
        }
    }

    // 4.关闭连接
    close(connect_fd);

    return 0;
}

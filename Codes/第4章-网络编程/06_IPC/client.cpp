#include <cstring>
#include <iostream>
using namespace std;
#include <arpa/inet.h>
#include <sys/un.h>
#include <unistd.h>

#define MAX_BUF_SIZE 1024

int main() {
    // 1.创建本地socket套接字
    int connect_fd = socket(AF_LOCAL, SOCK_STREAM, 0);
    if (-1 == connect_fd) {
        perror("socket");
        return -1;
    }

    // 2.绑定本地套接字文件
    struct sockaddr_un client_addr;
    client_addr.sun_family = AF_LOCAL;
    strcpy(client_addr.sun_path, "client.sock");

    int ret = bind(connect_fd, (struct sockaddr*)&client_addr, sizeof(client_addr));
    if (-1 == ret) {
        perror("bind");
        return -1;
    }

    // 3.连接客户端
    struct sockaddr_un server_addr;
    server_addr.sun_family = AF_LOCAL;
    strcpy(server_addr.sun_path, "server.sock");

    ret = connect(connect_fd, (struct sockaddr*)&server_addr, sizeof(server_addr));
    if (-1 == ret) {
        perror("connect");
        return -1;
    }

    char buf[MAX_BUF_SIZE] = {0};
    static int num = 0;
    // 4.开始通信
    while (1) {
        // 写
        bzero(buf, sizeof(buf));
        sprintf(buf, "hello , i am client , num : %d\n", num++);
        printf("send : %s", buf);
        send(connect_fd, buf, strlen(buf), 0);

        // 读
        bzero(buf, sizeof(buf));
        int len = recv(connect_fd, buf, sizeof(buf) - 1, 0);
        if (-1 == len) {
            perror("recv");
            return -1;
        }
        if (len > 0)
            printf("recv : %s", buf);
        else if (0 == len) {
            printf("server has closed...\n");
            break;
        }

        sleep(1);
    }

    return 0;
}

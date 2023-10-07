#include <cstring>
#include <iostream>
using namespace std;
#include <arpa/inet.h>
#include <unistd.h>

#define MAX_BUF_SIZE 1024
#define MAX_IPV4_STRING 16

// 广播的IP地址
const char* Broadcast_IP = "127.255.255.255";

int main() {
    // 1.创建通信的socket套接字
    int socket_fd = socket(AF_INET, SOCK_DGRAM, 0);
    if (-1 == socket_fd) {
        perror("socket");
        return -1;
    }

    // 开启广播设置
    int _optval = 1;
    setsockopt(socket_fd, SOL_SOCKET, SO_BROADCAST, &_optval, sizeof(_optval));

    // 2.绑定IP和端口，其实在这里我们不接受数据，帮不绑定其实无所谓
    struct sockaddr_in server_addr;
    // 地址族
    server_addr.sin_family = AF_INET;
    // IP
    inet_pton(AF_INET, "127.0.0.1", &server_addr.sin_addr.s_addr);
    // 端口
    server_addr.sin_port = htons(9999);

    int ret = bind(socket_fd, (struct sockaddr*)&server_addr, sizeof(server_addr));
    if (-1 == socket_fd) {
        perror("bind");
        return -1;
    }

    printf("server has initialized.\n");

    // 封装广播客户端的socket地址
    struct sockaddr_in All_Client_addr;
    All_Client_addr.sin_family = AF_INET;
    All_Client_addr.sin_port = htons(10000);
    inet_pton(AF_INET, Broadcast_IP, &All_Client_addr.sin_addr.s_addr);

    // 3.开始通信
    static int num = 0;
    char buf[MAX_BUF_SIZE] = {0};

    while (1) {
        // 服务端向所有的客户端广播数据
        bzero(buf, sizeof(buf));
        sprintf(buf, "hello , i am server , num = %d\n", num++);
        printf("send : %s", buf);

        sendto(socket_fd, buf, strlen(buf), 0, (struct sockaddr*)&All_Client_addr, sizeof(All_Client_addr));
        sleep(1);
    }

    // 4.关闭套接字
    close(socket_fd);

    return 0;
}

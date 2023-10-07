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

    // 设置多播属性，设置外出接口
    struct in_addr _optval;
    // 初始化多播地址
    inet_pton(AF_INET, Multicast_IP, &_optval.s_addr);
    setsockopt(socket_fd, IPPROTO_IP, IP_MULTICAST_IF, &_optval, sizeof(_optval));

    // 发送方，这里我就不绑定端口了

    printf("server has initialized.\n");

    // 封装广播客户端的socket地址
    struct sockaddr_in All_Client_addr;
    All_Client_addr.sin_family = AF_INET;
    All_Client_addr.sin_port = htons(10000);
    inet_pton(AF_INET, Multicast_IP, &All_Client_addr.sin_addr.s_addr);

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

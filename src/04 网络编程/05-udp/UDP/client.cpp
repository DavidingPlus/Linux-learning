#include <cstring>
#include <iostream>
using namespace std;
#include <arpa/inet.h>
#include <unistd.h>

#define MAX_BUF_SIZE 1024
#define MAX_IPV4_STRING 16

int main() {
    // 1.创建通信的socket套接字
    int socket_fd = socket(AF_INET, SOCK_DGRAM, 0);
    if (-1 == socket_fd) {
        perror("socket");
        return -1;
    }

    cout << socket_fd << endl;

    // 存储服务端地址信息
    struct sockaddr_in server_addr;
    // 地址族
    server_addr.sin_family = AF_INET;
    // IP
    inet_pton(AF_INET, "127.0.0.2", &server_addr.sin_addr.s_addr);
    // 端口
    server_addr.sin_port = htons(9999);

    char buf[MAX_BUF_SIZE] = {0};

    static int num = 0;

    // 2.开始通信
    while (1) {
        // 写
        bzero(buf, sizeof(buf));
        sprintf(buf, "hello i am client , %d\n", num++);
        printf("send : %s", buf);
        sendto(socket_fd, buf, strlen(buf), 0, (struct sockaddr*)&server_addr, sizeof(server_addr));

        sleep(1);

        // 读
        bzero(buf, sizeof(buf));

        int len = recvfrom(socket_fd, buf, sizeof(buf) - 1, 0, nullptr, nullptr);
        if (-1 == len) {
            perror("recvfrom");
            return -1;
        }
        // recvfrom返回0是可以接受的，不像read返回0表示对端关闭连接。因为UDP是无连接的，也就没有所谓的关闭。
        printf("recv : %s", buf);
    }

    // 4.关闭套接字
    close(socket_fd);

    return 0;
}

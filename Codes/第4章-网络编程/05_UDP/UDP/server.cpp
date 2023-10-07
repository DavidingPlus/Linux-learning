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

    // 2.绑定IP和端口
    struct sockaddr_in server_addr;
    // 地址族
    server_addr.sin_family = AF_INET;
    // IP
    server_addr.sin_addr.s_addr = INADDR_ANY;
    // 端口
    server_addr.sin_port = htons(9999);

    int ret = bind(socket_fd, (struct sockaddr*)&server_addr, sizeof(server_addr));
    if (-1 == socket_fd) {
        perror("bind");
        return -1;
    }

    printf("server has initialized.\n");

    cout << socket_fd << endl;

    char buf[MAX_BUF_SIZE] = {0};

    // 3.开始通信
    while (1) {
        bzero(buf, sizeof(buf));
        struct sockaddr_in client_addr;
        socklen_t client_addr_len = sizeof(client_addr);

        // 读
        int len = recvfrom(socket_fd, buf, sizeof(buf) - 1, 0, (struct sockaddr*)&client_addr, &client_addr_len);
        if (-1 == len) {
            perror("recvfrom");
            return -1;
        }
        // 获得客户端信息
        char client_ip[MAX_IPV4_STRING] = {0};
        inet_ntop(AF_INET, &client_addr.sin_addr.s_addr, client_ip, sizeof(client_ip));
        in_port_t client_port = ntohs(client_addr.sin_port);

        // recvfrom返回0是可以接受的，不像read返回0表示对端关闭连接。因为UDP是无连接的，也就没有所谓的关闭。
        printf("recv client (ip : %s , port : %d) : %s", client_ip, client_port, buf);

        // 写
        sendto(socket_fd, buf, strlen(buf), 0, (struct sockaddr*)&client_addr, client_addr_len);
    }

    // 4.关闭套接字
    close(socket_fd);

    return 0;
}

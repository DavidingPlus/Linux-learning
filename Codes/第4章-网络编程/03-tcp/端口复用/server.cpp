#include <cstring>
#include <iostream>
using namespace std;
#include <arpa/inet.h>
#include <unistd.h>

#define MAX_IPV4_STRING 16
#define MAXSIZE 1024

int main() {
    // 创建socket
    int listen_fd = socket(PF_INET, SOCK_STREAM, 0);

    if (listen_fd == -1) {
        perror("socket");
        return -1;
    }

    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(9999);

    // 设置端口复用(在绑定之前)
    // int optval = 1;
    // setsockopt(listen_fd, SOL_SOCKET, SO_REUSEPORT, &optval, sizeof(optval));

    // 绑定
    int ret = bind(listen_fd, (struct sockaddr *)&server_addr, sizeof(server_addr));
    if (ret == -1) {
        perror("bind");
        return -1;
    }

    // 监听
    ret = listen(listen_fd, 8);
    if (ret == -1) {
        perror("listen");
        return -1;
    }

    // 接收客户端连接
    struct sockaddr_in client_addr;
    socklen_t client_len = sizeof(client_addr);
    int connect_fd = accept(listen_fd, (struct sockaddr *)&client_addr, &client_len);
    if (connect_fd == -1) {
        perror("accpet");
        return -1;
    }

    // 获取客户端信息
    char client_ip[MAX_IPV4_STRING] = {0};
    inet_ntop(AF_INET, &client_addr.sin_addr.s_addr, client_ip, sizeof(client_ip));
    unsigned short client_port = ntohs(client_addr.sin_port);

    // 输出客户端的信息
    printf("client's ip is %s, and port is %d\n", client_ip, client_port);

    // 接收客户端发来的数据
    char buf[MAXSIZE] = {0};
    while (1) {
        int len = recv(connect_fd, buf, sizeof(buf), 0);
        if (len == -1) {
            perror("recv");
            return -1;
        } else if (0 == len) {
            printf("客户端已经断开连接...\n");
            break;
        } else if (len > 0)
            printf("read buf = %s", buf);

        // 小写转大写
        for (int i = 0; i < len; ++i)
            buf[i] = toupper(buf[i]);

        printf("after buf = %s", buf);

        // 大写字符串发给客户端
        ret = send(connect_fd, buf, strlen(buf) + 1, 0);
        if (ret == -1) {
            perror("send");
            return -1;
        }
    }

    close(connect_fd);
    close(listen_fd);

    return 0;
}

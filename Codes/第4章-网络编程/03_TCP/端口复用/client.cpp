#include <cstring>
#include <iostream>
using namespace std;
#include <arpa/inet.h>
#include <unistd.h>

#define MAXSIZE 1024

int main() {
    // 创建socket
    int connect_fd = socket(PF_INET, SOCK_STREAM, 0);
    if (connect_fd == -1) {
        perror("socket");
        return -1;
    }

    struct sockaddr_in server_addr;
    inet_pton(AF_INET, "127.0.0.1", &server_addr.sin_addr.s_addr);
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(9999);

    // 连接服务器
    int ret = connect(connect_fd, (struct sockaddr *)&server_addr, sizeof(server_addr));

    if (ret == -1) {
        perror("connect");
        return -1;
    }

    while (1) {
        char buf[MAXSIZE] = {0};
        fgets(buf, sizeof(buf), stdin);

        write(connect_fd, buf, strlen(buf) + 1);

        // 接收
        int len = read(connect_fd, buf, sizeof(buf));
        if (len == -1) {
            perror("read");
            return -1;
        } else if (len > 0)
            printf("read buf = %s", buf);
        else {
            printf("服务器已经断开连接...\n");
            break;
        }
    }

    close(connect_fd);

    return 0;
}

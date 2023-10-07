#include <cstring>
#include <iostream>
using namespace std;
#include <arpa/inet.h>
#include <unistd.h>

#define MAX_SIZE 1024

int main() {
    // 1.创建socket(用于建立连接的socket)
    int connect_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (-1 == connect_fd) {
        perror("socket");
        return -1;
    }

    // 2.connect进行连接
    sockaddr_in _s_addr;
    // 地址族
    _s_addr.sin_family = AF_INET;
    // 端口
    _s_addr.sin_port = htons(9999);
    // IP地址
    inet_pton(AF_INET, "127.0.0.1", &_s_addr.sin_addr.s_addr);

    int ret = connect(connect_fd, (struct sockaddr*)&_s_addr, sizeof(_s_addr));
    if (-1 == ret) {
        perror("connect");
        return -1;
    }

    cout << connect_fd << endl;

    // 3.进行通信，先发送信息，然后接受信息
    char buf[MAX_SIZE] = {0};
    const char* data = "hello,i am client\n";
    while (1) {
        // 写数据
        write(connect_fd, data, strlen(data));
        sleep(1);  // 写完让他睡1秒，服务端不需要写，因为那边会跟着这边的进行阻塞

        // 读数据
        bzero(buf, sizeof(buf));
        int len = read(connect_fd, buf, sizeof(buf) - 1);
        if (-1 == len) {
            perror("read");
            return -1;
        }
        if (len > 0)
            printf("recv server data : %s", buf);
        else if (0 == len) {
            printf("server closed...\n");
            break;
        }
    }

    // 4.关闭连接
    close(connect_fd);

    return 0;
}

#include <cstring>
#include <iostream>
using namespace std;
#include <arpa/inet.h>
#include <unistd.h>

#define MAX_SIZE 1024

// TCP通信服务端

int main() {
    // 1.创建socket(用于监听的socket)
    int listen_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (-1 == listen_fd) {
        perror("socket");
        return -1;
    }

    // 2.绑定
    // 创建socket地址结构体
    struct sockaddr_in _s_addr;

    // 设置地址族
    _s_addr.sin_family = PF_INET;  // 这两个族 PF_*(协议族) 和 AF_*(地址族) 用哪个都是可以的，他们之间是相互对应的

    // 设置IP地址，系统API已经帮我们转换为正确的网络字节序了

    // in_addr_t ip_num = 0;
    // inet_pton(AF_INET, "192.168.0.111", &ip_num);
    // _s_addr.sin_addr._s_addr = ip_num;

    _s_addr.sin_addr.s_addr = INADDR_ANY;  // 有可能有多个网卡然后存在多个IP，这个宏的值是0，表示客户端可以连接任意一个IP，只有服务端可以这么做，客户端肯定要指定连接的IP

    // 设置端口，还要转化为网络字节序
    _s_addr.sin_port = htons(9999);

    int ret = bind(listen_fd, (struct sockaddr*)&_s_addr, sizeof(struct sockaddr));
    if (-1 == ret) {
        perror("bind");
        return -1;
    }

    // 3.监听
    ret = listen(listen_fd, 8);
    if (-1 == ret) {
        perror("listen");
        return -1;
    }

    // 4.接受客户端的连接
    struct sockaddr_in _c_addr;
    socklen_t c_len = sizeof(_c_addr);

    // 如果连接成功获得的是用于和客户端通信的文件描述符
    int connect_fd = accept(listen_fd, (struct sockaddr*)&_c_addr, &c_len);
    if (-1 == connect_fd) {
        perror("accept");
        return -1;
    }

    cout << connect_fd << endl;

    // 输出客户端的信息
    // 注意这个16的由来
    char c_ip[16] = {0};
    // 这个函数不仅帮我们从数字转化为字符串，还帮我们从网络字节序转化为了主机字节序
    inet_ntop(AF_INET, &_c_addr.sin_addr.s_addr, c_ip, sizeof(c_ip));
    uint16_t port = ntohs(_c_addr.sin_port);
    printf("client ip : %s , port : %d\n", c_ip, port);

    // 5.进行通信，我们在这里让客户端发送数据，服务器接受，然后再发送数据

    char buf[MAX_SIZE] = {0};
    const char* data = "hello,i am server.\n";
    while (1) {
        // 读数据
        bzero(buf, sizeof(buf));
        int len = read(connect_fd, buf, sizeof(buf) - 1);
        if (-1 == len) {
            perror("read");
            return -1;
        }
        if (len > 0)  // 读取正常，读到了数据
            printf("recv client data : %s", buf);
        else if (0 == len) {  // 和管道类似，写端全部关闭就会返回0，也就是类似于读到文件末尾
            printf("client closed...\n");
            break;  // 退出
        }

        // 写数据
        write(connect_fd, data, strlen(data));
    }

    // 关闭文件描述符
    close(connect_fd);
    close(listen_fd);

    return 0;
}

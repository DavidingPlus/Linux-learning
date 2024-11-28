#include <cstring>
#include <iostream>
using namespace std;
#include <arpa/inet.h>
#include <poll.h>
#include <unistd.h>

#include "Client_Info.h"

#define MAX_BUF_SIZE 1024
#define MAX_CLIENT_SIZE 1024
#define MAX_POLLFD_SIZE 1025

// 全局存放客户端连接的IP和端口
class Client_Info cli_infos[MAX_CLIENT_SIZE];

// 全局存放需要检测的文件描述符数组
struct pollfd fds[MAX_POLLFD_SIZE];

int bigger(const int& val1, const int& val2) {
    return val1 > val2 ? val1 : val2;
}

void Communicate(const int& _index) {
    int _connect_fd = fds[_index].fd;

    char* _client_ip = cli_infos[_connect_fd].client_ip;
    in_port_t& _client_port = cli_infos[_connect_fd].client_port;

    char buf[MAX_BUF_SIZE] = {0};
    // 读
    bzero(buf, sizeof(buf));
    int len = read(_connect_fd, buf, sizeof(buf) - 1);
    if (-1 == len) {
        perror("read");
        exit(-1);
    }
    if (len > 0)
        printf("recv client (ip : %s , port : %d) : %s", _client_ip, _client_port, buf);
    else if (0 == len) {  // 客户端关闭
        printf("client ip : %s , port : %d has closed...\n", _client_ip, _client_port);
        // 关闭文件描述符
        close(_connect_fd);
        // 将对应的文件描述符置为-1
        fds[_index].fd = -1;
        return;
    }
    // 写
    write(_connect_fd, buf, strlen(buf));
}

int main() {
    // 1.创建socket
    int listen_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (-1 == listen_fd) {
        perror("socket");
        return -1;
    }

    // 设置一下端口复用
    int _optval = 1;
    setsockopt(listen_fd, SOL_SOCKET, SO_REUSEPORT, &_optval, sizeof(_optval));

    // 2.绑定IP和端口
    struct sockaddr_in server_addr;
    // 地址族
    server_addr.sin_family = AF_INET;
    // IP
    server_addr.sin_addr.s_addr = INADDR_ANY;
    // 端口
    server_addr.sin_port = htons(9999);

    int ret = bind(listen_fd, (struct sockaddr*)&server_addr, sizeof(server_addr));
    if (-1 == ret) {
        perror("bind");
        return -1;
    }

    printf("server has initialized.\n");

    // 3.开始监听
    ret = listen(listen_fd, 8);
    if (-1 == ret) {
        perror("listen");
        return -1;
    }

    // 使用NIO模型，使用poll解决问题
    // 初始化检测的文件描述符数组
    for (int i = 0; i < MAX_POLLFD_SIZE; ++i) {
        fds[i].fd = -1;
        fds[i].events = POLLIN;  // 表示一会儿要去检测读事件
    }
    // 加入监听的文件描述符
    // fds[]的序号是从0开始的，他的序号不一定和fd文件描述符匹配
    fds[0].fd = listen_fd;

    // 定义最大的文件描述符的fds[]数组的索引
    // 从前面看，这个是索引，和最大的文件描述符不对等
    int nfds = 0;

    while (1) {
        // 调用poll()函数，这是select()函数的改进版本
        ret = poll(fds, nfds + 1, -1);
        if (-1 == ret) {
            perror("select");
            return -1;
        } else if (0 == ret)
            // 为0表示超时并且没有检测到有改变的
            continue;  // 这里我们的设置因为是阻塞的，所以不会走到这里
        else if (ret > 0) {
            // 说明检测到了有文件描述符对应缓冲区的数据发生了改变
            if (fds[0].revents & POLLIN == POLLIN) {
                // 表示有新的客户端连接进来了
                struct sockaddr_in client_addr;
                socklen_t client_addr_len = sizeof(client_addr);
                int connect_fd = accept(listen_fd, (struct sockaddr*)&client_addr, &client_addr_len);

                if (-1 == connect_fd) {
                    perror("accept");
                    return -1;
                }

                // 获取客户端的信息
                char ip[MAX_IPV4_STRING] = {0};
                inet_ntop(AF_INET, &client_addr.sin_addr.s_addr, ip, sizeof(ip));

                in_port_t port = ntohs(client_addr.sin_port);

                // 打印信息
                printf("client ip : %s , port : %d has connected...\n", ip, port);

                // 将客户端的信息保存到全局数组中
                cli_infos[connect_fd] = Client_Info(ip, port);

                // 将新的文件描述符加入到事件中，注意文件描述符的优先用小的机制
                for (int i = 1; i < MAX_POLLFD_SIZE; ++i)
                    if (fds[i].fd == -1) {
                        fds[i].fd = connect_fd;
                        fds[i].events = POLLIN;
                        // 更新nfds
                        nfds = bigger(nfds, i);
                        break;
                    }
            }

            // 看完监听的文件描述符，看其他的文件描述符是否收到数据
            for (int i = 1; i < nfds + 1; ++i) {
                if (fds[i].revents & POLLIN == POLLIN)
                    Communicate(i);
            }
        }
    }

    // 4.关闭连接
    close(listen_fd);

    return 0;
}

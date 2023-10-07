#include <cstring>
#include <iostream>
using namespace std;
#include <arpa/inet.h>
#include <fcntl.h>
#include <sys/epoll.h>
#include <unistd.h>

#include "Client_Info.h"

#define MAX_CLIENT_SIZE 1024
// 我将一次读取的大小弄小点
#define MAX_BUF_SIZE 5

// 全局存放客户端连接的IP和端口
class Client_Info cli_infos[MAX_CLIENT_SIZE];

void Communicate(const struct epoll_event &_ret_event, const int &_epoll_fd) {
    int _connect_fd = _ret_event.data.fd;

    // ET工作模式不会通知第二次，只有再次变化的时候才会检测到，因此我们需要调用非阻塞的接口把数据读完
    char buf[MAX_BUF_SIZE] = {0};
    while (1) {
        int len = read(_connect_fd, buf, sizeof(buf) - 1);

        if (-1 == len) {
            // 里面有一种情况就是我写端没有关闭但是我在非阻塞的情况下已经把数据读完了，这个时候就会产生EAGAIN的错误
            if (errno == EAGAIN) {
                printf("read data over.\n");
                return;
            }

            perror("read");
            exit(-1);
        }

        // 读到正确数据
        if (len > 0) {
            printf("client (ip : %s , port : %d) recv : %s\n", cli_infos[_connect_fd].client_ip, cli_infos[_connect_fd].client_port, buf);
            write(_connect_fd, buf, strlen(buf));
            bzero(buf, sizeof(buf));
        }

        else if (0 == len) {
            // 写端，客户端关闭连接
            printf("client (ip : %s , port : %d) has closed...\n", cli_infos[_connect_fd].client_ip, cli_infos[_connect_fd].client_port);
            // 从检测事件中删除他
            epoll_ctl(_epoll_fd, EPOLL_CTL_DEL, _connect_fd, nullptr);
            // 关闭文件描述符
            close(_connect_fd);

            return;
        }
    }
}

int main() {
    // 1.创建socket套接字
    int listen_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (-1 == listen_fd) {
        perror("socket");
        return -1;
    }

    // 设置端口复用
    int _optval = 1;
    int ret = setsockopt(listen_fd, SOL_SOCKET, SO_REUSEPORT, &_optval, sizeof(_optval));
    if (-1 == ret) {
        perror("setsockopt");
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

    ret = bind(listen_fd, (struct sockaddr *)&server_addr, sizeof(server_addr));
    if (-1 == ret) {
        perror("bind");
        return -1;
    }

    // 3.监听端口
    ret = listen(listen_fd, 8);
    if (-1 == ret) {
        perror("listen");
        return -1;
    }

    printf("server has initialized.\n");

    // 4.用epoll技术实现接受客户端和进行通信
    // 创建epoll示例
    int epoll_fd = epoll_create(1);
    if (-1 == epoll_fd) {
        perror("epoll_create");
        return -1;
    }

    // 将监听套接字添加进入检测中
    struct epoll_event listen_event;
    listen_event.events = EPOLLIN;     // 检测读
    listen_event.data.fd = listen_fd;  // 文件描述符

    epoll_ctl(epoll_fd, EPOLL_CTL_ADD, listen_fd, &listen_event);

    int _maxevents = MAX_CLIENT_SIZE;

    // 开始检测
    while (1) {
        // 这个结构体数组存放了检测到的文件描述符的信息，保存在这里面
        // 内核中是把双链表中的数据写入到这里
        struct epoll_event ret_events[_maxevents];

        // 返回值是表示有多少个被检测到了；第三个参数可以一般放数组的最大容量
        int ret = epoll_wait(epoll_fd, ret_events, _maxevents, -1);
        if (-1 == ret) {
            perror("epoll_wait");
            return -1;
        }

        // 检测到了，开始处理
        for (int i = 0; i < ret; ++i) {
            if (ret_events[i].events && EPOLLIN == EPOLLIN) {
                if (ret_events[i].data.fd == listen_fd) {
                    // 表示有新客户端连接
                    struct sockaddr_in client_addr;
                    socklen_t client_addr_len = sizeof(client_addr);

                    int connect_fd = accept(listen_fd, (struct sockaddr *)&client_addr, &client_addr_len);
                    if (-1 == connect_fd) {
                        perror("accept");
                        return -1;
                    }

                    // 设置非阻塞，否则读完就阻塞在这里，read非阻塞通过文件描述符操作
                    int _flag = fcntl(connect_fd, F_GETFL);
                    _flag |= O_NONBLOCK;  // 不能把原来的属性设置没了所以先获得
                    fcntl(connect_fd, F_SETFL, _flag);

                    // 将客户端信息存入结构体数组，下标用connect_fd代替
                    inet_ntop(AF_INET, &client_addr.sin_addr.s_addr, cli_infos[connect_fd].client_ip, sizeof(cli_infos[connect_fd].client_ip));
                    cli_infos[connect_fd].client_port = ntohs(client_addr.sin_port);

                    printf("client (ip : %s , port : %d) has connected...\n", cli_infos[connect_fd].client_ip, cli_infos[connect_fd].client_port);

                    // 添加到检测中
                    struct epoll_event connect_event;
                    connect_event.data.fd = connect_fd;
                    connect_event.events = EPOLLIN | EPOLLET;  // 设置边沿触发，结合非阻塞的API使用!!!

                    epoll_ctl(epoll_fd, EPOLL_CTL_ADD, connect_fd, &connect_event);
                } else  // 客户端接收到数据
                    Communicate(ret_events[i], epoll_fd);
            }
        }
    }

    // 5.关闭连接
    close(epoll_fd);
    close(listen_fd);

    return 0;
}

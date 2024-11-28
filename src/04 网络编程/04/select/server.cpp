#include <cstring>
#include <iostream>
using namespace std;
#include <arpa/inet.h>
#include <unistd.h>

#include "Client_Info.h"

#define MAXSIZE 1024
#define MAX_CLIENT_SIZE 1024

// 全局存放客户端连接的IP和端口
class Client_Info cli_infos[MAX_CLIENT_SIZE];

// 全局存放需要检测的文件描述符的数组
fd_set read_set;

int bigger(const int& val1, const int& val2) {
    return val1 > val2 ? val1 : val2;
}

void Communicate(const int& _connect_fd) {
    char* _client_ip = cli_infos[_connect_fd].client_ip;
    in_port_t& _client_port = cli_infos[_connect_fd].client_port;

    char buf[MAXSIZE] = {0};
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
        // 这里关闭之后需要移除文件描述符集合中的标志位表示我不需要监听这个了
        FD_CLR(_connect_fd, &read_set);
        // 关闭文件描述符
        close(_connect_fd);
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

    // 使用NIO模型，创建fd_set集合，存放的是需要检测的文件描述符
    // 全局定义 read_set
    // 初始化
    FD_ZERO(&read_set);
    // 添加需要检测的文件描述符
    FD_SET(listen_fd, &read_set);
    // 定义最大的文件描述符序号(参数里面要加1)
    int max_fd = listen_fd;

    // 这个地方我不能把read_set集合拿进去让内核进行拷贝修改然后覆盖我的这个
    // 我们设想这样一种情况，AB都检测，A发数据，B的被修改为0，但是下一次我肯定还要检测B的啊

    while (1) {
        fd_set tmp_set = read_set;
        // 调用select系统函数，让内核帮忙检测哪些文件描述符有数据
        // 这里是在检测listen_fd，因为如果有客户端请求连接了，那么这里listen_fd肯定会有数据进来
        ret = select(max_fd + 1, &tmp_set, nullptr, nullptr, nullptr);
        if (-1 == ret) {
            perror("select");
            return -1;
        } else if (0 == ret)
            // 为0表示超时并且没有检测到有改变的
            continue;  // 这里我们的设置因为是阻塞的，所以不会走到这里
        else if (ret > 0) {
            // 说明检测到了有文件描述符对应缓冲区的数据发生了改变
            if (FD_ISSET(listen_fd, &tmp_set)) {
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

                // 将新的文件描述符加入到集合中，这样select()就可以监听客户端的数据了
                FD_SET(connect_fd, &read_set);
                // 更新max_fd
                max_fd = bigger(connect_fd, max_fd);
            }

            // 看完监听的文件描述符，还要看其他的文件描述符标识位
            for (int i = listen_fd + 1; i < max_fd + 1; ++i) {
                if (FD_ISSET(i, &tmp_set))
                    // 表示有数据到来，进行通信，服务端只处理一次，然后又重新检测是否有数据，有数据则又走这段代码
                    // 并且如果服务端里面处理用循环处理，那么这个客户端一直抢占者服务端，其他服务端没办法发送数据
                    Communicate(i);
            }
        }
    }

    // 4.关闭连接
    close(listen_fd);

    return 0;
}

#include <cstring>
#include <iostream>
using namespace std;
#include <arpa/inet.h>
#include <pthread.h>
#include <unistd.h>

#define MAXSIZE 1024
#define MAX_IPV4_STRING 16
#define MAX_INFO_SIZE 128

// 封装一个结构体来保存需要传给子线程的信息
struct Pthread_Info {
    int _connect_fd;                  // 用于通信的文件描述符
    struct sockaddr_in _client_addr;  // 客户端的socket地址信息
} p_infos[MAX_INFO_SIZE];

// 定义这个数组的计数器
int count = 0;

// 线程处理的回调函数
void* Communicate_Callback(void* args) {
    // 接受参数得到通信用到的信息
    int connect_fd = ((Pthread_Info*)args)->_connect_fd;
    struct sockaddr_in client_addr = ((Pthread_Info*)args)->_client_addr;

    in_port_t client_port = ntohs(client_addr.sin_port);

    char client_ip[MAX_IPV4_STRING] = {0};
    inet_ntop(AF_INET, &client_addr.sin_addr.s_addr, client_ip, sizeof(client_ip));
    printf("one client has connected , ip : %s , port : %d\n", client_ip, client_port);

    // 开始通信
    char buf[MAXSIZE] = {0};
    while (1) {
        // 读
        bzero(buf, sizeof(buf));
        int len = read(connect_fd, buf, sizeof(buf) - 1);
        if (-1 == len) {
            perror("read");
            return (void*)-1;
        }
        if (len > 0)
            printf("recv client ( ip : %s , port : %d ) data : %s", client_ip, client_port, buf);
        else if (0 == len) {  // 写端关闭，也就是客户端关闭连接，才会返回0
            printf("client ( ip : %s , port : %d ) has closed...\n", client_ip, client_port);
            close(connect_fd);
            printf("child thread has closed , tid : %ld\n", pthread_self());

            // // 释放堆上的这块内存
            // delete args;

            return nullptr;
        }

        // 写
        write(connect_fd, buf, strlen(buf));
    }

    // // 释放堆上的这块内存
    // delete args;

    return nullptr;
}

// 用多线程实现服务器并发
int main(int argc, char const* argv[]) {
    // 初始化数据
    int size = sizeof(p_infos) / sizeof(p_infos[0]);
    for (int i = 0; i < size; ++i) {
        // 将所有的都初始化为0
        bzero(&p_infos[i], sizeof(p_infos[i]));
        // 文件描述符初始化为-1，不能让他占据正在使用的
        p_infos[i]._connect_fd = -1;
    }

    // 命令行参数
    if (argc != 3) {
        printf("usage : %s  <ip_address>  <port>\n", argv[0]);
        return -1;
    }

    const char* server_ip = argv[1];
    const unsigned short server_port = atoi(argv[2]);

    // 1.创建socket套接字
    int listen_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (-1 == listen_fd) {
        perror("socket");
        return -1;
    }

    // 2.绑定IP和端口
    struct sockaddr_in server_addr;
    // 地址族
    server_addr.sin_family = AF_INET;
    // 端口
    server_addr.sin_port = htons(server_port);
    // IP
    inet_pton(AF_INET, server_ip, &server_addr.sin_addr.s_addr);

    int ret = bind(listen_fd, (struct sockaddr*)&server_addr, sizeof(server_addr));
    if (-1 == ret) {
        perror("bind");
        return -1;
    }

    // 3.监听
    ret = listen(listen_fd, 5);
    if (-1 == ret) {
        perror("listen");
        return -1;
    }

    printf("server has initialized...\n");

    // 4.接受客户端请求
    while (1) {
        // 思路：主线程不断接受客户端请求，然后创建子线程和客户端进行通信
        struct sockaddr_in client_addr;
        socklen_t client_addr_len = sizeof(client_addr);
        int connect_fd = accept(listen_fd, (struct sockaddr*)&client_addr, &client_addr_len);
        if (-1 == connect_fd) {
            perror("accept");
            return -1;
        }

        // 这里是局部变量，为了保证循环一次不会被释放，我们选择其他的方式
        // 我们可以选择用堆来存储，但是一是客户端数量多了没有办法进行限制，而是还要处理释放并且消耗资源大，所以我们可以开一个全局数组

        struct Pthread_Info& p_info = p_infos[count++];  // 创建数组成员的引用别名
        if (count >= MAX_INFO_SIZE) {
            // 超出最大客户端连接数量
            printf("client oversize , closing...\n");
            return -1;
        }
        p_info._connect_fd = connect_fd;
        p_info._client_addr = client_addr;  // 这个系统类实现了copy assignment，实现了深拷贝

        // 5.开始通信
        pthread_t tid;
        pthread_create(&tid, nullptr, Communicate_Callback, &p_info);
        // 将子线程分离，不用手动回收
        pthread_detach(tid);
    }

    // 6.关闭连接
    close(listen_fd);

    // 退出主线程，这里就这么写吧，主线程不可能比子线程早结束，因为主线程要等待
    pthread_exit(nullptr);

    return 0;
}

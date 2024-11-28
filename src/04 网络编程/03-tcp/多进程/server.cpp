#include <cstring>
#include <iostream>
using namespace std;
#include <arpa/inet.h>
#include <signal.h>
#include <sys/wait.h>
#include <unistd.h>

#define MAX_IPV4_STRING 16
#define MAXSIZE 1024

void Recycle_Callback(int signum) {
    while (1) {
        // 第二个参数可以用来获取子进程退出的状态
        int ret = waitpid(-1, nullptr, WNOHANG);
        if (-1 == ret)
            //-1表示回收错误，也就是没有子进程了，为了达到这个判断，我们使用非阻塞的，因为阻塞的他会阻塞
            return;
        else if (0 == ret)  // 0在非阻塞的情况下代表还有子进程活着，重开循环把结束的子进程尽可能都回收
            continue;
        else if (ret > 0)
            // 回收了某个子进程
            printf("child process (pid %d) has be recycled.\n", ret);
    }
}

// 接受多个客户端的连接，这个程序用多进程来处理
int main(int argc, char const* argv[]) {
    // 创建出来子进程父进程需要对其进行回收的操作，但是wait()或者waitpid()无论是阻塞还是非阻塞的情况都没办法实现我们想要的操作
    // 所以想到捕捉信号SIGCHID
    struct sigaction sig_child;
    sig_child.sa_flags = 0;
    sigemptyset(&sig_child.sa_mask);  // 不阻塞任何临时的信号
    sig_child.sa_handler = Recycle_Callback;

    sigaction(SIGCHLD, &sig_child, nullptr);

    // 命令行参数
    if (argc != 3) {
        printf("usage : %s  <ip_address>  <port>\n", argv[0]);
        return -1;
    }

    const char* server_ip = argv[1];
    const int server_port = atoi(argv[2]);

    // 1.创建socket套接字
    int listen_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (-1 == listen_fd) {
        perror("socket");
        return -1;
    }

    // 2.将套接字绑定IP和端口
    struct sockaddr_in server_addr;
    // 地址族
    server_addr.sin_family = AF_INET;
    // 端口，注意转换字节序
    server_addr.sin_port = htons(server_port);
    // IP
    inet_pton(AF_INET, server_ip, &server_addr.sin_addr.s_addr);

    int ret = bind(listen_fd, (struct sockaddr*)&server_addr, sizeof(server_addr));
    if (-1 == ret) {
        perror("bind");
        return -1;
    }

    // 3.开始监听
    ret = listen(listen_fd, 5);
    if (-1 == ret) {
        perror("listen");
        return -1;
    }

    // 不断循环等待客户端连接
    while (1) {
        // 4.接受请求
        struct sockaddr_in client_addr;
        socklen_t client_addr_len = sizeof(client_addr);
        int connect_fd = accept(listen_fd, (struct sockaddr*)&client_addr, &client_addr_len);
        if (-1 == connect_fd) {
            if (errno == EINTR)
                // 说明产生了信号发生了软中断，执行回来accept()就不阻塞了，这是预料之内的状态，我们对其进行细微处理
                continue;
            perror("accept");
            return -1;
        }

        // 5.开始通信，在子进程中进程通信
        pid_t pid = fork();
        if (-1 == pid) {
            perror("fork");
            return -1;
        }
        if (0 == pid) {  // 子进程
            //  输出连接的客户端的IP和端口
            char client_ip[MAX_IPV4_STRING] = {0};
            inet_ntop(AF_INET, &client_addr.sin_addr.s_addr, client_ip, sizeof(client_ip));
            in_port_t client_port = ntohs(client_addr.sin_port);
            printf("one client has connected , ip : %s , port : %d\n", client_ip, client_port);

            // 处理数据，接受客户端的数据并且相同返回
            char buf[MAXSIZE] = {0};
            while (1) {
                // 读
                bzero(buf, sizeof(buf));
                int len = read(connect_fd, buf, sizeof(buf) - 1);  // 这里老师提到要注意\0，我这么做也是可以的，我在读的时候留出\0的空间，写的时候我全部写入，这样我的buf最后肯定是有\0的
                if (-1 == len) {
                    perror("read");
                    return -1;
                }
                if (len > 0)
                    printf("recv client (ip : %s , port : %d) data : %s", client_ip, client_port, buf);
                else if (0 == pid) {
                    // 写端断开连接，子进程任务结束，退出
                    printf("client (ip : %s , port : %d) has closed...\n", client_ip, client_port);
                    close(connect_fd);
                    return 0;
                }

                // 写
                write(connect_fd, buf, strlen(buf));
            }
        } else if (pid > 0)  // 父进程
            continue;
    }

    // 6.断开连接
    close(listen_fd);

    return 0;
}

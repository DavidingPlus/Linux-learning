#include <cstring>
#include <iostream>
#include <string>
using namespace std;
#include <fcntl.h>
#include <unistd.h>
#define _size 1024

/*
    设置管道非阻塞
    int flags =  fcntl(fd[0],F_GETFL); //获取原来的flag
    flags | = O_NONBLOCK; //修改flag的值
    fcntl(fd[0],F_SETFL,flags); //设置新的flag
*/

int main() {
    // 子进程发送数据给父进程，父进程读取到数据输出

    // 在fork之前创建管道，因为要指向一个管道
    int pipefd[2];

    int ret = pipe(pipefd);
    if (ret == -1) {
        perror("pipe");
        return -1;
    }

    // 管道创建成功了，现在创建子进程
    pid_t pid = fork();
    if (pid == -1) {
        perror("fork");
        return -1;
    }

    if (pid > 0) {  // 父进程
        printf("i am parent process , pid : %d\n", getpid());

        char buf[_size] = {0};

        int flags = fcntl(pipefd[0], F_GETFL);
        flags |= O_NONBLOCK;
        fcntl(pipefd[0], F_SETFL, flags);

        // 关闭写端
        close(pipefd[1]);

        while (1) {
            // 读数据
            // read默认是阻塞的
            // 设置成为非阻塞
            int len = read(pipefd[0], buf, sizeof(buf));
            printf("len = %d\n", len);
            printf("parent recv : \"%s\" , pid : %d\n", buf, getpid());
            // 读完清空buf
            bzero(buf, _size);

            sleep(1);  // 子进程和父进程睡眠的时间不同，这样可以方便的观察是否阻塞
        }

    } else if (pid == 0) {  // 子进程
        printf("i am child process , pid : %d\n", getpid());
        const char *str = "hello,i am child";

        // 关闭读端
        close(pipefd[0]);

        while (1) {
            // 写数据
            write(pipefd[1], str, strlen(str));
            sleep(5);
        }
    }

    return 0;
}

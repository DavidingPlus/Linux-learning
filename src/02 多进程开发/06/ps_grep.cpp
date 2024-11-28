#include <iostream>
using namespace std;
#include <fcntl.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>
#define _size 1024

/*
    实现 ps aux 父子进程之间通信

    子进程：ps aux，子进程结束之后将数据发送给父进程
    父进程：获取到数据，打印

    思路：
    子进程需要执行 ps aux 命令，调用exec族函数，但是这些函数的默认输出端是在stdout_fileno
    所以需要使用dup2()函数将其重定向到管道的写端
    将读取的内容存到文本中，然后去执行grep命令即可
*/

int main() {
    // 创建管道
    int pipefd[2];

    int ret = pipe(pipefd);
    if (ret == -1) {
        perror("pipe");
        return -1;
    }

    // 创建子进程
    pid_t pid = fork();
    if (pid == -1) {
        perror("fork");
        return -1;
    }

    if (pid > 0) {
        // 父进程

        // 关闭写端
        close(pipefd[1]);

        // 读
        char buf[_size] = {0};
        int len = -1;

        // 先打开，如果不存在则创建，存在则删除再创建
        // 建议不要重复打开，这里就打开一次然后写，最后关闭即可
        int fd = open("file.txt", O_RDONLY);
        if (fd != -1)            // 存在，将其删除
            unlink("file.txt");  // 不能用exec()函数族，因为这就把这个主进程替换了，不会回来，可以选择用 unlink() 或者 remove()
        close(fd);

        // 然后创建一个
        fd = open("file.txt", O_RDWR | O_CREAT, 0664);
        if (fd == -1) {
            perror("open");
            return -1;
        }

        // -1 留一个结束符
        // 循环读
        while ((len = read(pipefd[0], buf, sizeof(buf) - 1)) != 0) {
            if (len == -1) {
                perror("read");
                return -1;
            }

            // printf("%s", buf);
            write(fd, buf, strlen(buf));

            bzero(buf, _size);
        }

        close(fd);

        // grep筛选root
        execlp("grep", "grep", "root", "file.txt", nullptr);

        // 父进程回收子进程资源防止出现僵尸进程
        wait(nullptr);
    } else if (pid == 0) {
        // 子进程

        // 关闭读端
        close(pipefd[0]);

        // 将标准输入stdout_fileno重定向到管道的写端
        // dup2() newfd指向oldfd指向的位置，oldfd被释放
        int ret = dup2(pipefd[1], STDOUT_FILENO);
        if (ret == -1) {
            perror("dup2");
            return -1;
        }

        // 使用exec函数族执行shell命令，他输出靠的是的是stdout_fileno
        // 为防止管道大小不够，循环的去执行保证指令被写完
        while (ret = execlp("ps", "ps", "aux", nullptr))
            if (ret == -1) {
                perror("execlp");
                return -1;
            }
    }

    return 0;
}

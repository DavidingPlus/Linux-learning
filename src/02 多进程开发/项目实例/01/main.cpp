/*
一个进程，创建两个子进程，利用exec函数族使两个子进程执行不同的程序。
子进程1执行ls -l命令后正常返回，
子进程2暂停5s后异常返回，
父进程阻塞方式等待进程1，非阻塞方式等待进程2，等父进程收集到进程2的返回信息后就退出。
*/

#include <iostream>
using namespace std;
#include <sys/wait.h>
#include <unistd.h>

int main() {
    // 创建子进程
    pid_t pid, pid1, pid2;
    int i;

    for (i = 0; i < 2; ++i) {
        // i==0创建子进程1
        // 用pid1和pid2来分别表示不同的父进程眼中的不同子进程
        // 用i=0和1来表示子进程段不同的子进程
        if (0 == i)
            pid = pid1 = fork();
        else
            pid = pid2 = fork();

        if (pid == -1) {
            perror("fork");
            return -1;
        }

        if (pid == 0)
            break;
    }

    if (pid > 0) {
        // 父进程
        waitpid(pid1, nullptr, 0);  // 阻塞方式等待子进程1

        while (1) {
            int ret = waitpid(pid2, nullptr, WNOHANG);  // 非阻塞方式等待子进程2
            if (-1 == ret) {
                perror("waitpid");
                return -1;
            }

            if (0 != ret)  // 子进程2结束了，退出
                break;

            printf("child process 2 is stilling running.\n");
            sleep(1);
        }

        printf("parent process over.\n");

    } else if (pid == 0) {
        // i来表示是哪个子进程
        if (0 == i) {
            // 子进程1
            printf("child process 1 is running.\n");
            int ret = execlp("ls", "ls", "-l", nullptr);
            if (-1 == ret) {
                perror("execlp");
                return -1;
            }
        } else if (1 == i) {
            // 子进程2
            printf("child process 2 is running.\n");
            sleep(5);
            exit(-1);
        }
    }

    return 0;
}

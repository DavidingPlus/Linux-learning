#include <iostream>
using namespace std;
#include <sys/wait.h>
#include <unistd.h>

/*
    #include <sys/wait.h>

    pid_t wait(int *wstatus);
功能：等待任意一个子进程结束，如果任意一个子进程结束了，此函数会回收这个子进程的资源
参数：int *wstatus
    进程退出时候的状态信息，传入的是一个int类型的地址，传出参数
返回值：
    成功 返回被回收的子进程的id
    失败 -1(所有的子进程都结束，调用函数失败) 并且修改errno

调用wait()函数，进程会阻塞，知道他的一个子进程退出或者收到一个不能被忽略的信号，这个时候才被唤醒
如果没有子进程，这个函数立刻返回-1；如果子进程都已经结束了，也会返回-1
*/

int main() {
    // 有一个父进程，创建5个子进程
    pid_t pid;

    for (int i = 0; i < 5; ++i) {
        pid = fork();

        if (pid == -1) {
            perror("fork");
            return -1;
        }

        if (pid == 0)  // 说明是子进程，如果不加这行代码，子进程也会走for循环，他也会fork()产生更多的孙子进程，重孙进程等等
            break;
    }

    if (pid > 0) {
        // 父进程
        while (1) {
            printf("parent , pid = %d\n", getpid());

            // int ret = wait(NULL);

            int status;
            int ret = wait(&status);

            if (ret == -1)  // 没有子进程
                break;

            // 有子进程，回收了子进程的资源
            if (WIFEXITED(status)) {  // 是不是正常退出
                printf("退出的状态码: %d\n", WEXITSTATUS(status));
            } else if (WIFSIGNALED(status)) {  // 是不是异常退出
                printf("被哪个信号干掉了: %d\n", WTERMSIG(status));
            }

            printf("child die,pid = %d\n", ret);

            sleep(1);
        }
    } else if (pid == 0) {
        // 子进程
        while (1) {
            printf("child , pid = %d\n", getpid());
            sleep(1);
        }

        exit(0);
    }

    return 0;
}

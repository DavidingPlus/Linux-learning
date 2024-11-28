#include <iostream>
using namespace std;
#include <signal.h>
#include <sys/wait.h>
#include <unistd.h>

/*
    SIGCHLD信号产生的三个条件
    - 子进程结束
    - 子进程暂停
    - 子进程从暂停状态继续运行
    都会给父进程发送该信号，父进程默认忽略该信号

    可以使用SIGCHLD信号解决僵尸进程的问题

*/

void myFunc(int num) {
    printf("捕捉到的信号 : %d\n", num);
    // 回收子进程PCB的资源
    // wait(nullptr);

    while (1) {
        int ret = waitpid(-1, nullptr, WNOHANG);
        if (ret > 0) {
            printf("chile die , pid = %d\n", getpid());
        } else if (0 == ret)
            // 说明还有子进程，这一次的循环捕捉回收没回收完毕
            break;
        else if (-1 == ret)
            // 说明没有子进程了
            break;
    }
}

int main() {
    // 创建子进程
    pid_t pid;
    for (int i = 0; i < 20; ++i) {
        pid = fork();
        if (0 == pid)
            break;
    }

    if (pid > 0) {
        // 父进程

        // 提前设置好阻塞信号集，阻塞SIGCHLD，因为子进程可能很快结束，父进程还没注册好
        sigset_t set;
        sigemptyset(&set);
        sigaddset(&set, SIGCHLD);
        sigprocmask(SIG_BLOCK, &set, nullptr);

        // 捕捉子进程死亡时发送的SIGCHLD信号
        struct sigaction act;
        act.sa_flags = 0;
        act.sa_handler = myFunc;
        sigemptyset(&act.sa_mask);

        sigaction(SIGCHLD, &act, nullptr);

        // 注册完信号捕捉之后解除阻塞
        sigprocmask(SIG_UNBLOCK, &set, nullptr);

        while (1) {
            printf("parent process pid : %d\n", getpid());
            sleep(2);
        }
    } else if (pid == 0) {
        // 子进程
        printf("child process pid : %d\n", getpid());
        // sleep(1);
    }

    return 0;
}

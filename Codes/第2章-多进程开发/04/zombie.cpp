#include <iostream>
using namespace std;
#include <unistd.h>

int main() {
    int pid = fork();
    if (pid == -1) {
        perror("fork");
        return -1;
    }

    // 判断是父进程还是子进程
    if (pid > 0) {   // 父进程走，返回的是创建的子进程编号
        while (1) {  // 强制让父进程一直循环，不退出，让子进程结束，父进程没办法回收他的资源
            printf("I am parent process, pid : %d , ppid : %d\n", getpid(), getppid());
            sleep(1);
        }
    } else if (pid == 0) {  // 子进程走
        printf("I am child process, pid : %d , ppid : %d\n", getpid(), getppid());
    }

    return 0;
}

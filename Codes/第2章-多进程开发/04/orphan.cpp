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
    if (pid > 0) {  // 父进程走，返回的是创建的子进程编号
        printf("I am parent process, pid : %d , ppid : %d\n", getpid(), getppid());
    } else if (pid == 0) {  // 子进程走
        sleep(1);           // 强制让子进程睡1秒，让父进程跑完，子进程称为孤儿进程
        printf("I am child process, pid : %d , ppid : %d\n", getpid(), getppid());
    }

    // for
    for (int i = 0; i < 3; ++i) {
        printf("i : %d , pid : %d\n", i, getpid());
    }

    return 0;
}

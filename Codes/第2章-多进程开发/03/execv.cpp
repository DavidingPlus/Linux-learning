#include <iostream>
using namespace std;
#include <unistd.h>

/*
    #include <unistd.h>
    int execv(const char *path, char *const argv[]);
参数：
    path：文件路径
    argv：是需要的参数的一个字符串数组

返回值：
    只有调用错误，才会有返回值，为-1，且设置errno
    调用成功不会有返回值

*/

int main() {
    // 创建一个子进程，在子进程中执行exec函数族当中的函数
    pid_t child_pid = fork();
    if (child_pid == -1) {
        perror("fork");
        return -1;
    }

    if (child_pid > 0) {
        // 父进程
        printf("I am parent process , pid : %d\n", getpid());
        sleep(1);
    } else if (child_pid == 0) {
        // 子进程
        // execl("hello", "hello", nullptr);

        // 可以执行linux系统自己的可执行程序，比如ps命令(/usr/bin/ps)
        // 用字符串数组来保存命令的参数
        char* argv[] = {"ps", "aux", nullptr};
        execv("/usr/bin/ps", argv);

        printf("I am child process , pid : %d\n", getpid());
    }

    for (int i = 0; i < 3; ++i)
        printf("i = %d , pid = %d\n", i, getpid());

    return 0;
}

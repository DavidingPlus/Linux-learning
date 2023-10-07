#include <cstring>
#include <iostream>
using namespace std;
#include <sys/wait.h>
#include <unistd.h>

#define MAX_BUFFER_SIZE 1024

int main() {
    char buf[MAX_BUFFER_SIZE] = {0};

    while (1) {
        // 读取命令(简单的没有参数的)
        bzero(buf, sizeof(buf));
        fgets(buf, sizeof(buf) - 1, stdin);
        // 将buf中的'\n'字符替换成'\0'，不然命令会出错
        buf[strlen(buf) - 1] = 0;

        pid_t pid = fork();
        if (-1 == pid) {
            perror("fork");
            return -1;
        }

        if (pid > 0) {
            // 父进程回收子进程，等待执行，回收一次读取下一次命令
            waitpid(pid, nullptr, 0);
            printf("child process has been recycled, pid :%d\n", pid);
        } else if (0 == pid)
            // 子进程调用exec函数族执行命令
            execlp(buf, buf, nullptr);
    }

    return 0;
}

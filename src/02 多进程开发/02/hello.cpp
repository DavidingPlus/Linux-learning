#include <iostream>
using namespace std;
#include <unistd.h>

int main() {
    printf("begin\n");

    pid_t child_pid = fork();
    if (child_pid == -1) {
        perror("fork");
        return -1;
    }

    if (child_pid > 0) {
        printf("我是父进程: pid = %d, ppid = %d\n", getpid(), getppid());

        int i;
        for (i = 0; i < 10; i++) {
            printf("i = %d\n", i);
            sleep(1);
        }

    } else if (child_pid == 0) {
        printf("我是子进程: pid = %d, ppid = %d\n", getpid(), getppid());

        int j;
        for (j = 0; j < 10; j++) {
            printf("j = %d\n", j);
            sleep(1);
        }
    }

    return 0;
}

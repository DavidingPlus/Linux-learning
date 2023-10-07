#include <iostream>
using namespace std;
#include <unistd.h>

/*
    #include <unistd.h>

    pid_t fork(void);
作用：用于创建子进程
返回值：
    fork()的返回值会返回两次，一次是在父进程中，一次是在子进程中
    为什么会走两次呢？因为这个cpp文件编译出来是一个可执行文件 a.out，这个文件执行后是进程 ./a.out，自然创建出来的也是 ./a.out进程，也就是走的是这个cpp程序，所以会有两个pid
    但是注意一点：子进程是不会执行fork()函数的，因为fork()函数是父进程执行的，但是他会持有返回值，在子进程当中就是0
    成功 父进程中返回子进程的pid，子进程中返回0
    失败 父进程中返回-1，没有子进程被创建，然后修改errno
*/

int main() {
    int num = 10;

    int pid = fork();
    if (pid == -1) {
        perror("fork");
        return -1;
    }

    // 判断是父进程还是子进程
    if (pid > 0) {  // 父进程走，返回的是创建的子进程编号
        cout << pid << endl;
        printf("I am parent process, pid : %d , ppid : %d\n", getpid(), getppid());

        printf("parent num : %d\n", num);
        num += 10;
        printf("parent num +=10 : %d\n", num);

    } else if (pid == 0) {  // 子进程走
        printf("I am child process, pid : %d , ppid : %d\n", getpid(), getppid());

        printf("child num : %d\n", num);
        num += 100;
        printf("child num +=100 : %d\n", num);
    }

    // for
    for (int i = 0;; ++i) {
        printf("i : %d , pid : %d\n", i, getpid());
        sleep(1);
    }

    return 0;
}

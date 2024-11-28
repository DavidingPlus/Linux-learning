#include <iostream>
using namespace std;
#include <sys/wait.h>
#include <unistd.h>

/*
    #include <sys/wait.h>

    pid_t waitpid(pid_t pid, int *_Nullable wstatus, int options);
作用：回收指定进程号的子进程，可以设置是否阻塞
参数：
    pid：> 0 表示某个子进程的id
        == 0 回收当前进程组的所有子进程(我自己的子进程不一定和我属于一个组，有可能被我给出去了)
        == -1 回收所有的子进程，相当于wait()，最常用
        < -1 回收某个进程组当中的所有子进程，组号是这个参数的绝对值
    options：设置阻塞或者非阻塞
        0 阻塞
        WNOHANG：非阻塞
返回值：
    > 0 返回子进程的id
    == 0 options = WNOHANG，表示还有子进程活着
    ==-1 错误或者没有子进程了

这么来看,waitpid(-1,&status,0) 相当于是 wait(&status)
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
            sleep(1);

            // int ret = wait(NULL);

            int status;
            // int ret = waitpid(-1, &status, 0);        // 阻塞
            int ret = waitpid(-1, &status, WNOHANG);  // 非阻塞

            if (ret == -1)  // 没有子进程
                break;
            else if (ret == 0)
                // 非阻塞就是执行到这个位置判断一下，然后遇到了就回收了，没有就走了
                // ret==0表明还有子进程活着，重开循环判断
                // 非阻塞的好处：父进程不用一直阻塞这等待子进程结束，可以做自己的逻辑，然后每隔一段时间就回来看子进程是否运行完毕然后回收。提高效率
                continue;
            else if (ret > 0) {
                // 有子进程，回收了子进程的资源
                if (WIFEXITED(status)) {  // 是不是正常退出
                    printf("退出的状态码: %d\n", WEXITSTATUS(status));
                } else if (WIFSIGNALED(status)) {  // 是不是异常退出
                    printf("被哪个信号干掉了: %d\n", WTERMSIG(status));
                }
                printf("child die,pid = %d\n", ret);
            }
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

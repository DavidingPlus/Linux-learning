#include <iostream>
using namespace std;
#include <signal.h>

/*
    #include <signal.h>

    int kill(pid_t pid, int sig);
作用：给任何的进程或者进程组pid，发送任何的信号sig
参数：
    pid：
        >  0  将信号发送给指定的进程
        == 0  将信号发送给当前的进程组中所有的进程
        == -1 将信号发送给每一个有权限接受这个信号的进程
        <  -1 这个pid = 某个进程组的ID的相反数，给这个进程组中所有的进程发送信号

    sig：需要发送的信号编号或者宏值，如果是0则表示不发送任何信号
返回值：成功 0 ; 失败 -1，并设置errno


    int raise(int sig);
作用：给当前进程发送信号
参数：sig：需要发送的信号编号或者宏值，如果是0则表示不发送任何信号
返回值： 成功 0 ; 失败 非 0
    kill(getpid(),sig);


    void abort(void);
功能：发送一个SIGABRT信号给当前的进程，默认是杀死当前的进程
    kill(getpid(),SIGABRT);
*/

int main() {
    pid_t pid = fork();
    if (pid == -1) {
        perror("fork");
        return -1;
    }

    if (pid > 0) {
        printf("parent process\n");
        sleep(2);
        printf("kill child process now\n");
        kill(pid, SIGINT);
    } else if (pid == 0) {
        for (int i = 0; i < 5; ++i) {
            printf("child process\n");
            sleep(1);
        }
    }

    return 0;
}

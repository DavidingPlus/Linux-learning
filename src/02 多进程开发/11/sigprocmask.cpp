#include <iostream>
using namespace std;
#include <signal.h>
#include <unistd.h>

/*
    #include <signal.h>

    int sigprocmask(int how, const sigset_t *_Nullable restrict set,
                                sigset_t *_Nullable restrict oldset);
功能：将自定义信号集中的数据设置到内核当中(设置阻塞，接触阻塞，替换)
参数：
    how：如何对内核阻塞信号集进行处理
        SIG_BLOCK：将用户设置的阻塞信号集添加到内核中，原来的数据不变
            假设中内核中默认的阻塞信号集是mask，则 mask | set (添加的公式)
        SIG_UNBLOCK：根据用户设置的数据，对内核中的数据进行接触阻塞
            mask & = ~ set (去除的公式)
            比如 mask 1 0 1 1 1 ，set 0 0 1 0 1，解除这两位的阻塞
            那么就是 ~set 1 1 0 1 0 ，然后想与就得到 1 0 0 1 0
        SIG_SETMASK：覆盖内核中原来的值

    set：已经初始化好的用户自定义的信号集
    oldset：保存的之前内核中的阻塞信号集的状态，传出参数，一般不使用，设置为nullptr即可
返回值：
    成功 0
    失败 -1，并且设置errno，有两个值：EFAULT，EINVAL

    int sigpending(sigset_t *set);
功能：获取内核中的未决信号集
参数：set，传出参数，保存的是内核中的未决信号集
返回值：
    成功 0，失败 -1，设置errno
*/

// 编写一个程序，把所有的常规信号(1-31)的未决状态打印到屏幕
// 设置某些信号是阻塞的，通过键盘产生这些信号
int main() {
    // 设置 2号信号 SIGINT(ctrl+C) 和 3号信号SIGQUIT(ctrl+\) 阻塞
    sigset_t set;
    // 清空
    int ret = sigemptyset(&set);
    if (-1 == ret) {
        perror("sigemptyset");
        return -1;
    }

    // 将2号和3号信号添加进去
    ret = sigaddset(&set, SIGINT);
    if (-1 == ret) {
        perror("sigaddset");
        return -1;
    }
    ret = sigaddset(&set, SIGQUIT);
    if (-1 == ret) {
        perror("sigaddset");
        return -1;
    }

    // 修改内核中的信号集
    ret = sigprocmask(SIG_BLOCK, &set, nullptr);
    if (-1 == ret) {
        perror("sigprocmask");
        return -1;
    }

    int count = 0;

    // 在循环当中获取未决信号集的数据
    while (1) {
        sigset_t pendingset;
        ret = sigemptyset(&pendingset);
        if (-1 == ret) {
            perror("sigemptyset");
            return -1;
        }

        sigpending(&pendingset);

        // 遍历前32位 即1-31号(0号没用)
        for (int i = 1; i < 32; ++i) {
            ret = sigismember(&pendingset, i);
            if (-1 == ret) {
                perror("sigismember");
                return -1;
            }

            if (1 == ret)
                printf("1");
            else if (0 == ret)
                printf("0");
        }
        puts("");

        // 为了防止只能通过kill -9 命令杀死该进程，现在我们计数，到10就接触阻塞
        if (count++ == 10) {
            printf("2号信号SIGINT和3号信号SIGQUIT已经解除阻塞\n");
            ret = sigprocmask(SIG_UNBLOCK, &set, nullptr);
            if (-1 == ret) {
                perror("sigprocmask");
                return -1;
            }
        }
        sleep(1);
    }

    return 0;
}

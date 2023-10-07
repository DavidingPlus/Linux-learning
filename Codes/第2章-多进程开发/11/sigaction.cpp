#include <iostream>
using namespace std;
#include <signal.h>
#include <sys/time.h>

/*
    #include <signal.h>

    int sigaction(int signum,
                        const struct sigaction *_Nullable restrict act,
                        struct sigaction *_Nullable restrict oldact);
作用：用来检查或者改变信号的处理，信号捕捉
参数：
    signum：需要捕捉的信号的编号或者宏值
    act：捕捉到信号之后相应的处理动作
    oldact：上一次对信号捕捉的相关的设置，一般不使用，传递nullptr
返回值：
    成功 0
    失败 -1，设置errno

    struct sigaction {
                //函数指针，指向的函数就是信号捕捉到之后的处理函数
                void     (*sa_handler)(int);
                //函数指针，一般不使用
                void     (*sa_sigaction)(int, siginfo_t *, void *);
                //临时阻塞信号集，在信号捕捉函数执行过程中会临时阻塞某些信号，执行完之后恢复
                sigset_t   sa_mask;
                //指定是用第一个回调处理sa_handler还是第二个sa_sigaction，0表示第一个，SA_SIGINFO表示第二个，还有其他的值，但是用的少
                int        sa_flags;
                //被废弃掉了，不需要用，传入nullptr
                void     (*sa_restorer)(void);
            };

*/

void myalarm(int num) {
    printf("捕捉到了信号的编号是: %d\n", num);
}

int main() {
    struct sigaction act;
    act.sa_flags = 0;
    act.sa_handler = myalarm;
    sigemptyset(&act.sa_mask);  // 清空吧，表示不要临时阻塞任何信号

    int ret = sigaction(SIGALRM, &act, nullptr);
    if (-1 == ret) {
        perror("signal");
        return -1;
    }

    // 过三秒，会发送信号
    struct timeval _value;
    _value.tv_sec = 3;
    _value.tv_usec = 0;

    // 每隔两秒，会发送信号
    struct timeval _interavl;
    _interavl.tv_sec = 2;
    _interavl.tv_usec = 0;

    // itimerval结构体
    struct itimerval new_value;
    new_value.it_value = _value;
    new_value.it_interval = _interavl;

    // 设置定时器
    int rets = setitimer(ITIMER_REAL, &new_value, nullptr);  // 非阻塞
    printf("定时器开始了\n");                                // 立刻执行，表明是非阻塞的
    if (rets == -1) {
        perror("setitimer");
        return -1;
    }

    while (1)
        ;

    return 0;
}

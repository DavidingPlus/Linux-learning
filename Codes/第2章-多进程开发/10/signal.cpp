#include <iostream>
using namespace std;
#include <signal.h>
#include <sys/time.h>

/*
    #include <signal.h>

    typedef void (*sighandler_t)(int);
    sighandler_t signal(int signum, sighandler_t handler);
作用：设置某个信号的捕捉行为，
参数：
    signum：要捕捉的信号
        注意：SIGKILL 和 SIGSTOP 不能被捕捉，不能被忽略；
            因为这两个信号都是带有强制性的杀死或者暂停进程，这个是需要保证权威的，否则强制性都解决不了就可以被不法分子利用了，比如制作病毒让进程一直运行消耗资源，这两个信号没有办法解决
    hander：捕捉到信号要如何处理
        SIG_IGN：忽略信号
        SIG_DFL：用信号默认的行为
        回调函数：这个函数是内核调用，程序员只负责写，捕捉到信号后如何去处理信号
            回调函数需要程序员实现，提前准备好，函数的类型根据实际需求，看函数指针的定义
            不是程序员调用的，而是当信号产生由内核调用
            函数指针是实现回调的手段，函数实现后，将函数名放到函数指针的位置就可以了

返回值：
    成功，返回上一次注册的信号处理函数的地址；第一次返回nullptr
    失败，返回SIG_ERR，设置errno
*/

void myalarm(int num) {
    printf("捕捉到了信号的编号是: %d\n", num);
}

int main() {
    // 注册信号捕捉，需要提前注册，避免定时器开始执行后可能信号捕捉还没生效导致错过信号捕捉的情况
    // signal(SIGALRM, SIG_IGN);  // 信号产生后忽略信号，程序会一直执行
    // signal(SIGALRM, SIG_DFL);  // 按照默认的方式处理信号，程序延迟3秒的时候开始计时，发送信号然后终止

    // typedef void (*sighandler_t)(int); 函数指针的类型，int类型的参数表示捕捉到的信号的值
    sighandler_t ret = signal(SIGALRM, myalarm);
    if (ret == SIG_ERR) {
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

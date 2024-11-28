#include <iostream>
using namespace std;
#include <sys/time.h>

/*
    #include <sys/time.h>

    int setitimer(int which, const struct itimerval *restrict new_value,
                struct itimerval *_Nullable restrict old_value);
作用：设置定时器；可以替代alarm函数。精度可以达到微秒，并且还可以实现周期性的定时
参数：
    which：指定的是定时器以什么时间计时
        - ITIMER_REAL：真实时间(包含内核+用户+消耗的时间(例如I/O))，时间到达发送 SIGALRM 常用
        - ITIMER_VIRTUAL：用户时间，时间到达发送 SIGVTALRM
        - ITIMER_PROF：以该进程在用户态和内核态所消耗的时间来计算，时间到达发送 SIGPROF

    new_value：设置定时器属性

        struct itimerval { //定时器的结构体
            struct timeval it_interval;  // 每个阶段的时间，间隔时间
            struct timeval it_value;     // 延迟多长时间执行定时器
        };

        struct timeval { //时间的结构体
            time_t tv_sec;        // 秒数
            suseconds_t tv_usec;  // 微秒
        };

        eg：过10秒(it_value)后，每隔2秒(it_interval)定时一次

    old_value：记录上一次定时的时间参数，是一个传出参数，函数将上一次的状态心如进去，一般不使用，就指定nullptr就可以了
返回值：
    成功 0
    失败 -1，设置errno

*/

// 过3秒以后，每隔2秒定时一次
int main() {
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
    int ret = setitimer(ITIMER_REAL, &new_value, nullptr);  // 非阻塞
    printf("定时器开始了\n");                               // 立刻执行，表明是非阻塞的
    if (ret == -1) {
        perror("setitimer");
        return -1;
    }

    while (1)
        ;

    return 0;
}

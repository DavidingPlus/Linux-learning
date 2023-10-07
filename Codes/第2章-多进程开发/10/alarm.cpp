#include <iostream>
using namespace std;
#include <unistd.h>

/*
    #include <unistd.h>

    unsigned int alarm(unsigned int seconds);
作用：去设置定时器；函数调用，开始倒计时；
    当倒计时为0的时候，函数会给当前的进程发送一个信号 SIGALARM
参数：
    seconds：倒计时时长，单位秒，当参数为0的时候，计时器无效(不进行倒计时，也不发送信号)
        取消一个定时器 alarm(0)
返回值：
    - 之前没有定时器，返回0
    - 之前有定时器，返回之前定时器剩余的时间

- SIGALARM 信号：默认终止当前的进程，每一个进程都有且只有唯一的一个定时器
    alarm(10); ->返回0
    //过了一秒
    alarm(5);  ->返回9

该函数不阻塞，设置之后会继续往下执行
*/

int main() {
    int seconds = alarm(5);             // 不阻塞
    printf("seconds : %d\n", seconds);  // 0

    sleep(2);
    seconds = alarm(2);
    printf("seconds : %d\n", seconds);  // 3

    while (1)
        ;

    return 0;
}

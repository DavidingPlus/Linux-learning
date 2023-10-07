#include <iostream>
using namespace std;
#include <unistd.h>

/*
    实际的时间 = 内核时间 + 用户时间 + 消耗的时间(比如I/O操作)
    进行文件I/O操作的时候比较浪费时间

    定时器，和进程的状态无关(自然定时法)；无论进程处于什么状态，这个alarm()都会计时
*/

// 1秒钟电脑能数多少个数
int main() {
    alarm(1);

    int i = 0;
    while (1)
        printf("%d\n", i++);

    return 0;
}

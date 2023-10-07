#include <iostream>
using namespace std;
#include <signal.h>

/*
    #include <signal.h>

以下的信号集相关的函数都是对自定义的信号集进行操作，我们不能直接修改系统当中的未决信号集和阻塞信号集!!!

    int sigemptyset(sigset_t *set);
功能：清空信号集中的数据，将信号集中的所有标志位置为0
参数：set，传出参数，需要操作的信号集
返回值：成功 0；失败 -1，修改errno

    int sigfillset(sigset_t *set);
功能：将信号集中的所有标志位置为1
参数：set，传出参数，需要操作的信号集
返回值：成功 0；失败 -1，修改errno

    int sigaddset(sigset_t *set, int signum);
功能：设置信号集中的某一个信号对应的标志位为1，表示阻塞这个信号
参数：set，传出参数，需要操作的信号集；signum：需要设置为阻塞的信号
返回值：成功 0；失败 -1，修改errno

    int sigdelset(sigset_t *set, int signum);
功能：设置信号集中的某一个信号对应的标志位为0，表示不阻塞这个信号
参数：set，传出参数，需要操作的信号集；signum：需要设置不为阻塞的信号
返回值：成功 0；失败 -1，修改errno

    int sigismember(const sigset_t *set, int signum);
功能：判断某个信号是否阻塞
参数：set，需要操作的信号集；signum：需要查看是否阻塞的信号
返回值：(与前面不一样!!!)
    1 是成员，signum被阻塞；0 不是成员，不阻塞
    -1 表示失败，修改errno

*/

void Judge(const sigset_t& set, const int& signum) {
    int ret = sigismember(&set, signum);
    if (ret == -1) {
        perror("sigismember");
        exit(-1);
    }

    if (ret == 1)
        printf("信号%d在set当中\n", signum);
    else if (ret == 0)
        printf("信号%d不在set当中\n", signum);
}

int main() {
    // 创建一个信号集
    sigset_t set;

    // 这么创建的数据一般是随机的，我们一般用系统的api清空
    int ret = sigemptyset(&set);
    if (ret == -1) {
        perror("sigemptyset");
        return -1;
    }

    // 判断SIGINT是否在信号集set中
    Judge(set, SIGINT);  // 2号信号不在

    // 添加几个信号
    ret = sigaddset(&set, SIGINT);
    if (ret == -1) {
        perror("sigaddset");
        return -1;
    }

    ret = sigaddset(&set, SIGQUIT);
    if (ret == -1) {
        perror("sigaddset");
        return -1;
    }

    // 判断是否在信号集set中
    Judge(set, SIGINT);   // 2号信号在
    Judge(set, SIGQUIT);  // 3号信号在

    // 删除一个信号
    ret = sigdelset(&set, SIGQUIT);
    if (ret == -1) {
        perror("sigdelset");
        return -1;
    }

    // 判断SIGQUIT是否在信号集set中
    Judge(set, SIGQUIT);  // 3号信号不在

    return 0;
}

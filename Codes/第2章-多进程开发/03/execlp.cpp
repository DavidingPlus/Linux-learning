#include <iostream>
using namespace std;
#include <unistd.h>

/*
    #include <unistd.h>
    int execlp(const char *file, const char *arg, ... , (char *) NULL );
参数：
    会到环境变量中查找可执行文件，找到了就执行，找不到就不成功!!!
    file：需要执行的可执行文件的文件名，不包含路径!!!


    arg：字符串，是可执行文件所需要的参数列表
        第一个参数(arg当中的第一个参数!!)一般没有作用，为了方便，一般写的是可执行的程序的名称
        从第二参数开始往后，就是程序执行所需要的参数列表
        参数最后需要以NULL结束(哨兵)

返回值：
    只有调用错误，才会有返回值，为-1，且设置errno
    调用成功不会有返回值

*/

int main() {
    // 创建一个子进程，在子进程中执行exec函数族当中的函数
    pid_t child_pid = fork();
    if (child_pid == -1) {
        perror("fork");
        return -1;
    }

    if (child_pid > 0) {
        // 父进程
        printf("I am parent process , pid : %d\n", getpid());
        sleep(1);
    } else if (child_pid == 0) {
        // 子进程
        // execl("hello", "hello", nullptr);

        // 可以执行linux系统自己的可执行程序，比如ps命令(/usr/bin/ps)
        execlp("ps", "ps", "aux", nullptr);

        printf("I am child process , pid : %d\n", getpid());
    }

    for (int i = 0; i < 3; ++i)
        printf("i = %d , pid = %d\n", i, getpid());

    return 0;
}

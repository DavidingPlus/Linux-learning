#include <cstring>
#include <ctime>
#include <iostream>
using namespace std;
#include <fcntl.h>
#include <signal.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <unistd.h>

// 写一个守护进程，每隔两秒获取系统时间，将这个时间写到磁盘文件中

void _deal(int num) {
    // 获取系统时间写入磁盘文件
    time_t _time = time(nullptr);
    // 将time()获得的距离计算机元年(1970-1-1 00:00:00)的秒数转化为当前的时间
    struct tm* _localtime = localtime(&_time);

    const char* str = asctime(_localtime);
    // 如果不存在则创建，存在则追加
    int ret = access("time.txt", F_OK);
    int fd = -1;
    if (-1 == ret)
        // 不存在
        fd = open("time.txt", O_RDWR | O_CREAT, 0664);
    else if (0 == ret)
        // 存在
        fd = open("time.txt", O_RDWR | O_APPEND);
    if (-1 == fd) {
        perror("open");
        exit(-1);
    }

    ret = write(fd, str, strlen(str));
    if (-1 == ret) {
        perror("write");
        exit(-1);
    }
}

int main() {
    // 创建子进程，退出父进程
    pid_t pid = fork();
    if (-1 == pid) {
        perror("fork");
        return -1;
    }

    if (pid > 0)
        // 父进程
        return 0;
    else if (0 == pid) {
        // 子进程

        // 如果存在time.txt，将其删除，准备工作
        int ret = access("time.txt", F_OK);
        if (0 == ret)
            unlink("time.txt");

        // 在子进程中重新创建一个会话，脱离原来的控制终端
        pid_t sid = setsid();
        if (-1 == pid) {
            perror("setsid");
            return -1;
        }

        // 设置umask
        umask(022);

        // 更改工作目录
        chdir("/mnt/d/Code/Cpp/深入学习/Linux方向/牛客网Linux网络课程/第2章-多进程开发/13");

        // 关闭，以及重定向文件描述符
        int fd = open("/dev/null", O_RDWR);
        dup2(fd, STDIN_FILENO);
        dup2(fd, STDOUT_FILENO);
        dup2(fd, STDERR_FILENO);

        // 业务逻辑

        // 注册信号捕捉器
        struct sigaction _act;
        _act.sa_flags = 0;
        _act.sa_handler = _deal;
        sigemptyset(&_act.sa_mask);
        sigaction(SIGALRM, &_act, nullptr);

        // 创建定时器
        itimerval _new;
        // 延迟时间
        _new.it_interval.tv_sec = 2;
        _new.it_interval.tv_usec = 0;
        // 周期时间
        _new.it_value.tv_sec = 2;
        _new.it_value.tv_usec = 0;

        ret = setitimer(ITIMER_REAL, &_new, nullptr);
        if (-1 == ret) {
            perror("setitimer");
            return -1;
        }

        // 不让进程结束，不然无法记录
        while (1)
            sleep(10);
    }

    return 0;
}

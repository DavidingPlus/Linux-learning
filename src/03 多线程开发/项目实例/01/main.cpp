/*
任务 1：编写程序,主线程创建 3 个对等线程 T1、T2、T3，每个线程利用循环执行 5 次
printf 输出操作，两次循环间随机等待 1-5s 时间。主线程等待所有对等线程结束后终止进程。各对等
线程的输出操作是：
T1：输出“My name is <您的姓名 xxx>”
T2：输出“My student number is <您的学号 xxx>”
T3：输出“Current time <当前时间，包括年月日时分秒>
*/

#include <cstring>
#include <ctime>
#include <iostream>
using namespace std;
#include <pthread.h>
#include <unistd.h>

void* CALLBACK_name(void* arg) {
    for (int i = 0; i < 5; ++i) {
        printf("My name is lzx0626.(%d)\n", i + 1);
        sleep(rand() % 5 + 1);
    }

    return nullptr;
}

void* CALLBACK_number(void* arg) {
    for (int i = 0; i < 5; ++i) {
        printf("My student number is 2021091202022.(%d)\n", i + 1);

        sleep(rand() % 5 + 1);
    }

    return nullptr;
}

void* CALLBACK_time(void* arg) {
    time_t _time = 0;
    struct tm* _local;
    for (int i = 0; i < 5; ++i) {
        // 获得时间
        time(&_time);
        _local = localtime(&_time);

        const char* TIME = asctime(_local);

        printf("Current time is %s", TIME);

        sleep(rand() % 5 + 1);
    }

    return nullptr;
}

int main() {
    // 创建线程
    pthread_t tid1, tid2, tid3;

    pthread_create(&tid1, nullptr, CALLBACK_name, nullptr);
    pthread_create(&tid2, nullptr, CALLBACK_number, nullptr);
    pthread_create(&tid3, nullptr, CALLBACK_time, nullptr);

    // 主线程分离线程，不用手动释放
    pthread_detach(tid1);
    pthread_detach(tid2);
    pthread_detach(tid3);

    // 主线程执行的很快，需要退出主线程来防止进程结束
    pthread_exit(nullptr);

    return 0;
}

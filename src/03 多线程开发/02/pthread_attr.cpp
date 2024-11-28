#include <cstring>
#include <iostream>
using namespace std;
#include <pthread.h>
#include <unistd.h>

/*
    #include <pthread.h>

    int pthread_attr_init(pthread_attr_t *attr);
作用：初始化线程属性变量

    int pthread_attr_destroy(pthread_attr_t *attr);
作用：释放线程属性资源

    int pthread_attr_setdetachstate(pthread_attr_t *attr, int detachstate);
作用：获取线程分离的状态属性

    int pthread_attr_getdetachstate(const pthread_attr_t *attr, int *detachstate);
作用：设置线程分离的状态属性
*/

void* call_back(void* arg) {
    printf("child thread id : %ld\n", pthread_self());

    return nullptr;
}

int main() {
    // 创建一个线程属性变量
    pthread_attr_t attr;
    // 初始化属性变量
    pthread_attr_init(&attr);
    // 设置属性
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);  // 表示设置了线程分离

    // 创建一个线程
    pthread_t tid;
    int ret = pthread_create(&tid, &attr, call_back, nullptr);  // 这里第二个参数，表示线程属性就需要传递进来了
    if (0 != ret) {
        const char* errstr = strerror(ret);
        printf("error pthread_create : %s\n", errstr);
        return -1;
    }

    // 获取线程栈的大小
    size_t _size;

    pthread_attr_getstacksize(&attr, &_size);
    printf("thread stack size : %ld\n", _size);

    // 输出主线程和子线程的id
    printf("tid : %ld , main thread id : %ld\n", tid, pthread_self());

    // 释放线程属性资源，初始化了必要释放!!!
    pthread_attr_destroy(&attr);

    // 退出主线程防止主线程结束导致进程结束导致程序结束
    pthread_exit(nullptr);

    return 0;
}

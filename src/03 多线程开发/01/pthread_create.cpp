#include <cstring>
#include <iostream>
using namespace std;
#include <pthread.h>
#include <unistd.h>

/*
    一般情况下，main函数所在的线程称为主线程(main线程)，其余创建的线程称为子线程
    程序中默认只有一个进程，fork()函数调用，变为2个进程
    程序中默认只有一个线程，pthread_create()调用，变为2个线程

    #include <pthread.h>

    int pthread_create(pthread_t *restrict thread,
                        const pthread_attr_t *restrict attr,
                        void *(*start_routine)(void *),
                        void *restrict arg);
作用：创建一个子线程
参数：
    thread：类型是pthread_t指针，传出参数，线程创建成功后，子线程的线程ID被写到在变量中
    attr：设置线程的属性，一般使用默认值，传递nullptr
    start_routine：函数指针，这个函数是子线程需要处理的逻辑代码
    arg：给第三个参数使用，传参
返回值：
    成功 0
    失败 返回一个错误号，这个错误号和之前的errno不太一样(实现方式一样，但是含义不同)
        不能通过perror()去获取错误号信息
        如何获取？ char* strerror(int errnum);

*/

// 主线程和子线程执行的代码段是不一样的，这个回调函数里面是子线程执行的代码逻辑
void* call_back(void* args) {
    printf("child thread...\n");
    printf("arg value : %d\n", *(int*)args);

    return nullptr;
}

// main函数里面是主线程执行的逻辑
int main() {
    // 创建一个子线程
    pthread_t tid;

    int num = 10;

    int ret = pthread_create(&tid, nullptr, call_back, (void*)&num);
    if (0 != ret) {
        const char* _error = strerror(ret);
        printf("error : %s\n", _error);
        return -1;
    }

    for (int i = 0; i < 5; ++i)
        printf("%d\n", i);

    sleep(1);  // 保证子线程万一没有创建好主线程就执行完了

    return 0;
}

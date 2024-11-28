#include <cstring>
#include <iostream>
using namespace std;
#include <pthread.h>
#include <unistd.h>

/*
    #include <pthread.h>

    void pthread_exit(void *retval);
作用：终止一个线程，在哪个线程中调用，就表示终止哪个线程
参数：
    retval：需要传递一个指针，作为一个返回值，可以在pthread_join()中获取到

    pthread_t pthread_self(void);
作用：获取当前线程的线程id(unsigned long int 无符号长整形)

    int pthread_equal(pthread_t t1.pthread_t t1);
作用：比较两个线程id是否相等
    不同的操作系统对于 pthread_t 的实现不一样，有的是无符号的长整型，有的是用结构体去实现的，不能简单的用 == 号判断

*/

void* call_back(void* arg) {
    // 为了测试pthread_exit()会不会影响其他线程的正常运行，这里睡三秒，让主线程跑完
    sleep(3);
    printf("child thread id : %ld\n", pthread_self());

    // 这里两个对等是因为子线程结束并不决定整个进程的结束，主线程 return 0 就代表进程结束退出，子线程则不一样
    return nullptr;  // pthread_exit(nullptr)
}

int main() {
    // 创建一个子线程
    pthread_t tid;

    int ret = pthread_create(&tid, nullptr, call_back, nullptr);
    if (0 != ret) {
        const char* str = strerror(ret);
        printf("error : %s\n", str);
        return -1;
    }

    // 主线程
    for (int i = 0; i < 5; ++i)
        printf("%d\n", i);

    printf("tid : %ld , parent thread id : %ld\n", tid, pthread_self());

    // 让主线程退出，当主线程退出的时候不会影响其他正常运行的线程
    pthread_exit(nullptr);

    // 这一行代码没有执行，说明主线程退出后执行return 0结束掉整个进程；而是当所有线程跑完进程才结束，因此不会对其他的线程产生影响
    printf("main thread exit.\n");

    return 0;
}

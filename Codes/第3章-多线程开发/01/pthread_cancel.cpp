#include <pthread.h>
#include <unistd.h>
using namespace std;
#include <cstring>
#include <iostream>

/*
    #include <pthread.h>

    int pthread_cancel(pthread_t thread);
作用：取消线程，让线程终止；底层的执行是发送一个取消的请求给线程(有可能是信号)，取消(或者终止)线程是要执行到某个条件点才能终止
    取消某个线程可以终止某个线程的运行，但是并不是立马终止，而是子线程执行到了一个取消点，线程才会终止
    取消点：系统规定好的一些系统调用(比如就有printf())，我们可以粗略的理解为从用户区到内核区的切换，这个位置称之为取消点

*/

void* call_back(void* arg) {
    printf("child thread id : %ld\n", pthread_self());

    for (int i = 0; i < 5; ++i)
        printf("child : %d\n", i);

    return nullptr;
}

int main() {
    // 创建一个线程
    pthread_t tid;
    int ret = pthread_create(&tid, nullptr, call_back, nullptr);
    if (0 != ret) {
        const char* errstr = strerror(ret);
        printf("error pthread_create : %s\n", errstr);
        return -1;
    }

    // 取消线程
    pthread_cancel(tid);

    for (int i = 0; i < 5; ++i)
        printf("%d\n", i);

    // 输出主线程和子线程的id
    printf("tid : %ld , main thread id : %ld\n", tid, pthread_self());

    pthread_exit(nullptr);

    return 0;
}

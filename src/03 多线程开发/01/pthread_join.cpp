#include <cstring>
#include <iostream>
using namespace std;
#include <pthread.h>
#include <unistd.h>

/*
    #include <pthread.h>

    int pthread_join(pthread_t thread, void **retval);
作用：和一个已经终止的线程进行连接
    说白了就是回收子线程的资源，防止产生僵尸线程
    这个函数是阻塞函数，调用一次只能回收一个子线程
    一般在主线程中去使用(父线程回收子线程的资源)
参数：
    thread：需要回收的子线程id
    retval：接受子线程退出时的返回值，是个二级指针；如果不需要则传递nullptr
返回值：
    成功 0
    失败 返回错误号(用strerror())
*/

int value = 10;

void* call_back(void* arg) {
    // 为了测试pthread_exit()会不会影响其他线程的正常运行，这里睡三秒，让主线程跑完
    printf("child thread id : %ld\n", pthread_self());
    sleep(3);

    // 这里两个对等是因为子线程结束并不决定整个进程的结束，主线程 return 0 就代表进程结束退出，子线程则不一样
    // 这里给一个返回值
    // int value = 10;               // 局部变量，这是存在于自己的栈空间当中，子线程结束之后就被释放，所以主线程无法接收到；一般用全局变量
    pthread_exit((void*)&value);  // return (void*)&value;
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

    // 主线程调用pthread_join()去回收子线程资源
    // 这里可以选择接受子线程执行的返回值也可以选择不要，不要就传递nullptr
    // 为什么是二级指针？返回值是一级指针，我要传递他的指针做传出参数才能接受到!!!
    int* thread_retval;
    // ret = pthread_join(tid, nullptr);
    ret = pthread_join(tid, (void**)&thread_retval);
    if (0 != ret) {
        const char* str = strerror(ret);
        printf("error : %s\n", str);
        return -1;
    }

    printf("exit data : %d\n", *thread_retval);

    printf("回收子线程资源成功\n");  // 这一行代码会在回收子线程之后结束

    // 由于子线程已经结束，主线程已经回收完了所有的资源，所以不用担心主线程结束会导致进程结束子线程没跑完的问题了
    // 所以这一行有无没有区别，有下面一行执行不了，没有下面一行会执行
    pthread_exit(nullptr);

    printf("main thread exit.\n");

    return 0;
}

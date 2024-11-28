#include <cstring>
#include <iostream>
using namespace std;
#include <pthread.h>

/*
    #include <pthread.h>

    int pthread_detach(pthread_t thread);
作用：分离一个线程，被分离的线程在终止的时候会自动释放资源返回给系统
    - 不能多次分离，会产生不可预期的结果
    - 不能去连接(join)一个已经分离的线程，如果操作了会报错(会自动释放资源)
参数：需要分离的线程id
返回值：
    成功 0
    失败 错误号
*/

void* call_back(void* arg) {
    printf("child thread id : %ld\n", pthread_self());

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

    // 输出主线程和子线程的id
    printf("tid : %ld , main thread id : %ld\n", tid, pthread_self());

    // 设置子线程分离，子线程分离后，结束时候对应的资源就不需要主线程手动回收了
    ret = pthread_detach(tid);
    if (0 != ret) {
        const char* errstr = strerror(ret);
        printf("error pthread_detach : %s\n", errstr);
        return -1;
    }

    // 设置分离后，对分离的子线程进行连接，我偏要手动释放，程序执行结果就会报错
    // ret = pthread_join(tid, nullptr);
    // if (0 != ret) {
    //     const char* errstr = strerror(ret);
    //     printf("error pthread_join : %s\n", errstr);
    //     return -1;
    // }

    // 退出主线程防止主线程结束导致进程结束导致程序结束
    pthread_exit(nullptr);

    return 0;
}

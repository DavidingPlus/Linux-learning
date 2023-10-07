#include <iostream>
using namespace std;
#include <pthread.h>
#include <unistd.h>

/*
    读写锁的类型 pthread_rwlock_t

    int pthread_rwlock_init(pthread_rwlock_t *restrict rwlock, const pthread_rwlockattr_t *restrict attr);

    int pthread_rwlock_destroy(pthread_rwlock_t *rwlock);

    int pthread_rwlock_rdlock(pthread_rwlock_t *rwlock);

    int pthread_rwlock_tryrdlock(pthread_rwlock_t *rwlock);

    int pthread_rwlock_wrlock(pthread_rwlock_t *rwlock);

    int pthread_rwlock_trywrlock(pthread_rwlock_t *rwlock);

    int pthread_rwlock_unlock(pthread_rwlock_t *rwlock);
*/

// 定义全局变量
int num = 1;

// 定义读写互斥量
pthread_rwlock_t _rwlock;

void* WRITE_CALLBACK(void* arg) {
    while (1) {
        pthread_rwlock_wrlock(&_rwlock);

        printf("++write, tid : %ld , num : %d\n", pthread_self(), ++num);

        pthread_rwlock_unlock(&_rwlock);

        usleep(1000);
    }

    return nullptr;
}

void* READ_CALLBACK(void* arg) {
    while (1) {
        pthread_rwlock_rdlock(&_rwlock);

        printf("===read, tid : %ld , num : %d\n", pthread_self(), num);

        pthread_rwlock_unlock(&_rwlock);

        usleep(1000);
    }

    return nullptr;
}

// 案例：创建8个线程，操作同一个全局变量
// 3个线程不定时的写一个全局变量，5个线程不定时的读这个全局变量
int main() {
    // 初始化读写互斥量
    pthread_rwlock_init(&_rwlock, nullptr);

    // 创建3个写线程，5个读线程
    pthread_t wr_tids[3], rd_tids[5];

    for (int i = 0; i < 3; ++i)
        pthread_create(&wr_tids[i], nullptr, WRITE_CALLBACK, nullptr);

    for (int i = 0; i < 5; ++i)
        pthread_create(&rd_tids[i], nullptr, READ_CALLBACK, nullptr);

    // 分离线程
    for (int i = 0; i < 3; ++i)
        pthread_detach(wr_tids[i]);

    for (int i = 0; i < 5; ++i)
        pthread_detach(rd_tids[i]);

    // 退出主线程
    pthread_exit(nullptr);

    // 释放读写互斥量
    pthread_rwlock_destroy(&_rwlock);

    return 0;
}

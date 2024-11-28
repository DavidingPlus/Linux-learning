#include <iostream>
using namespace std;
#include <pthread.h>
#include <unistd.h>

pthread_mutex_t mutex1, mutex2;

void* CALLBACK_A(void* arg) {
    pthread_mutex_lock(&mutex1);
    sleep(1);  // 这里睡一秒，让线程B得到抢占权
    pthread_mutex_lock(&mutex2);

    printf("thread A , tid : %ld is working.\n", pthread_self());

    // 释放锁的时候最好反着来，因为加锁是有顺序的
    pthread_mutex_unlock(&mutex2);
    pthread_mutex_unlock(&mutex1);

    return nullptr;
}

void* CALLBACK_B(void* arg) {
    pthread_mutex_lock(&mutex2);
    sleep(1);  // 这里睡一秒，让线程A得到抢占权
    pthread_mutex_lock(&mutex1);

    printf("thread B , tid : %ld is working.\n", pthread_self());

    pthread_mutex_unlock(&mutex1);
    pthread_mutex_unlock(&mutex2);

    return nullptr;
}

int main() {
    // 初始化互斥信号量
    pthread_mutex_init(&mutex1, nullptr);
    pthread_mutex_init(&mutex2, nullptr);

    pthread_t tid1, tid2;

    // 创建子线程
    pthread_create(&tid1, nullptr, &CALLBACK_A, nullptr);
    pthread_create(&tid2, nullptr, &CALLBACK_B, nullptr);

    // 回收子线程
    pthread_join(tid1, nullptr);
    pthread_join(tid2, nullptr);

    // 释放信号量
    pthread_mutex_destroy(&mutex1);
    pthread_mutex_destroy(&mutex2);

    return 0;
}
